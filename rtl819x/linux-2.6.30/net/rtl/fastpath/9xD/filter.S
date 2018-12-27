	.file	1 "filter.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
#APP
	.macro _ssnop; sll $0, $0, 1; .endm
	.macro _ehb; sll $0, $0, 3; .endm
	.macro mtc0_tlbw_hazard; nop; nop; .endm
	.macro tlbw_use_hazard; nop; nop; nop; .endm
	.macro tlb_probe_hazard; nop; nop; nop; .endm
	.macro irq_enable_hazard; _ssnop; _ssnop; _ssnop;; .endm
	.macro irq_disable_hazard; nop; nop; nop; .endm
	.macro back_to_back_c0_hazard; _ssnop; _ssnop; _ssnop;; .endm
		.macro	raw_local_irq_enable				
	.set	push						
	.set	reorder						
	.set	noat						
	mfc0	$1,$12						
	ori	$1,0x1f						
	xori	$1,0x1e						
	mtc0	$1,$12						
	irq_enable_hazard					
	.set	pop						
	.endm
		.macro	raw_local_irq_disable
	.set	push						
	.set	noat						
	mfc0	$1,$12						
	ori	$1,0x1f						
	xori	$1,0x1f						
	.set	noreorder					
	mtc0	$1,$12						
	irq_disable_hazard					
	.set	pop						
	.endm							

		.macro	raw_local_save_flags flags			
	.set	push						
	.set	reorder						
	mfc0	\flags, $12					
	.set	pop						
	.endm							

		.macro	raw_local_irq_save result			
	.set	push						
	.set	reorder						
	.set	noat						
	mfc0	\result, $12					
	ori	$1, \result, 0x1f				
	xori	$1, 0x1f					
	.set	noreorder					
	mtc0	$1, $12						
	irq_disable_hazard					
	.set	pop						
	.endm							

		.macro	raw_local_irq_restore flags			
	.set	push						
	.set	noreorder					
	.set	noat						
	mfc0	$1, $12						
	andi	\flags, 1					
	ori	$1, 0x1f					
	xori	$1, 0x1f					
	or	\flags, $1					
	mtc0	\flags, $12					
	irq_disable_hazard					
	.set	pop						
	.endm							

#NO_APP
	.section	.text.str2hexnum,"ax",@progbits
	.align	2
	.globl	str2hexnum
	.set	nomips16
	.ent	str2hexnum
	.type	str2hexnum, @function
str2hexnum:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$L2
	move	$2,$0

$L7:
	sltu	$2,$5,10
	bne	$2,$0,$L3
	addiu	$2,$3,-65

	addiu	$5,$3,-87
	bne	$6,$0,$L3
	sltu	$2,$2,6

	bne	$2,$0,$L5
	addiu	$5,$3,-55

	j	$31
	li	$2,-1			# 0xffffffff

$L5:
$L3:
	addu	$2,$5,$7
$L2:
	lbu	$3,0($4)
	sll	$7,$2,4
	addiu	$4,$4,1
	addiu	$6,$3,-97
	addiu	$5,$3,-48
	bne	$3,$0,$L7
	sltu	$6,$6,6

	j	$31
	nop

	.set	macro
	.set	reorder
	.end	str2hexnum
	.size	str2hexnum, .-str2hexnum
	.section	.text.strtoint,"ax",@progbits
	.align	2
	.globl	strtoint
	.set	nomips16
	.ent	strtoint
	.type	strtoint, @function
strtoint:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	move	$2,$0
	j	$L11
	li	$3,10			# 0xa

$L13:
	beq	$7,$0,$L15
	mflo	$5

	addu	$2,$6,$5
$L11:
	lb	$5,0($4)
	mult	$2,$3
	addiu	$4,$4,1
	addiu	$6,$5,-48
	bne	$5,$0,$L13
	sltu	$7,$6,10

$L15:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	strtoint
	.size	strtoint, .-strtoint
	.section	.text.Add_Pattern_ACL_For_ContentFilter,"ax",@progbits
	.align	2
	.globl	Add_Pattern_ACL_For_ContentFilter
	.set	nomips16
	.ent	Add_Pattern_ACL_For_ContentFilter
	.type	Add_Pattern_ACL_For_ContentFilter, @function
Add_Pattern_ACL_For_ContentFilter:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	Add_Pattern_ACL_For_ContentFilter
	.size	Add_Pattern_ACL_For_ContentFilter, .-Add_Pattern_ACL_For_ContentFilter
	.section	.text.Del_Pattern_ACL_For_ContentFilter,"ax",@progbits
	.align	2
	.globl	Del_Pattern_ACL_For_ContentFilter
	.set	nomips16
	.ent	Del_Pattern_ACL_For_ContentFilter
	.type	Del_Pattern_ACL_For_ContentFilter, @function
Del_Pattern_ACL_For_ContentFilter:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	Del_Pattern_ACL_For_ContentFilter
	.size	Del_Pattern_ACL_For_ContentFilter, .-Del_Pattern_ACL_For_ContentFilter
	.section	.text.filter_addconnect,"ax",@progbits
	.align	2
	.globl	filter_addconnect
	.set	nomips16
	.ent	filter_addconnect
	.type	filter_addconnect, @function
filter_addconnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	andi	$3,$4,0xff
	lui	$2,%hi(ConnectedIp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$2,$3,$2
	j	$31
	sw	$4,0($2)

	.set	macro
	.set	reorder
	.end	filter_addconnect
	.size	filter_addconnect, .-filter_addconnect
	.section	.text.filter_delconnect,"ax",@progbits
	.align	2
	.globl	filter_delconnect
	.set	nomips16
	.ent	filter_delconnect
	.type	filter_delconnect, @function
filter_delconnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	andi	$4,$4,0x00ff
	lui	$2,%hi(ConnectedIp)
	sll	$4,$4,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$4,$4,$2
	j	$31
	sw	$0,0($4)

	.set	macro
	.set	reorder
	.end	filter_delconnect
	.size	filter_delconnect, .-filter_delconnect
	.section	.text.filter_checkConnect,"ax",@progbits
	.align	2
	.globl	filter_checkConnect
	.set	nomips16
	.ent	filter_checkConnect
	.type	filter_checkConnect, @function
filter_checkConnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	andi	$5,$4,0xff
	lui	$3,%hi(ConnectedIp)
	sll	$5,$5,2
	addiu	$3,$3,%lo(ConnectedIp)
	addu	$3,$5,$3
	lw	$3,0($3)
	beq	$3,$0,$L28
	li	$2,1			# 0x1

	xor	$2,$3,$4
	sltu	$2,$0,$2
$L28:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	filter_checkConnect
	.size	filter_checkConnect, .-filter_checkConnect
	.section	.text.register_customRspHook,"ax",@progbits
	.align	2
	.globl	register_customRspHook
	.set	nomips16
	.ent	register_customRspHook
	.type	register_customRspHook, @function
register_customRspHook:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	beq	$4,$0,$L30
	lui	$2,%hi(fast_path_cusRsp401_func)

	j	$L31
	sw	$4,%lo(fast_path_cusRsp401_func)($2)

$L30:
	lui	$3,%hi(GenerateHTTP401)
	addiu	$3,$3,%lo(GenerateHTTP401)
	sw	$3,%lo(fast_path_cusRsp401_func)($2)
$L31:
	beq	$5,$0,$L32
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)

	j	$L33
	sw	$5,%lo(fast_path_cusRspTCPFinAck_func)($2)

$L32:
	lui	$3,%hi(GenerateTCPFINACK)
	addiu	$3,$3,%lo(GenerateTCPFINACK)
	sw	$3,%lo(fast_path_cusRspTCPFinAck_func)($2)
$L33:
	beq	$6,$0,$L34
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)

	j	$31
	sw	$6,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L34:
	lui	$3,%hi(GenerateTCPENDACK)
	addiu	$3,$3,%lo(GenerateTCPENDACK)
	j	$31
	sw	$3,%lo(fast_path_cusRspTCPEndAck_func)($2)

	.set	macro
	.set	reorder
	.end	register_customRspHook
	.size	register_customRspHook, .-register_customRspHook
	.section	.text.unregister_customRspHook,"ax",@progbits
	.align	2
	.globl	unregister_customRspHook
	.set	nomips16
	.ent	unregister_customRspHook
	.type	unregister_customRspHook, @function
unregister_customRspHook:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(fast_path_cusRsp401_func)
	sw	$0,%lo(fast_path_cusRsp401_func)($2)
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)
	sw	$0,%lo(fast_path_cusRspTCPFinAck_func)($2)
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)
	j	$31
	sw	$0,%lo(fast_path_cusRspTCPEndAck_func)($2)

	.set	macro
	.set	reorder
	.end	unregister_customRspHook
	.size	unregister_customRspHook, .-unregister_customRspHook
	.section	.text.rtl_schedule_filter_fastpath,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	rtl_schedule_filter_fastpath
	.type	rtl_schedule_filter_fastpath, @function
rtl_schedule_filter_fastpath:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	addiu	$4,$sp,16
	jal	do_gettimeofday
	sw	$5,24($sp)

	li	$4,65536			# 0x10000
	lw	$3,16($sp)
	ori	$4,$4,0x5180
	li	$2,7			# 0x7
	lw	$5,24($sp)
	div	$0,$3,$4
	lw	$4,8($5)
	mflo	$6
	addiu	$6,$6,4
	div	$0,$6,$2
	li	$6,1			# 0x1
	mfhi	$2
	sll	$2,$6,$2
	ori	$2,$2,0x80
	and	$4,$2,$4
	beq	$4,$0,$L43
	move	$2,$0

	li	$6,3600			# 0xe10
	li	$2,60			# 0x3c
	div	$0,$3,$6
	li	$6,24			# 0x18
	lw	$4,0($5)
	mflo	$7
	div	$0,$3,$2
	mflo	$3
	div	$0,$7,$6
	mfhi	$6
	div	$0,$3,$2
	mfhi	$8
	mult	$6,$2
	mflo	$6
	bne	$4,$0,$L41
	addu	$8,$6,$8

	lw	$3,4($5)
	bne	$3,$0,$L42
	li	$2,1			# 0x1

	j	$L43
	nop

$L41:
	sltu	$4,$8,$4
	bne	$4,$0,$L40
	nop

$L42:
	lw	$2,4($5)
	sltu	$8,$2,$8
	j	$L43
	xori	$2,$8,0x1

$L40:
	move	$2,$0
$L43:
	lw	$31,36($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	rtl_schedule_filter_fastpath
	.size	rtl_schedule_filter_fastpath, .-rtl_schedule_filter_fastpath
	.section	.text.rtl_ip_range_filter_fastpath,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	rtl_ip_range_filter_fastpath
	.type	rtl_ip_range_filter_fastpath, @function
rtl_ip_range_filter_fastpath:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	jal	rtl_ip_hdr
	sw	$5,16($sp)

	lw	$5,16($sp)
	lw	$2,12($2)
	lw	$4,0($5)
	sltu	$4,$2,$4
	bne	$4,$0,$L47
	move	$3,$0

	lw	$3,4($5)
	sltu	$3,$3,$2
	xori	$3,$3,0x1
$L47:
	lw	$31,28($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	rtl_ip_range_filter_fastpath
	.size	rtl_ip_range_filter_fastpath, .-rtl_ip_range_filter_fastpath
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"filter_table\000"
	.section	.text.filter_exit_fastpath,"ax",@progbits
	.align	2
	.globl	filter_exit_fastpath
	.set	nomips16
	.ent	filter_exit_fastpath
	.type	filter_exit_fastpath, @function
filter_exit_fastpath:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(res12)
	lw	$5,%lo(res12)($16)
	beq	$5,$0,$L52
	nop

	lui	$4,%hi($LC0)
	jal	remove_proc_entry
	addiu	$4,$4,%lo($LC0)

	sw	$0,%lo(res12)($16)
$L52:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_exit_fastpath
	.size	filter_exit_fastpath, .-filter_exit_fastpath
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"enable_dos\000"
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	filter_exit
	.set	nomips16
	.ent	filter_exit
	.type	filter_exit, @function
filter_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(res3)
	lw	$5,%lo(res3)($16)
	beq	$5,$0,$L54
	nop

	lui	$4,%hi($LC1)
	jal	remove_proc_entry
	addiu	$4,$4,%lo($LC1)

	sw	$0,%lo(res3)($16)
$L54:
	lui	$4,%hi(dos_timer)
	jal	del_timer
	addiu	$4,$4,%lo(dos_timer)

	lw	$31,20($sp)
	lw	$16,16($sp)
	j	filter_exit_fastpath
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_exit
	.size	filter_exit, .-filter_exit
	.section	.text.filter_table_head_init,"ax",@progbits
	.align	2
	.globl	filter_table_head_init
	.set	nomips16
	.ent	filter_table_head_init
	.type	filter_table_head_init, @function
filter_table_head_init:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(table_list_head)
	addiu	$4,$16,%lo(table_list_head)
	move	$5,$0
	jal	memset
	li	$6,32			# 0x20

	addiu	$3,$16,%lo(table_list_head)
	sw	$3,%lo(table_list_head)($16)
	lui	$4,%hi(table_list_head+8)
	lw	$31,20($sp)
	addiu	$4,$4,%lo(table_list_head+8)
	lw	$16,16($sp)
	move	$2,$0
	li	$5,1			# 0x1
	sw	$5,24($3)
	sw	$4,12($3)
	sw	$3,4($3)
	sw	$4,8($3)
	sw	$0,16($3)
	sw	$0,20($3)
	sw	$0,28($3)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_table_head_init
	.size	filter_table_head_init, .-filter_table_head_init
	.section	.text.filter_table_init,"ax",@progbits
	.align	2
	.globl	filter_table_init
	.set	nomips16
	.ent	filter_table_init
	.type	filter_table_init, @function
filter_table_init:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	filter_table_head_init
	nop

	lui	$4,%hi($LC0)
	addiu	$4,$4,%lo($LC0)
	move	$5,$0
	jal	create_proc_entry
	move	$6,$0

	lui	$3,%hi(res12)
	beq	$2,$0,$L59
	sw	$2,%lo(res12)($3)

	lui	$3,%hi(filter_table_write_proc)
	addiu	$3,$3,%lo(filter_table_write_proc)
	sw	$3,68($2)
	lui	$3,%hi(filter_table_read_proc)
	addiu	$3,$3,%lo(filter_table_read_proc)
	sw	$3,64($2)
$L59:
	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_table_init
	.size	filter_table_init, .-filter_table_init
	.section	.text.filter_init_fastpath,"ax",@progbits
	.align	2
	.globl	filter_init_fastpath
	.set	nomips16
	.ent	filter_init_fastpath
	.type	filter_init_fastpath, @function
filter_init_fastpath:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$3,1			# 0x1
	lui	$2,%hi(filter_rule_num)
	jal	filter_table_init
	sw	$3,%lo(filter_rule_num)($2)

	move	$2,$0
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_init_fastpath
	.size	filter_init_fastpath, .-filter_init_fastpath
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	filter_init
	.set	nomips16
	.ent	filter_init
	.type	filter_init, @function
filter_init:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(item)
	sw	$0,%lo(item)($2)
	lui	$4,%hi($LC1)
	lui	$2,%hi(none_dos_drop_pkt_cnt)
	addiu	$4,$4,%lo($LC1)
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
	move	$5,$0
	lui	$2,%hi(url_count)
	move	$6,$0
	jal	create_proc_entry
	sb	$0,%lo(url_count)($2)

	lui	$3,%hi(res3)
	beq	$2,$0,$L64
	sw	$2,%lo(res3)($3)

	lui	$3,%hi(dos_write_proc)
	addiu	$3,$3,%lo(dos_write_proc)
	sw	$3,68($2)
	lui	$3,%hi(dos_read_proc)
	addiu	$3,$3,%lo(dos_read_proc)
	sw	$3,64($2)
$L64:
	lui	$16,%hi(dos_timer)
	move	$6,$0
	addiu	$4,$16,%lo(dos_timer)
	jal	init_timer_key
	move	$5,$0

	lui	$4,%hi(jiffies)
	lw	$3,%lo(jiffies)($4)
	addiu	$2,$16,%lo(dos_timer)
	lw	$5,%lo(jiffies)($4)
	lui	$16,%hi(_tcpDosScanBitmap)
	move	$4,$2
	addiu	$3,$3,100
	sw	$3,8($2)
	lui	$3,%hi(dos_timer_fn)
	addiu	$3,$3,%lo(dos_timer_fn)
	addiu	$5,$5,100
	sw	$3,12($2)
	jal	mod_timer
	sw	$0,16($2)

	addiu	$4,$16,%lo(_tcpDosScanBitmap)
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	addiu	$3,$16,%lo(_tcpDosScanBitmap)
	li	$2,1			# 0x1
	sb	$2,3($3)
	sb	$2,63($3)
	sb	$2,58($3)
	sb	$2,41($3)
	sb	$2,40($3)
	sb	$2,33($3)
	sb	$2,32($3)
	sb	$2,9($3)
	sb	$2,8($3)
	jal	filter_init_fastpath
	sb	$2,%lo(_tcpDosScanBitmap)($16)

	jal	igmp_delete_init_netlink
	nop

	move	$2,$0
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_init
	.size	filter_init, .-filter_init
	.section	.rodata.str1.4
	.align	2
$LC2:
	.ascii	"DoS: Whole System SYN Flood Attack\012\000"
	.align	2
$LC3:
	.ascii	"DoS: Whole System FIN Flood Attack\012\000"
	.align	2
$LC4:
	.ascii	"DoS: Whole System UDP Flood Attack\012\000"
	.align	2
$LC5:
	.ascii	"DoS: Whole System ICMP Flood Attack\012\000"
	.align	2
$LC6:
	.ascii	"DoS: Per-source SYN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC7:
	.ascii	"DoS: Per-source FIN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC8:
	.ascii	"DoS: Per-source UDP Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC9:
	.ascii	"DoS: Per-source ICMP Flood Attack source=%u.%u.%u.%u des"
	.ascii	"tination=%u.%u.%u.%u\012\000"
	.align	2
$LC10:
	.ascii	"DoS: Port Scan Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\012\000"
	.align	2
$LC11:
	.ascii	"DoS: Tcp Scan Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\012\000"
	.align	2
$LC12:
	.ascii	"DoS: Tcp SYN With Data Attack source=%u.%u.%u.%u destina"
	.ascii	"tion=%u.%u.%u.%u\012\000"
	.align	2
$LC13:
	.ascii	"DoS: IP Land Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC14:
	.ascii	"DoS: UdpEchoChargen Attack source=%u.%u.%u.%u destinatio"
	.ascii	"n=%u.%u.%u.%u\012\000"
	.align	2
$LC15:
	.ascii	"DoS: UdpBomb Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC16:
	.ascii	"DoS: PingOfDeath Attack source=%u.%u.%u.%u destination=%"
	.ascii	"u.%u.%u.%u\012\000"
	.align	2
$LC17:
	.ascii	"DoS: IcmpSmurf Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\012\000"
	.align	2
$LC18:
	.ascii	"DoS: IpSpoof Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC19:
	.ascii	"DoS: TearDrop Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\012\000"
	.section	.text.dos_timer_fn,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	dos_timer_fn
	.type	dos_timer_fn, @function
dos_timer_fn:
	.frame	$sp,48,$31		# vars= 0, regs= 2/0, args= 40, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	lui	$2,%hi(item)
	lw	$2,%lo(item)($2)
	beq	$2,$0,$L67
	lui	$3,%hi(block_count)

	lui	$4,%hi(block_time)
	lw	$4,%lo(block_time)($4)
	lw	$5,%lo(block_count)($3)
	slt	$4,$5,$4
	bne	$4,$0,$L68
	lui	$4,%hi(block)

	sw	$0,%lo(block)($4)
	sw	$0,%lo(block_count)($3)
$L68:
	lui	$3,%hi(block)
	lw	$4,%lo(block)($3)
	li	$3,1			# 0x1
	bne	$4,$3,$L108
	andi	$3,$2,0x1e

	lui	$3,%hi(block_count)
	lw	$4,%lo(block_count)($3)
	addiu	$4,$4,1
	sw	$4,%lo(block_count)($3)
	andi	$3,$2,0x1e
$L108:
	beq	$3,$0,$L70
	lui	$13,%hi(dos_pkt+36828)

	lui	$3,%hi(dos_pkt)
	andi	$14,$2,0x2
	addiu	$13,$13,%lo(dos_pkt+36828)
	addiu	$3,$3,%lo(dos_pkt)
	andi	$12,$2,0x4
	move	$4,$0
	move	$5,$0
	andi	$11,$2,0x8
	move	$6,$0
	move	$7,$0
	andi	$10,$2,0x10
	li	$9,1			# 0x1
$L76:
	lb	$8,0($3)
	bne	$8,$9,$L71
	nop

	beq	$14,$0,$L72
	nop

	lw	$8,4($3)
	slt	$15,$0,$8
	addu	$8,$7,$8
	j	$L72
	movn	$7,$8,$15

$L103:
	lw	$8,8($3)
	slt	$15,$0,$8
	addu	$8,$6,$8
	j	$L73
	movn	$6,$8,$15

$L104:
	lw	$8,12($3)
	slt	$15,$0,$8
	addu	$8,$5,$8
	j	$L74
	movn	$5,$8,$15

$L105:
	lw	$8,16($3)
	slt	$15,$0,$8
	addu	$8,$4,$8
	movn	$4,$8,$15
$L71:
	bne	$3,$13,$L76
	addiu	$3,$3,36

	lui	$3,%hi(whole_syn_threshold)
	lw	$3,%lo(whole_syn_threshold)($3)
	slt	$7,$3,$7
	beq	$7,$0,$L77
	andi	$7,$2,0x2

	beq	$7,$0,$L77
	nop

	blez	$3,$L77
	lui	$3,%hi(LogFlag)

	lw	$7,%lo(LogFlag)($3)
	ori	$7,$7,0x2
	sw	$7,%lo(LogFlag)($3)
$L77:
	lui	$3,%hi(whole_fin_threshold)
	lw	$3,%lo(whole_fin_threshold)($3)
	slt	$6,$3,$6
	beq	$6,$0,$L78
	andi	$6,$2,0x4

	beq	$6,$0,$L78
	nop

	blez	$3,$L78
	lui	$3,%hi(LogFlag)

	lw	$6,%lo(LogFlag)($3)
	ori	$6,$6,0x4
	sw	$6,%lo(LogFlag)($3)
$L78:
	lui	$3,%hi(whole_udp_threshold)
	lw	$3,%lo(whole_udp_threshold)($3)
	slt	$5,$3,$5
	beq	$5,$0,$L79
	andi	$5,$2,0x8

	beq	$5,$0,$L79
	nop

	blez	$3,$L79
	lui	$3,%hi(LogFlag)

	lw	$5,%lo(LogFlag)($3)
	ori	$5,$5,0x8
	sw	$5,%lo(LogFlag)($3)
$L79:
	lui	$3,%hi(whole_icmp_threshold)
	lw	$3,%lo(whole_icmp_threshold)($3)
	slt	$4,$3,$4
	beq	$4,$0,$L70
	andi	$2,$2,0x10

	beq	$2,$0,$L109
	lui	$2,%hi(LogFlag)

	blez	$3,$L109
	nop

	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x10
	sw	$3,%lo(LogFlag)($2)
$L70:
	lui	$2,%hi(LogFlag)
$L109:
	lw	$16,%lo(LogFlag)($2)
	andi	$2,$16,0x2
	beq	$2,$0,$L110
	andi	$2,$16,0x4

	lui	$4,%hi($LC2)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC2)

	andi	$2,$16,0x4
$L110:
	beq	$2,$0,$L111
	andi	$2,$16,0x8

	lui	$4,%hi($LC3)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC3)

	andi	$2,$16,0x8
