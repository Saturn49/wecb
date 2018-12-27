#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <asm/ptrace.h>
#include <asm/stacktrace.h>
#include <linux/stacktrace.h>
#include <linux/kernel.h>
#include <asm/sections.h>
#include <asm/inst.h>
#include <asm/irq_regs.h>
#include <linux/pagemap.h>

struct stackframe {
	unsigned long sp;
	unsigned long pc;
	unsigned long ra;
};

static inline int get_mem(unsigned long addr, unsigned long *result)
{
	unsigned long *address = (unsigned long *) addr;
	if (!access_ok(VERIFY_READ, addr, sizeof(unsigned long)))
		return -1;
	if (__copy_from_user_inatomic(result, address, sizeof(unsigned long)))
		return -3;
	return 0;
}

/*
 * These two instruction helpers were taken from process.c
 */
static inline int is_ra_save_ins(union mips_instruction *ip)
{
	/* sw / sd $ra, offset($sp) */
	return (ip->i_format.opcode == sw_op || ip->i_format.opcode == sd_op)
		&& ip->i_format.rs == 29 && ip->i_format.rt == 31;
}

static inline int is_sp_move_ins(union mips_instruction *ip)
{
	/* addiu/daddiu sp,sp,-imm */
	if (ip->i_format.rs != 29 || ip->i_format.rt != 29)
		return 0;
	if (ip->i_format.opcode == addiu_op || ip->i_format.opcode == daddiu_op)
		return 1;
	return 0;
}

/*
 * Looks for specific instructions that mark the end of a function.
 * This usually means we ran into the code area of the previous function.
 */
static inline int is_end_of_function_marker(union mips_instruction *ip)
{
	/* jr ra */
	if (ip->r_format.func == jr_op && ip->r_format.rs == 31)
		return 1;
	/* lui gp */
	//if (ip->i_format.opcode == lui_op && ip->i_format.rt == 28)
	//	return 1;
	return 0;
}

/*
 * TODO for userspace stack unwinding:
 * - handle cases where the stack is adjusted inside a function
 *     (generally doesn't happen)
 * - find optimal value for max_instr_check
 * - try to find a way to handle leaf functions
 */

static inline int unwind_user_frame(struct stackframe *old_frame,
				    const unsigned int max_instr_check)
{
	off_t ra_offset = 0;
	size_t stack_size = 0;
	unsigned long addr;


	if (old_frame->pc == 0 || old_frame->sp == 0)
		return -9;

	for (addr = old_frame->pc; (addr + max_instr_check*4 > old_frame->pc)
		&& (!ra_offset || !stack_size); addr -= 4) {
		union mips_instruction ip;

		if (get_mem(addr, (unsigned long *) &ip))
			return -11;

		if (is_sp_move_ins(&ip)) {
			int stack_adjustment = ip.i_format.simmediate;
			if (stack_adjustment > 0)
				/* This marks the end of the previous function,
				   which means we overran. */
				break;
			stack_size = (unsigned) stack_adjustment;
		} else if (is_ra_save_ins(&ip)) {
			int ra_slot = ip.i_format.simmediate;
			if (ra_slot < 0)
				/* This shouldn't happen. */
				break;
			ra_offset = ra_slot;
		} else if (is_end_of_function_marker(&ip))
			break;
	}

	if (!stack_size && !old_frame->ra)
		return -1;

	if (ra_offset) {
		old_frame->pc = old_frame->sp + ra_offset;
		if (get_mem(old_frame->pc, &(old_frame->pc)))
			return -13;
	} else {
		if (!old_frame->ra)
			return -1;

		//leaf
		old_frame->pc = old_frame->ra;
		old_frame->ra = 0;
	}

	old_frame->sp = old_frame->sp - stack_size;

	return 0;
}

static void print_file_path(struct vm_area_struct *vma)
{
	struct path *path;
	char *pathname;
	char *tmp;

        tmp = (char*)__get_free_page(GFP_TEMPORARY);
	if (!tmp)
		return;

	path = &vma->vm_file->f_path;
	path_get(path);
	pathname = d_path(path, tmp, PAGE_SIZE);
	if (IS_ERR(pathname))
		goto out;

        printk("%s", pathname);
out:
	path_put(path);
	free_page((unsigned long)tmp);
}

void dump_user_backtrace(struct pt_regs *regs, int signr)
{
	struct stackframe frame = { .sp = regs->regs[29],
				    .pc = regs->cp0_epc,
				    .ra = regs->regs[31] };
	const unsigned long high_addr = ALIGN(frame.sp, THREAD_SIZE);
	const unsigned int max_instr_check = PAGE_SIZE / 4;
	const unsigned long low_addr = high_addr - THREAD_SIZE;
	struct vm_area_struct *vma;

	printk("\nUserspace Call Trace: process %s, pid %d, signal %d\n", current->comm, current->pid, signr);
	do {
		printk("[<%08lx>] ", frame.pc);
		vma = find_vma(current->mm, frame.pc);
		if (!vma || !vma->vm_file)
			printk("(unknown)");
		else
			print_file_path(vma);
		printk("\n");
		if (frame.sp < low_addr || frame.sp > high_addr)
			break;
	} while (!unwind_user_frame(&frame, max_instr_check));
}