$L111:
	beq	$2,$0,$L112
	andi	$2,$16,0x10

	lui	$4,%hi($LC4)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC4)

	andi	$2,$16,0x10
$L112:
	beq	$2,$0,$L113
	andi	$2,$16,0x20

	lui	$4,%hi($LC5)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC5)

	andi	$2,$16,0x20
$L113:
	beq	$2,$0,$L114
	andi	$2,$16,0x40

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,5($3)
	lui	$4,%hi(attack_saddr+4)
	lbu	$7,6($3)
	lbu	$10,7($3)
	lui	$3,%hi(attack_daddr+4)
	lbu	$8,5($2)
	lbu	$9,%lo(attack_daddr+4)($3)
	lbu	$5,%lo(attack_saddr+4)($4)
	lui	$4,%hi($LC6)
	lbu	$3,7($2)
	addiu	$4,$4,%lo($LC6)
	lbu	$2,6($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x40
$L114:
	beq	$2,$0,$L115
	andi	$2,$16,0x80

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,9($3)
	lui	$4,%hi(attack_saddr+8)
	lbu	$7,10($3)
	lbu	$10,11($3)
	lui	$3,%hi(attack_daddr+8)
	lbu	$8,9($2)
	lbu	$9,%lo(attack_daddr+8)($3)
	lbu	$5,%lo(attack_saddr+8)($4)
	lui	$4,%hi($LC7)
	lbu	$3,11($2)
	addiu	$4,$4,%lo($LC7)
	lbu	$2,10($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x80
$L115:
	beq	$2,$0,$L116
	andi	$2,$16,0x100

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,13($3)
	lui	$4,%hi(attack_saddr+12)
	lbu	$7,14($3)
	lbu	$10,15($3)
	lui	$3,%hi(attack_daddr+12)
	lbu	$8,13($2)
	lbu	$9,%lo(attack_daddr+12)($3)
	lbu	$5,%lo(attack_saddr+12)($4)
	lui	$4,%hi($LC8)
	lbu	$3,15($2)
	addiu	$4,$4,%lo($LC8)
	lbu	$2,14($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x100
$L116:
	beq	$2,$0,$L117
	andi	$2,$16,0x200

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,17($3)
	lui	$4,%hi(attack_saddr+16)
	lbu	$7,18($3)
	lbu	$10,19($3)
	lui	$3,%hi(attack_daddr+16)
	lbu	$8,17($2)
	lbu	$9,%lo(attack_daddr+16)($3)
	lbu	$5,%lo(attack_saddr+16)($4)
	lui	$4,%hi($LC9)
	lbu	$3,19($2)
	addiu	$4,$4,%lo($LC9)
	lbu	$2,18($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x200
$L117:
	beq	$2,$0,$L118
	andi	$2,$16,0x8000

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,21($3)
	lui	$4,%hi(attack_saddr+20)
	lbu	$7,22($3)
	lbu	$10,23($3)
	lui	$3,%hi(attack_daddr+20)
	lbu	$8,21($2)
	lbu	$9,%lo(attack_daddr+20)($3)
	lbu	$5,%lo(attack_saddr+20)($4)
	lui	$4,%hi($LC10)
	lbu	$3,23($2)
	addiu	$4,$4,%lo($LC10)
	lbu	$2,22($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x8000
$L118:
	beq	$2,$0,$L119
	li	$2,65536			# 0x10000

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,45($3)
	lui	$4,%hi(attack_saddr+44)
	lbu	$7,46($3)
	lbu	$10,47($3)
	lui	$3,%hi(attack_daddr+44)
	lbu	$8,45($2)
	lbu	$9,%lo(attack_daddr+44)($3)
	lbu	$5,%lo(attack_saddr+44)($4)
	lui	$4,%hi($LC11)
	lbu	$3,47($2)
	addiu	$4,$4,%lo($LC11)
	lbu	$2,46($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	li	$2,65536			# 0x10000
$L119:
	and	$2,$16,$2
	beq	$2,$0,$L120
	andi	$2,$16,0x800

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,49($3)
	lui	$4,%hi(attack_saddr+48)
	lbu	$7,50($3)
	lbu	$10,51($3)
	lui	$3,%hi(attack_daddr+48)
	lbu	$8,49($2)
	lbu	$9,%lo(attack_daddr+48)($3)
	lbu	$5,%lo(attack_saddr+48)($4)
	lui	$4,%hi($LC12)
	lbu	$3,51($2)
	addiu	$4,$4,%lo($LC12)
	lbu	$2,50($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x800
$L120:
	beq	$2,$0,$L121
	li	$2,262144			# 0x40000

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,29($3)
	lui	$4,%hi(attack_saddr+28)
	lbu	$7,30($3)
	lbu	$10,31($3)
	lui	$3,%hi(attack_daddr+28)
	lbu	$8,29($2)
	lbu	$9,%lo(attack_daddr+28)($3)
	lbu	$5,%lo(attack_saddr+28)($4)
	lui	$4,%hi($LC13)
	lbu	$3,31($2)
	addiu	$4,$4,%lo($LC13)
	lbu	$2,30($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	li	$2,262144			# 0x40000
$L121:
	and	$2,$16,$2
	beq	$2,$0,$L122
	li	$2,131072			# 0x20000

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,56($3)
	lui	$4,%hi($LC14)
	lbu	$6,57($3)
	addiu	$4,$4,%lo($LC14)
	lbu	$7,58($3)
	lbu	$10,59($3)
	lbu	$9,56($2)
	lbu	$3,59($2)
	lbu	$8,57($2)
	lbu	$2,58($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	li	$2,131072			# 0x20000
$L122:
	and	$2,$16,$2
	beq	$2,$0,$L123
	andi	$2,$16,0x4000

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,53($3)
	lui	$4,%hi(attack_saddr+52)
	lbu	$7,54($3)
	lbu	$10,55($3)
	lui	$3,%hi(attack_daddr+52)
	lbu	$8,53($2)
	lbu	$9,%lo(attack_daddr+52)($3)
	lbu	$5,%lo(attack_saddr+52)($4)
	lui	$4,%hi($LC15)
	lbu	$3,55($2)
	addiu	$4,$4,%lo($LC15)
	lbu	$2,54($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x4000
$L123:
	beq	$2,$0,$L124
	andi	$2,$16,0x400

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,41($3)
	lui	$4,%hi(attack_saddr+40)
	lbu	$7,42($3)
	lbu	$10,43($3)
	lui	$3,%hi(attack_daddr+40)
	lbu	$8,41($2)
	lbu	$9,%lo(attack_daddr+40)($3)
	lbu	$5,%lo(attack_saddr+40)($4)
	lui	$4,%hi($LC16)
	lbu	$3,43($2)
	addiu	$4,$4,%lo($LC16)
	lbu	$2,42($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x400
$L124:
	beq	$2,$0,$L125
	andi	$2,$16,0x1000

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,25($3)
	lui	$4,%hi(attack_saddr+24)
	lbu	$7,26($3)
	lbu	$10,27($3)
	lui	$3,%hi(attack_daddr+24)
	lbu	$8,25($2)
	lbu	$9,%lo(attack_daddr+24)($3)
	lbu	$5,%lo(attack_saddr+24)($4)
	lui	$4,%hi($LC17)
	lbu	$3,27($2)
	addiu	$4,$4,%lo($LC17)
	lbu	$2,26($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	andi	$2,$16,0x1000
$L125:
	beq	$2,$0,$L96
	lui	$3,%hi(attack_saddr)

	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,33($3)
	lui	$4,%hi(attack_saddr+32)
	lbu	$7,34($3)
	lbu	$10,35($3)
	lui	$3,%hi(attack_daddr+32)
	lbu	$8,33($2)
	lbu	$9,%lo(attack_daddr+32)($3)
	lbu	$5,%lo(attack_saddr+32)($4)
	lui	$4,%hi($LC18)
	lbu	$3,35($2)
	addiu	$4,$4,%lo($LC18)
	lbu	$2,34($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

$L96:
	andi	$16,$16,0x2000
	beq	$16,$0,$L126
	lui	$2,%hi(LogFlag)

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$6,37($3)
	lui	$4,%hi(attack_saddr+36)
	lbu	$7,38($3)
	lbu	$10,39($3)
	lui	$3,%hi(attack_daddr+36)
	lbu	$8,37($2)
	lbu	$9,%lo(attack_daddr+36)($3)
	lbu	$5,%lo(attack_saddr+36)($4)
	lui	$4,%hi($LC19)
	lbu	$3,39($2)
	addiu	$4,$4,%lo($LC19)
	lbu	$2,38($2)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$2,28($sp)
	jal	scrlog_printk
	sw	$3,32($sp)

	lui	$2,%hi(LogFlag)
$L126:
	lui	$3,%hi(dos_pkt+36864)
	sw	$0,%lo(LogFlag)($2)
	lui	$2,%hi(dos_pkt)
	addiu	$3,$3,%lo(dos_pkt+36864)
	addiu	$2,$2,%lo(dos_pkt)
$L98:
	sb	$0,0($2)
	sw	$0,24($2)
	sw	$0,4($2)
	sw	$0,8($2)
	sw	$0,12($2)
	sw	$0,16($2)
	sw	$0,20($2)
	addiu	$2,$2,36
	bne	$2,$3,$L98
	nop

$L67:
	lui	$2,%hi(none_dos_drop_pkt_cnt)
	lw	$2,%lo(none_dos_drop_pkt_cnt)($2)
	sltu	$3,$2,3001
	bne	$3,$0,$L99
	lui	$3,%hi(item_backup)

	lui	$2,%hi(item)
	lw	$3,%lo(item)($2)
	beq	$3,$0,$L100
	lui	$4,%hi(item_backup)

	sw	$3,%lo(item_backup)($4)
	j	$L100
	sw	$0,%lo(item)($2)

$L99:
	lw	$4,%lo(item_backup)($3)
	beq	$4,$0,$L101
	lui	$5,%hi(item)

	sw	$4,%lo(item)($5)
	sw	$0,%lo(item_backup)($3)
$L101:
	beq	$2,$0,$L127
	lui	$2,%hi(jiffies)

$L100:
	lui	$2,%hi(none_dos_drop_pkt_cnt)
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
	lui	$2,%hi(jiffies)
$L127:
	lui	$4,%hi(dos_timer)
	lw	$5,%lo(jiffies)($2)
	addiu	$4,$4,%lo(dos_timer)
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$5,$5,100
	j	mod_timer
	addiu	$sp,$sp,48

$L72:
	bne	$12,$0,$L103
	nop

$L73:
	bne	$11,$0,$L104
	nop

$L74:
	bne	$10,$0,$L105
	nop

	j	$L71
	nop

	.set	macro
	.set	reorder
	.end	dos_timer_fn
	.size	dos_timer_fn, .-dos_timer_fn
	.section	.rodata.str1.4
	.align	2
$LC20:
	.ascii	"%s\012\000"
	.section	.text.dos_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	dos_read_proc
	.type	dos_read_proc, @function
dos_read_proc:
	.frame	$sp,40,$31		# vars= 8, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$16,$6
	lui	$6,%hi(dos_flag)
	move	$17,$5
	lui	$5,%hi($LC20)
	addiu	$6,$6,%lo(dos_flag)
	addiu	$5,$5,%lo($LC20)
	move	$18,$4
	jal	sprintf
	sw	$7,16($sp)

	lw	$7,16($sp)
	addu	$3,$7,$16
	slt	$3,$3,$2
	bne	$3,$0,$L129
	li	$4,1			# 0x1

	lw	$3,56($sp)
	sw	$4,0($3)
$L129:
	subu	$2,$2,$16
	addu	$16,$18,$16
	slt	$3,$7,$2
	sw	$16,0($17)
	movz	$7,$2,$3
	slt	$2,$7,0
	lw	$31,36($sp)
	movn	$7,$0,$2
	lw	$18,32($sp)
	move	$2,$7
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	dos_read_proc
	.size	dos_read_proc, .-dos_read_proc
	.section	.rodata.str1.4
	.align	2
$LC21:
	.ascii	"br0\000"
	.align	2
$LC22:
	.ascii	"ppp0\000"
	.align	2
$LC23:
	.ascii	"eth1\000"
	.align	2
$LC24:
	.ascii	"wlan0\000"
	.section	.text.need_filter,"ax",@progbits
	.align	2
	.globl	need_filter
	.set	nomips16
	.ent	need_filter
	.type	need_filter, @function
need_filter:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	jal	rtl_ip_hdr
	lui	$4,%hi($LC21)
	addiu	$4,$4,%lo($LC21)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L155
	lui	$4,%hi($LC22)
	.set	macro
	.set	reorder

	lw	$2,292($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L155
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi($LC21)
	lw	$2,12($2)
	.set	noreorder
	.set	nomacro
	j	$L134
	addiu	$6,$3,%lo($LC21)
	.set	macro
	.set	reorder

$L136:
#APP
 # 75 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$7,($4)
1:	lbu	$1,($5)
	addiu	$4,1
	bne	$1,$7,2f
	addiu	$5,1
	bnez	$7,1b
	lbu	$7,($4)
	move	$7,$1
2:	subu	$7,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	beq	$7,$0,$L135
	lw	$2,0($2)
$L134:
	addiu	$5,$2,35
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L136
	move	$4,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L155
	lui	$4,%hi($LC22)
	.set	macro
	.set	reorder

$L135:
	lw	$3,20($2)
	lw	$4,16($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L137
	move	$2,$0
	.set	macro
	.set	reorder

	lui	$4,%hi($LC22)
$L155:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC22)
	.set	macro
	.set	reorder

	beq	$2,$0,$L138
	lw	$2,292($2)
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L140
	move	$3,$2
	.set	macro
	.set	reorder

	lui	$2,%hi($LC22)
	lw	$3,12($3)
	.set	noreorder
	.set	nomacro
	j	$L141
	addiu	$6,$2,%lo($LC22)
	.set	macro
	.set	reorder

$L143:
#APP
 # 75 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$7,($4)
1:	lbu	$1,($5)
	addiu	$4,1
	bne	$1,$7,2f
	addiu	$5,1
	bnez	$7,1b
	lbu	$7,($4)
	move	$7,$1
2:	subu	$7,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	beq	$7,$0,$L142
	lw	$3,0($3)
$L141:
	addiu	$5,$3,35
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L143
	move	$4,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L137
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L142:
	lw	$4,16($16)
	lw	$2,16($3)
	.set	noreorder
	.set	nomacro
	j	$L156
	xor	$2,$4,$2
	.set	macro
	.set	reorder

$L138:
	lui	$4,%hi($LC23)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC23)
	.set	macro
	.set	reorder

	beq	$2,$0,$L144
	lw	$2,292($2)
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L140
	move	$3,$2
	.set	macro
	.set	reorder

	lui	$2,%hi($LC23)
	lw	$3,12($3)
	.set	noreorder
	.set	nomacro
	j	$L146
	addiu	$6,$2,%lo($LC23)
	.set	macro
	.set	reorder

$L148:
#APP
 # 75 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$7,($4)
1:	lbu	$1,($5)
	addiu	$4,1
	bne	$1,$7,2f
	addiu	$5,1
	bnez	$7,1b
	lbu	$7,($4)
	move	$7,$1
2:	subu	$7,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	beq	$7,$0,$L151
	lw	$3,0($3)
$L146:
	addiu	$5,$3,35
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L148
	move	$4,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L137
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L144:
	lui	$4,%hi($LC24)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC24)
	.set	macro
	.set	reorder

	beq	$2,$0,$L140
	lw	$2,292($2)
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L140
	move	$3,$2
	.set	macro
	.set	reorder

	lui	$2,%hi($LC24)
	lw	$3,12($3)
	.set	noreorder
	.set	nomacro
	j	$L150
	addiu	$6,$2,%lo($LC24)
	.set	macro
	.set	reorder

$L152:
#APP
 # 75 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$7,($4)
1:	lbu	$1,($5)
	addiu	$4,1
	bne	$1,$7,2f
	addiu	$5,1
	bnez	$7,1b
	lbu	$7,($4)
	move	$7,$1
2:	subu	$7,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	beq	$7,$0,$L151
	lw	$3,0($3)
$L150:
	addiu	$5,$3,35
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L152
	move	$4,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L137
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L151:
	lw	$4,16($16)
	lw	$2,20($3)
	xor	$2,$4,$2
$L156:
	.set	noreorder
	.set	nomacro
	j	$L137
	sltu	$2,$0,$2
	.set	macro
	.set	reorder

$L140:
	li	$2,1			# 0x1
$L137:
	lw	$31,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	need_filter
	.size	need_filter, .-need_filter
	.section	.rodata.str1.4
	.align	2
$LC25:
	.ascii	"dos_filter: alloc skb fail!\012\000"
	.section	.text.GenerateTCPENDACK,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	GenerateTCPENDACK
	.type	GenerateTCPENDACK, @function
GenerateTCPENDACK:
	.frame	$sp,104,$31		# vars= 48, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$fp,96($sp)
	sw	$23,92($sp)
	sw	$22,88($sp)
	sw	$21,84($sp)
	sw	$20,80($sp)
	sw	$19,76($sp)
	sw	$18,72($sp)
	sw	$17,68($sp)
	sw	$16,64($sp)
	move	$18,$4
	lui	$4,%hi($LC21)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC21)
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,60($sp)
	.set	macro
	.set	reorder

	lw	$5,60($sp)
	addiu	$4,$2,12
	lbu	$19,0($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	move	$17,$2
	.set	macro
	.set	reorder

	andi	$19,$19,0xf
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L158
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L168
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	j	$L171
$L172:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L158
$L168:
	move	$4,$18
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L161
	move	$20,$2
	.set	macro
	.set	reorder

#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	bne	$2,$0,$L162
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

$L162:
	lui	$4,%hi($LC25)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC25)
	.set	macro
	.set	reorder

	j	$L158
$L161:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$20
	lbu	$5,0($2)
	andi	$5,$5,0xf
	addiu	$5,$5,5
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$5,2
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	addiu	$4,$sp,56
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_lock_from_hh
	move	$21,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$20
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	move	$22,$2
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	addiu	$4,$22,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$2
	.set	macro
	.set	reorder

	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	bne	$2,$0,$L163
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

$L163:
	sll	$19,$19,2
	move	$5,$0
	li	$23,268369920			# 0xfff0000
	addu	$19,$17,$19
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	addiu	$16,$sp,36
	.set	macro
	.set	reorder

	ori	$23,$23,0xffff
	li	$5,1342177280			# 0x50000000
	lhu	$2,2($19)
	lui	$21,%hi(url_cache)
	move	$4,$20
	addiu	$22,$21,%lo(url_cache)
	sh	$2,36($sp)
	lw	$2,48($sp)
	lhu	$3,0($19)
	and	$2,$2,$23
	sh	$3,38($sp)
	or	$2,$2,$5
	li	$3,-327680			# 0xfffb0000
	lw	$6,8($19)
	ori	$3,$3,0xffff
	lw	$5,4($19)
	sw	$2,48($sp)
	sb	$0,13($16)
	addiu	$5,$5,1
	lw	$7,48($sp)
	sw	$6,40($sp)
	li	$6,1048576			# 0x100000
	and	$3,$7,$3
	sw	$5,44($sp)
	li	$5,-131072			# 0xfffe0000
	lhu	$2,12($22)
	or	$3,$3,$6
	ori	$5,$5,0xffff
	sh	$0,54($sp)
	and	$3,$3,$5
	sw	$3,48($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$2,50($sp)
	.set	macro
	.set	reorder

	move	$6,$0
	move	$4,$16
	sh	$0,52($sp)
	addiu	$19,$2,-34
	lw	$3,16($17)
	move	$5,$19
	lw	$fp,12($17)
	andi	$19,$19,0xffff
	sw	$3,60($sp)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$19,$19,6
	.set	macro
	.set	reorder

	move	$4,$20
	lw	$3,60($sp)
#APP
 # 141 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$3, $fp		
	sltu	$1, $3, $fp	
	addu	$3, $1		
	addu	$3, $19		
	sltu	$1, $3, $19	
	addu	$3, $1		
	addu	$3, $2		
	sltu	$1, $3, $2	
	addu	$3, $1		
	.set	pop
 # 0 "" 2
 # 83 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $3, 16	
	addu	$3, $1		
	sltu	$1, $3, $1	
	srl	$3, $3, 16	
	addu	$3, $1		
	xori	$3, 0xffff	
	.set	pop
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$3,52($sp)
	.set	macro
	.set	reorder

	li	$6,20			# 0x14
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,34
	.set	macro
	.set	reorder

	move	$4,$18
	move	$5,$0
	lw	$2,16($sp)
	lbu	$3,14($22)
	and	$23,$2,$23
	li	$2,6			# 0x6
	sb	$2,25($sp)
	li	$2,1073741824			# 0x40000000
	or	$23,$23,$2
	li	$2,-251723776			# 0xf0ff0000
	ori	$2,$2,0xffff
	sh	$0,22($sp)
	and	$23,$23,$2
	li	$2,83886080			# 0x5000000
	or	$23,$23,$2
	sw	$23,16($sp)
	sb	$0,17($sp)
	lw	$2,12($17)
	sw	$2,32($sp)
	lw	$2,16($17)
	sb	$3,24($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sw	$2,28($sp)
	.set	macro
	.set	reorder

	li	$3,40			# 0x28
	lhu	$2,%lo(url_cache)($21)
	addiu	$4,$sp,16
	sh	$3,18($sp)
	addiu	$3,$sp,32
	addiu	$2,$2,1
	sh	$2,20($sp)
	sh	$0,26($sp)
	lw	$7,4($4)
	lw	$6,16($sp)
	lw	$5,8($4)
	srl	$8,$6,22
	addu	$6,$7,$6
	addu	$2,$6,$5
	sltu	$7,$6,$7
	lw	$6,28($sp)
	addu	$7,$2,$7
	andi	$8,$8,0x3c
	sltu	$5,$7,$5
	addu	$4,$4,$8
	addu	$7,$7,$6
	addu	$5,$7,$5
	sltu	$2,$5,$6
	addu	$2,$2,$5
$L164:
	lw	$5,0($3)
	addiu	$3,$3,4
	addu	$2,$2,$5
	sltu	$5,$2,$5
	.set	noreorder
	.set	nomacro
	bne	$3,$4,$L164
	addu	$2,$5,$2
	.set	macro
	.set	reorder

	move	$4,$20
#APP
 # 83 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $2, 16	
	addu	$2, $1		
	sltu	$1, $2, $1	
	srl	$2, $2, 16	
	addu	$2, $1		
	xori	$2, 0xffff	
	.set	pop
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,26($sp)
	.set	macro
	.set	reorder

	addiu	$5,$sp,16
	addiu	$4,$2,14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,20			# 0x14
	.set	macro
	.set	reorder

	lui	$2,%hi(url_cache+16)
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$0,%lo(url_cache+16)($2)
	.set	macro
	.set	reorder

$L158:
	lw	$31,100($sp)
	move	$2,$0
	lw	$fp,96($sp)
	lw	$23,92($sp)
	lw	$22,88($sp)
	lw	$21,84($sp)
	lw	$20,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,104
	.set	macro
	.set	reorder

$L171:
#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	bne	$2,$0,$L158
	j	$L172
	.end	GenerateTCPENDACK
	.size	GenerateTCPENDACK, .-GenerateTCPENDACK
	.section	.text.GenerateTCPFINACK,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	GenerateTCPFINACK
	.type	GenerateTCPFINACK, @function
GenerateTCPFINACK:
	.frame	$sp,104,$31		# vars= 48, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$fp,96($sp)
	sw	$23,92($sp)
	sw	$22,88($sp)
	sw	$21,84($sp)
	sw	$20,80($sp)
	sw	$19,76($sp)
	sw	$18,72($sp)
	sw	$17,68($sp)
	sw	$16,64($sp)
	move	$18,$4
	lui	$4,%hi($LC21)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC21)
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,60($sp)
	.set	macro
	.set	reorder

	lw	$5,60($sp)
	addiu	$4,$2,12
	lbu	$19,0($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	move	$17,$2
	.set	macro
	.set	reorder

	andi	$19,$19,0xf
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L184
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	j	$L187
$L188:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L174
$L184:
	move	$4,$18
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L177
	move	$20,$2
	.set	macro
	.set	reorder

#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	bne	$2,$0,$L178
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

$L178:
	lui	$4,%hi($LC25)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC25)
	.set	macro
	.set	reorder

	j	$L174
$L177:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$20
	lbu	$5,0($2)
	andi	$5,$5,0xf
	addiu	$5,$5,5
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$5,2
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	addiu	$4,$sp,56
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_lock_from_hh
	move	$21,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$20
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	move	$22,$2
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	addiu	$4,$22,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$2
	.set	macro
	.set	reorder

	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	bne	$2,$0,$L179
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

$L179:
	sll	$19,$19,2
	move	$5,$0
	li	$23,268369920			# 0xfff0000
	addu	$19,$17,$19
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	lui	$21,%hi(url_cache)
	.set	macro
	.set	reorder

	ori	$23,$23,0xffff
	addiu	$16,$sp,36
	lhu	$2,2($19)
	addiu	$22,$21,%lo(url_cache)
	move	$4,$20
	lw	$6,48($sp)
	sh	$2,36($sp)
	li	$2,1342177280			# 0x50000000
	lhu	$3,0($19)
	and	$6,$6,$23
	or	$6,$6,$2
	lhu	$2,12($22)
	sh	$3,38($sp)
	lw	$3,8($19)
	lw	$5,4($19)
	sw	$6,48($sp)
	sb	$0,13($16)
	lw	$6,48($sp)
	sw	$3,40($sp)
	li	$3,-327680			# 0xfffb0000
	ori	$3,$3,0xffff
	sw	$5,44($sp)
	li	$5,1048576			# 0x100000
	and	$3,$6,$3
	sh	$0,54($sp)
	or	$3,$3,$5
	li	$5,65536			# 0x10000
	or	$3,$3,$5
	sw	$3,48($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$2,50($sp)
	.set	macro
	.set	reorder

	move	$6,$0
	move	$4,$16
	sh	$0,52($sp)
	addiu	$19,$2,-34
	lw	$3,16($17)
	move	$5,$19
	lw	$fp,12($17)
	andi	$19,$19,0xffff
	sw	$3,60($sp)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$19,$19,6
	.set	macro
	.set	reorder

	move	$4,$20
	lw	$3,60($sp)
#APP
 # 141 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$3, $fp		
	sltu	$1, $3, $fp	
	addu	$3, $1		
	addu	$3, $19		
	sltu	$1, $3, $19	
	addu	$3, $1		
	addu	$3, $2		
	sltu	$1, $3, $2	
	addu	$3, $1		
	.set	pop
 # 0 "" 2
 # 83 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $3, 16	
	addu	$3, $1		
	sltu	$1, $3, $1	
	srl	$3, $3, 16	
	addu	$3, $1		
	xori	$3, 0xffff	
	.set	pop
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$3,52($sp)
	.set	macro
	.set	reorder

	li	$6,20			# 0x14
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,34
	.set	macro
	.set	reorder

	move	$4,$18
	move	$5,$0
	lw	$2,16($sp)
	lbu	$3,14($22)
	and	$23,$2,$23
	li	$2,6			# 0x6
	sb	$2,25($sp)
	li	$2,1073741824			# 0x40000000
	or	$23,$23,$2
	li	$2,-251723776			# 0xf0ff0000
	ori	$2,$2,0xffff
	sh	$0,22($sp)
	and	$23,$23,$2
	li	$2,83886080			# 0x5000000
	or	$23,$23,$2
	sw	$23,16($sp)
	sb	$0,17($sp)
	lw	$2,12($17)
	sw	$2,32($sp)
	lw	$2,16($17)
	sb	$3,24($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sw	$2,28($sp)
	.set	macro
	.set	reorder

	li	$3,40			# 0x28
	lhu	$2,%lo(url_cache)($21)
	addiu	$4,$sp,16
	sh	$3,18($sp)
	addiu	$3,$sp,32
	addiu	$2,$2,1
	sh	$2,20($sp)
	sh	$0,26($sp)
	lw	$7,4($4)
	lw	$6,16($sp)
	lw	$5,8($4)
	srl	$8,$6,22
	addu	$6,$7,$6
	addu	$2,$6,$5
	sltu	$7,$6,$7
	lw	$6,28($sp)
	addu	$7,$2,$7
	andi	$8,$8,0x3c
	sltu	$5,$7,$5
	addu	$4,$4,$8
	addu	$7,$7,$6
	addu	$5,$7,$5
	sltu	$2,$5,$6
	addu	$2,$2,$5
$L180:
	lw	$5,0($3)
	addiu	$3,$3,4
	addu	$2,$2,$5
	sltu	$5,$2,$5
	.set	noreorder
	.set	nomacro
	bne	$3,$4,$L180
	addu	$2,$5,$2
	.set	macro
	.set	reorder

	move	$4,$20
#APP
 # 83 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $2, 16	
	addu	$2, $1		
	sltu	$1, $2, $1	
	srl	$2, $2, 16	
	addu	$2, $1		
	xori	$2, 0xffff	
	.set	pop
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,26($sp)
	.set	macro
	.set	reorder

	addiu	$5,$sp,16
	addiu	$4,$2,14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,20			# 0x14
	.set	macro
	.set	reorder

	lui	$2,%hi(url_cache)
	move	$4,$20
	addiu	$2,$2,%lo(url_cache)
	li	$3,1			# 0x1
	sb	$3,16($2)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$0,15($2)
	.set	macro
	.set	reorder

$L174:
	lw	$31,100($sp)
	move	$2,$0
	lw	$fp,96($sp)
	lw	$23,92($sp)
	lw	$22,88($sp)
	lw	$21,84($sp)
	lw	$20,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,104
	.set	macro
	.set	reorder

$L187:
#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	bne	$2,$0,$L174
	j	$L188
	.end	GenerateTCPFINACK
	.size	GenerateTCPFINACK, .-GenerateTCPFINACK
	.section	.text.unregister_customRspStr,"ax",@progbits
	.align	2
	.globl	unregister_customRspStr
	.set	nomips16
	.ent	unregister_customRspStr
	.type	unregister_customRspStr, @function
unregister_customRspStr:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(block_message)
	lw	$4,%lo(block_message)($16)
	beq	$4,$0,$L191
	nop

	jal	kfree
	nop

	sw	$0,%lo(block_message)($16)
$L191:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	unregister_customRspStr
	.size	unregister_customRspStr, .-unregister_customRspStr
	.section	.text.filter_table_flush,"ax",@progbits
	.align	2
	.globl	filter_table_flush
	.set	nomips16
	.ent	filter_table_flush
	.type	filter_table_flush, @function
filter_table_flush:
	.frame	$sp,48,$31		# vars= 0, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$22,40($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lui	$18,%hi(table_list_head)
	lw	$16,%lo(table_list_head)($18)
	li	$21,1048576			# 0x100000
	li	$20,2097152			# 0x200000
	ori	$21,$21,0x100
	addiu	$18,$18,%lo(table_list_head)
	lw	$17,0($16)
	j	$L193
	ori	$20,$20,0x200

$L196:
	lw	$2,8($16)
	lw	$19,0($2)
	j	$L194
	addiu	$22,$16,8

$L195:
	lw	$3,4($2)
	lw	$5,0($2)
	sw	$3,4($5)
	sw	$5,0($3)
	sw	$21,0($2)
	jal	kfree
	sw	$20,4($2)

	move	$2,$19
	lw	$3,24($16)
	addiu	$3,$3,-1
	sw	$3,24($16)
	lw	$19,0($19)
$L194:
	bne	$2,$22,$L195
	move	$4,$2

	lw	$2,4($16)
	move	$4,$16
	lw	$3,0($16)
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$21,0($16)
	sw	$20,4($16)
	jal	kfree
	move	$16,$17

	lw	$2,24($18)
	addiu	$2,$2,-1
	sw	$2,24($18)
	lw	$17,0($17)
$L193:
	bne	$16,$18,$L196
	lui	$3,%hi(filter_rule_num)

	li	$4,1			# 0x1
	lw	$31,44($sp)
	move	$2,$0
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	sw	$4,%lo(filter_rule_num)($3)
	lui	$3,%hi(log_enable)
	sw	$0,%lo(log_enable)($3)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	filter_table_flush
	.size	filter_table_flush, .-filter_table_flush
	.section	.rodata.str1.4
	.align	2
$LC26:
	.ascii	"HTTP/1.1 401 The web site is blocked by administrator\015"
	.ascii	"\012Server: Embedded HTTP Server 2.00\015\012Connection:"
	.ascii	" close\015\012\015\012<HTML><HEAD><TITLE>401 The web sit"
	.ascii	"e is blocked by administrator</TITLE></HEAD>\012<BODY BG"
	.ascii	"COLOR=\"#ffffff\"><H4>401 The web site is blocked by adm"
	.ascii	"inistrator</H4></BODY></HTML>\012\000"
	.section	.text.GenerateHTTP401,"ax",@progbits
	.align	2
	.globl	GenerateHTTP401
	.set	nomips16
	.ent	GenerateHTTP401
	.type	GenerateHTTP401, @function
GenerateHTTP401:
	.frame	$sp,216,$31		# vars= 160, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-216
	sw	$31,212($sp)
	sw	$fp,208($sp)
	sw	$23,204($sp)
	sw	$22,200($sp)
	sw	$21,196($sp)
	sw	$20,192($sp)
	sw	$19,188($sp)
	sw	$18,184($sp)
	sw	$17,180($sp)
	sw	$16,176($sp)
	move	$18,$4
	lui	$4,%hi($LC21)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC21)
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,164($sp)
	.set	macro
	.set	reorder

	lw	$5,164($sp)
	addiu	$4,$2,12
	lbu	$19,0($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	move	$17,$2
	.set	macro
	.set	reorder

	andi	$19,$19,0xf
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L199
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L217
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	j	$L226
$L227:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L199
$L217:
	move	$4,$18
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L223
	move	$20,$2
	.set	macro
	.set	reorder

	lui	$3,%hi(url_cache)
	move	$4,$2
	addiu	$22,$sp,36
	addiu	$fp,$3,%lo(url_cache)
	sll	$19,$19,2
	addiu	$21,$sp,16
	sw	$3,172($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	addu	$19,$17,$19
	.set	macro
	.set	reorder

	move	$4,$20
	lbu	$5,0($2)
	andi	$5,$5,0xf
	addiu	$5,$5,5
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$5,2
	.set	macro
	.set	reorder

	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$4,$20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	addiu	$4,$sp,160
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_lock_from_hh
	move	$23,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$0
	.set	macro
	.set	reorder

	move	$4,$23
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	sw	$2,164($sp)
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	lw	$5,164($sp)
	addiu	$4,$5,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$2
	.set	macro
	.set	reorder

	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$23
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$4,$0
	.set	macro
	.set	reorder

	move	$4,$18
	lhu	$2,2($19)
	sh	$2,36($sp)
	lhu	$2,0($19)
	sh	$2,38($sp)
	lw	$2,8($19)
	lw	$23,4($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,40($sp)
	.set	macro
	.set	reorder

	li	$6,-4			# 0xfffffffc
	move	$4,$20
	lw	$7,12($19)
	li	$19,268369920			# 0xfff0000
	lbu	$5,0($17)
	ori	$19,$19,0xffff
	srl	$7,$7,28
	andi	$5,$5,0xf
	addu	$7,$7,$5
	lw	$5,48($sp)
	mult	$7,$6
	li	$6,1342177280			# 0x50000000
	and	$5,$5,$19
	or	$5,$5,$6
	lhu	$6,12($fp)
	sw	$5,48($sp)
	li	$5,-327680			# 0xfffb0000
	sb	$0,13($22)
	ori	$5,$5,0xffff
	lw	$7,48($sp)
	sh	$0,54($sp)
	and	$5,$7,$5
	mflo	$7
	addu	$23,$7,$23
	li	$7,1048576			# 0x100000
	addu	$23,$23,$2
	li	$2,-589824			# 0xfff70000
	or	$5,$5,$7
	ori	$2,$2,0xffff
	sw	$23,44($sp)
	and	$5,$5,$2
	sw	$5,48($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$6,50($sp)
	.set	macro
	.set	reorder

	move	$6,$0
	move	$4,$22
	sh	$0,52($sp)
	addiu	$23,$2,-34
	lw	$8,16($17)
	move	$5,$23
	lw	$7,12($17)
	andi	$23,$23,0xffff
	sw	$8,164($sp)
	addiu	$23,$23,6
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	sw	$7,168($sp)
	.set	macro
	.set	reorder

	move	$4,$20
	lw	$7,168($sp)
	lw	$8,164($sp)
#APP
 # 141 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$8, $7		
	sltu	$1, $8, $7	
	addu	$8, $1		
	addu	$8, $23		
	sltu	$1, $8, $23	
	addu	$8, $1		
	addu	$8, $2		
	sltu	$1, $8, $2	
	addu	$8, $1		
	.set	pop
 # 0 "" 2
 # 83 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $8, 16	
	addu	$8, $1		
	sltu	$1, $8, $1	
	srl	$8, $8, 16	
	addu	$8, $1		
	xori	$8, 0xffff	
	.set	pop
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$8,52($sp)
	.set	macro
	.set	reorder

	move	$5,$22
	li	$6,20			# 0x14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,34
	.set	macro
	.set	reorder

	li	$7,40			# 0x28
	move	$4,$20
	lw	$2,16($sp)
	lw	$3,172($sp)
	and	$19,$2,$19
	li	$2,6			# 0x6
	sb	$2,25($sp)
	li	$2,1073741824			# 0x40000000
	or	$19,$19,$2
	li	$2,-251723776			# 0xf0ff0000
	ori	$2,$2,0xffff
	sh	$0,22($sp)
	and	$19,$19,$2
	li	$2,83886080			# 0x5000000
	or	$19,$19,$2
	lbu	$2,14($fp)
	sw	$19,16($sp)
	sb	$0,17($sp)
	lw	$5,12($17)
	lhu	$6,%lo(url_cache)($3)
	sw	$5,32($sp)
	addiu	$6,$6,1
	lw	$3,16($17)
	sh	$7,18($sp)
	sb	$2,24($sp)
	sh	$0,26($sp)
	sh	$6,20($sp)
	lw	$7,4($21)
	lw	$8,16($sp)
	lw	$6,8($21)
	addu	$8,$7,$8
	sltu	$7,$8,$7
	addu	$8,$8,$6
	addu	$7,$8,$7
	sltu	$6,$7,$6
	addu	$7,$7,$3
	addu	$6,$7,$6
	sltu	$7,$6,$3
	addu	$2,$6,$5
	addu	$2,$2,$7
	sltu	$5,$2,$5
	sw	$3,28($sp)
	addu	$2,$5,$2
#APP
 # 83 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $2, 16	
	addu	$2, $1		
	sltu	$1, $2, $1	
	srl	$2, $2, 16	
	addu	$2, $1		
	xori	$2, 0xffff	
	.set	pop
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,26($sp)
	.set	macro
	.set	reorder

	move	$5,$21
	li	$6,20			# 0x14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,14
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$20
	.set	macro
	.set	reorder

	li	$6,54			# 0x36
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,56
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	move	$4,$20
	.set	macro
	.set	reorder

	lui	$2,%hi(block_message)
	lw	$4,%lo(block_message)($2)
	bne	$4,$0,$L218
	lui	$4,%hi($LC26)
	addiu	$4,$4,%lo($LC26)
$L218:
	jal	strlen
	move	$4,$18
	li	$5,32			# 0x20
	move	$6,$2
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L206
	move	$19,$2
	.set	macro
	.set	reorder

$L223:
#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	bne	$2,$0,$L207
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

$L207:
	lui	$4,%hi($LC25)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC25)
	.set	macro
	.set	reorder

	j	$L199
$L206:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$19
	move	$17,$2
	lbu	$2,0($2)
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$17,$22
	lw	$3,12($22)
	srl	$3,$3,28
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$2,2
	.set	macro
	.set	reorder

	lui	$2,%hi(block_message)
	lw	$4,%lo(block_message)($2)
	bne	$4,$0,$L219
	lui	$4,%hi($LC26)
	addiu	$4,$4,%lo($LC26)
$L219:
	jal	strlen
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	addiu	$4,$sp,160
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_lock_from_hh
	move	$20,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	move	$21,$2
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	addiu	$4,$21,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$2
	.set	macro
	.set	reorder

	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$20
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L228
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	move	$5,$0
$L228:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$4,$19
	.set	macro
	.set	reorder

	lbu	$16,0($17)
	lw	$3,12($17)
	lw	$2,16($17)
	andi	$16,$16,0xf
	sll	$16,$16,2
	sw	$3,16($17)
	sw	$2,12($17)
	addu	$16,$17,$16
	lhu	$2,0($16)
	lhu	$3,2($16)
	lw	$21,12($16)
	sh	$2,2($16)
	lui	$2,%hi(block_message)
	sh	$3,0($16)
	srl	$21,$21,28
	lw	$5,%lo(block_message)($2)
	sll	$21,$21,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L211
	addu	$21,$16,$21
	.set	macro
	.set	reorder

	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$5,164($sp)
	.set	macro
	.set	reorder

	move	$4,$21
	lw	$5,164($sp)
	.set	noreorder
	.set	nomacro
	j	$L220
	addiu	$6,$2,1
	.set	macro
	.set	reorder

$L211:
	lui	$20,%hi($LC26)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$20,%lo($LC26)
	.set	macro
	.set	reorder

	move	$4,$21
	addiu	$5,$20,%lo($LC26)
	move	$6,$2
$L220:
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$fp,-4			# 0xfffffffc
	.set	macro
	.set	reorder

	move	$4,$18
	lw	$2,8($16)
	lui	$18,%hi(url_cache)
	lw	$21,4($16)
	addiu	$20,$18,%lo(url_cache)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,4($16)
	.set	macro
	.set	reorder

	move	$4,$19
	lw	$5,12($22)
	lbu	$3,0($17)
	srl	$5,$5,28
	andi	$3,$3,0xf
	addu	$3,$5,$3
	mult	$3,$fp
	mflo	$3
	addu	$21,$3,$21
	addu	$21,$21,$2
	sw	$21,8($16)
	lhu	$2,12($20)
	sb	$0,13($16)
	sh	$2,14($16)
	li	$2,-327680			# 0xfffb0000
	lw	$3,12($16)
	ori	$2,$2,0xffff
	sh	$0,18($16)
	and	$2,$3,$2
	li	$3,1048576			# 0x100000
	or	$2,$2,$3
	li	$3,524288			# 0x80000
	or	$2,$2,$3
	li	$3,-131072			# 0xfffe0000
	ori	$3,$3,0xffff
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,12($16)
	.set	macro
	.set	reorder

	move	$6,$0
	move	$4,$16
	lbu	$21,0($17)
	sh	$0,16($16)
	andi	$21,$21,0xf
	lw	$23,16($17)
	mult	$21,$fp
	lw	$22,12($17)
	mflo	$21
	addiu	$21,$21,-14
	addu	$21,$21,$2
	move	$5,$21
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	andi	$21,$21,0xffff
	.set	macro
	.set	reorder

	addiu	$21,$21,6
	move	$4,$19
#APP
 # 141 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$23, $22		
	sltu	$1, $23, $22	
	addu	$23, $1		
	addu	$23, $21		
	sltu	$1, $23, $21	
	addu	$23, $1		
	addu	$23, $2		
	sltu	$1, $23, $2	
	addu	$23, $1		
	.set	pop
 # 0 "" 2
 # 83 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $23, 16	
	addu	$23, $1		
	sltu	$1, $23, $1	
	srl	$23, $23, 16	
	addu	$23, $1		
	xori	$23, 0xffff	
	.set	pop
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$23,16($16)
	.set	macro
	.set	reorder

	addiu	$3,$17,16
	addiu	$2,$2,-14
	sh	$2,2($17)
	lbu	$2,14($20)
	sh	$0,6($17)
	sb	$2,8($17)
	lhu	$2,%lo(url_cache)($18)
	lw	$5,0($17)
	addiu	$2,$2,2
	sh	$2,4($17)
	srl	$7,$5,22
	lw	$6,4($17)
	andi	$7,$7,0x3c
	sh	$0,10($17)
	addu	$7,$17,$7
	lw	$4,8($17)
	addu	$5,$6,$5
	sltu	$6,$5,$6
	addu	$2,$5,$4
	lw	$5,12($17)
	addu	$6,$2,$6
	sltu	$4,$6,$4
	addu	$6,$6,$5
	addu	$4,$6,$4
	sltu	$2,$4,$5
	addu	$2,$2,$4
$L213:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$2,$4
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$3,$7,$L213
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	move	$4,$19
#APP
 # 83 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $2, 16	
	addu	$2, $1		
	sltu	$1, $2, $1	
	srl	$2, $2, 16	
	addu	$2, $1		
	xori	$2, 0xffff	
	.set	pop
 # 0 "" 2
#NO_APP
	sh	$2,10($17)
	lui	$2,%hi(url_cache+15)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sb	$3,%lo(url_cache+15)($2)
	.set	macro
	.set	reorder

	addiu	$4,$sp,56
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,54			# 0x36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	move	$4,$19
	.set	macro
	.set	reorder

$L199:
	lw	$31,212($sp)
	move	$2,$0
	lw	$fp,208($sp)
	lw	$23,204($sp)
	lw	$22,200($sp)
	lw	$21,196($sp)
	lw	$20,192($sp)
	lw	$19,188($sp)
	lw	$18,184($sp)
	lw	$17,180($sp)
	lw	$16,176($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,216
	.set	macro
	.set	reorder

$L226:
#APP
 # 167 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 169 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 185 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
	bne	$2,$0,$L199
	j	$L227
	.end	GenerateHTTP401
	.size	GenerateHTTP401, .-GenerateHTTP401
	.section	.rodata.str1.4
	.align	2
$LC27:
	.ascii	"%s\000"
	.section	.text.register_customRspStr,"ax",@progbits
	.align	2
	.globl	register_customRspStr
	.set	nomips16
	.ent	register_customRspStr
	.type	register_customRspStr, @function
register_customRspStr:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$16,$4
	jal	unregister_customRspStr
	lui	$17,%hi(block_message)

	beq	$16,$0,$L230
	nop

$L231 = .
	jal	strlen
	move	$4,$16

	li	$5,208			# 0xd0
	jal	__kmalloc
	addiu	$4,$2,1

	beq	$2,$0,$L233
	sw	$2,%lo(block_message)($17)

	lui	$5,%hi($LC27)
	move	$4,$2
	addiu	$5,$5,%lo($LC27)
	j	$L234
	move	$6,$16

$L230:
	lui	$16,%hi($LC26)
	jal	strlen
	addiu	$4,$16,%lo($LC26)

	li	$5,208			# 0xd0
	jal	__kmalloc
	move	$4,$2

	beq	$2,$0,$L233
	sw	$2,%lo(block_message)($17)

	lui	$5,%hi($LC27)
	move	$4,$2
	addiu	$6,$16,%lo($LC26)
	addiu	$5,$5,%lo($LC27)
$L234:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	sprintf
	addiu	$sp,$sp,32

$L233:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	register_customRspStr
	.size	register_customRspStr, .-register_customRspStr
	.section	.text.filter_table_regist,"ax",@progbits
	.align	2
	.globl	filter_table_regist
	.set	nomips16
	.ent	filter_table_regist
	.type	filter_table_regist, @function
filter_table_regist:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$2,%hi(table_list_head+24)
	move	$17,$4
	lw	$2,%lo(table_list_head+24)($2)
	bne	$2,$0,$L237
	move	$16,$5

	jal	filter_table_head_init
	nop

$L236 = .
$L237:
	lui	$2,%hi(malloc_sizes+4)
	lw	$4,%lo(malloc_sizes+4)($2)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	lui	$5,%hi(table_list_head)
	sw	$17,16($2)
	move	$3,$2
	addiu	$6,$2,8
	li	$2,1			# 0x1
	addiu	$4,$5,%lo(table_list_head)
	sw	$2,24($3)
	move	$2,$0
	sw	$3,0($3)
	sw	$3,4($3)
	sw	$16,28($3)
	sw	$6,12($3)
	sw	$0,20($3)
	sw	$6,8($3)
	lw	$6,%lo(table_list_head)($5)
	sw	$3,4($6)
	sw	$6,0($3)
	sw	$4,4($3)
	lw	$6,24($4)
	sw	$3,%lo(table_list_head)($5)
	addiu	$3,$6,1
	sw	$3,24($4)
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	filter_table_regist
	.size	filter_table_regist, .-filter_table_regist
	.section	.text.strtok,"ax",@progbits
	.align	2
	.globl	strtok
	.set	nomips16
	.ent	strtok
	.type	strtok, @function
strtok:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$17,$5
	bne	$4,$0,$L241
	move	$16,$4

	lui	$2,%hi(___strtok)
	lw	$16,%lo(___strtok)($2)
	beq	$16,$0,$L246
	move	$2,$16

$L241:
	move	$4,$16
	jal	strspn
	move	$5,$17

	addu	$16,$16,$2
	lb	$2,0($16)
	bne	$2,$0,$L243
	lui	$2,%hi(___strtok)

	move	$16,$0
	j	$L242
	sw	$0,%lo(___strtok)($2)

$L243:
	move	$5,$17
	jal	strpbrk
	move	$4,$16

	beq	$2,$0,$L247
	lui	$3,%hi(___strtok)

	lb	$3,0($2)
	beq	$3,$0,$L247
	lui	$3,%hi(___strtok)

	sb	$0,0($2)
	addiu	$2,$2,1
$L247:
	sw	$2,%lo(___strtok)($3)
$L242:
	move	$2,$16
$L246:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	strtok
	.size	strtok, .-strtok
	.section	.rodata.str1.4
	.align	2
$LC28:
	.ascii	" \000"
	.section	.text.dos_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	dos_write_proc
	.type	dos_write_proc, @function
dos_write_proc:
	.frame	$sp,136,$31		# vars= 80, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	sw	$fp,128($sp)
	sw	$23,124($sp)
	sw	$22,120($sp)
	sw	$21,116($sp)
	sw	$20,112($sp)
	sw	$19,108($sp)
	sw	$18,104($sp)
	sw	$17,100($sp)
	sw	$16,96($sp)
	sltu	$3,$6,2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L249
	move	$16,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L267
	li	$2,-14			# 0xfffffff2
	.set	macro
	.set	reorder

	addiu	$4,$5,80
	lw	$3,24($28)
	or	$2,$4,$5
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L249
	lui	$2,%hi(dos_flag)
	.set	macro
	.set	reorder

	li	$6,80			# 0x50
	addiu	$4,$2,%lo(dos_flag)
#APP
 # 2866 "net/rtl/fastpath/filter.c" 1
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L249
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	addiu	$2,$2,%lo(dos_flag)
	li	$5,80			# 0x50
	move	$3,$4
#APP
 # 50 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($2)
	subu	$5,1
	sb	$1,($3)
	beqz	$1,2f
	addiu	$3,1
	bnez	$5,1b
	addiu	$2,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	li	$18,1			# 0x1
	lui	$fp,%hi(per_fin_threshold)
	lui	$23,%hi(per_syn_threshold)
	lui	$22,%hi(whole_icmp_threshold)
	lui	$21,%hi(whole_udp_threshold)
	lui	$20,%hi(whole_fin_threshold)
	.set	noreorder
	.set	nomacro
	j	$L250
	lui	$19,%hi(whole_syn_threshold)
	.set	macro
	.set	reorder

$L266:
	move	$4,$17
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	move	$6,$0
	.set	macro
	.set	reorder

	addiu	$3,$18,-1
	andi	$3,$3,0x00ff
	sltu	$4,$3,13
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L251
	lui	$4,%hi($L265)
	.set	macro
	.set	reorder

	sll	$3,$3,2
	addiu	$4,$4,%lo($L265)
	addu	$3,$4,$3
	lw	$3,0($3)
	j	$3
	.rdata
	.align	2
	.align	2
$L265:
	.word	$L252
	.word	$L253
	.word	$L254
	.word	$L255
	.word	$L256
	.word	$L257
	.word	$L258
	.word	$L259
	.word	$L260
	.word	$L261
	.word	$L262
	.word	$L263
	.word	$L264
	.section	.text.dos_write_proc
$L252:
	lui	$3,%hi(op_mode)
	.set	noreorder
	.set	nomacro
	j	$L251
	sb	$2,%lo(op_mode)($3)
	.set	macro
	.set	reorder

$L253:
	move	$4,$17
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lui	$4,%hi(lan_addr)
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(lan_addr)($4)
	.set	macro
	.set	reorder

$L254:
	move	$4,$17
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lui	$3,%hi(lan_mask)
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(lan_mask)($3)
	.set	macro
	.set	reorder

$L255:
	lui	$4,%hi(item)
	sw	$2,%lo(item)($4)
	lui	$2,%hi(item_backup)
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$0,%lo(item_backup)($2)
	.set	macro
	.set	reorder

$L256:
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(whole_syn_threshold)($19)
	.set	macro
	.set	reorder

$L257:
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(whole_fin_threshold)($20)
	.set	macro
	.set	reorder

$L258:
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(whole_udp_threshold)($21)
	.set	macro
	.set	reorder

$L259:
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(whole_icmp_threshold)($22)
	.set	macro
	.set	reorder

$L260:
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(per_syn_threshold)($23)
	.set	macro
	.set	reorder

$L261:
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(per_fin_threshold)($fp)
	.set	macro
	.set	reorder

$L262:
	lui	$3,%hi(per_udp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(per_udp_threshold)($3)
	.set	macro
	.set	reorder

$L263:
	lui	$4,%hi(per_icmp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L251
	sw	$2,%lo(per_icmp_threshold)($4)
	.set	macro
	.set	reorder

$L264:
	lui	$3,%hi(block_time)
	sw	$2,%lo(block_time)($3)
$L251:
	addiu	$18,$18,1
	move	$4,$0
	andi	$18,$18,0x00ff
$L250:
	lui	$5,%hi($LC28)
	.set	noreorder
	.set	nomacro
	jal	strtok
	addiu	$5,$5,%lo($LC28)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L266
	move	$17,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L267
	move	$2,$16
	.set	macro
	.set	reorder

$L249:
	li	$2,-14			# 0xfffffff2
$L267:
	lw	$31,132($sp)
	lw	$fp,128($sp)
	lw	$23,124($sp)
	lw	$22,120($sp)
	lw	$21,116($sp)
	lw	$20,112($sp)
	lw	$19,108($sp)
	lw	$18,104($sp)
	lw	$17,100($sp)
	lw	$16,96($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,136
	.set	macro
	.set	reorder

	.end	dos_write_proc
	.size	dos_write_proc, .-dos_write_proc
	.section	.rodata.str1.4
	.align	2
$LC29:
	.ascii	"type=%d num=%d\012\000"
	.align	2
$LC30:
	.ascii	"index=%d\011\000"
	.align	2
$LC31:
	.ascii	"mac=%02x:%02x:%02x:%02x:%02x:%02x\012\000"
	.align	2
$LC32:
	.ascii	"index=%d\011\011\000"
	.align	2
$LC33:
	.ascii	"url=%s\012\000"
	.align	2
$LC34:
	.ascii	"start=%x\011\000"
	.align	2
$LC35:
	.ascii	"end=%x\011\000"
	.align	2
$LC36:
	.ascii	"day=%x\012\000"
	.align	2
$LC37:
	.ascii	"unknow table type\012\000"
	.section	.text.filter_table_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	filter_table_read_proc
	.type	filter_table_read_proc, @function
filter_table_read_proc:
	.frame	$sp,80,$31		# vars= 8, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L270 = .
$L271 = .
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$fp,72($sp)
	sw	$23,68($sp)
	sw	$22,64($sp)
	sw	$21,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$16,%hi(malloc_sizes)
	move	$22,$4
	sw	$5,84($sp)
	addiu	$16,$16,%lo(malloc_sizes)
	move	$21,$6
	lw	$4,52($16)
	li	$5,208			# 0xd0
	move	$20,$7
	jal	kmem_cache_alloc
	move	$18,$0

	beq	$2,$0,$L273
	move	$17,$2

$L272 = .
$L274 = .
	lw	$4,44($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	move	$4,$17
	beq	$2,$0,$L292
	move	$16,$2

	lui	$2,%hi(table_list_head)
	lui	$fp,%hi($LC32)
	lui	$23,%hi($LC30)
	addiu	$fp,$fp,%lo($LC32)
	lw	$18,%lo(table_list_head)($2)
	j	$L276
	addiu	$23,$23,%lo($LC30)

$L288:
	lw	$7,24($18)
	move	$4,$17
	lw	$6,16($18)
	addiu	$5,$5,%lo($LC29)
	jal	sprintf
	addiu	$7,$7,-1

	li	$3,3			# 0x3
	lw	$2,16($18)
	beq	$2,$3,$L279
	nop

	li	$3,4			# 0x4
	beq	$2,$3,$L280
	nop

	li	$3,2			# 0x2
	bne	$2,$3,$L291
	move	$4,$16

	lw	$19,8($18)
	j	$L281
	addiu	$3,$18,8

$L282:
	lw	$6,20($19)
	move	$5,$23
	jal	sprintf
	sw	$3,36($sp)

	move	$4,$17
	jal	strcat
	move	$5,$16

	lui	$5,%hi($LC31)
	move	$4,$16
	lbu	$2,30($19)
	addiu	$5,$5,%lo($LC31)
	lbu	$6,28($19)
	lbu	$7,29($19)
	sw	$2,16($sp)
	lbu	$2,31($19)
	sw	$2,20($sp)
	lbu	$2,32($19)
	sw	$2,24($sp)
	lbu	$2,33($19)
	jal	sprintf
	sw	$2,28($sp)

	move	$4,$17
	jal	strcat
	move	$5,$16

	lw	$19,0($19)
	lw	$3,36($sp)
$L281:
	bne	$19,$3,$L282
	move	$4,$16

	j	$L283
	nop

$L279:
	lw	$19,8($18)
	j	$L284
	addiu	$3,$18,8

$L285:
	lw	$6,20($19)
	move	$5,$fp
	jal	sprintf
	sw	$3,36($sp)

	move	$4,$17
	jal	strcat
	move	$5,$16

	lui	$5,%hi($LC33)
	move	$4,$16
	addiu	$6,$19,28
	jal	sprintf
	addiu	$5,$5,%lo($LC33)

	move	$4,$17
	jal	strcat
	move	$5,$16

	lw	$19,0($19)
	lw	$3,36($sp)
$L284:
	bne	$19,$3,$L285
	move	$4,$16

	j	$L283
	nop

$L280:
	lw	$19,8($18)
	j	$L286
	addiu	$3,$18,8

$L287:
	lw	$6,20($19)
	move	$4,$16
	move	$5,$23
	sw	$3,36($sp)
	jal	sprintf
	sw	$2,32($sp)

	move	$4,$17
	jal	strcat
	move	$5,$16

	lui	$5,%hi($LC34)
	move	$4,$16
	lw	$6,28($19)
	jal	sprintf
	addiu	$5,$5,%lo($LC34)

	move	$4,$17
	jal	strcat
	move	$5,$16

	lui	$5,%hi($LC35)
	move	$4,$16
	lw	$2,32($sp)
	lw	$6,4($2)
	jal	sprintf
	addiu	$5,$5,%lo($LC35)

	move	$4,$17
	jal	strcat
	move	$5,$16

	lui	$5,%hi($LC36)
	move	$4,$16
	lw	$2,32($sp)
	lw	$6,8($2)
	jal	sprintf
	addiu	$5,$5,%lo($LC36)

	move	$4,$17
	jal	strcat
	move	$5,$16

	lw	$19,0($19)
	lw	$3,36($sp)
$L286:
	bne	$19,$3,$L287
	addiu	$2,$19,28

	j	$L283
	nop

$L291:
	lui	$5,%hi($LC37)
	jal	sprintf
	addiu	$5,$5,%lo($LC37)

	move	$4,$17
	jal	strcat
	move	$5,$16

$L283:
	lw	$18,0($18)
$L276:
	lui	$3,%hi(table_list_head)
	addiu	$3,$3,%lo(table_list_head)
	bne	$18,$3,$L288
	lui	$5,%hi($LC29)

	lui	$5,%hi($LC20)
	move	$4,$22
	addiu	$5,$5,%lo($LC20)
	jal	sprintf
	move	$6,$17

	addu	$3,$20,$21
	slt	$3,$3,$2
	bne	$3,$0,$L289
	li	$4,1			# 0x1

	lw	$3,96($sp)
	sw	$4,0($3)
$L289:
	subu	$2,$2,$21
	addu	$21,$22,$21
	move	$4,$17
	slt	$18,$20,$2
	lw	$3,84($sp)
	movz	$20,$2,$18
	slt	$18,$20,0
	sw	$21,0($3)
	jal	kfree
	movn	$20,$0,$18

	move	$18,$20
	move	$4,$16
$L292:
	jal	kfree
	nop

$L273:
	move	$2,$18
	lw	$31,76($sp)
	lw	$fp,72($sp)
	lw	$23,68($sp)
	lw	$22,64($sp)
	lw	$21,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	j	$31
	addiu	$sp,$sp,80

	.set	macro
	.set	reorder
	.end	filter_table_read_proc
	.size	filter_table_read_proc, .-filter_table_read_proc
	.section	.text.scan_table,"ax",@progbits
	.align	2
	.globl	scan_table
	.set	nomips16
	.ent	scan_table
	.type	scan_table, @function
scan_table:
	.frame	$sp,48,$31		# vars= 0, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$22,40($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	move	$17,$6
	move	$19,$4
	move	$22,$5
	beq	$6,$0,$L294
	move	$20,$7

	lw	$2,28($6)
	beq	$2,$0,$L294
	addiu	$21,$6,8

	lw	$16,8($6)
	j	$L295
	move	$18,$0

$L303:
	lw	$2,16($16)
	andi	$3,$2,0x1
	bne	$3,$0,$L296
	move	$18,$16

	lw	$3,20($16)
	bne	$22,$3,$L297
	nop

$L296:
	andi	$3,$2,0x200
	beq	$3,$0,$L298
	nop

	andi	$2,$2,0x2
	lw	$6,0($20)
	beq	$2,$0,$L297
	nop

	beq	$6,$0,$L294
	move	$4,$19

	lw	$5,20($16)
	jal	scan_table
	move	$7,$6

	bne	$2,$0,$L302
	li	$2,1			# 0x1

	j	$L297
	nop

$L298:
	andi	$2,$2,0x100
	bne	$2,$0,$L300
	move	$4,$19

	lw	$2,28($17)
	jalr	$2
	addiu	$5,$16,28

	beq	$2,$0,$L297
	nop

$L300:
	lw	$2,16($16)
	andi	$3,$2,0x2
	beq	$3,$0,$L310
	andi	$3,$2,0x1

	lw	$6,0($20)
	beq	$6,$0,$L294
	move	$4,$19

	lw	$5,20($16)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L297
	nop

	lw	$2,16($16)
	andi	$3,$2,0x1
$L310:
	beq	$3,$0,$L299
	srl	$2,$2,3

	j	$L306
	xori	$2,$2,0x1

$L297:
	lw	$16,0($16)
$L295:
	bne	$16,$21,$L303
	nop

	beq	$18,$0,$L294
	nop

	lw	$3,16($18)
	andi	$4,$3,0x1
	beq	$4,$0,$L302
	move	$2,$0

	srl	$2,$3,3
$L306:
	j	$L302
	andi	$2,$2,0x1

$L294:
	j	$L302
	li	$2,-1			# 0xffffffff

$L299:
	li	$2,1			# 0x1
$L302:
	lw	$31,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	scan_table
	.size	scan_table, .-scan_table
	.section	.text.filter_item_regist,"ax",@progbits
	.align	2
	.globl	filter_item_regist
	.set	nomips16
	.ent	filter_item_regist
	.type	filter_item_regist, @function
filter_item_regist:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(table_list_head)
	lw	$3,%lo(table_list_head)($2)
	j	$L312
	addiu	$6,$2,%lo(table_list_head)

$L315:
	lw	$7,16($3)
	bne	$7,$4,$L313
	nop

	lw	$4,8($3)
	addiu	$6,$3,8
	move	$2,$0
	sw	$5,4($4)
	sw	$4,0($5)
	sw	$6,4($5)
	lw	$4,24($3)
	sw	$5,8($3)
	addiu	$4,$4,1
	j	$31
	sw	$4,24($3)

$L313:
	move	$3,$2
$L312:
	lw	$2,0($3)
	bne	$3,$6,$L315
	nop

	j	$31
	li	$2,-1			# 0xffffffff

	.set	macro
	.set	reorder
	.end	filter_item_regist
	.size	filter_item_regist, .-filter_item_regist
	.section	.rodata.str1.4
	.align	2
$LC38:
	.ascii	"init\000"
	.align	2
$LC39:
	.ascii	"flush\000"
	.align	2
$LC40:
	.ascii	"enable_log\000"
	.align	2
$LC41:
	.ascii	"add:\000"
	.align	2
$LC42:
	.ascii	":\000"
	.align	2
$LC43:
	.ascii	";\000"
	.align	2
$LC44:
	.ascii	"#\000"
	.align	2
$LC45:
	.ascii	"-\000"
	.section	.text.filter_table_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	filter_table_write_proc
	.type	filter_table_write_proc, @function
filter_table_write_proc:
	.frame	$sp,96,$31		# vars= 40, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
$L318 = .
$L319 = .
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$fp,88($sp)
	sw	$23,84($sp)
	sw	$22,80($sp)
	sw	$21,76($sp)
	sw	$20,72($sp)
	sw	$19,68($sp)
	sw	$18,64($sp)
	sw	$17,60($sp)
	sw	$16,56($sp)
	lui	$2,%hi(malloc_sizes+60)
	move	$19,$5
	lw	$4,%lo(malloc_sizes+60)($2)
	li	$5,208			# 0xd0
	move	$17,$6
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$18,-14			# 0xfffffff2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L321
	move	$16,$2
	.set	macro
	.set	reorder

	lui	$20,%hi(get_info_from_usr_space)
	addiu	$4,$20,%lo(get_info_from_usr_space)
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$2,28($sp)
	.set	macro
	.set	reorder

	addiu	$4,$20,%lo(get_info_from_usr_space)
	move	$6,$2
	.set	noreorder
	.set	nomacro
	jal	memset
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L322
	addu	$2,$19,$17
	.set	macro
	.set	reorder

	move	$18,$17
	or	$2,$2,$19
	lw	$3,24($28)
	or	$2,$2,$17
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L324
	move	$6,$17
	.set	macro
	.set	reorder

	addiu	$4,$20,%lo(get_info_from_usr_space)
	move	$5,$19
#APP
 # 656 "net/rtl/fastpath/filter_v2.c" 1
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
$L324:
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L380
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L325
	lui	$3,%hi(get_info_from_usr_space)
	.set	macro
	.set	reorder

	move	$2,$16
	addiu	$3,$3,%lo(get_info_from_usr_space)
#APP
 # 50 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$17,1
	sb	$1,($2)
	beqz	$1,2f
	addiu	$2,1
	bnez	$17,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
$L325:
	lui	$4,%hi(get_info_from_usr_space)
	lui	$17,%hi($LC38)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$4,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

	addu	$2,$16,$2
	sb	$0,-1($2)
	lw	$19,28($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$17,%lo($LC38)
	.set	macro
	.set	reorder

	addiu	$5,$17,%lo($LC38)
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L326
	lui	$17,%hi($LC39)
	.set	macro
	.set	reorder

	lui	$20,%hi($LC28)
	addiu	$17,$sp,28
	addiu	$5,$20,%lo($LC28)
	move	$4,$17
	lui	$19,%hi(filter_tlb)
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$20,$5
	.set	macro
	.set	reorder

	addiu	$19,$19,%lo(filter_tlb)
	sw	$2,16($sp)
	move	$5,$20
$L381:
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	sll	$3,$2,3
	addu	$3,$19,$3
	lw	$5,4($3)
	.set	noreorder
	.set	nomacro
	jal	filter_table_regist
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$2,28($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L381
	move	$5,$20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L379
	move	$4,$16
	.set	macro
	.set	reorder

$L326:
	lw	$19,28($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$17,%lo($LC39)
	.set	macro
	.set	reorder

	addiu	$5,$17,%lo($LC39)
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L329
	lui	$17,%hi($LC40)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	filter_table_flush
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L379
	move	$4,$16
	.set	macro
	.set	reorder

$L329:
	lw	$19,28($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$17,%lo($LC40)
	.set	macro
	.set	reorder

	addiu	$5,$17,%lo($LC40)
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L330
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(log_enable)
	.set	noreorder
	.set	nomacro
	j	$L328
	sw	$3,%lo(log_enable)($2)
	.set	macro
	.set	reorder

$L330:
	lui	$17,%hi($LC41)
	lw	$19,28($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$17,%lo($LC41)
	.set	macro
	.set	reorder

	addiu	$5,$17,%lo($LC41)
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L379
	move	$4,$16
	.set	macro
	.set	reorder

	addiu	$17,$sp,28
	lui	$5,%hi($LC42)
	addiu	$5,$5,%lo($LC42)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	strsep
	lui	$21,%hi(table_list_head)
	.set	macro
	.set	reorder

	lui	$19,%hi($LC28)
	lui	$fp,%hi(malloc_sizes)
	addiu	$19,$19,%lo($LC28)
	sw	$2,20($sp)
	addiu	$fp,$fp,%lo(malloc_sizes)
	lui	$20,%hi(filter_rule_num)
	addiu	$23,$21,%lo(table_list_head)
	lui	$5,%hi($LC43)
$L384:
	addiu	$4,$sp,28
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC43)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L328
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	move	$4,$2
	li	$6,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$5,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L382
	lui	$5,%hi($LC44)
	.set	macro
	.set	reorder

	lw	$4,20($sp)
	jal	strlen
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L379
	move	$4,$16
	.set	macro
	.set	reorder

$L331:
	lui	$5,%hi($LC44)
$L382:
	addiu	$4,$sp,20
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC44)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L328
	addiu	$22,$sp,32
	.set	macro
	.set	reorder

	lui	$5,%hi($LC44)
$L383:
	addiu	$4,$sp,20
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC44)
	.set	macro
	.set	reorder

	move	$5,$19
	addiu	$4,$sp,16
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	addiu	$4,$sp,16
	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$17,$2
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	sw	$2,40($sp)
	li	$2,2			# 0x2
	.set	noreorder
	.set	nomacro
	beq	$17,$2,$L343
	slt	$2,$17,3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L337
	li	$2,3			# 0x3
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	bne	$17,$2,$L332
	j	$L371
$L337:
	.set	noreorder
	.set	nomacro
	beq	$17,$2,$L369
	li	$2,4			# 0x4
	.set	macro
	.set	reorder

	bne	$17,$2,$L332
	j	$L370
$L371:
	lw	$4,20($fp)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L328
	move	$17,$2
	.set	macro
	.set	reorder

	addiu	$2,$2,8
	move	$5,$19
	addiu	$4,$sp,16
	sw	$2,12($17)
	sw	$17,0($17)
	sw	$17,4($17)
	sw	$2,8($17)
	sw	$0,24($17)
	lw	$3,40($sp)
	lw	$2,%lo(filter_rule_num)($20)
	sw	$3,16($17)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$2,20($17)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	li	$6,4			# 0x4
	move	$5,$22
	addiu	$4,$17,28
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	addiu	$4,$17,32
	move	$5,$22
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$2,%lo(table_list_head)($21)
	j	$L339
$L342:
	lw	$4,16($2)
	.set	noreorder
	.set	nomacro
	bne	$4,$5,$L340
	addiu	$4,$2,8
	.set	macro
	.set	reorder

	lw	$3,8($2)
	sw	$17,4($3)
	sw	$3,0($17)
	sw	$4,4($17)
	lw	$3,24($2)
	sw	$17,8($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L341
	sw	$3,24($2)
	.set	macro
	.set	reorder

$L340:
	move	$2,$3
$L339:
	lw	$3,0($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$23,$L342
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

$L341:
	lw	$2,16($sp)
	bne	$2,$0,$L371
	j	$L332
$L334 = .
$L343:
	lw	$4,20($fp)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L328
	move	$17,$2
	.set	macro
	.set	reorder

	addiu	$2,$2,8
	addiu	$4,$sp,16
	move	$5,$19
	sw	$2,12($17)
	sw	$17,0($17)
	sw	$17,4($17)
	sw	$2,8($17)
	sw	$0,24($17)
	lw	$2,%lo(filter_rule_num)($20)
	lw	$6,40($sp)
	sw	$2,20($17)
	sw	$6,16($17)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$17,48($sp)
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	sw	$2,24($sp)
$L345:
	lui	$5,%hi($LC45)
	addiu	$4,$sp,24
	addiu	$5,$5,%lo($LC45)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$3,48($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	sb	$2,28($3)
	lw	$2,24($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L345
	addiu	$3,$3,1
	.set	macro
	.set	reorder

	lw	$2,%lo(table_list_head)($21)
	.set	noreorder
	.set	nomacro
	j	$L346
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

$L348:
	lw	$5,16($2)
	bne	$5,$3,$L347
	lw	$3,8($2)
	addiu	$4,$2,8
	sw	$17,4($3)
	sw	$3,0($17)
	sw	$4,4($17)
	lw	$3,24($2)
	sw	$17,8($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L332
	sw	$3,24($2)
	.set	macro
	.set	reorder

$L347:
	move	$2,$4
$L346:
	lw	$4,0($2)
	bne	$2,$23,$L348
	j	$L332
$L369:
	lw	$4,20($fp)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L328
	move	$17,$2
	.set	macro
	.set	reorder

	addiu	$4,$sp,16
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$19
	.set	macro
	.set	reorder

	sw	$2,24($sp)
	lb	$2,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L350
	addiu	$2,$17,8
	.set	macro
	.set	reorder

	sw	$2,12($17)
	sw	$17,0($17)
	sw	$17,4($17)
	sw	$2,8($17)
	sw	$0,24($17)
	lw	$3,40($sp)
	lw	$2,%lo(filter_rule_num)($20)
	sw	$3,16($17)
	sw	$2,20($17)
	lw	$4,24($sp)
	jal	strlen
	sltu	$2,$2,41
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L352
	li	$3,39			# 0x27
	.set	macro
	.set	reorder

	lw	$4,24($sp)
	jal	strlen
	move	$3,$2
$L352:
	lw	$4,24($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L353
	move	$5,$3
	.set	macro
	.set	reorder

	addiu	$2,$17,28
#APP
 # 50 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($4)
	subu	$5,1
	sb	$1,($2)
	beqz	$1,2f
	addiu	$2,1
	bnez	$5,1b
	addiu	$4,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
$L353:
	addu	$3,$17,$3
	sb	$0,28($3)
	lw	$2,%lo(table_list_head)($21)
	j	$L354
$L356:
	lw	$4,16($2)
	.set	noreorder
	.set	nomacro
	bne	$4,$5,$L355
	addiu	$4,$2,8
	.set	macro
	.set	reorder

	lw	$3,8($2)
	sw	$17,4($3)
	sw	$3,0($17)
	sw	$4,4($17)
	lw	$3,24($2)
	sw	$17,8($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L350
	sw	$3,24($2)
	.set	macro
	.set	reorder

$L355:
	move	$2,$3
$L354:
	lw	$3,0($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$23,$L356
	li	$5,3			# 0x3
	.set	macro
	.set	reorder

$L350:
	lw	$2,16($sp)
	bne	$2,$0,$L369
	j	$L332
$L370:
	lw	$4,20($fp)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L328
	move	$17,$2
	.set	macro
	.set	reorder

	addiu	$6,$17,28
	addiu	$2,$2,8
	move	$5,$19
	sw	$6,44($sp)
	addiu	$4,$sp,16
	sw	$2,12($17)
	sw	$17,0($17)
	sw	$17,4($17)
	sw	$2,8($17)
	sw	$0,24($17)
	lw	$3,40($sp)
	lw	$2,%lo(filter_rule_num)($20)
	sw	$3,16($17)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$2,20($17)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	li	$6,4			# 0x4
	move	$5,$22
	lw	$4,44($sp)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	addiu	$4,$sp,16
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$19
	.set	macro
	.set	reorder

	lw	$3,32($sp)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L358
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L358
	li	$5,1440			# 0x5a0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L359
	sw	$5,32($sp)
	.set	macro
	.set	reorder

$L358:
	lw	$4,24($sp)
	jal	str2hexnum
	sw	$2,32($sp)
$L359:
	lw	$2,44($sp)
	li	$6,4			# 0x4
	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,4
	.set	macro
	.set	reorder

	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	move	$5,$22
	li	$6,4			# 0x4
	lw	$3,44($sp)
	addiu	$4,$3,8
	li	$3,255			# 0xff
	movn	$3,$2,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,32($sp)
	.set	macro
	.set	reorder

	lw	$2,%lo(table_list_head)($21)
	j	$L361
$L364:
	lw	$4,16($2)
	.set	noreorder
	.set	nomacro
	bne	$4,$5,$L362
	addiu	$4,$2,8
	.set	macro
	.set	reorder

	lw	$3,8($2)
	sw	$17,4($3)
	sw	$3,0($17)
	sw	$4,4($17)
	lw	$3,24($2)
	sw	$17,8($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L363
	sw	$3,24($2)
	.set	macro
	.set	reorder

$L362:
	move	$2,$3
$L361:
	lw	$3,0($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$23,$L364
	li	$5,4			# 0x4
	.set	macro
	.set	reorder

$L363:
	lw	$2,16($sp)
	bne	$2,$0,$L370
$L332:
	lw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L383
	lui	$5,%hi($LC44)
	.set	macro
	.set	reorder

	lw	$2,%lo(filter_rule_num)($20)
	addiu	$2,$2,1
	sw	$2,%lo(filter_rule_num)($20)
	lw	$2,28($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L384
	lui	$5,%hi($LC43)
	.set	macro
	.set	reorder

$L328:
	.set	noreorder
	.set	nomacro
	j	$L379
	move	$4,$16
	.set	macro
	.set	reorder

$L322:
	move	$4,$16
$L380:
	li	$18,-14			# 0xfffffff2
$L379:
	jal	kfree
$L321:
	move	$2,$18
	lw	$31,92($sp)
	lw	$fp,88($sp)
	lw	$23,84($sp)
	lw	$22,80($sp)
	lw	$21,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,96
	.set	macro
	.set	reorder

	.end	filter_table_write_proc
	.size	filter_table_write_proc, .-filter_table_write_proc
	.section	.rodata.str1.4
	.align	2
$LC46:
	.ascii	"[ip_range_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\012\000"
	.align	2
$LC47:
	.ascii	"[mac_filter]%d.%d.%d.%d(%02x:%02x:%02x:%02x:%02x:%02x) ="
	.ascii	"==> %d.%d.%d.%d [Blocked]\012\000"
	.align	2
$LC48:
	.ascii	"[url_key_filter]%d.%d.%d.%d ==> %d.%d.%d.%d(%s)  [Blocke"
	.ascii	"d]\012\000"
	.align	2
$LC49:
	.ascii	"[schedule_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\012\000"
	.section	.text.log_fastFilter,"ax",@progbits
	.align	2
	.globl	log_fastFilter
	.set	nomips16
	.ent	log_fastFilter
	.type	log_fastFilter, @function
log_fastFilter:
	.frame	$sp,184,$31		# vars= 8, regs= 9/0, args= 136, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	sw	$23,176($sp)
	sw	$22,172($sp)
	sw	$21,168($sp)
	sw	$20,164($sp)
	sw	$19,160($sp)
	sw	$18,156($sp)
	sw	$17,152($sp)
	sw	$16,148($sp)
	move	$17,$5
	move	$18,$4
	jal	do_gettimeofday
	addiu	$4,$sp,136

	jal	rtl_ip_hdr
	move	$4,$17

	move	$4,$17
	jal	rtl_eth_hdr
	move	$16,$2

	li	$3,2			# 0x2
	beq	$18,$3,$L388
	nop

	slt	$2,$18,3
	beq	$2,$0,$L391
	li	$2,3			# 0x3

	li	$2,1			# 0x1
	bne	$18,$2,$L386
	nop

	j	$L409
	nop

$L391:
	beq	$18,$2,$L389
	li	$2,4			# 0x4

	bne	$18,$2,$L386
	nop

	j	$L410
	nop

$L409:
	beq	$16,$0,$L386
	lui	$4,%hi($LC46)

	lw	$2,16($16)
	lw	$3,12($16)
	j	$L405
	addiu	$4,$4,%lo($LC46)

$L388:
	beq	$2,$0,$L386
	nop

	beq	$16,$0,$L386
	nop

	lw	$18,12($16)
	jal	rtl_eth_hdr
	move	$4,$17

	move	$4,$17
	jal	rtl_eth_hdr
	move	$23,$2

	move	$4,$17
	addiu	$23,$23,84
	jal	rtl_eth_hdr
	move	$22,$2

	move	$4,$17
	jal	rtl_eth_hdr
	move	$21,$2

	move	$4,$17
	jal	rtl_eth_hdr
	move	$20,$2

	move	$4,$17
	jal	rtl_eth_hdr
	move	$19,$2

	move	$4,$0
	andi	$5,$18,0xff
	lw	$3,16($16)
	addiu	$8,$sp,20
	srl	$6,$18,16
	sw	$5,16($sp)
	srl	$7,$18,8
	andi	$6,$6,0xff
	andi	$7,$7,0xff
	j	$L393
	srl	$5,$18,24

$L392:
	lbu	$9,0($11)
	addiu	$4,$4,1
	sb	$9,0($10)
$L393:
	sltu	$9,$4,14
	addu	$11,$23,$4
	bne	$9,$0,$L392
	addu	$10,$8,$4

	addiu	$22,$22,98
	move	$4,$0
	j	$L395
	addiu	$8,$sp,36

$L394:
	lbu	$9,0($11)
	addiu	$4,$4,1
	sb	$9,0($10)
$L395:
	sltu	$9,$4,14
	addu	$11,$22,$4
	bne	$9,$0,$L394
	addu	$10,$8,$4

	addiu	$21,$21,112
	move	$4,$0
	j	$L397
	addiu	$8,$sp,52

$L396:
	lbu	$9,0($11)
	addiu	$4,$4,1
	sb	$9,0($10)
$L397:
	sltu	$9,$4,14
	addu	$11,$21,$4
	bne	$9,$0,$L396
	addu	$10,$8,$4

	addiu	$20,$20,126
	move	$4,$0
	j	$L399
	addiu	$8,$sp,68

$L398:
	lbu	$9,0($11)
	addiu	$4,$4,1
	sb	$9,0($10)
$L399:
	sltu	$9,$4,14
	addu	$11,$20,$4
	bne	$9,$0,$L398
	addu	$10,$8,$4

	addiu	$19,$19,140
	move	$4,$0
	j	$L401
	addiu	$8,$sp,84

$L400:
	lbu	$9,0($11)
	addiu	$4,$4,1
	sb	$9,0($10)
$L401:
	sltu	$9,$4,14
	addu	$11,$19,$4
	bne	$9,$0,$L400
	addu	$10,$8,$4

	addiu	$2,$2,154
	move	$4,$0
	j	$L403
	addiu	$8,$sp,100

$L402:
	lbu	$9,0($11)
	addiu	$4,$4,1
	sb	$9,0($10)
$L403:
	sltu	$9,$4,14
	addu	$11,$2,$4
	bne	$9,$0,$L402
	addu	$10,$8,$4

	srl	$2,$3,16
	lui	$4,%hi($LC47)
	srl	$8,$3,8
	addiu	$4,$4,%lo($LC47)
	andi	$2,$2,0xff
	andi	$8,$8,0xff
	sw	$2,120($sp)
	andi	$2,$3,0xff
	sw	$8,124($sp)
	srl	$3,$3,24
	sw	$2,128($sp)
	jal	scrlog_printk
	sw	$3,116($sp)

	j	$L386
	nop

$L389:
	beq	$16,$0,$L386
	lui	$5,%hi(log_info)

	lw	$2,16($16)
	lw	$3,12($16)
	lui	$4,%hi($LC48)
	addiu	$5,$5,%lo(log_info)
	srl	$8,$2,16
	addiu	$4,$4,%lo($LC48)
	srl	$6,$3,16
	j	$L406
	sw	$5,36($sp)

$L410:
	beq	$16,$0,$L386
	lui	$4,%hi($LC49)

	lw	$2,16($16)
	addiu	$4,$4,%lo($LC49)
	lw	$3,12($16)
$L405:
	srl	$8,$2,16
	srl	$6,$3,16
$L406:
	srl	$5,$2,8
	srl	$7,$3,8
	andi	$8,$8,0xff
	andi	$5,$5,0xff
	sw	$8,24($sp)
	sw	$5,28($sp)
	andi	$8,$2,0xff
	srl	$5,$3,24
	sw	$8,32($sp)
	andi	$3,$3,0xff
	srl	$2,$2,24
	sw	$3,16($sp)
	andi	$6,$6,0x00ff
	sw	$2,20($sp)
	jal	scrlog_printk
	andi	$7,$7,0x00ff

$L386:
	lw	$31,180($sp)
	move	$2,$0
	lw	$23,176($sp)
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	j	$31
	addiu	$sp,$sp,184

	.set	macro
	.set	reorder
	.end	log_fastFilter
	.size	log_fastFilter, .-log_fastFilter
	.section	.text.do_filter,"ax",@progbits
	.align	2
	.globl	do_filter
	.set	nomips16
	.ent	do_filter
	.type	do_filter, @function
do_filter:
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(table_list_head)
	move	$17,$4
	li	$20,-1			# 0xffffffff
	addiu	$19,$2,%lo(table_list_head)
	lw	$16,%lo(table_list_head)($2)
	j	$L412
	li	$18,1			# 0x1

$L417:
	jal	scan_table
	move	$4,$17

	beq	$2,$20,$L413
	nop

	bne	$2,$18,$L414
	lui	$3,%hi(log_enable)

	lw	$3,%lo(log_enable)($3)
	beq	$3,$0,$L416
	li	$2,1			# 0x1

	lw	$4,16($16)
	jal	log_fastFilter
	move	$5,$17

	j	$L416
	li	$2,1			# 0x1

$L414:
	lw	$16,0($16)
$L412:
	move	$6,$16
	move	$5,$0
	bne	$16,$19,$L417
	move	$7,$16

$L413:
	move	$2,$0
$L416:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	do_filter
	.size	do_filter, .-do_filter
	.section	.text.FilterWeb_v2,"ax",@progbits
	.align	2
	.globl	FilterWeb_v2
	.set	nomips16
	.ent	FilterWeb_v2
	.type	FilterWeb_v2, @function
FilterWeb_v2:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	jal	rtl_ip_hdr
	move	$16,$4

	jal	need_filter
	move	$4,$16

	bne	$2,$0,$L420
	nop

	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L420:
	move	$4,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	do_filter
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	FilterWeb_v2
	.size	FilterWeb_v2, .-FilterWeb_v2
	.section	.text.filter_enter,"ax",@progbits
	.align	2
	.globl	filter_enter
	.set	nomips16
	.ent	filter_enter
	.type	filter_enter, @function
filter_enter:
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(item)
	lw	$2,%lo(item)($2)
	bne	$2,$0,$L424
	move	$17,$4

	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	beq	$2,$0,$L424
	lui	$3,%hi(none_dos_drop_pkt_cnt)

	j	$L499
	li	$2,1			# 0x1

$L424:
	jal	rtl_get_skb_type
	move	$4,$17

	bne	$2,$0,$L425
	li	$2,4			# 0x4

	jal	rtl_get_skb_protocol
	move	$4,$17

	li	$3,2048			# 0x800
	bne	$2,$3,$L425
	li	$2,4			# 0x4

	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	bne	$2,$0,$L507
	lui	$2,%hi(item)

	jal	FilterWeb_v2
	move	$4,$17

	beq	$2,$0,$L428
	lui	$3,%hi(fast_path_cusRsp401_func)

	lw	$2,%lo(fast_path_cusRsp401_func)($3)
	j	$L505
	nop

$L428:
	lw	$2,%lo(fast_path_cusRsp401_func)($3)
	beq	$2,$0,$L427
	lui	$3,%hi(url_cache)

	li	$2,1			# 0x1
	addiu	$3,$3,%lo(url_cache)
	lbu	$4,15($3)
	bne	$4,$2,$L430
	nop

	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)
	lw	$2,%lo(fast_path_cusRspTCPFinAck_func)($2)
	j	$L505
	nop

$L430:
	lbu	$3,16($3)
	bne	$3,$2,$L507
	lui	$2,%hi(item)

	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)
	lw	$2,%lo(fast_path_cusRspTCPEndAck_func)($2)
$L505:
	beq	$2,$0,$L429
	nop

	jalr	$2
	move	$4,$17

	j	$L425
	move	$2,$0

$L427:
	lui	$2,%hi(item)
$L507:
	lw	$2,%lo(item)($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L431
	move	$4,$17

	lui	$18,%hi(wan_ppp_dev)
	jal	rtl_ip_hdr
	lui	$19,%hi(wan_dev)

	lui	$4,%hi($LC22)
	addiu	$4,$4,%lo($LC22)
	lbu	$20,0($2)
	jal	rtl_get_dev_by_name
	move	$16,$2

	lui	$4,%hi($LC23)
	addiu	$4,$4,%lo($LC23)
	andi	$20,$20,0xf
	jal	rtl_get_dev_by_name
	sw	$2,%lo(wan_ppp_dev)($18)

	move	$4,$17
	jal	rtl_get_skb_dev
	sw	$2,%lo(wan_dev)($19)

	beq	$2,$0,$L431
	nop

	jal	rtl_get_skb_dev
	move	$4,$17

	lw	$3,%lo(wan_dev)($19)
	beq	$3,$2,$L508
	lui	$2,%hi(item)

	jal	rtl_get_skb_dev
	move	$4,$17

	lw	$3,%lo(wan_ppp_dev)($18)
	bne	$3,$2,$L431
	lui	$2,%hi(item)

$L508:
	sll	$20,$20,2
	lw	$3,%lo(item)($2)
	andi	$2,$3,0x3fe
	beq	$2,$0,$L433
	addu	$20,$16,$20

	lw	$2,12($16)
	li	$4,36			# 0x24
	andi	$5,$2,0x3ff
	mult	$5,$4
	andi	$5,$2,0xff
	lui	$4,%hi(ConnectedIp)
	sll	$5,$5,2
	addiu	$4,$4,%lo(ConnectedIp)
	addu	$4,$5,$4
	lw	$5,0($4)
	lui	$4,%hi(dos_pkt)
	addiu	$4,$4,%lo(dos_pkt)
	mflo	$6
	bne	$2,$5,$L434
	addu	$4,$4,$6

	lw	$5,24($4)
	bne	$5,$2,$L434
	nop

	j	$L433
	sb	$0,0($4)

$L434:
	lw	$6,24($4)
	lb	$5,0($4)
	beq	$6,$2,$L435
	nop

	bne	$5,$0,$L433
	lui	$5,%hi(cur_p_pkt)

	sw	$4,%lo(cur_p_pkt)($5)
	sw	$2,24($4)
	li	$2,1			# 0x1
	j	$L436
	sb	$2,0($4)

$L435:
	li	$2,1			# 0x1
	bne	$5,$2,$L458
	move	$2,$0

	lui	$2,%hi(cur_p_pkt)
	j	$L436
	sw	$4,%lo(cur_p_pkt)($2)

$L493:
	lui	$2,%hi(cur_p_pkt)
	lbu	$4,9($16)
	lw	$5,%lo(cur_p_pkt)($2)
	li	$2,6			# 0x6
	bne	$4,$2,$L439
	move	$2,$0

	lbu	$4,13($20)
	li	$2,2			# 0x2
	andi	$4,$4,0x3f
	bne	$4,$2,$L439
	move	$2,$0

	lui	$2,%hi(block)
	lw	$4,%lo(block)($2)
	li	$2,1			# 0x1
	bne	$4,$2,$L438
	lui	$4,%hi(attack_saddr+4)

	lw	$6,%lo(attack_saddr+4)($4)
	lw	$4,12($16)
	beq	$6,$4,$L439
	li	$2,-1			# 0xffffffff

$L438:
	lw	$4,4($5)
	lui	$2,%hi(per_syn_threshold)
	addiu	$4,$4,1
	sw	$4,4($5)
	lw	$2,%lo(per_syn_threshold)($2)
	slt	$4,$2,$4
	beq	$4,$0,$L437
	andi	$4,$3,0x20

	beq	$4,$0,$L437
	nop

	blez	$2,$L437
	lui	$4,%hi(LogFlag)

	lw	$5,12($16)
	lui	$2,%hi(attack_saddr+4)
	lui	$6,%hi(block)
	sw	$5,%lo(attack_saddr+4)($2)
	li	$8,1			# 0x1
	lw	$5,%lo(LogFlag)($4)
	move	$2,$0
	lw	$7,16($16)
	ori	$5,$5,0x20
	sw	$8,%lo(block)($6)
	lui	$6,%hi(attack_daddr+4)
	sw	$7,%lo(attack_daddr+4)($6)
	j	$L439
	sw	$5,%lo(LogFlag)($4)

$L437:
	move	$2,$0
$L439:
	andi	$4,$3,0x44
$L527:
	beq	$4,$0,$L509
	andi	$4,$3,0x88

	lui	$4,%hi(cur_p_pkt)
	lbu	$5,9($16)
	lw	$6,%lo(cur_p_pkt)($4)
	li	$4,6			# 0x6
	bne	$5,$4,$L443
	move	$4,$0

	lbu	$4,13($20)
	li	$5,1			# 0x1
	andi	$4,$4,0x3f
	bne	$4,$5,$L441
	lui	$5,%hi(block)

	lw	$5,%lo(block)($5)
	bne	$5,$4,$L442
	lui	$5,%hi(attack_saddr+8)

	lw	$7,%lo(attack_saddr+8)($5)
	lw	$5,12($16)
	beq	$7,$5,$L443
	li	$4,-1			# 0xffffffff

$L442:
	lw	$5,8($6)
	lui	$4,%hi(per_fin_threshold)
	addiu	$5,$5,1
	sw	$5,8($6)
	lw	$4,%lo(per_fin_threshold)($4)
	slt	$5,$4,$5
	beq	$5,$0,$L441
	andi	$5,$3,0x40

	beq	$5,$0,$L441
	nop

	blez	$4,$L441
	lui	$5,%hi(LogFlag)

	lw	$6,12($16)
	lui	$4,%hi(attack_saddr+8)
	lui	$7,%hi(block)
	sw	$6,%lo(attack_saddr+8)($4)
	li	$9,1			# 0x1
	lw	$6,%lo(LogFlag)($5)
	move	$4,$0
	lw	$8,16($16)
	ori	$6,$6,0x40
	sw	$9,%lo(block)($7)
	lui	$7,%hi(attack_daddr+8)
	sw	$8,%lo(attack_daddr+8)($7)
	j	$L443
	sw	$6,%lo(LogFlag)($5)

$L441:
	move	$4,$0
$L443:
	or	$2,$2,$4
	andi	$4,$3,0x88
$L509:
	beq	$4,$0,$L510
	andi	$4,$3,0x110

	lui	$4,%hi(cur_p_pkt)
	lbu	$5,9($16)
	lw	$6,%lo(cur_p_pkt)($4)
	li	$4,17			# 0x11
	bne	$5,$4,$L447
	move	$4,$0

	lui	$4,%hi(block)
	lw	$5,%lo(block)($4)
	li	$4,1			# 0x1
	bne	$5,$4,$L446
	lui	$5,%hi(attack_saddr+12)

	lw	$7,%lo(attack_saddr+12)($5)
	lw	$5,12($16)
	beq	$7,$5,$L447
	li	$4,-1			# 0xffffffff

$L446:
	lw	$5,12($6)
	lui	$4,%hi(per_udp_threshold)
	addiu	$5,$5,1
	sw	$5,12($6)
	lw	$4,%lo(per_udp_threshold)($4)
	slt	$5,$4,$5
	beq	$5,$0,$L445
	andi	$5,$3,0x80

	beq	$5,$0,$L445
	nop

	blez	$4,$L445
	lui	$5,%hi(LogFlag)

	lw	$6,12($16)
	lui	$4,%hi(attack_saddr+12)
	lui	$7,%hi(block)
	sw	$6,%lo(attack_saddr+12)($4)
	li	$9,1			# 0x1
	lw	$6,%lo(LogFlag)($5)
	move	$4,$0
	lw	$8,16($16)
	ori	$6,$6,0x80
	sw	$9,%lo(block)($7)
	lui	$7,%hi(attack_daddr+12)
	sw	$8,%lo(attack_daddr+12)($7)
	j	$L447
	sw	$6,%lo(LogFlag)($5)

$L445:
	move	$4,$0
$L447:
	or	$2,$2,$4
	andi	$4,$3,0x110
$L510:
	beq	$4,$0,$L511
	andi	$4,$3,0x200

	lui	$5,%hi(cur_p_pkt)
	lbu	$4,9($16)
	lw	$6,%lo(cur_p_pkt)($5)
	li	$5,1			# 0x1
	bne	$4,$5,$L449
	lui	$5,%hi(block)

	lw	$5,%lo(block)($5)
	bne	$5,$4,$L450
	lui	$5,%hi(attack_saddr+16)

	lw	$7,%lo(attack_saddr+16)($5)
	lw	$5,12($16)
	beq	$7,$5,$L451
	li	$4,-1			# 0xffffffff

$L450:
	lw	$5,16($6)
	lui	$4,%hi(per_icmp_threshold)
	addiu	$5,$5,1
	sw	$5,16($6)
	lw	$4,%lo(per_icmp_threshold)($4)
	slt	$5,$4,$5
	beq	$5,$0,$L449
	andi	$5,$3,0x100

	beq	$5,$0,$L449
	nop

	blez	$4,$L449
	lui	$5,%hi(LogFlag)

	lw	$6,12($16)
	lui	$4,%hi(attack_saddr+16)
	lui	$7,%hi(block)
	sw	$6,%lo(attack_saddr+16)($4)
	li	$9,1			# 0x1
	lw	$6,%lo(LogFlag)($5)
	move	$4,$0
	lw	$8,16($16)
	ori	$6,$6,0x100
	sw	$9,%lo(block)($7)
	lui	$7,%hi(attack_daddr+16)
	sw	$8,%lo(attack_daddr+16)($7)
	j	$L451
	sw	$6,%lo(LogFlag)($5)

$L449:
	move	$4,$0
$L451:
	or	$2,$2,$4
	andi	$4,$3,0x200
$L511:
	beq	$4,$0,$L512
	li	$4,4194304			# 0x400000

	lbu	$6,9($16)
	lui	$4,%hi(cur_p_pkt)
	li	$5,6			# 0x6
	lw	$4,%lo(cur_p_pkt)($4)
	bne	$6,$5,$L513
	li	$5,17			# 0x11

	lhu	$5,32($4)
	beq	$5,$0,$L514
	nop

	lhu	$6,2($20)
	beq	$6,$5,$L514
	nop

	lw	$5,20($4)
	addiu	$5,$5,1
	sw	$5,20($4)
	lhu	$5,32($4)
$L514:
	bne	$5,$0,$L453
	nop

	lhu	$5,2($20)
	sh	$5,32($4)
$L453:
	lbu	$6,9($16)
	li	$5,17			# 0x11
$L513:
	bne	$6,$5,$L515
	li	$5,8388608			# 0x800000

	lhu	$5,32($4)
	beq	$5,$0,$L516
	nop

	lhu	$6,2($20)
	beq	$6,$5,$L516
	nop

	lw	$5,20($4)
	addiu	$5,$5,1
	sw	$5,20($4)
	lhu	$5,32($4)
$L516:
	bne	$5,$0,$L515
	li	$5,8388608			# 0x800000

	lhu	$5,2($20)
	sh	$5,32($4)
	li	$5,8388608			# 0x800000
$L515:
	and	$5,$3,$5
	lw	$4,20($4)
	beq	$5,$0,$L457
	nop

	j	$L501
	slt	$4,$4,11

$L457:
	slt	$4,$4,201
$L501:
	bne	$4,$0,$L512
	li	$4,4194304			# 0x400000

	lui	$4,%hi(LogFlag)
	lw	$5,%lo(LogFlag)($4)
	ori	$5,$5,0x200
	sw	$5,%lo(LogFlag)($4)
	lui	$4,%hi(attack_saddr+20)
	lw	$5,12($16)
	sw	$5,%lo(attack_saddr+20)($4)
	lui	$4,%hi(attack_daddr+20)
	lw	$5,16($16)
	sw	$5,%lo(attack_daddr+20)($4)
	li	$4,4194304			# 0x400000
$L512:
	and	$4,$3,$4
	bne	$4,$0,$L517
	andi	$4,$3,0x8000

$L433:
	move	$2,$0
$L458:
	andi	$4,$3,0x8000
$L517:
	beq	$4,$0,$L518
	li	$5,65536			# 0x10000

	lbu	$5,9($16)
	li	$4,6			# 0x6
	bne	$5,$4,$L461
	move	$4,$0

	lbu	$5,13($20)
	lui	$4,%hi(_tcpDosScanBitmap)
	addiu	$4,$4,%lo(_tcpDosScanBitmap)
	andi	$5,$5,0x3f
	addu	$4,$5,$4
	lb	$4,0($4)
	beq	$4,$0,$L460
	lui	$5,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$6,%lo(LogFlag)($5)
	ori	$6,$6,0x8000
	sw	$6,%lo(LogFlag)($5)
	lui	$5,%hi(attack_saddr+44)
	lw	$6,12($16)
	sw	$6,%lo(attack_saddr+44)($5)
	lui	$5,%hi(attack_daddr+44)
	lw	$6,16($16)
	j	$L461
	sw	$6,%lo(attack_daddr+44)($5)

$L460:
	move	$4,$0
$L461:
	or	$2,$2,$4
	li	$5,65536			# 0x10000
$L518:
	and	$4,$3,$5
	beq	$4,$0,$L519
	andi	$4,$3,0x800

	lbu	$6,9($16)
	li	$4,6			# 0x6
	bne	$6,$4,$L465
	move	$4,$0

	lbu	$6,13($20)
	li	$4,2			# 0x2
	andi	$6,$6,0x3f
	bne	$6,$4,$L465
	move	$4,$0

	lbu	$6,0($16)
	lw	$4,12($20)
	andi	$6,$6,0xf
	lhu	$7,2($16)
	sll	$6,$6,2
	srl	$4,$4,28
	subu	$6,$7,$6
	sll	$4,$4,2
	bne	$6,$4,$L520
	lui	$6,%hi(LogFlag)

	lhu	$4,6($16)
	andi	$4,$4,0x2000
	beq	$4,$0,$L463
	nop

$L520:
	li	$4,-1			# 0xffffffff
	lw	$7,%lo(LogFlag)($6)
	or	$5,$7,$5
	sw	$5,%lo(LogFlag)($6)
	lui	$5,%hi(attack_saddr+48)
	lw	$6,12($16)
	sw	$6,%lo(attack_saddr+48)($5)
	lui	$5,%hi(attack_daddr+48)
	lw	$6,16($16)
	j	$L465
	sw	$6,%lo(attack_daddr+48)($5)

$L463:
	move	$4,$0
$L465:
	or	$2,$2,$4
	andi	$4,$3,0x800
$L519:
	beq	$4,$0,$L521
	li	$4,262144			# 0x40000

	lw	$6,12($16)
	lw	$5,16($16)
	bne	$6,$5,$L468
	move	$4,$0

	lui	$5,%hi(LogFlag)
	li	$4,-1			# 0xffffffff
	lw	$6,%lo(LogFlag)($5)
	ori	$6,$6,0x800
	sw	$6,%lo(LogFlag)($5)
	lui	$5,%hi(attack_saddr+28)
	lw	$6,12($16)
	sw	$6,%lo(attack_saddr+28)($5)
	lui	$5,%hi(attack_daddr+28)
	lw	$6,16($16)
	sw	$6,%lo(attack_daddr+28)($5)
$L468:
	or	$2,$2,$4
	li	$4,262144			# 0x40000
$L521:
	and	$4,$3,$4
	beq	$4,$0,$L522
	li	$6,131072			# 0x20000

	lbu	$5,9($16)
	li	$4,17			# 0x11
	bne	$5,$4,$L472
	move	$4,$0

	lhu	$4,2($20)
	li	$7,7			# 0x7
	beq	$4,$7,$L471
	nop

	beq	$4,$5,$L471
	li	$6,19			# 0x13

	beq	$4,$6,$L471
	nop

	lhu	$4,0($20)
	beq	$4,$7,$L471
	nop

	beq	$4,$5,$L523
	lui	$5,%hi(LogFlag)

	bne	$4,$6,$L470
	nop

$L471:
	lui	$5,%hi(LogFlag)
$L523:
	li	$6,262144			# 0x40000
	lw	$7,%lo(LogFlag)($5)
	li	$4,-1			# 0xffffffff
	or	$6,$7,$6
	sw	$6,%lo(LogFlag)($5)
	lui	$5,%hi(attack_saddr+56)
	lw	$6,12($16)
	sw	$6,%lo(attack_saddr+56)($5)
	lui	$5,%hi(attack_daddr+56)
	lw	$6,16($16)
	j	$L472
	sw	$6,%lo(attack_daddr+56)($5)

$L470:
	move	$4,$0
$L472:
	or	$2,$2,$4
	li	$6,131072			# 0x20000
$L522:
	and	$4,$3,$6
	beq	$4,$0,$L524
	andi	$4,$3,0x4000

	lbu	$5,9($16)
	li	$4,17			# 0x11
	bne	$5,$4,$L475
	move	$4,$0

	lhu	$4,6($16)
	andi	$4,$4,0x3fff
	bne	$4,$0,$L475
	move	$4,$0

	lbu	$4,0($16)
	lhu	$7,2($16)
	andi	$4,$4,0xf
	lhu	$5,4($20)
	sll	$4,$4,2
	subu	$4,$7,$4
	slt	$4,$5,$4
	beq	$4,$0,$L474
	lui	$5,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$7,%lo(LogFlag)($5)
	or	$6,$7,$6
	sw	$6,%lo(LogFlag)($5)
	lui	$5,%hi(attack_saddr+52)
	lw	$6,12($16)
	sw	$6,%lo(attack_saddr+52)($5)
	lui	$5,%hi(attack_daddr+52)
	lw	$6,16($16)
	j	$L475
	sw	$6,%lo(attack_daddr+52)($5)

$L474:
	move	$4,$0
$L475:
	or	$2,$2,$4
	andi	$4,$3,0x4000
$L524:
	beq	$4,$0,$L525
	andi	$4,$3,0x400

	lhu	$4,6($16)
	andi	$5,$4,0x2000
	bne	$5,$0,$L477
	andi	$4,$4,0x1fff

	beq	$4,$0,$L477
	sll	$4,$4,3

	lbu	$6,0($16)
	lhu	$5,2($16)
	andi	$6,$6,0xf
	sll	$6,$6,2
	subu	$4,$4,$6
	addu	$4,$4,$5
	li	$5,65536			# 0x10000
	sltu	$4,$4,$5
	bne	$4,$0,$L477
	lui	$5,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$6,%lo(LogFlag)($5)
	ori	$6,$6,0x4000
	sw	$6,%lo(LogFlag)($5)
	lui	$5,%hi(attack_saddr+40)
	lw	$6,12($16)
	sw	$6,%lo(attack_saddr+40)($5)
	lui	$5,%hi(attack_daddr+40)
	lw	$6,16($16)
	j	$L478
	sw	$6,%lo(attack_daddr+40)($5)

$L477:
	move	$4,$0
$L478:
	or	$2,$2,$4
	andi	$4,$3,0x400
$L525:
	beq	$4,$0,$L479
	li	$5,1			# 0x1

	lbu	$4,0($16)
	lbu	$6,9($16)
	bne	$6,$5,$L480
	andi	$4,$4,0xf

	sll	$4,$4,2
	addu	$4,$16,$4
	lbu	$5,0($4)
	li	$4,8			# 0x8
	bne	$5,$4,$L481
	move	$4,$0

	lui	$4,%hi(lan_addr)
	lui	$5,%hi(lan_mask)
	lw	$6,%lo(lan_addr)($4)
	lw	$4,12($16)
	lw	$5,%lo(lan_mask)($5)
	xor	$4,$6,$4
	and	$4,$4,$5
	bne	$4,$0,$L480
	lui	$5,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$6,%lo(LogFlag)($5)
	ori	$6,$6,0x400
	sw	$6,%lo(LogFlag)($5)
	lui	$5,%hi(attack_saddr+24)
	lw	$6,12($16)
	sw	$6,%lo(attack_saddr+24)($5)
	lui	$5,%hi(attack_daddr+24)
	lw	$6,16($16)
	j	$L481
	sw	$6,%lo(attack_daddr+24)($5)

$L480:
	move	$4,$0
$L481:
	or	$2,$2,$4
$L479:
	andi	$4,$3,0x1000
	beq	$4,$0,$L482
	lui	$4,%hi(lan_addr)

	lui	$5,%hi(lan_mask)
	lw	$6,%lo(lan_addr)($4)
	lw	$4,12($16)
	lw	$5,%lo(lan_mask)($5)
	xor	$4,$6,$4
	and	$4,$4,$5
	bne	$4,$0,$L484
	move	$4,$0

	lbu	$5,9($16)
	li	$4,1			# 0x1
	beq	$5,$4,$L483
	lui	$5,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$6,%lo(LogFlag)($5)
	ori	$6,$6,0x1000
	sw	$6,%lo(LogFlag)($5)
	lui	$5,%hi(attack_saddr+32)
	lw	$6,12($16)
	sw	$6,%lo(attack_saddr+32)($5)
	lui	$5,%hi(attack_daddr+32)
	lw	$6,16($16)
	j	$L484
	sw	$6,%lo(attack_daddr+32)($5)

$L483:
	move	$4,$0
$L484:
	or	$2,$2,$4
$L482:
	andi	$3,$3,0x2000
	beq	$3,$0,$L485
	li	$4,36			# 0x24

	lw	$3,12($16)
	andi	$5,$3,0x3ff
	mult	$5,$4
	andi	$5,$3,0xff
	lui	$4,%hi(ConnectedIp)
	sll	$5,$5,2
	addiu	$4,$4,%lo(ConnectedIp)
	addu	$4,$5,$4
	lw	$5,0($4)
	lui	$4,%hi(dos_pkt)
	addiu	$4,$4,%lo(dos_pkt)
	mflo	$6
	bne	$3,$5,$L486
	addu	$4,$4,$6

	lw	$5,24($4)
	bne	$5,$3,$L486
	nop

	j	$L487
	sb	$0,0($4)

$L486:
	lw	$6,24($4)
	lb	$5,0($4)
	beq	$6,$3,$L488
	nop

	bne	$5,$0,$L487
	lui	$5,%hi(cur_p_pkt)

	sw	$4,%lo(cur_p_pkt)($5)
	sw	$3,24($4)
	li	$3,1			# 0x1
	j	$L489
	sb	$3,0($4)

$L488:
	li	$3,1			# 0x1
	bne	$5,$3,$L487
	lui	$3,%hi(cur_p_pkt)

	j	$L526
	sw	$4,%lo(cur_p_pkt)($3)

$L494:
	bne	$4,$0,$L492
	move	$4,$0

$L495:
	lhu	$4,6($16)
	andi	$6,$4,0x3fff
	beq	$6,$0,$L487
	sll	$7,$4,3

	lhu	$6,30($3)
	andi	$7,$7,0xffff
	slt	$7,$7,$6
	bne	$7,$0,$L490
	nop

	andi	$4,$4,0x2000
	bne	$4,$0,$L491
	nop

	sh	$0,30($3)
	j	$L492
	sh	$0,28($3)

$L491:
	sh	$5,28($3)
	move	$4,$0
	lbu	$5,0($16)
	lhu	$7,2($16)
	andi	$5,$5,0xf
	addu	$6,$7,$6
	sll	$5,$5,2
	subu	$6,$6,$5
	j	$L492
	sh	$6,30($3)

$L490:
	lui	$3,%hi(LogFlag)
	li	$4,-1			# 0xffffffff
	lw	$5,%lo(LogFlag)($3)
	ori	$5,$5,0x2000
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_saddr+36)
	lw	$5,12($16)
	sw	$5,%lo(attack_saddr+36)($3)
	lui	$3,%hi(attack_daddr+36)
	lw	$5,16($16)
	j	$L492
	sw	$5,%lo(attack_daddr+36)($3)

$L487:
	move	$4,$0
$L492:
	or	$2,$2,$4
$L485:
	bne	$2,$0,$L425
	move	$2,$0

$L431:
	jal	rtl_ip_hdr
	move	$4,$17

	lbu	$3,9($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L426
	lui	$3,%hi(none_dos_drop_pkt_cnt)

$L499:
	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	addiu	$4,$4,1
	j	$L425
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)

$L426:
	j	$L425
	li	$2,4			# 0x4

$L429:
	move	$2,$0
$L425:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L436:
	andi	$2,$3,0x22
	bne	$2,$0,$L493
	move	$2,$0

	j	$L527
	andi	$4,$3,0x44

$L489:
	lui	$3,%hi(cur_p_pkt)
$L526:
	lw	$3,%lo(cur_p_pkt)($3)
	lhu	$5,4($16)
	lhu	$4,28($3)
	bne	$5,$4,$L494
	nop

	j	$L495
	nop

	.set	macro
	.set	reorder
	.end	filter_enter
	.size	filter_enter, .-filter_enter
	.section	.text.rtl_mac_filter_fastpath,"ax",@progbits
	.align	2
	.globl	rtl_mac_filter_fastpath
	.set	nomips16
	.ent	rtl_mac_filter_fastpath
	.type	rtl_mac_filter_fastpath, @function
rtl_mac_filter_fastpath:
	.frame	$sp,48,$31		# vars= 8, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	move	$16,$0
	move	$18,$4
	li	$17,6			# 0x6
	addu	$2,$5,$16
$L534:
	move	$4,$18
	lbu	$19,0($2)
	jal	rtl_skb_mac_header
	sw	$5,16($sp)

	addu	$2,$2,$16
	lbu	$2,6($2)
	lw	$5,16($sp)
	beq	$19,$2,$L529
	addiu	$16,$16,1

	j	$L530
	move	$2,$0

$L529:
	bne	$16,$17,$L534
	addu	$2,$5,$16

	li	$2,1			# 0x1
$L530:
	lw	$31,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	rtl_mac_filter_fastpath
	.size	rtl_mac_filter_fastpath, .-rtl_mac_filter_fastpath
	.section	.text.T.842,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	T.842
	.type	T.842, @function
T.842:
	.frame	$sp,72,$31		# vars= 16, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$fp,64($sp)
	sw	$23,60($sp)
	sw	$22,56($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	move	$17,$0
	move	$19,$4
	lw	$20,88($sp)
	move	$21,$5
	move	$16,$0
	lw	$18,92($sp)
	li	$8,13			# 0xd
	move	$fp,$6
	li	$3,10			# 0xa
	subu	$23,$5,$7
	move	$22,$4
	sw	$0,0($18)
	j	$L536
	sw	$0,0($20)

$L546:
	lb	$2,0($22)
	bne	$2,$8,$L537
	nop

	andi	$2,$16,0x1
	beq	$2,$0,$L539
	addiu	$16,$16,1

	j	$L541
	move	$16,$0

$L537:
	bne	$2,$3,$L538
	andi	$2,$16,0x1

	beq	$2,$0,$L538
	addiu	$16,$16,1

$L539:
	slt	$2,$16,4
	beq	$2,$0,$L540
	move	$4,$22

	j	$L549
	sw	$3,24($sp)

$L538:
	move	$16,$0
$L541:
	move	$4,$22
	sw	$3,24($sp)
$L549:
	addiu	$22,$22,1
	move	$6,$7
	sw	$7,16($sp)
	move	$5,$fp
	jal	memcmp
	sw	$8,20($sp)

	lw	$3,24($sp)
	lw	$7,16($sp)
	lw	$8,20($sp)
	beq	$2,$0,$L542
	move	$2,$0

	j	$L536
	addiu	$17,$17,1

$L542:
	addu	$17,$17,$7
	sw	$17,0($20)
	addu	$19,$19,$17
	j	$L543
	li	$3,13			# 0xd

$L544:
	bne	$5,$0,$L540
	addiu	$2,$2,1

$L543:
	lb	$4,0($19)
	addu	$5,$2,$17
	sltu	$5,$21,$5
	bne	$4,$3,$L544
	addiu	$19,$19,1

	sw	$2,0($18)
	j	$L545
	li	$2,1			# 0x1

$L536:
	sltu	$2,$23,$17
	beq	$2,$0,$L546
	nop

$L540:
	move	$2,$0
$L545:
	lw	$31,68($sp)
	lw	$fp,64($sp)
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
	.end	T.842
	.size	T.842, .-T.842
	.section	.rodata.str1.4
	.align	2
$LC50:
	.ascii	"GET \000"
	.align	2
$LC51:
	.ascii	"Host: \000"
	.section	.text.rtl_url_filter_fastpath,"ax",@progbits
	.align	2
	.globl	rtl_url_filter_fastpath
	.set	nomips16
	.ent	rtl_url_filter_fastpath
	.type	rtl_url_filter_fastpath, @function
rtl_url_filter_fastpath:
	.frame	$sp,72,$31		# vars= 16, regs= 7/0, args= 24, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$21,64($sp)
	sw	$20,60($sp)
	sw	$19,56($sp)
	sw	$18,52($sp)
	sw	$17,48($sp)
	sw	$16,44($sp)
	move	$16,$4
	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$18,$5
	.set	macro
	.set	reorder

	sltu	$2,$2,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L569
	move	$17,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$16
	.set	macro
	.set	reorder

	lbu	$3,9($2)
	move	$19,$2
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L569
	move	$17,$0
	.set	macro
	.set	reorder

$L552 = .
$L553 = .
	lbu	$21,0($19)
	lui	$2,%hi(malloc_sizes+68)
	lw	$4,%lo(malloc_sizes+68)($2)
	andi	$21,$21,0xf
	sll	$21,$21,2
	addu	$21,$19,$21
	lw	$17,12($21)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	srl	$17,$17,28
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L551
	move	$16,$2
	.set	macro
	.set	reorder

	sll	$17,$17,2
	lbu	$20,0($19)
	lui	$5,%hi($LC50)
	addu	$17,$21,$17
	addiu	$5,$5,%lo($LC50)
	move	$4,$17
	li	$6,4			# 0x4
	lhu	$21,2($19)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	andi	$20,$20,0xf
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L558
	lui	$6,%hi($LC51)
	.set	macro
	.set	reorder

	li	$2,-8			# 0xfffffff8
	move	$4,$17
	mult	$20,$2
	addiu	$19,$sp,24
	addiu	$2,$sp,28
	addiu	$6,$6,%lo($LC51)
	li	$7,6			# 0x6
	sw	$2,20($sp)
	mflo	$20
	sw	$19,16($sp)
	addu	$20,$20,$21
	.set	noreorder
	.set	nomacro
	jal	T.842
	move	$5,$20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L570
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$4,28($sp)
	lw	$3,24($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L557
	addu	$3,$17,$3
	.set	macro
	.set	reorder

	move	$2,$16
#APP
 # 50 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$4,1
	sb	$1,($2)
	beqz	$1,2f
	addiu	$2,1
	bnez	$4,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
$L557:
	lw	$2,28($sp)
	lui	$6,%hi($LC50)
	move	$5,$20
	addiu	$6,$6,%lo($LC50)
	move	$4,$17
	li	$7,4			# 0x4
	addu	$2,$16,$2
	sb	$0,0($2)
	addiu	$2,$sp,32
	sw	$19,16($sp)
	.set	noreorder
	.set	nomacro
	jal	T.842
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L570
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,32($sp)
	addiu	$3,$3,-9
	.set	noreorder
	.set	nomacro
	bgtz	$3,$L559
	sw	$3,32($sp)
	.set	macro
	.set	reorder

$L558:
	move	$4,$16
$L570:
	.set	noreorder
	.set	nomacro
	j	$L567
	move	$17,$0
	.set	macro
	.set	reorder

$L559:
	lw	$2,28($sp)
	lw	$4,24($sp)
	addu	$2,$16,$2
	addu	$17,$17,$4
#APP
 # 50 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($17)
	subu	$3,1
	sb	$1,($2)
	beqz	$1,2f
	addiu	$2,1
	bnez	$3,1b
	addiu	$17,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	lw	$2,32($sp)
	move	$4,$16
	lw	$3,28($sp)
	addu	$2,$16,$2
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	strlen
	sb	$0,0($2)
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$17,$2
	.set	macro
	.set	reorder

	sltu	$17,$17,$2
	.set	noreorder
	.set	nomacro
	bne	$17,$0,$L561
	move	$20,$2
	.set	macro
	.set	reorder

	addu	$19,$16,$2
	.set	noreorder
	.set	nomacro
	j	$L562
	move	$17,$16
	.set	macro
	.set	reorder

$L564:
	addiu	$19,$19,1
	addiu	$17,$17,1
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$20
	.set	macro
	.set	reorder

	bne	$2,$0,$L562
	.set	noreorder
	.set	nomacro
	j	$L563
	li	$17,1			# 0x1
	.set	macro
	.set	reorder

$L562:
	lb	$2,0($19)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L564
	move	$4,$17
	.set	macro
	.set	reorder

$L561:
	move	$17,$0
$L563:
	lui	$4,%hi(log_info)
	lui	$6,%hi($LC27)
	move	$7,$16
	addiu	$4,$4,%lo(log_info)
	addiu	$6,$6,%lo($LC27)
	.set	noreorder
	.set	nomacro
	jal	snprintf
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	move	$4,$16
$L567:
	jal	kfree
	.set	noreorder
	.set	nomacro
	j	$L568
	move	$2,$17
	.set	macro
	.set	reorder

$L551:
	move	$17,$0
$L569:
	move	$2,$17
$L568:
	lw	$31,68($sp)
	lw	$21,64($sp)
	lw	$20,60($sp)
	lw	$19,56($sp)
	lw	$18,52($sp)
	lw	$17,48($sp)
	lw	$16,44($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

	.end	rtl_url_filter_fastpath
	.size	rtl_url_filter_fastpath, .-rtl_url_filter_fastpath
	.globl	filter_tlb
	.data
	.align	2
	.type	filter_tlb, @object
	.size	filter_tlb, 40
filter_tlb:
	.word	0
	.word	0
	.word	1
	.word	rtl_ip_range_filter_fastpath
	.word	2
	.word	rtl_mac_filter_fastpath
	.word	3
	.word	rtl_url_filter_fastpath
	.word	4
	.word	rtl_schedule_filter_fastpath
	.globl	fast_path_cusRsp401_func
	.section	.bss,"aw",@nobits
	.align	2
	.type	fast_path_cusRsp401_func, @object
	.size	fast_path_cusRsp401_func, 4
fast_path_cusRsp401_func:
	.space	4
	.globl	fast_path_cusRspTCPFinAck_func
	.align	2
	.type	fast_path_cusRspTCPFinAck_func, @object
	.size	fast_path_cusRspTCPFinAck_func, 4
fast_path_cusRspTCPFinAck_func:
	.space	4
	.globl	fast_path_cusRspTCPEndAck_func
	.align	2
	.type	fast_path_cusRspTCPEndAck_func, @object
	.size	fast_path_cusRspTCPEndAck_func, 4
fast_path_cusRspTCPEndAck_func:
	.space	4
	.local	res3
	.comm	res3,4,4
	.section	.dram-l34-fwd,"aw",@progbits
	.align	2
	.type	item, @object
	.size	item, 4
item:
	.space	4
	.align	2
	.type	none_dos_drop_pkt_cnt, @object
	.size	none_dos_drop_pkt_cnt, 4
none_dos_drop_pkt_cnt:
	.space	4
	.type	url_count, @object
	.size	url_count, 1
url_count:
	.space	1
	.local	block_count
	.comm	block_count,4,4
	.local	block_time
	.comm	block_time,4,4
	.local	block
	.comm	block,4,4
	.local	item_backup
	.comm	item_backup,4,4
	.local	url_cache
	.comm	url_cache,20,4
	.local	block_message
	.comm	block_message,4,4
	.local	res12
	.comm	res12,4,4
	.local	log_enable
	.comm	log_enable,4,4
	.local	get_info_from_usr_space
	.comm	get_info_from_usr_space,1024,4
	.local	log_info
	.comm	log_info,32,4
	.local	filter_rule_num
	.comm	filter_rule_num,4,4
	.local	LogFlag
	.comm	LogFlag,4,4
	.local	ConnectedIp
	.comm	ConnectedIp,1024,4
	.local	dos_pkt
	.comm	dos_pkt,36864,4
	.local	cur_p_pkt
	.comm	cur_p_pkt,4,4
	.local	dos_timer
	.comm	dos_timer,24,4
	.local	_tcpDosScanBitmap
	.comm	_tcpDosScanBitmap,64,4
	.local	op_mode
	.comm	op_mode,1,1
	.local	whole_syn_threshold
	.comm	whole_syn_threshold,4,4
	.local	whole_fin_threshold
	.comm	whole_fin_threshold,4,4
	.local	whole_udp_threshold
	.comm	whole_udp_threshold,4,4
	.local	whole_icmp_threshold
	.comm	whole_icmp_threshold,4,4
	.local	per_syn_threshold
	.comm	per_syn_threshold,4,4
	.local	per_fin_threshold
	.comm	per_fin_threshold,4,4
	.local	per_udp_threshold
	.comm	per_udp_threshold,4,4
	.local	per_icmp_threshold
	.comm	per_icmp_threshold,4,4
	.local	lan_addr
	.comm	lan_addr,4,4
	.local	lan_mask
	.comm	lan_mask,4,4
	.local	attack_saddr
	.comm	attack_saddr,56,4
	.local	attack_daddr
	.comm	attack_daddr,56,4
	.local	dos_flag
	.comm	dos_flag,80,4
	.globl	table_list_head
	.section	.bss
	.align	2
	.type	table_list_head, @object
	.size	table_list_head, 32
table_list_head:
	.space	32
	.globl	wan_dev
	.align	2
	.type	wan_dev, @object
	.size	wan_dev, 4
wan_dev:
	.space	4
	.globl	wan_ppp_dev
	.align	2
	.type	wan_ppp_dev, @object
	.size	wan_ppp_dev, 4
wan_ppp_dev:
	.space	4
	.globl	___strtok
	.align	2
	.type	___strtok, @object
	.size	___strtok, 4
___strtok:
	.space	4
	.ident	"GCC: (GNU) 4.4.5-1.5.5p4"
