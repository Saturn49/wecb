	.file	1 "fastpath_core.c"
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
	.section	.text.cache_timeout,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	cache_timeout
	.type	cache_timeout, @function
cache_timeout:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lbu	$2,0($4)
	beq	$2,$0,$L7
	lui	$6,%hi(max_udp_frag_entry)

	lw	$2,4($4)
	lw	$5,8($4)
	lhu	$3,2($4)
	xor	$5,$5,$2
	lbu	$2,1($4)
	srl	$7,$5,16
	lw	$6,%lo(max_udp_frag_entry)($6)
	xor	$3,$3,$5
	lw	$5,40($4)
	xor	$3,$3,$7
	addiu	$6,$6,-1
	xor	$2,$3,$2
	sb	$0,0($4)
	srl	$7,$3,8
	lw	$3,44($4)
	xor	$2,$2,$7
	beq	$5,$0,$L3
	and	$2,$2,$6

	j	$L4
	sw	$3,44($5)

$L3:
	li	$5,12			# 0xc
	lui	$6,%hi(udp_cache_table)
	mult	$2,$5
	lw	$6,%lo(udp_cache_table)($6)
	lw	$6,0($6)
	mflo	$5
	addu	$5,$6,$5
	sw	$3,4($5)
$L4:
	lw	$3,44($4)
	li	$6,12			# 0xc
	lw	$5,40($4)
	mult	$2,$6
	sw	$5,0($3)
	lui	$3,%hi(udp_cache_table)
	lw	$6,%lo(udp_cache_table)($3)
	lui	$3,%hi(udp_cache_list_free)
	mflo	$2
	addiu	$3,$3,%lo(udp_cache_list_free)
	lw	$6,0($6)
	addiu	$5,$4,48
	addu	$2,$6,$2
	lw	$6,8($2)
	addiu	$6,$6,-1
	sw	$6,8($2)
	sw	$0,48($4)
	lw	$2,4($3)
	sw	$2,52($4)
	lw	$2,4($3)
	sw	$4,0($2)
	lw	$2,8($3)
	sw	$5,4($3)
	addiu	$2,$2,1
	sw	$2,8($3)
$L7:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	cache_timeout
	.size	cache_timeout, .-cache_timeout
	.section	.text.rtk_addArp,"ax",@progbits
	.align	2
	.globl	rtk_addArp
	.set	nomips16
	.ent	rtk_addArp
	.type	rtk_addArp, @function
rtk_addArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addArp
	.size	rtk_addArp, .-rtk_addArp
	.section	.text.rtk_modifyArp,"ax",@progbits
	.align	2
	.globl	rtk_modifyArp
	.set	nomips16
	.ent	rtk_modifyArp
	.type	rtk_modifyArp, @function
rtk_modifyArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_modifyArp
	.size	rtk_modifyArp, .-rtk_modifyArp
	.section	.text.rtk_delArp,"ax",@progbits
	.align	2
	.globl	rtk_delArp
	.set	nomips16
	.ent	rtk_delArp
	.type	rtk_delArp, @function
rtk_delArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	li	$2,-4			# 0xfffffffc

	.set	macro
	.set	reorder
	.end	rtk_delArp
	.size	rtk_delArp, .-rtk_delArp
	.section	.text.rtk_addRoute,"ax",@progbits
	.align	2
	.globl	rtk_addRoute
	.set	nomips16
	.ent	rtk_addRoute
	.type	rtk_addRoute, @function
rtk_addRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addRoute
	.size	rtk_addRoute, .-rtk_addRoute
	.section	.text.rtk_modifyRoute,"ax",@progbits
	.align	2
	.globl	rtk_modifyRoute
	.set	nomips16
	.ent	rtk_modifyRoute
	.type	rtk_modifyRoute, @function
rtk_modifyRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_modifyRoute
	.size	rtk_modifyRoute, .-rtk_modifyRoute
	.section	.text.rtk_delRoute,"ax",@progbits
	.align	2
	.globl	rtk_delRoute
	.set	nomips16
	.ent	rtk_delRoute
	.type	rtk_delRoute, @function
rtk_delRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	li	$2,-4			# 0xfffffffc

	.set	macro
	.set	reorder
	.end	rtk_delRoute
	.size	rtk_delRoute, .-rtk_delRoute
	.section	.text.rtk_addSession,"ax",@progbits
	.align	2
	.globl	rtk_addSession
	.set	nomips16
	.ent	rtk_addSession
	.type	rtk_addSession, @function
rtk_addSession:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addSession
	.size	rtk_addSession, .-rtk_addSession
	.section	.text.rtk_delSession,"ax",@progbits
	.align	2
	.globl	rtk_delSession
	.set	nomips16
	.ent	rtk_delSession
	.type	rtk_delSession, @function
rtk_delSession:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_delSession
	.size	rtk_delSession, .-rtk_delSession
	.section	.text.FastPath_Route,"ax",@progbits
	.align	2
	.globl	FastPath_Route
	.set	nomips16
	.ent	FastPath_Route
	.type	FastPath_Route, @function
FastPath_Route:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(route_list_inuse)
	move	$6,$0
	lw	$3,%lo(route_list_inuse)($2)
	j	$L25
	move	$2,$0

$L28:
	lw	$5,8($3)
	sltu	$7,$5,$6
	bne	$7,$0,$L26
	and	$8,$4,$5

	lw	$7,4($3)
	bne	$8,$7,$L26
	nop

	j	$L27
	addiu	$2,$3,16

$L26:
	move	$5,$6
$L27:
	lw	$3,44($3)
	move	$6,$5
$L25:
	bne	$3,$0,$L28
	nop

	j	$31
	nop

	.set	macro
	.set	reorder
	.end	FastPath_Route
	.size	FastPath_Route, .-FastPath_Route
	.section	.text.mark_path_invalid,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	mark_path_invalid
	.type	mark_path_invalid, @function
mark_path_invalid:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	srl	$8,$4,8
	lhu	$2,18($sp)
	lui	$3,%hi(path_table_list_max)
	xor	$8,$8,$4
	lw	$3,%lo(path_table_list_max)($3)
	lui	$10,%hi(table_path)
	srl	$11,$6,16
	lw	$10,%lo(table_path)($10)
	andi	$5,$5,0xffff
	addiu	$3,$3,-1
	xor	$8,$8,$6
	lw	$10,0($10)
	xor	$8,$8,$11
	srl	$11,$5,4
	xor	$8,$8,$11
	andi	$7,$7,0xffff
	xor	$8,$8,$7
	xor	$8,$8,$2
	srl	$11,$8,12
	xor	$8,$11,$8
	li	$11,12			# 0xc
	and	$8,$8,$3
	mult	$8,$11
	mflo	$8
	addu	$10,$10,$8
	lw	$10,0($10)
	j	$L31
	li	$9,255			# 0xff

$L35:
	lw	$8,12($10)
	bne	$8,$5,$L32
	nop

	lw	$8,8($10)
	bne	$8,$7,$L32
	nop

	lw	$8,16($10)
	bne	$8,$4,$L32
	nop

	lw	$8,20($10)
	bne	$8,$6,$L32
	nop

	lbu	$11,0($10)
	bne	$11,$9,$L32
	nop

	lbu	$9,2($10)
	li	$6,1			# 0x1
	bne	$9,$6,$L33
	sb	$0,0($10)

	lw	$6,24($10)
	lhu	$9,30($10)
	j	$L34
	nop

$L33:
	lw	$6,32($10)
	lhu	$9,38($10)
	j	$L34
	nop

$L32:
	lw	$10,64($10)
$L31:
	bne	$10,$0,$L35
	nop

	j	$31
	nop

$L34:
	beq	$6,$0,$L44
	nop

	lbu	$11,2($10)
	li	$10,1			# 0x1
	bne	$11,$10,$L37
	srl	$10,$4,16

	srl	$10,$8,8
	xor	$10,$10,$8
	xor	$2,$10,$2
	srl	$10,$7,4
	xor	$2,$2,$10
	xor	$2,$2,$6
	xor	$9,$2,$9
	j	$L43
	srl	$6,$6,16

$L37:
	xor	$10,$10,$4
	xor	$10,$10,$5
	xor	$2,$10,$2
	xor	$2,$2,$6
	srl	$6,$6,8
	xor	$6,$2,$6
	srl	$9,$9,4
$L43:
	xor	$9,$6,$9
	li	$2,12			# 0xc
	srl	$6,$9,12
	xor	$9,$6,$9
	and	$3,$9,$3
	mult	$3,$2
	lui	$2,%hi(table_path)
	lw	$2,%lo(table_path)($2)
	lw	$2,0($2)
	mflo	$3
	addu	$3,$2,$3
	lw	$2,0($3)
	j	$L39
	li	$6,255			# 0xff

$L41:
	lw	$3,24($2)
	bne	$3,$8,$L40
	nop

	lw	$3,28($2)
	bne	$3,$7,$L40
	nop

	lw	$3,32($2)
	bne	$3,$4,$L40
	nop

	lw	$3,36($2)
	bne	$3,$5,$L40
	nop

	lbu	$3,0($2)
	bne	$3,$6,$L40
	nop

	sb	$0,0($2)
$L40:
	lw	$2,64($2)
$L39:
	bne	$2,$0,$L41
	nop

$L44:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mark_path_invalid
	.size	mark_path_invalid, .-mark_path_invalid
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"%s:%s\012\000"
	.align	2
$LC1:
	.ascii	"Realtek FastPath\000"
	.align	2
$LC2:
	.ascii	"v1.03\000"
	.section	.text.get_fastpath_module_info,"ax",@progbits
	.align	2
	.globl	get_fastpath_module_info
	.set	nomips16
	.ent	get_fastpath_module_info
	.type	get_fastpath_module_info, @function
get_fastpath_module_info:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	beq	$4,$0,$L49
	lui	$5,%hi($LC0)

	lui	$6,%hi($LC1)
	lui	$7,%hi($LC2)
	addiu	$5,$5,%lo($LC0)
	addiu	$6,$6,%lo($LC1)
	addiu	$7,$7,%lo($LC2)
	j	sprintf
	nop

$L49:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_fastpath_module_info
	.size	get_fastpath_module_info, .-get_fastpath_module_info
	.section	.rodata.str1.4
	.align	2
$LC3:
	.ascii	"napt num: %d, path num: %d.\012\000"
	.section	.text.fastpath_dump_napt_entry_num,"ax",@progbits
	.align	2
	.globl	fastpath_dump_napt_entry_num
	.set	nomips16
	.ent	fastpath_dump_napt_entry_num
	.type	fastpath_dump_napt_entry_num, @function
fastpath_dump_napt_entry_num:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(path_list_inuse)
	move	$16,$5
	lw	$2,%lo(path_list_inuse)($2)
	j	$L51
	move	$7,$0

$L52:
	lw	$2,72($2)
	addiu	$7,$7,1
$L51:
	bne	$2,$0,$L52
	move	$6,$0

	lui	$2,%hi(napt_list_inuse)
	lw	$2,%lo(napt_list_inuse)($2)
	j	$L53
	nop

$L54:
	lw	$2,44($2)
	addiu	$6,$6,1
$L53:
	bne	$2,$0,$L54
	lui	$5,%hi($LC3)

	addu	$4,$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC3)

	addu	$2,$2,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fastpath_dump_napt_entry_num
	.size	fastpath_dump_napt_entry_num, .-fastpath_dump_napt_entry_num
	.section	.text.init_table_path,"ax",@progbits
	.align	2
	.globl	init_table_path
	.set	nomips16
	.ent	init_table_path
	.type	init_table_path, @function
init_table_path:
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L57 = .
$L58 = .
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$2,%hi(malloc_sizes+4)
	move	$17,$4
	lw	$4,%lo(malloc_sizes+4)($2)
	move	$19,$5
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$16,%hi(table_path)

	move	$18,$2
	beq	$2,$0,$L59
	sw	$2,%lo(table_path)($16)

$L60 = .
	li	$4,12			# 0xc
	lui	$2,%hi(path_list_inuse)
	mult	$17,$4
	lui	$7,%hi(path_list_free)
	addiu	$6,$2,%lo(path_list_inuse)
	addiu	$3,$7,%lo(path_list_free)
	li	$5,32			# 0x20
	sw	$0,%lo(path_list_inuse)($2)
	lui	$2,%hi(path_table_list_max)
	mflo	$4
	sw	$6,4($6)
	sw	$3,4($3)
	sw	$0,8($6)
	sw	$0,8($3)
	sw	$0,%lo(path_list_free)($7)
	jal	__kmalloc
	sw	$17,%lo(path_table_list_max)($2)

	sw	$2,0($18)
	lw	$2,%lo(table_path)($16)
	lw	$2,0($2)
	beq	$2,$0,$L59
	move	$2,$0

	j	$L61
	li	$3,12			# 0xc

$L62:
	lw	$5,%lo(table_path)($16)
	addiu	$2,$2,1
	lw	$5,0($5)
	mflo	$4
	addu	$5,$5,$4
	sw	$0,8($5)
	lw	$5,%lo(table_path)($16)
	lw	$5,0($5)
	addu	$5,$5,$4
	sw	$0,0($5)
	lw	$5,%lo(table_path)($16)
	lw	$5,0($5)
	addu	$4,$5,$4
	sw	$4,4($4)
$L61:
	slt	$4,$2,$17
	bne	$4,$0,$L62
	mult	$2,$3

	lui	$18,%hi(malloc_sizes)
	lui	$16,%hi(path_list_free)
	addiu	$18,$18,%lo(malloc_sizes)
	addiu	$16,$16,%lo(path_list_free)
	j	$L63
	move	$17,$0

$L64:
$L65 = .
	lw	$4,20($18)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	bne	$2,$0,$L66
	addiu	$3,$2,72

	j	$L67
	li	$2,-2			# 0xfffffffe

$L66:
	sw	$0,72($2)
	lw	$4,4($16)
	addiu	$17,$17,1
	sw	$4,76($2)
	lw	$4,4($16)
	sw	$2,0($4)
	lw	$2,8($16)
	sw	$3,4($16)
	addiu	$2,$2,1
	sw	$2,8($16)
$L63:
	slt	$2,$17,$19
	bne	$2,$0,$L64
	move	$2,$0

	j	$L67
	nop

$L59:
	li	$2,-1			# 0xffffffff
$L67:
	lw	$31,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	init_table_path
	.size	init_table_path, .-init_table_path
	.section	.text.init_table_napt,"ax",@progbits
	.align	2
	.globl	init_table_napt
	.set	nomips16
	.ent	init_table_napt
	.type	init_table_napt, @function
init_table_napt:
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L70 = .
$L71 = .
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$2,%hi(malloc_sizes+4)
	move	$17,$4
	lw	$4,%lo(malloc_sizes+4)($2)
	move	$19,$5
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$16,%hi(table_napt)

	move	$18,$2
	beq	$2,$0,$L72
	sw	$2,%lo(table_napt)($16)

$L73 = .
	li	$4,12			# 0xc
	lui	$2,%hi(napt_list_inuse)
	mult	$17,$4
	lui	$7,%hi(napt_list_free)
	addiu	$6,$2,%lo(napt_list_inuse)
	addiu	$3,$7,%lo(napt_list_free)
	li	$5,32			# 0x20
	sw	$0,%lo(napt_list_inuse)($2)
	lui	$2,%hi(napt_table_list_max)
	mflo	$4
	sw	$6,4($6)
	sw	$3,4($3)
	sw	$0,8($6)
	sw	$0,8($3)
	sw	$0,%lo(napt_list_free)($7)
	jal	__kmalloc
	sw	$17,%lo(napt_table_list_max)($2)

	sw	$2,0($18)
	lw	$2,%lo(table_napt)($16)
	lw	$2,0($2)
	beq	$2,$0,$L72
	move	$2,$0

	j	$L74
	li	$3,12			# 0xc

$L75:
	lw	$5,%lo(table_napt)($16)
	addiu	$2,$2,1
	lw	$5,0($5)
	mflo	$4
	addu	$5,$5,$4
	sw	$0,8($5)
	lw	$5,%lo(table_napt)($16)
	lw	$5,0($5)
	addu	$5,$5,$4
	sw	$0,0($5)
	lw	$5,%lo(table_napt)($16)
	lw	$5,0($5)
	addu	$4,$5,$4
	sw	$4,4($4)
$L74:
	slt	$4,$2,$17
	bne	$4,$0,$L75
	mult	$2,$3

	lui	$18,%hi(malloc_sizes)
	lui	$16,%hi(napt_list_free)
	addiu	$18,$18,%lo(malloc_sizes)
	addiu	$16,$16,%lo(napt_list_free)
	j	$L76
	move	$17,$0

$L77:
$L78 = .
	lw	$4,12($18)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	bne	$2,$0,$L79
	addiu	$3,$2,44

	j	$L80
	li	$2,-2			# 0xfffffffe

$L79:
	sw	$0,44($2)
	lw	$4,4($16)
	addiu	$17,$17,1
	sw	$4,48($2)
	lw	$4,4($16)
	sw	$2,0($4)
	lw	$2,8($16)
	sw	$3,4($16)
	addiu	$2,$2,1
	sw	$2,8($16)
$L76:
	slt	$2,$17,$19
	bne	$2,$0,$L77
	move	$2,$0

	j	$L80
	nop

$L72:
	li	$2,-1			# 0xffffffff
$L80:
	lw	$31,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	init_table_napt
	.size	init_table_napt, .-init_table_napt
	.section	.rodata.str1.4
	.align	2
$LC4:
	.ascii	"lo\000"
	.section	.text.enter_fast_path,"ax",@progbits
	.align	2
	.globl	enter_fast_path
	.set	nomips16
	.ent	enter_fast_path
	.type	enter_fast_path, @function
enter_fast_path:
	.frame	$sp,88,$31		# vars= 24, regs= 10/0, args= 24, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$fp,80($sp)
	sw	$23,76($sp)
	sw	$22,72($sp)
	sw	$21,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev_name
	move	$21,$4
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$17,$2
	.set	macro
	.set	reorder

	move	$16,$2
	lbu	$2,8($2)
	sltu	$2,$2,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L83
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lbu	$20,9($16)
	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L84
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$20,$2,$L85
	move	$4,$0
	.set	macro
	.set	reorder

$L84:
	lbu	$19,0($16)
	move	$4,$16
	move	$6,$21
	andi	$19,$19,0xf
	sll	$19,$19,2
	addu	$19,$16,$19
	.set	noreorder
	.set	nomacro
	jal	fast_path_pre_process_check
	move	$5,$19
	.set	macro
	.set	reorder

	move	$4,$2
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L85
	lui	$3,%hi($LC4)
	.set	macro
	.set	reorder

	addiu	$2,$3,%lo($LC4)
#APP
 # 75 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$3,($17)
1:	lbu	$1,($2)
	addiu	$17,1
	bne	$1,$3,2f
	addiu	$2,1
	bnez	$3,1b
	lbu	$3,($17)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	beq	$3,$0,$L83
	lhu	$5,0($19)
	lhu	$7,2($19)
	lhu	$17,6($16)
	sw	$5,36($sp)
	lw	$22,12($16)
	sw	$7,32($sp)
	lw	$fp,16($16)
	.set	noreorder
	.set	nomacro
	bne	$20,$4,$L86
	andi	$2,$17,0x3fff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L87
	li	$2,8192			# 0x2000
	.set	macro
	.set	reorder

	bne	$17,$2,$L88
	j	$L87
$L204:
	move	$4,$22
	move	$6,$fp
	.set	noreorder
	.set	nomacro
	jal	mark_path_invalid
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L85
	move	$4,$0
	.set	macro
	.set	reorder

$L171:
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L85
	move	$4,$0
	.set	macro
	.set	reorder

$L88:
	lw	$3,12($19)
	li	$2,1048576			# 0x100000
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L86
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L86
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

$L86:
	lhu	$2,6($16)
	andi	$2,$2,0x3fff
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L89
	li	$2,8192			# 0x2000
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	move	$18,$0
	move	$8,$0
	move	$23,$0
	.set	noreorder
	.set	nomacro
	j	$L90
	sw	$2,24($sp)
	.set	macro
	.set	reorder

$L89:
	.set	noreorder
	.set	nomacro
	bne	$17,$2,$L91
	move	$8,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L92
	li	$23,1			# 0x1
	.set	macro
	.set	reorder

$L91:
	andi	$2,$17,0x2000
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L92
	move	$23,$0
	.set	macro
	.set	reorder

	andi	$17,$17,0x1fff
	.set	noreorder
	.set	nomacro
	bne	$17,$0,$L92
	li	$8,1			# 0x1
	.set	macro
	.set	reorder

	move	$8,$0
	move	$23,$0
$L92:
	lw	$3,12($16)
	lui	$5,%hi(max_udp_frag_entry)
	lw	$4,16($16)
	lhu	$2,4($16)
	xor	$9,$4,$3
	lw	$10,%lo(max_udp_frag_entry)($5)
	lui	$5,%hi(udp_cache_table)
	xor	$7,$2,$9
	lw	$11,%lo(udp_cache_table)($5)
	srl	$9,$9,16
	addiu	$10,$10,-1
	xor	$7,$7,$9
	lw	$5,0($11)
	xor	$9,$7,$20
	srl	$7,$7,8
	xor	$7,$9,$7
	li	$9,12			# 0xc
	and	$7,$7,$10
	mult	$7,$9
	mflo	$7
	addu	$5,$5,$7
	lw	$18,0($5)
	.set	noreorder
	.set	nomacro
	j	$L94
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

$L97:
	lhu	$5,2($18)
	bne	$5,$2,$L95
	lw	$5,4($18)
	bne	$5,$3,$L95
	lw	$5,8($18)
	bne	$5,$4,$L95
	lbu	$5,1($18)
	bne	$5,$20,$L95
	lbu	$5,0($18)
	beq	$5,$6,$L96
$L95:
	lw	$18,40($18)
$L94:
	bne	$18,$0,$L97
	j	$L203
$L96:
	lhu	$9,12($18)
	sw	$0,24($sp)
	sw	$9,36($sp)
	lhu	$11,14($18)
	.set	noreorder
	.set	nomacro
	beq	$23,$0,$L90
	sw	$11,32($sp)
	.set	macro
	.set	reorder

	li	$23,1			# 0x1
$L206:
	sw	$23,24($sp)
$L90:
	srl	$2,$22,8
	lw	$11,32($sp)
	lui	$3,%hi(path_table_list_max)
	xor	$9,$fp,$22
	lw	$10,%lo(path_table_list_max)($3)
	li	$7,255			# 0xff
	srl	$12,$fp,16
	li	$6,2			# 0x2
	li	$5,1			# 0x1
	xor	$9,$9,$2
	sw	$12,28($sp)
	addiu	$10,$10,-1
	xor	$9,$9,$12
	lw	$12,36($sp)
	lui	$2,%hi(table_path)
	xor	$9,$9,$20
	lw	$3,%lo(table_path)($2)
	li	$4,17			# 0x11
	xor	$9,$9,$11
	srl	$11,$12,4
	lw	$2,0($3)
	xor	$9,$9,$11
	srl	$11,$9,12
	xor	$9,$11,$9
	and	$9,$9,$10
	li	$10,12			# 0xc
	mult	$9,$10
	mflo	$9
	addu	$2,$2,$9
	lw	$17,0($2)
	.set	noreorder
	.set	nomacro
	j	$L100
	li	$3,6			# 0x6
	.set	macro
	.set	reorder

$L166:
	lw	$2,12($17)
	lw	$11,36($sp)
	bne	$2,$11,$L101
	lw	$2,8($17)
	lw	$12,32($sp)
	bne	$2,$12,$L101
	lw	$2,16($17)
	bne	$2,$22,$L101
	lw	$9,20($17)
	bne	$9,$fp,$L101
	lbu	$2,0($17)
	bne	$2,$7,$L101
	lw	$2,4($17)
	bne	$2,$6,$L102
	bne	$20,$3,$L101
	j	$L103
$L102:
	bne	$2,$5,$L101
	bne	$20,$4,$L101
$L103:
	lw	$5,32($17)
	move	$4,$21
	lbu	$7,1($16)
	move	$6,$22
	sw	$8,40($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_route_input
	sw	$9,44($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L85
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_dst_check
	move	$4,$21
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffff
	lw	$8,40($sp)
	lw	$9,44($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L109
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L106
	addiu	$3,$19,16
	.set	macro
	.set	reorder

	lhu	$3,6($19)
	addiu	$2,$19,6
	movz	$2,$0,$3
	move	$3,$2
$L106:
	.set	noreorder
	.set	nomacro
	beq	$23,$0,$L108
	lui	$5,%hi(udp_cache_list_free)
	.set	macro
	.set	reorder

	lui	$4,%hi(max_udp_frag_entry)
	lw	$2,%lo(udp_cache_list_free)($5)
	lw	$11,%lo(max_udp_frag_entry)($4)
	lhu	$7,4($16)
	beq	$2,$0,$L109
	lw	$4,36($sp)
	lw	$6,48($2)
	sh	$4,12($2)
	li	$4,1			# 0x1
	lw	$12,32($sp)
	sb	$4,0($2)
	sh	$12,14($2)
	sh	$7,2($2)
	sw	$22,4($2)
	sw	$9,8($2)
	sb	$20,1($2)
	lw	$4,52($2)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L110
	addiu	$5,$5,%lo(udp_cache_list_free)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L111
	sw	$4,52($6)
	.set	macro
	.set	reorder

$L110:
	sw	$4,4($5)
$L111:
	lw	$4,52($2)
	xor	$9,$9,$22
	lui	$8,%hi(udp_cache_list_free)
	lw	$5,48($2)
	addiu	$8,$8,%lo(udp_cache_list_free)
	xor	$7,$7,$9
	srl	$9,$9,16
	addiu	$11,$11,-1
	addiu	$6,$2,40
	sw	$5,0($4)
	xor	$7,$7,$9
	lui	$5,%hi(udp_cache_table)
	lw	$10,8($8)
	srl	$12,$7,8
	addiu	$4,$2,16
	xor	$7,$7,$20
	xor	$7,$7,$12
	addiu	$10,$10,-1
	sw	$10,8($8)
	and	$7,$7,$11
	li	$8,12			# 0xc
	sw	$0,40($2)
	mult	$7,$8
	lw	$8,%lo(udp_cache_table)($5)
	lw	$8,0($8)
	mflo	$7
	addu	$8,$8,$7
	lw	$8,4($8)
	sw	$8,44($2)
	lw	$8,%lo(udp_cache_table)($5)
	lw	$8,0($8)
	addu	$8,$8,$7
	lw	$8,4($8)
	sw	$2,0($8)
	lw	$8,%lo(udp_cache_table)($5)
	lw	$8,0($8)
	addu	$8,$8,$7
	sw	$6,4($8)
	lw	$5,%lo(udp_cache_table)($5)
	lw	$5,0($5)
	addu	$7,$5,$7
	lw	$5,8($7)
	addiu	$5,$5,1
	sw	$5,8($7)
	lui	$5,%hi(jiffies)
	lw	$5,%lo(jiffies)($5)
	addiu	$5,$5,1000
	sw	$5,24($2)
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$3,40($sp)
	.set	macro
	.set	reorder

	lw	$3,40($sp)
	.set	noreorder
	.set	nomacro
	j	$L205
	move	$4,$21
	.set	macro
	.set	reorder

$L109:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L85
	move	$4,$0
	.set	macro
	.set	reorder

$L108:
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L205
	move	$4,$21
	.set	macro
	.set	reorder

	addiu	$4,$18,16
	.set	noreorder
	.set	nomacro
	jal	del_timer
	sw	$3,40($sp)
	.set	macro
	.set	reorder

	lui	$6,%hi(max_udp_frag_entry)
	lw	$2,4($18)
	lw	$5,8($18)
	lhu	$4,2($18)
	xor	$5,$5,$2
	lbu	$2,1($18)
	srl	$7,$5,16
	lw	$6,%lo(max_udp_frag_entry)($6)
	xor	$4,$4,$5
	lw	$5,40($18)
	xor	$4,$4,$7
	addiu	$6,$6,-1
	xor	$2,$4,$2
	sb	$0,0($18)
	srl	$7,$4,8
	lw	$3,40($sp)
	xor	$2,$2,$7
	lw	$4,44($18)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L113
	and	$2,$2,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L114
	sw	$4,44($5)
	.set	macro
	.set	reorder

$L113:
	li	$5,12			# 0xc
	lui	$6,%hi(udp_cache_table)
	mult	$2,$5
	lw	$6,%lo(udp_cache_table)($6)
	lw	$6,0($6)
	mflo	$5
	addu	$5,$6,$5
	sw	$4,4($5)
$L114:
	lw	$4,44($18)
	li	$6,12			# 0xc
	lw	$5,40($18)
	mult	$2,$6
	sw	$5,0($4)
	lui	$4,%hi(udp_cache_table)
	lw	$6,%lo(udp_cache_table)($4)
	lui	$4,%hi(udp_cache_list_free)
	mflo	$2
	addiu	$4,$4,%lo(udp_cache_list_free)
	lw	$6,0($6)
	addiu	$5,$18,48
	addu	$2,$6,$2
	lw	$6,8($2)
	addiu	$6,$6,-1
	sw	$6,8($2)
	sw	$0,48($18)
	lw	$2,4($4)
	sw	$2,52($18)
	lw	$2,4($4)
	sw	$18,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	sw	$2,8($4)
	move	$4,$21
$L205:
	sw	$3,40($sp)
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	fast_path_before_nat_check
	move	$6,$20
	.set	macro
	.set	reorder

	move	$4,$2
	li	$2,1			# 0x1
	lw	$3,40($sp)
	beq	$4,$2,$L85
	lbu	$2,1($17)
	sltu	$4,$2,13
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L115
	lui	$4,%hi($L121)
	.set	macro
	.set	reorder

	sll	$2,$2,2
	addiu	$4,$4,%lo($L121)
	addu	$2,$4,$2
	lw	$2,0($2)
	lhu	$4,10($16)
	j	$2
	.rdata
	.align	2
	.align	2
$L121:
	.word	$L116
	.word	$L117
	.word	$L118
	.word	$L118
	.word	$L119
	.word	$L115
	.word	$L115
	.word	$L115
	.word	$L120
	.word	$L115
	.word	$L115
	.word	$L115
	.word	$L120
	.section	.text.enter_fast_path
$L117:
	lw	$2,24($17)
	srl	$5,$22,16
	andi	$22,$22,0xffff
	srl	$6,$2,16
	subu	$6,$0,$6
	andi	$2,$2,0xffff
	subu	$2,$6,$2
	addu	$2,$2,$5
	addu	$22,$2,$22
	addu	$4,$22,$4
	.set	noreorder
	.set	nomacro
	bgez	$4,$L122
	andi	$22,$4,0xffff
	.set	macro
	.set	reorder

	subu	$4,$0,$4
	andi	$2,$4,0xffff
	sra	$4,$4,16
	addu	$4,$2,$4
	sra	$2,$4,16
	addu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	j	$L173
	nor	$4,$0,$4
	.set	macro
	.set	reorder

$L122:
	sra	$4,$4,16
	addu	$4,$22,$4
	sra	$2,$4,16
	addu	$4,$2,$4
$L173:
	sh	$4,10($16)
	lw	$2,24($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L131
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L125
	beq	$3,$0,$L131
	lhu	$4,0($3)
	beq	$4,$0,$L131
	lw	$6,24($17)
	lw	$5,12($16)
	srl	$2,$6,16
	subu	$2,$0,$2
	andi	$6,$6,0xffff
	srl	$7,$5,16
	subu	$2,$2,$6
	addu	$2,$2,$7
	andi	$5,$5,0xffff
	addu	$2,$2,$5
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L126
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L175
	nor	$2,$0,$2
	.set	macro
	.set	reorder

$L126:
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
$L175:
	.set	noreorder
	.set	nomacro
	j	$L131
	sh	$2,0($3)
	.set	macro
	.set	reorder

$L125:
	lw	$5,24($17)
	lw	$4,12($16)
	srl	$6,$5,16
	lhu	$2,0($3)
	subu	$6,$0,$6
	andi	$5,$5,0xffff
	subu	$5,$6,$5
	srl	$6,$4,16
	addu	$5,$5,$6
	andi	$4,$4,0xffff
	addu	$4,$5,$4
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bgez	$2,$L127
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L174
	nor	$2,$0,$2
	.set	macro
	.set	reorder

$L127:
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
$L174:
	sh	$2,0($3)
	li	$2,1048576			# 0x100000
	lw	$3,12($19)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L131
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L131
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

	j	$L131
$L118:
	lw	$2,24($17)
	srl	$5,$22,16
	andi	$22,$22,0xffff
	srl	$6,$2,16
	subu	$6,$0,$6
	andi	$2,$2,0xffff
	subu	$2,$6,$2
	addu	$2,$2,$5
	addu	$22,$2,$22
	addu	$4,$22,$4
	.set	noreorder
	.set	nomacro
	bgez	$4,$L129
	andi	$22,$4,0xffff
	.set	macro
	.set	reorder

	subu	$4,$0,$4
	andi	$2,$4,0xffff
	sra	$4,$4,16
	addu	$4,$2,$4
	sra	$2,$4,16
	addu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	j	$L176
	nor	$4,$0,$4
	.set	macro
	.set	reorder

$L129:
	sra	$4,$4,16
	addu	$4,$22,$4
	sra	$2,$4,16
	addu	$4,$2,$4
$L176:
	sh	$4,10($16)
	lw	$4,24($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L131
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L132
	beq	$3,$0,$L133
	lhu	$4,0($3)
	beq	$4,$0,$L133
	lw	$6,24($17)
	lw	$5,12($16)
	srl	$2,$6,16
	lhu	$8,0($19)
	subu	$2,$0,$2
	andi	$6,$6,0xffff
	srl	$7,$5,16
	subu	$2,$2,$6
	addu	$2,$2,$7
	andi	$5,$5,0xffff
	addu	$2,$2,$5
	lw	$7,28($17)
	addu	$2,$2,$8
	subu	$2,$2,$7
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L134
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L178
	nor	$2,$0,$2
	.set	macro
	.set	reorder

$L134:
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
$L178:
	.set	noreorder
	.set	nomacro
	j	$L133
	sh	$2,0($3)
	.set	macro
	.set	reorder

$L132:
	lw	$5,24($17)
	lw	$4,12($16)
	srl	$2,$5,16
	lhu	$6,0($19)
	subu	$2,$0,$2
	andi	$5,$5,0xffff
	srl	$7,$4,16
	subu	$2,$2,$5
	addu	$2,$2,$7
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	lw	$7,28($17)
	addu	$2,$2,$6
	lhu	$6,0($3)
	subu	$2,$2,$7
	addu	$2,$6,$2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L135
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L177
	nor	$2,$0,$2
	.set	macro
	.set	reorder

$L135:
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
$L177:
	sh	$2,0($3)
	li	$2,1048576			# 0x100000
	lw	$3,12($19)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L133
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L133
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

$L133:
	lw	$2,28($17)
	sh	$2,0($19)
$L131:
	lw	$2,24($17)
	.set	noreorder
	.set	nomacro
	j	$L116
	sw	$2,12($16)
	.set	macro
	.set	reorder

$L119:
	lw	$5,32($17)
	andi	$fp,$fp,0xffff
	lw	$6,28($sp)
	andi	$22,$5,0xffff
	addu	$2,$fp,$6
	srl	$5,$5,16
	subu	$22,$2,$22
	subu	$5,$22,$5
	addu	$4,$5,$4
	.set	noreorder
	.set	nomacro
	bgez	$4,$L137
	andi	$22,$4,0xffff
	.set	macro
	.set	reorder

	subu	$4,$0,$4
	andi	$5,$4,0xffff
	sra	$4,$4,16
	addu	$4,$5,$4
	sra	$5,$4,16
	addu	$4,$5,$4
	.set	noreorder
	.set	nomacro
	j	$L179
	nor	$4,$0,$4
	.set	macro
	.set	reorder

$L137:
	sra	$4,$4,16
	addu	$4,$22,$4
	sra	$5,$4,16
	addu	$4,$5,$4
$L179:
	sh	$4,10($16)
	lw	$7,24($sp)
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L188
	li	$4,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$4,$L140
	beq	$3,$0,$L188
	lhu	$4,0($3)
	beq	$4,$0,$L188
	lw	$5,32($17)
	andi	$fp,$5,0xffff
	subu	$fp,$2,$fp
	srl	$5,$5,16
	subu	$fp,$fp,$5
	.set	noreorder
	.set	nomacro
	j	$L194
	addu	$fp,$4,$fp
	.set	macro
	.set	reorder

$L140:
	lw	$4,32($17)
	lhu	$fp,0($3)
	andi	$5,$4,0xffff
	subu	$2,$2,$5
	srl	$4,$4,16
	subu	$2,$2,$4
	addu	$fp,$fp,$2
$L194:
	.set	noreorder
	.set	nomacro
	bgez	$fp,$L142
	andi	$2,$fp,0xffff
	.set	macro
	.set	reorder

	subu	$fp,$0,$fp
	andi	$2,$fp,0xffff
	sra	$fp,$fp,16
	addu	$fp,$2,$fp
	sra	$2,$fp,16
	addu	$fp,$2,$fp
	.set	noreorder
	.set	nomacro
	j	$L180
	nor	$fp,$0,$fp
	.set	macro
	.set	reorder

$L142:
	sra	$fp,$fp,16
	addu	$fp,$2,$fp
	sra	$2,$fp,16
	addu	$fp,$2,$fp
$L180:
	.set	noreorder
	.set	nomacro
	j	$L188
	sh	$fp,0($3)
	.set	macro
	.set	reorder

$L120:
	lw	$2,32($17)
	andi	$fp,$fp,0xffff
	lw	$9,28($sp)
	andi	$22,$2,0xffff
	addu	$fp,$fp,$9
	srl	$2,$2,16
	subu	$22,$fp,$22
	subu	$22,$22,$2
	addu	$22,$22,$4
	.set	noreorder
	.set	nomacro
	bgez	$22,$L143
	andi	$2,$22,0xffff
	.set	macro
	.set	reorder

	subu	$22,$0,$22
	andi	$2,$22,0xffff
	sra	$22,$22,16
	addu	$22,$2,$22
	sra	$2,$22,16
	addu	$22,$2,$22
	.set	noreorder
	.set	nomacro
	j	$L181
	nor	$22,$0,$22
	.set	macro
	.set	reorder

$L143:
	sra	$22,$22,16
	addu	$22,$2,$22
	sra	$2,$22,16
	addu	$22,$2,$22
$L181:
	sh	$22,10($16)
	lw	$11,24($sp)
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L188
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L146
	beq	$3,$0,$L147
	lhu	$4,0($3)
	beq	$4,$0,$L147
	lw	$5,32($17)
	lhu	$7,2($19)
	andi	$2,$5,0xffff
	lw	$6,36($17)
	subu	$2,$fp,$2
	srl	$5,$5,16
	subu	$2,$2,$5
	addu	$2,$2,$7
	subu	$2,$2,$6
	.set	noreorder
	.set	nomacro
	j	$L196
	addu	$2,$4,$2
	.set	macro
	.set	reorder

$L146:
	lw	$4,32($17)
	lhu	$5,2($19)
	andi	$2,$4,0xffff
	lw	$6,36($17)
	subu	$2,$fp,$2
	srl	$4,$4,16
	subu	$2,$2,$4
	addu	$2,$2,$5
	lhu	$5,0($3)
	subu	$2,$2,$6
	addu	$2,$5,$2
$L196:
	.set	noreorder
	.set	nomacro
	bgez	$2,$L149
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L182
	nor	$2,$0,$2
	.set	macro
	.set	reorder

$L149:
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
$L182:
	sh	$2,0($3)
$L147:
	lw	$2,36($17)
	.set	noreorder
	.set	nomacro
	j	$L188
	sh	$2,2($19)
	.set	macro
	.set	reorder

$L115:
	lw	$4,24($17)
	srl	$2,$22,16
	andi	$22,$22,0xffff
	lhu	$5,10($16)
	addu	$22,$22,$2
	andi	$2,$4,0xffff
	subu	$2,$22,$2
	srl	$4,$4,16
	subu	$2,$2,$4
	addu	$2,$2,$5
	.set	noreorder
	.set	nomacro
	bgez	$2,$L150
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L183
	nor	$2,$0,$2
	.set	macro
	.set	reorder

$L150:
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
$L183:
	sh	$2,10($16)
	andi	$fp,$fp,0xffff
	lw	$2,32($17)
	lw	$12,28($sp)
	andi	$5,$2,0xffff
	lhu	$4,10($16)
	addu	$fp,$fp,$12
	srl	$2,$2,16
	subu	$5,$fp,$5
	subu	$5,$5,$2
	addu	$5,$5,$4
	.set	noreorder
	.set	nomacro
	bgez	$5,$L152
	andi	$2,$5,0xffff
	.set	macro
	.set	reorder

	subu	$5,$0,$5
	andi	$2,$5,0xffff
	sra	$5,$5,16
	addu	$5,$2,$5
	sra	$2,$5,16
	addu	$5,$2,$5
	.set	noreorder
	.set	nomacro
	j	$L184
	nor	$5,$0,$5
	.set	macro
	.set	reorder

$L152:
	sra	$5,$5,16
	addu	$5,$2,$5
	sra	$2,$5,16
	addu	$5,$2,$5
$L184:
	sh	$5,10($16)
	lw	$2,24($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L154
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L155
	beq	$3,$0,$L156
	lhu	$2,0($3)
	beq	$2,$0,$L156
	lw	$5,24($17)
	lhu	$7,0($19)
	andi	$4,$5,0xffff
	lw	$6,28($17)
	subu	$4,$22,$4
	srl	$5,$5,16
	subu	$4,$4,$5
	addu	$4,$4,$7
	subu	$4,$4,$6
	.set	noreorder
	.set	nomacro
	j	$L200
	addu	$4,$2,$4
	.set	macro
	.set	reorder

$L155:
	lw	$2,24($17)
	lhu	$5,0($19)
	andi	$4,$2,0xffff
	lw	$6,28($17)
	subu	$4,$22,$4
	srl	$2,$2,16
	subu	$4,$4,$2
	addu	$4,$4,$5
	lhu	$5,0($3)
	subu	$4,$4,$6
	addu	$4,$5,$4
$L200:
	.set	noreorder
	.set	nomacro
	bgez	$4,$L160
	andi	$2,$4,0xffff
	.set	macro
	.set	reorder

	subu	$4,$0,$4
	andi	$2,$4,0xffff
	sra	$4,$4,16
	addu	$4,$2,$4
	sra	$2,$4,16
	addu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	j	$L186
	nor	$4,$0,$4
	.set	macro
	.set	reorder

$L160:
	sra	$4,$4,16
	addu	$4,$2,$4
	sra	$2,$4,16
	addu	$4,$2,$4
$L186:
	sh	$4,0($3)
	lw	$4,32($17)
	lhu	$5,2($19)
	andi	$2,$4,0xffff
	lw	$6,36($17)
	subu	$2,$fp,$2
	srl	$4,$4,16
	subu	$2,$2,$4
	addu	$2,$2,$5
	lhu	$5,0($3)
	subu	$2,$2,$6
	addu	$2,$5,$2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L162
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L187
	nor	$2,$0,$2
	.set	macro
	.set	reorder

$L162:
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
$L187:
	sh	$2,0($3)
$L156:
	lw	$2,28($17)
	sh	$2,0($19)
	lw	$2,36($17)
	sh	$2,2($19)
$L154:
	lw	$2,24($17)
	sw	$2,12($16)
$L188:
	lw	$2,32($17)
	sw	$2,16($16)
$L116:
	lbu	$2,8($16)
	lbu	$3,9($16)
	addiu	$4,$2,-1
	sll	$2,$2,8
	andi	$4,$4,0x00ff
	lhu	$5,10($16)
	or	$2,$2,$3
	sb	$4,8($16)
	sll	$4,$4,8
	addu	$2,$2,$5
	or	$3,$4,$3
	subu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bgez	$2,$L163
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L189
	nor	$2,$0,$2
	.set	macro
	.set	reorder

$L163:
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
$L189:
	sh	$2,10($16)
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$5,48($17)
	lw	$6,52($17)
	.set	noreorder
	.set	nomacro
	jal	fastpath_set_qos_mark
	move	$4,$21
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	move	$4,$16
	lw	$2,%lo(jiffies)($2)
	move	$5,$19
	move	$6,$21
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_xmit_check
	sw	$2,40($17)
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L85
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	ip_finish_output3
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L85
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

$L101:
	lw	$17,64($17)
$L100:
	bne	$17,$0,$L166
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_inDev
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$19
	lw	$31,84($sp)
	move	$6,$21
	lw	$fp,80($sp)
	lw	$23,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	.set	noreorder
	.set	nomacro
	j	fast_path_post_process_return_check
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

$L83:
	move	$4,$0
$L85:
	lw	$31,84($sp)
	move	$2,$4
	lw	$fp,80($sp)
	lw	$23,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

$L87:
	lw	$2,12($19)
	li	$3,65536			# 0x10000
	and	$3,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L171
	li	$3,393216			# 0x60000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L204
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

$L203:
	.set	noreorder
	.set	nomacro
	bne	$23,$0,$L206
	li	$23,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L85
	move	$4,$0
	.set	macro
	.set	reorder

	.end	enter_fast_path
	.size	enter_fast_path, .-enter_fast_path
	.section	.text.rtl_fp_mark_invalid,"ax",@progbits
	.align	2
	.globl	rtl_fp_mark_invalid
	.set	nomips16
	.ent	rtl_fp_mark_invalid
	.type	rtl_fp_mark_invalid, @function
rtl_fp_mark_invalid:
	.frame	$sp,72,$31		# vars= 8, regs= 10/0, args= 24, gp= 0
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
	li	$5,1			# 0x1
	move	$20,$4
	jal	rtl_get_ct_ip_by_dir
	move	$6,$0

	move	$4,$20
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$23,$2

	move	$4,$20
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$21,$2

	move	$4,$20
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$19,$2

	move	$4,$20
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	move	$17,$2

	move	$4,$20
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	move	$22,$2

	move	$4,$20
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,24($sp)

	li	$6,1			# 0x1
	move	$4,$20
	move	$5,$0
	jal	rtl_get_ct_port_by_dir
	move	$18,$2

	move	$4,$20
	li	$5,1			# 0x1
	jal	rtl_get_ct_protonum
	move	$16,$2

	move	$5,$0
	move	$4,$20
	jal	rtl_get_ct_protonum
	move	$fp,$2

	lw	$7,24($sp)
	move	$4,$23
	move	$5,$22
	sw	$fp,16($sp)
	move	$6,$21
	jal	mark_path_invalid
	move	$20,$2

	move	$4,$19
	sw	$20,16($sp)
	move	$5,$18
	move	$6,$17
	jal	mark_path_invalid
	move	$7,$16

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
	.end	rtl_fp_mark_invalid
	.size	rtl_fp_mark_invalid, .-rtl_fp_mark_invalid
	.section	.text.rtl_br_fdb_time_update,"ax",@progbits
	.align	2
	.globl	rtl_br_fdb_time_update
	.set	nomips16
	.ent	rtl_br_fdb_time_update
	.type	rtl_br_fdb_time_update, @function
rtl_br_fdb_time_update:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	move	$17,$4
	move	$4,$0
	move	$16,$5
	move	$5,$6
	jal	rtl_get_hw_fdb_age
	li	$6,2			# 0x2

	li	$3,300			# 0x12c
	beq	$2,$3,$L212
	li	$3,450			# 0x1c2

	beq	$2,$3,$L213
	li	$3,150			# 0x96

	bne	$2,$3,$L220
	move	$4,$16

	j	$L221
	lui	$2,%hi(jiffies)

$L213:
	lui	$2,%hi(jiffies)
	move	$4,$16
	lw	$5,%lo(jiffies)($2)
	j	$L217
	nop

$L212:
	lui	$2,%hi(jiffies)
	move	$4,$16
	lw	$5,%lo(jiffies)($2)
	j	$L217
	addiu	$5,$5,-15000

$L221:
	lw	$5,%lo(jiffies)($2)
	addiu	$5,$5,-30000
$L217:
	jal	rtl_set_fdb_aging
	nop

	move	$4,$16
$L220:
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	jal	rtl_get_fdb_aging
	nop

	move	$4,$17
	jal	rtl_hold_time
	move	$18,$2

	li	$3,-1			# 0xffffffff
	addu	$2,$2,$18
	subu	$2,$16,$2
	slt	$2,$2,0
	lw	$31,28($sp)
	movn	$3,$0,$2
	lw	$18,24($sp)
	move	$2,$3
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	rtl_br_fdb_time_update
	.size	rtl_br_fdb_time_update, .-rtl_br_fdb_time_update
	.section	.text.rtk_idleNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_idleNaptConnection
	.set	nomips16
	.ent	rtk_idleNaptConnection
	.type	rtk_idleNaptConnection, @function
rtk_idleNaptConnection:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$15,6			# 0x6
	lw	$3,4($4)
	li	$2,17			# 0x11
	lw	$6,20($4)
	li	$8,-16777216			# 0xff000000
	lw	$7,24($4)
	srl	$11,$3,8
	xor	$14,$6,$3
	lw	$10,0($4)
	xor	$14,$14,$7
	lw	$9,8($4)
	xor	$14,$14,$11
	lui	$11,%hi(path_table_list_max)
	lw	$13,%lo(path_table_list_max)($11)
	lui	$11,%hi(table_path)
	srl	$12,$6,16
	lw	$11,%lo(table_path)($11)
	xor	$14,$14,$12
	xori	$24,$10,0x2
	addiu	$13,$13,-1
	lw	$12,0($11)
	srl	$11,$9,4
	movz	$2,$15,$24
	xor	$14,$14,$11
	lui	$11,%hi(jiffies)
	xor	$14,$14,$2
	lw	$11,%lo(jiffies)($11)
	srl	$15,$14,12
	xor	$14,$15,$14
	li	$15,12			# 0xc
	and	$14,$14,$13
	mult	$14,$15
	ori	$15,$8,0x100
	mflo	$14
	addu	$14,$12,$14
	lw	$14,0($14)
	j	$L225
	ori	$8,$8,0xff00

$L229:
	lw	$24,4($14)
	bne	$24,$10,$L226
	nop

	lw	$24,0($14)
	and	$24,$24,$8
	bne	$24,$15,$L226
	nop

	lw	$24,16($14)
	bne	$24,$3,$L226
	nop

	lw	$24,12($14)
	bne	$24,$9,$L226
	nop

	lw	$25,24($14)
	lw	$24,12($4)
	bne	$25,$24,$L226
	nop

	lw	$25,28($14)
	lw	$24,16($4)
	bne	$25,$24,$L226
	nop

	lw	$24,20($14)
	bne	$24,$6,$L226
	nop

	lw	$24,8($14)
	bne	$24,$7,$L226
	nop

	lw	$8,40($14)
	subu	$15,$11,$5
	subu	$15,$15,$8
	bgez	$15,$L227
	nop

	lw	$4,56($14)
	j	$L235
	addu	$5,$5,$8

$L226:
	lw	$14,64($14)
$L225:
	bne	$14,$0,$L229
	nop

$L227:
	lw	$8,12($4)
	srl	$25,$6,8
	lw	$14,16($4)
	srl	$24,$7,4
	li	$4,-16777216			# 0xff000000
	xor	$15,$8,$6
	xor	$15,$15,$14
	xor	$15,$15,$25
	srl	$25,$8,16
	xor	$15,$15,$25
	xor	$15,$15,$24
	xor	$2,$15,$2
	srl	$15,$2,12
	xor	$2,$15,$2
	li	$15,12			# 0xc
	and	$13,$2,$13
	mult	$13,$15
	ori	$2,$4,0x200
	mflo	$13
	addu	$12,$12,$13
	lw	$12,0($12)
	j	$L230
	ori	$4,$4,0xff00

$L233:
	lw	$13,4($12)
	bne	$13,$10,$L231
	nop

	lw	$13,0($12)
	and	$13,$13,$4
	bne	$13,$2,$L231
	nop

	lw	$13,20($12)
	bne	$13,$8,$L231
	nop

	lw	$13,8($12)
	bne	$13,$14,$L231
	nop

	lw	$13,24($12)
	bne	$13,$6,$L231
	nop

	lw	$13,28($12)
	bne	$13,$7,$L231
	nop

	lw	$13,32($12)
	bne	$13,$3,$L231
	nop

	lw	$13,36($12)
	bne	$13,$9,$L231
	nop

	lw	$2,40($12)
	subu	$11,$11,$5
	subu	$11,$11,$2
	bgez	$11,$L232
	nop

	lw	$4,56($12)
	addu	$5,$5,$2
$L235:
	jal	rtl_set_ct_timeout_expires
	nop

	j	$L228
	li	$2,-1			# 0xffffffff

$L231:
	lw	$12,64($12)
$L230:
	bne	$12,$0,$L233
	nop

$L232:
	move	$2,$0
$L228:
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	rtk_idleNaptConnection
	.size	rtk_idleNaptConnection, .-rtk_idleNaptConnection
	.section	.text.rtl_fpTimer_update,"ax",@progbits
	.align	2
	.globl	rtl_fpTimer_update
	.set	nomips16
	.ent	rtl_fpTimer_update
	.type	rtl_fpTimer_update, @function
rtl_fpTimer_update:
	.frame	$sp,72,$31		# vars= 32, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lui	$2,%hi(jiffies)
	move	$16,$4
	move	$5,$0
	lw	$19,%lo(jiffies)($2)
	jal	rtl_get_ct_protonum
	nop

	li	$3,17			# 0x11
	bne	$2,$3,$L237
	move	$4,$16

	jal	rtl_get_ct_udp_status
	move	$4,$16

	andi	$2,$2,0x2
	beq	$2,$0,$L238
	lui	$2,%hi(nf_ct_udp_timeout)

	lui	$2,%hi(nf_ct_udp_timeout_stream)
	lw	$17,%lo(nf_ct_udp_timeout_stream)($2)
	j	$L239
	li	$18,1			# 0x1

$L238:
	lw	$17,%lo(nf_ct_udp_timeout)($2)
	j	$L239
	li	$18,1			# 0x1

$L237:
	jal	rtl_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	bne	$2,$3,$L243
	li	$2,-1			# 0xffffffff

	jal	rtl_get_ct_tcp_state
	move	$4,$16

	sltu	$2,$2,6
	beq	$2,$0,$L243
	li	$2,-1			# 0xffffffff

	move	$4,$16
	jal	rtl_tcp_get_timeouts
	li	$18,2			# 0x2

	move	$17,$2
$L239:
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$20,$2

	bne	$20,$2,$L241
	li	$6,1			# 0x1

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$18,16($sp)

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,20($sp)

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	sw	$2,24($sp)

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	sw	$2,28($sp)

	move	$4,$16
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$2,32($sp)

	move	$4,$16
	j	$L245
	move	$5,$0

$L241:
	move	$5,$0
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$20,$2

	bne	$20,$2,$L243
	li	$2,-1			# 0xffffffff

	move	$4,$16
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$18,16($sp)

	move	$4,$16
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,20($sp)

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	sw	$2,24($sp)

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	sw	$2,28($sp)

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$2,32($sp)

	move	$4,$16
	li	$5,1			# 0x1
$L245:
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,36($sp)

	addiu	$4,$sp,16
	move	$5,$17
	jal	rtk_idleNaptConnection
	sw	$2,40($sp)

	beq	$2,$0,$L240
	addu	$5,$17,$19

	jal	rtl_check_for_acc
	move	$4,$16

	j	$L243
	move	$2,$0

$L240:
	li	$2,-1			# 0xffffffff
$L243:
	lw	$31,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
	.end	rtl_fpTimer_update
	.size	rtl_fpTimer_update, .-rtl_fpTimer_update
	.section	.text.rtk_delNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_delNaptConnection
	.set	nomips16
	.ent	rtk_delNaptConnection
	.type	rtk_delNaptConnection, @function
rtk_delNaptConnection:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	li	$2,6			# 0x6
	move	$16,$4
	lw	$3,0($4)
	li	$18,17			# 0x11
	xori	$3,$3,0x2
	movz	$18,$2,$3
#APP
 # 1182 "net/rtl/fastpath/fastpath_core.c" 1
	raw_local_irq_save	$17
 # 0 "" 2
#NO_APP
	lw	$4,20($4)
	jal	filter_delconnect
	lui	$9,%hi(napt_table_list_max)
	lw	$3,4($16)
	lw	$6,8($16)
	lw	$2,20($16)
	srl	$7,$3,16
	lw	$5,24($16)
	xor	$8,$6,$3
	xor	$8,$8,$2
	lw	$4,0($16)
	xor	$8,$8,$5
	lw	$10,%lo(napt_table_list_max)($9)
	xor	$8,$8,$7
	lui	$7,%hi(table_napt)
	srl	$11,$2,16
	addiu	$10,$10,-1
	andi	$9,$4,0xffff
	lw	$7,%lo(table_napt)($7)
	xor	$8,$8,$11
	xor	$8,$8,$9
	lw	$7,0($7)
	srl	$9,$8,12
	xor	$9,$9,$8
	and	$9,$9,$10
	li	$10,12			# 0xc
	mult	$9,$10
	mflo	$9
	addu	$7,$7,$9
	lw	$8,0($7)
	j	$L249
$L273:
	lw	$7,4($8)
	bne	$7,$4,$L250
	lw	$7,8($8)
	bne	$7,$3,$L250
	lw	$7,12($8)
	bne	$7,$6,$L250
	lw	$10,16($8)
	lw	$7,12($16)
	bne	$10,$7,$L250
	lw	$10,20($8)
	lw	$7,16($16)
	bne	$10,$7,$L250
	lw	$7,24($8)
	bne	$7,$2,$L250
	lw	$7,28($8)
	bne	$7,$5,$L250
	lw	$3,36($8)
	sb	$0,0($8)
	lw	$2,40($8)
	beq	$3,$0,$L251
	.set	noreorder
	.set	nomacro
	j	$L252
	sw	$2,40($3)
	.set	macro
	.set	reorder

$L251:
	lui	$3,%hi(table_napt)
	lw	$3,%lo(table_napt)($3)
	lw	$3,0($3)
	addu	$3,$3,$9
	sw	$2,4($3)
$L252:
	lw	$2,40($8)
	lw	$3,36($8)
	sw	$3,0($2)
	lui	$2,%hi(table_napt)
	lw	$2,%lo(table_napt)($2)
	lw	$2,0($2)
	addu	$9,$2,$9
	lw	$2,8($9)
	addiu	$2,$2,-1
	sw	$2,8($9)
	lw	$3,44($8)
	lw	$2,48($8)
	beq	$3,$0,$L253
	.set	noreorder
	.set	nomacro
	j	$L254
	sw	$2,48($3)
	.set	macro
	.set	reorder

$L253:
	lui	$3,%hi(napt_list_inuse+4)
	sw	$2,%lo(napt_list_inuse+4)($3)
$L254:
	lw	$2,48($8)
	lui	$3,%hi(napt_list_inuse)
	lw	$5,44($8)
	addiu	$3,$3,%lo(napt_list_inuse)
	addiu	$4,$8,44
	sw	$5,0($2)
	lui	$2,%hi(napt_list_free)
	lw	$5,8($3)
	addiu	$2,$2,%lo(napt_list_free)
	addiu	$5,$5,-1
	sw	$5,8($3)
	sw	$0,44($8)
	lw	$3,4($2)
	sw	$3,48($8)
	lw	$3,4($2)
	sw	$8,0($3)
	lui	$8,%hi(path_table_list_max)
	lw	$3,8($2)
	sw	$4,4($2)
	addiu	$3,$3,1
	sw	$3,8($2)
	lw	$3,4($16)
	lw	$2,20($16)
	lw	$5,24($16)
	srl	$6,$3,8
	xor	$9,$2,$3
	lw	$4,8($16)
	xor	$9,$9,$5
	lw	$10,%lo(path_table_list_max)($8)
	xor	$9,$9,$6
	lui	$6,%hi(table_path)
	srl	$11,$2,16
	addiu	$10,$10,-1
	srl	$8,$4,4
	lw	$6,%lo(table_path)($6)
	xor	$9,$9,$11
	xor	$9,$9,$8
	lw	$6,0($6)
	xor	$9,$9,$18
	srl	$8,$9,12
	xor	$8,$8,$9
	and	$8,$8,$10
	li	$10,12			# 0xc
	mult	$8,$10
	mflo	$8
	addu	$6,$6,$8
	lw	$6,0($6)
	.set	noreorder
	.set	nomacro
	j	$L255
	li	$7,1			# 0x1
	.set	macro
	.set	reorder

$L262:
	lw	$10,4($6)
	lw	$9,0($16)
	bne	$10,$9,$L256
	lbu	$9,2($6)
	bne	$9,$7,$L256
	lw	$9,16($6)
	bne	$9,$3,$L256
	lw	$9,12($6)
	bne	$9,$4,$L256
	lw	$10,24($6)
	lw	$9,12($16)
	bne	$10,$9,$L256
	lw	$10,28($6)
	lw	$9,16($16)
	bne	$10,$9,$L256
	lw	$9,20($6)
	bne	$9,$2,$L256
	lw	$9,8($6)
	bne	$9,$5,$L256
	lw	$3,64($6)
	sb	$0,0($6)
	lw	$2,68($6)
	beq	$3,$0,$L257
	.set	noreorder
	.set	nomacro
	j	$L258
	sw	$2,68($3)
	.set	macro
	.set	reorder

$L257:
	lui	$3,%hi(table_path)
	lw	$3,%lo(table_path)($3)
	lw	$3,0($3)
	addu	$3,$3,$8
	sw	$2,4($3)
$L258:
	lw	$2,68($6)
	lw	$3,64($6)
	sw	$3,0($2)
	lui	$2,%hi(table_path)
	lw	$2,%lo(table_path)($2)
	lw	$2,0($2)
	addu	$8,$2,$8
	lw	$2,8($8)
	addiu	$2,$2,-1
	sw	$2,8($8)
	lw	$3,72($6)
	lw	$2,76($6)
	beq	$3,$0,$L259
	.set	noreorder
	.set	nomacro
	j	$L260
	sw	$2,76($3)
	.set	macro
	.set	reorder

$L259:
	lui	$3,%hi(path_list_inuse+4)
	sw	$2,%lo(path_list_inuse+4)($3)
$L260:
	lw	$2,76($6)
	lui	$3,%hi(path_list_inuse)
	lw	$5,72($6)
	addiu	$3,$3,%lo(path_list_inuse)
	addiu	$4,$6,72
	sw	$5,0($2)
	lui	$2,%hi(path_list_free)
	lw	$5,8($3)
	addiu	$2,$2,%lo(path_list_free)
	addiu	$5,$5,-1
	sw	$5,8($3)
	sw	$0,72($6)
	lw	$3,4($2)
	sw	$3,76($6)
	lw	$3,4($2)
	sw	$6,0($3)
	lw	$3,8($2)
	sw	$4,4($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L261
	sw	$3,8($2)
	.set	macro
	.set	reorder

$L256:
	lw	$6,64($6)
$L255:
	bne	$6,$0,$L262
$L261:
	lw	$3,20($16)
	lui	$8,%hi(path_table_list_max)
	lw	$2,12($16)
	lw	$5,16($16)
	srl	$6,$3,8
	xor	$9,$2,$3
	lw	$4,24($16)
	xor	$9,$9,$5
	lw	$8,%lo(path_table_list_max)($8)
	xor	$9,$9,$6
	lui	$6,%hi(table_path)
	srl	$10,$2,16
	addiu	$8,$8,-1
	xor	$9,$9,$10
	lw	$6,%lo(table_path)($6)
	srl	$10,$4,4
	xor	$9,$9,$10
	lw	$6,0($6)
	xor	$18,$9,$18
	srl	$9,$18,12
	xor	$18,$9,$18
	and	$18,$18,$8
	li	$8,12			# 0xc
	mult	$18,$8
	mflo	$18
	addu	$6,$6,$18
	lw	$6,0($6)
	.set	noreorder
	.set	nomacro
	j	$L263
	li	$7,2			# 0x2
	.set	macro
	.set	reorder

$L270:
	lw	$9,4($6)
	lw	$8,0($16)
	bne	$9,$8,$L264
	lbu	$8,2($6)
	bne	$8,$7,$L264
	lw	$8,20($6)
	bne	$8,$2,$L264
	lw	$8,8($6)
	bne	$8,$5,$L264
	lw	$8,24($6)
	bne	$8,$3,$L264
	lw	$8,28($6)
	bne	$8,$4,$L264
	lw	$9,32($6)
	lw	$8,4($16)
	bne	$9,$8,$L264
	lw	$9,36($6)
	lw	$8,8($16)
	bne	$9,$8,$L264
	lw	$3,64($6)
	sb	$0,0($6)
	lw	$2,68($6)
	beq	$3,$0,$L265
	.set	noreorder
	.set	nomacro
	j	$L266
	sw	$2,68($3)
	.set	macro
	.set	reorder

$L265:
	lui	$3,%hi(table_path)
	lw	$3,%lo(table_path)($3)
	lw	$3,0($3)
	addu	$3,$3,$18
	sw	$2,4($3)
$L266:
	lw	$2,68($6)
	lw	$3,64($6)
	sw	$3,0($2)
	lui	$2,%hi(table_path)
	lw	$2,%lo(table_path)($2)
	lw	$2,0($2)
	addu	$18,$2,$18
	lw	$2,8($18)
	addiu	$2,$2,-1
	sw	$2,8($18)
	lw	$3,72($6)
	lw	$2,76($6)
	beq	$3,$0,$L267
	.set	noreorder
	.set	nomacro
	j	$L268
	sw	$2,76($3)
	.set	macro
	.set	reorder

$L267:
	lui	$3,%hi(path_list_inuse+4)
	sw	$2,%lo(path_list_inuse+4)($3)
$L268:
	lw	$2,76($6)
	lui	$3,%hi(path_list_inuse)
	lw	$5,72($6)
	addiu	$3,$3,%lo(path_list_inuse)
	addiu	$4,$6,72
	sw	$5,0($2)
	lui	$2,%hi(path_list_free)
	lw	$5,8($3)
	addiu	$2,$2,%lo(path_list_free)
	addiu	$5,$5,-1
	sw	$5,8($3)
	sw	$0,72($6)
	lw	$3,4($2)
	sw	$3,76($6)
	lw	$3,4($2)
	sw	$6,0($3)
	lw	$3,8($2)
	sw	$4,4($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L269
	sw	$3,8($2)
	.set	macro
	.set	reorder

$L264:
	lw	$6,64($6)
$L263:
	bne	$6,$0,$L270
$L269:
	andi	$2,$17,0x1
	bne	$2,$0,$L271
#APP
 # 134 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/irqflags.h" 1
	raw_local_irq_restore	$17
 # 0 "" 2
#NO_APP
	j	$L272
$L271:
#APP
 # 134 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/irqflags.h" 1
	raw_local_irq_restore	$17
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	j	$L272
	move	$2,$0
	.set	macro
	.set	reorder

$L250:
	lw	$8,36($8)
$L249:
	bne	$8,$0,$L273
#APP
 # 134 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/irqflags.h" 1
	raw_local_irq_restore	$17
 # 0 "" 2
#NO_APP
	li	$2,-4			# 0xfffffffc
$L272:
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	rtk_delNaptConnection
	.size	rtk_delNaptConnection, .-rtk_delNaptConnection
	.section	.text.rtk_addNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_addNaptConnection
	.set	nomips16
	.ent	rtk_addNaptConnection
	.type	rtk_addNaptConnection, @function
rtk_addNaptConnection:
	.frame	$sp,152,$31		# vars= 96, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-152
	sw	$31,148($sp)
	sw	$fp,144($sp)
	sw	$23,140($sp)
	sw	$22,136($sp)
	sw	$21,132($sp)
	sw	$20,128($sp)
	sw	$19,124($sp)
	sw	$18,120($sp)
	sw	$17,116($sp)
	sw	$16,112($sp)
	move	$17,$5
	move	$16,$4
	move	$23,$6
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L344
	move	$18,$7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L344
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lw	$3,0($16)
	lw	$4,20($16)
	li	$fp,17			# 0x11
	xori	$3,$3,0x2
	.set	noreorder
	.set	nomacro
	jal	filter_addconnect
	movz	$fp,$2,$3
	.set	macro
	.set	reorder

#APP
 # 784 "net/rtl/fastpath/fastpath_core.c" 1
	raw_local_irq_save	$2
 # 0 "" 2
#NO_APP
	sw	$2,56($sp)
	lui	$8,%hi(napt_table_list_max)
	lw	$4,4($16)
	lw	$6,8($16)
	lw	$3,20($16)
	srl	$7,$4,16
	lw	$5,24($16)
	xor	$9,$6,$4
	xor	$9,$9,$3
	lw	$2,0($16)
	xor	$9,$9,$5
	lw	$10,%lo(napt_table_list_max)($8)
	xor	$9,$9,$7
	lui	$7,%hi(table_napt)
	srl	$11,$3,16
	addiu	$10,$10,-1
	andi	$8,$2,0xffff
	lw	$7,%lo(table_napt)($7)
	xor	$9,$9,$11
	xor	$9,$9,$8
	lw	$7,0($7)
	srl	$8,$9,12
	xor	$8,$8,$9
	and	$8,$8,$10
	li	$10,12			# 0xc
	mult	$8,$10
	mflo	$8
	addu	$7,$7,$8
	lw	$7,0($7)
	j	$L280
$L286:
	lw	$9,4($7)
	bne	$9,$2,$L281
	lw	$9,8($7)
	bne	$9,$4,$L281
	lw	$9,12($7)
	bne	$9,$6,$L281
	lw	$10,16($7)
	lw	$9,12($16)
	bne	$10,$9,$L281
	lw	$10,20($7)
	lw	$9,16($16)
	bne	$10,$9,$L281
	lw	$9,24($7)
	bne	$9,$3,$L281
	lw	$9,28($7)
	bne	$9,$5,$L281
	lb	$3,212($23)
	li	$2,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L282
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$fp,$2,$L283
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$fp,$2,$L282
$L283:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$23
	.set	macro
	.set	reorder

	sb	$2,212($23)
$L282:
	lw	$3,56($sp)
	andi	$2,$3,0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L345
	move	$2,$3
	.set	macro
	.set	reorder

	lw	$2,56($sp)
$L345:
#APP
 # 134 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/irqflags.h" 1
	raw_local_irq_restore	$2
 # 0 "" 2
#NO_APP
	.set	noreorder
	.set	nomacro
	j	$L285
	move	$3,$0
	.set	macro
	.set	reorder

$L281:
	lw	$7,36($7)
$L280:
	bne	$7,$0,$L286
	lui	$4,%hi(napt_list_free)
	lw	$3,%lo(napt_list_free)($4)
	beq	$3,$0,$L287
	sw	$2,4($3)
	lw	$2,4($16)
	lw	$5,44($3)
	sw	$2,8($3)
	lw	$6,8($16)
	lw	$2,48($3)
	sw	$6,12($3)
	lw	$6,12($16)
	sw	$6,16($3)
	lw	$6,16($16)
	sw	$6,20($3)
	lw	$6,20($16)
	sw	$6,24($3)
	lw	$6,24($16)
	sw	$18,32($3)
	sw	$6,28($3)
	li	$6,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L288
	sb	$6,0($3)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L289
	sw	$2,48($5)
	.set	macro
	.set	reorder

$L288:
	addiu	$4,$4,%lo(napt_list_free)
	sw	$2,4($4)
$L289:
	lw	$2,48($3)
	lui	$5,%hi(napt_list_free)
	lw	$4,44($3)
	addiu	$5,$5,%lo(napt_list_free)
	addiu	$7,$3,44
	addiu	$6,$3,36
	sw	$4,0($2)
	lui	$2,%hi(napt_list_inuse)
	lw	$9,8($5)
	addiu	$2,$2,%lo(napt_list_inuse)
	lui	$4,%hi(table_napt)
	addiu	$9,$9,-1
	sw	$9,8($5)
	sw	$0,44($3)
	lw	$5,4($2)
	sw	$5,48($3)
	lw	$5,4($2)
	sw	$3,0($5)
	lw	$5,8($2)
	sw	$7,4($2)
	addiu	$5,$5,1
	sw	$5,8($2)
	sw	$0,36($3)
	lw	$2,%lo(table_napt)($4)
	lw	$2,0($2)
	addu	$2,$2,$8
	lw	$2,4($2)
	sw	$2,40($3)
	lw	$2,%lo(table_napt)($4)
	lw	$2,0($2)
	addu	$2,$2,$8
	lw	$2,4($2)
	sw	$3,0($2)
	lw	$2,%lo(table_napt)($4)
	lw	$2,0($2)
	addu	$2,$2,$8
	sw	$6,4($2)
	lw	$2,%lo(table_napt)($4)
	lw	$2,0($2)
	addu	$8,$2,$8
	lw	$2,8($8)
	addiu	$2,$2,1
	sw	$2,8($8)
	li	$2,33			# 0x21
	sb	$2,17($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl865x_getLanDev
	sb	$0,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl865x_getWanDev
	move	$20,$2
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	move	$19,$2
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,68($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	lbu	$22,0($2)
	move	$21,$2
	andi	$22,$22,0xf
	sll	$22,$22,2
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	addu	$22,$2,$22
	.set	macro
	.set	reorder

	li	$6,6			# 0x6
	addiu	$5,$2,6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	li	$6,6			# 0x6
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,28
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	sw	$2,84($sp)
	lw	$5,12($21)
	sw	$5,100($sp)
	lhu	$2,0($22)
	sw	$2,92($sp)
	lw	$3,16($21)
	sw	$3,96($sp)
	lhu	$5,2($22)
	.set	noreorder
	.set	nomacro
	jal	rtl_store_skb_dst
	sw	$5,88($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi(path_list_free)
	lw	$18,%lo(path_list_free)($2)
	lui	$2,%hi(path_table_list_max)
	lw	$2,%lo(path_table_list_max)($2)
	sw	$2,80($sp)
	lw	$2,4($16)
	sw	$2,64($sp)
	lw	$3,8($16)
	sw	$3,76($sp)
	lw	$4,20($16)
	sw	$4,60($sp)
	lw	$5,24($16)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L290
	sw	$5,72($sp)
	.set	macro
	.set	reorder

	lw	$2,0($16)
	move	$4,$0
	sw	$2,4($18)
	lw	$6,4($16)
	sw	$6,16($18)
	lw	$2,8($16)
	sw	$2,12($18)
	lw	$2,20($16)
	sw	$2,20($18)
	lw	$2,24($16)
	sw	$2,8($18)
	lw	$5,12($16)
	sw	$5,24($18)
	lw	$2,16($16)
	sw	$2,28($18)
	lw	$8,20($16)
	sw	$8,32($18)
	lw	$2,24($16)
	sw	$2,36($18)
	lui	$2,%hi(route_list_inuse)
	lw	$2,%lo(route_list_inuse)($2)
	.set	noreorder
	.set	nomacro
	j	$L291
	move	$7,$0
	.set	macro
	.set	reorder

$L294:
	lw	$3,8($2)
	sltu	$9,$3,$4
	.set	noreorder
	.set	nomacro
	bne	$9,$0,$L292
	and	$10,$3,$8
	.set	macro
	.set	reorder

	lw	$9,4($2)
	bne	$10,$9,$L292
	.set	noreorder
	.set	nomacro
	j	$L293
	addiu	$7,$2,16
	.set	macro
	.set	reorder

$L292:
	move	$3,$4
$L293:
	lw	$2,44($2)
	move	$4,$3
$L291:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L294
	li	$3,-1			# 0xffffffff
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	sw	$7,44($18)
	sb	$3,0($18)
	sb	$2,2($18)
	.set	noreorder
	.set	nomacro
	beq	$6,$5,$L295
	sb	$0,1($18)
	.set	macro
	.set	reorder

	sb	$2,1($18)
$L295:
	lw	$3,12($18)
	lw	$2,28($18)
	beq	$3,$2,$L296
	lbu	$2,1($18)
	ori	$2,$2,0x2
	sb	$2,1($18)
$L296:
	sw	$0,48($18)
	addiu	$3,$sp,36
	sw	$0,52($18)
	move	$4,$3
	move	$5,$0
	lw	$2,4($16)
	li	$6,14			# 0xe
	sw	$2,12($21)
	lw	$2,8($16)
	sh	$2,0($22)
	lw	$2,20($16)
	sw	$2,16($21)
	lw	$2,24($16)
	sh	$2,2($22)
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$3,104($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$3,104($sp)
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L297
	move	$6,$3
	.set	macro
	.set	reorder

	lw	$4,4($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	move	$5,$20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L297
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$3,$2
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L298
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	sw	$3,104($sp)
	.set	macro
	.set	reorder

	lw	$3,104($sp)
	bne	$2,$3,$L298
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,12($2)
	lbu	$2,13($2)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L298
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffea
	.set	macro
	.set	reorder

$L298:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	li	$6,6			# 0x6
	addiu	$4,$2,6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,36
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dmac
	move	$5,$20
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	lw	$3,68($sp)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L297
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L299
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$0
	move	$6,$20
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$7,$19
	.set	macro
	.set	reorder

$L299:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L300
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L348
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L300:
	lw	$2,48($18)
	sw	$2,48($18)
$L348:
	lui	$2,%hi(nf_hooks+160)
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L302
	move	$4,$17
	.set	macro
	.set	reorder

	li	$5,4			# 0x4
	move	$6,$20
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$7,$19
	.set	macro
	.set	reorder

$L302:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L303
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L297
	sw	$2,52($18)
	.set	macro
	.set	reorder

$L303:
	lw	$2,52($18)
	sw	$2,52($18)
$L297:
	lw	$2,12($16)
	addiu	$3,$sp,36
	move	$4,$3
	move	$5,$0
	li	$6,14			# 0xe
	sw	$2,12($21)
	lw	$2,16($16)
	sh	$2,0($22)
	lw	$2,20($16)
	sw	$2,16($21)
	lw	$2,24($16)
	sh	$2,2($22)
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$3,104($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$3,104($sp)
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L305
	move	$6,$3
	.set	macro
	.set	reorder

	lw	$4,20($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	move	$5,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L351
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$3,$2
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L306
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	sw	$3,104($sp)
	.set	macro
	.set	reorder

	lw	$3,104($sp)
	bne	$2,$3,$L306
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,12($2)
	lbu	$2,13($2)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L306
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffea
	.set	macro
	.set	reorder

$L306:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	li	$6,6			# 0x6
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,36
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_smac
	move	$5,$19
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	lw	$4,68($sp)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L351
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lui	$2,%hi(nf_hooks+160)
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L307
	move	$4,$17
	.set	macro
	.set	reorder

	li	$5,4			# 0x4
	move	$6,$20
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$7,$19
	.set	macro
	.set	reorder

$L307:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L308
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L305
	sw	$2,52($18)
	.set	macro
	.set	reorder

$L308:
	lw	$2,52($18)
	sw	$2,52($18)
$L305:
	li	$2,6			# 0x6
$L351:
	.set	noreorder
	.set	nomacro
	bne	$fp,$2,$L310
	lui	$2,%hi(nf_ct_udp_timeout_stream)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L349
	sw	$2,60($18)
	.set	macro
	.set	reorder

$L310:
	lw	$2,%lo(nf_ct_udp_timeout_stream)($2)
	sw	$2,60($18)
$L349:
	lui	$2,%hi(jiffies)
	sw	$23,56($18)
	lw	$2,%lo(jiffies)($2)
	lw	$3,72($18)
	sw	$2,40($18)
	lw	$2,76($18)
	beq	$3,$0,$L312
	.set	noreorder
	.set	nomacro
	j	$L313
	sw	$2,76($3)
	.set	macro
	.set	reorder

$L312:
	lui	$3,%hi(path_list_free+4)
	sw	$2,%lo(path_list_free+4)($3)
$L313:
	lw	$5,60($sp)
	lui	$8,%hi(path_list_free)
	lw	$3,64($sp)
	addiu	$8,$8,%lo(path_list_free)
	lw	$2,76($18)
	lw	$4,72($18)
	xor	$6,$5,$3
	lw	$5,72($sp)
	srl	$3,$3,8
	xor	$6,$6,$5
	lw	$5,80($sp)
	sw	$4,0($2)
	xor	$6,$6,$3
	lui	$2,%hi(path_list_inuse)
	lw	$4,60($sp)
	addiu	$2,$2,%lo(path_list_inuse)
	lw	$10,8($8)
	addiu	$9,$5,-1
	srl	$3,$4,16
	lw	$4,76($sp)
	addiu	$5,$18,72
	addiu	$10,$10,-1
	xor	$6,$6,$3
	lui	$3,%hi(table_path)
	sw	$10,8($8)
	srl	$7,$4,4
	addiu	$4,$18,64
	sw	$0,72($18)
	xor	$6,$6,$7
	lw	$8,4($2)
	xor	$6,$6,$fp
	srl	$7,$6,12
	sw	$8,76($18)
	xor	$7,$7,$6
	li	$8,12			# 0xc
	lw	$6,4($2)
	and	$7,$7,$9
	mult	$7,$8
	sw	$18,0($6)
	lw	$6,8($2)
	sw	$5,4($2)
	addiu	$5,$6,1
	mflo	$7
	sw	$5,8($2)
	sw	$0,64($18)
	lw	$2,%lo(table_path)($3)
	lw	$2,0($2)
	addu	$2,$2,$7
	lw	$2,4($2)
	sw	$2,68($18)
	lw	$2,%lo(table_path)($3)
	lw	$2,0($2)
	addu	$2,$2,$7
	lw	$2,4($2)
	sw	$18,0($2)
	lw	$2,%lo(table_path)($3)
	lw	$2,0($2)
	addu	$2,$2,$7
	sw	$4,4($2)
	lw	$2,%lo(table_path)($3)
	lw	$2,0($2)
	addu	$7,$2,$7
	lw	$2,8($7)
	addiu	$2,$2,1
	sw	$2,8($7)
$L290:
	lui	$2,%hi(path_list_free)
	lw	$18,%lo(path_list_free)($2)
	lui	$2,%hi(path_table_list_max)
	lw	$2,%lo(path_table_list_max)($2)
	sw	$2,80($sp)
	lw	$2,20($16)
	sw	$2,64($sp)
	lw	$3,24($16)
	sw	$3,76($sp)
	lw	$4,12($16)
	sw	$4,60($sp)
	lw	$5,16($16)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L314
	sw	$5,72($sp)
	.set	macro
	.set	reorder

	lw	$2,0($16)
	move	$5,$0
	sw	$2,4($18)
	lw	$2,20($16)
	sw	$2,16($18)
	lw	$2,24($16)
	sw	$2,12($18)
	lw	$6,12($16)
	sw	$6,20($18)
	lw	$2,16($16)
	sw	$2,8($18)
	lw	$2,20($16)
	sw	$2,24($18)
	lw	$2,24($16)
	sw	$2,28($18)
	lw	$4,4($16)
	sw	$4,32($18)
	lw	$2,8($16)
	sw	$2,36($18)
	lui	$2,%hi(route_list_inuse)
	lw	$2,%lo(route_list_inuse)($2)
	.set	noreorder
	.set	nomacro
	j	$L315
	move	$7,$0
	.set	macro
	.set	reorder

$L318:
	lw	$3,8($2)
	sltu	$8,$3,$5
	.set	noreorder
	.set	nomacro
	bne	$8,$0,$L316
	and	$9,$3,$4
	.set	macro
	.set	reorder

	lw	$8,4($2)
	bne	$9,$8,$L316
	.set	noreorder
	.set	nomacro
	j	$L317
	addiu	$7,$2,16
	.set	macro
	.set	reorder

$L316:
	move	$3,$5
$L317:
	lw	$2,44($2)
	move	$5,$3
$L315:
	bne	$2,$0,$L318
	li	$2,2			# 0x2
	sb	$2,2($18)
	li	$2,-1			# 0xffffffff
	sw	$7,44($18)
	sb	$2,0($18)
	.set	noreorder
	.set	nomacro
	beq	$6,$4,$L319
	sb	$0,1($18)
	.set	macro
	.set	reorder

	li	$2,4			# 0x4
	sb	$2,1($18)
$L319:
	lw	$3,8($18)
	lw	$2,36($18)
	beq	$3,$2,$L320
	lbu	$2,1($18)
	ori	$2,$2,0x8
	sb	$2,1($18)
$L320:
	sw	$0,48($18)
	addiu	$3,$sp,36
	sw	$0,52($18)
	move	$4,$3
	move	$5,$0
	lw	$2,20($16)
	li	$6,14			# 0xe
	sw	$2,12($21)
	lw	$2,24($16)
	sh	$2,0($22)
	lw	$2,12($16)
	sw	$2,16($21)
	lw	$2,16($16)
	sh	$2,2($22)
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$3,104($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$3,104($sp)
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L321
	move	$6,$3
	.set	macro
	.set	reorder

	lw	$4,20($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	move	$5,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L321
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$3,$2
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L322
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	sw	$3,104($sp)
	.set	macro
	.set	reorder

	lw	$3,104($sp)
	bne	$2,$3,$L322
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,12($2)
	lbu	$2,13($2)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L322
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffea
	.set	macro
	.set	reorder

$L322:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	li	$6,6			# 0x6
	addiu	$4,$2,6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,36
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dmac
	move	$5,$19
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	lw	$3,68($sp)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L323
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L323
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$0
	move	$6,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$7,$20
	.set	macro
	.set	reorder

$L323:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L324
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L321
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L324:
	lw	$2,48($18)
	sw	$2,48($18)
$L321:
	lw	$2,20($16)
	addiu	$3,$sp,36
	move	$4,$3
	move	$5,$0
	li	$6,14			# 0xe
	sw	$2,12($21)
	lw	$2,24($16)
	sh	$2,0($22)
	lw	$2,4($16)
	sw	$2,16($21)
	lw	$2,8($16)
	sh	$2,2($22)
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$3,104($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$3,104($sp)
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L326
	move	$6,$3
	.set	macro
	.set	reorder

	lw	$4,4($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	move	$5,$20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L326
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$16,$2
	li	$2,1			# 0x1
	bne	$16,$2,$L327
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$16,$L327
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,12($2)
	lbu	$2,13($2)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L327
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffea
	.set	macro
	.set	reorder

$L327:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	li	$6,6			# 0x6
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,36
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_smac
	move	$5,$20
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	lw	$4,68($sp)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L326
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L326
	move	$4,$17
	.set	macro
	.set	reorder

	li	$5,4			# 0x4
	move	$6,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$7,$20
	.set	macro
	.set	reorder

$L326:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$2,52($18)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$fp,$2,$L328
	lui	$2,%hi(nf_ct_udp_timeout_stream)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L350
	sw	$2,60($18)
	.set	macro
	.set	reorder

$L328:
	lw	$2,%lo(nf_ct_udp_timeout_stream)($2)
	sw	$2,60($18)
$L350:
	lui	$2,%hi(jiffies)
	sw	$23,56($18)
	lw	$2,%lo(jiffies)($2)
	lw	$3,72($18)
	sw	$2,40($18)
	lw	$2,76($18)
	beq	$3,$0,$L330
	.set	noreorder
	.set	nomacro
	j	$L331
	sw	$2,76($3)
	.set	macro
	.set	reorder

$L330:
	lui	$3,%hi(path_list_free+4)
	sw	$2,%lo(path_list_free+4)($3)
$L331:
	lw	$5,60($sp)
	lui	$8,%hi(path_list_free)
	lw	$3,64($sp)
	addiu	$8,$8,%lo(path_list_free)
	lw	$2,76($18)
	lw	$4,72($18)
	xor	$6,$5,$3
	lw	$5,72($sp)
	srl	$3,$3,8
	xor	$6,$6,$5
	lw	$5,80($sp)
	sw	$4,0($2)
	xor	$6,$6,$3
	lui	$2,%hi(path_list_inuse)
	lw	$4,60($sp)
	addiu	$2,$2,%lo(path_list_inuse)
	lw	$10,8($8)
	addiu	$9,$5,-1
	srl	$3,$4,16
	lw	$4,76($sp)
	addiu	$5,$18,72
	addiu	$10,$10,-1
	xor	$6,$6,$3
	lui	$3,%hi(table_path)
	sw	$10,8($8)
	srl	$7,$4,4
	addiu	$4,$18,64
	sw	$0,72($18)
	xor	$6,$6,$7
	lw	$8,4($2)
	xor	$6,$6,$fp
	srl	$7,$6,12
	sw	$8,76($18)
	xor	$7,$7,$6
	li	$8,12			# 0xc
	lw	$6,4($2)
	and	$7,$7,$9
	mult	$7,$8
	sw	$18,0($6)
	lw	$6,8($2)
	sw	$5,4($2)
	addiu	$5,$6,1
	mflo	$7
	sw	$5,8($2)
	sw	$0,64($18)
	lw	$2,%lo(table_path)($3)
	lw	$2,0($2)
	addu	$2,$2,$7
	lw	$2,4($2)
	sw	$2,68($18)
	lw	$2,%lo(table_path)($3)
	lw	$2,0($2)
	addu	$2,$2,$7
	lw	$2,4($2)
	sw	$18,0($2)
	lw	$2,%lo(table_path)($3)
	lw	$2,0($2)
	addu	$2,$2,$7
	sw	$4,4($2)
	lw	$2,%lo(table_path)($3)
	lw	$2,0($2)
	addu	$7,$2,$7
	lw	$2,8($7)
	addiu	$2,$2,1
	sw	$2,8($7)
$L314:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$5,$sp,20
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	li	$6,6			# 0x6
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,28
	.set	macro
	.set	reorder

	lw	$5,84($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	lw	$2,100($sp)
	sw	$2,12($21)
	lw	$3,92($sp)
	sh	$3,0($22)
	lw	$5,96($sp)
	sw	$5,16($21)
	lw	$2,88($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dst
	sh	$2,2($22)
	.set	macro
	.set	reorder

	beq	$20,$0,$L332
#APP
 # 94 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 96 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	    $2, 452($20)		# atomic_sub		
       nop                                             
	subu	$2, 1					
	sc	    $2, 452($20)					
	beqz	$2, 2f					
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 110 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
$L332:
	beq	$19,$0,$L333
#APP
 # 94 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
 # 96 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	1:	ll	    $2, 452($19)		# atomic_sub		
       nop                                             
	subu	$2, 1					
	sc	    $2, 452($19)					
	beqz	$2, 2f					
	.subsection 2					
2:	b	1b					
	.previous					

 # 0 "" 2
 # 110 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/atomic.h" 1
	   

 # 0 "" 2
#NO_APP
$L333:
	lw	$3,56($sp)
	andi	$2,$3,0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L342
	move	$2,$3
	.set	macro
	.set	reorder

	lw	$2,56($sp)
$L342:
#APP
 # 134 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/irqflags.h" 1
	raw_local_irq_restore	$2
 # 0 "" 2
#NO_APP
	lb	$3,212($23)
	li	$2,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L285
	move	$3,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L347
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

$L287:
	lw	$4,56($sp)
	andi	$2,$4,0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L343
	move	$2,$4
	.set	macro
	.set	reorder

	lw	$2,56($sp)
$L343:
#APP
 # 134 "/home198/backup/backup1/qian_yu/3.2/9xD/linux-2.6.30/arch/rlx/include/asm/irqflags.h" 1
	raw_local_irq_restore	$2
 # 0 "" 2
#NO_APP
$L344:
	.set	noreorder
	.set	nomacro
	j	$L285
	li	$3,-1			# 0xffffffff
	.set	macro
	.set	reorder

$L347:
	.set	noreorder
	.set	nomacro
	beq	$fp,$2,$L338
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$fp,$2,$L337
$L338:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$23
	.set	macro
	.set	reorder

	move	$3,$0
	.set	noreorder
	.set	nomacro
	j	$L285
	sb	$2,212($23)
	.set	macro
	.set	reorder

$L337:
	move	$3,$0
$L285:
	lw	$31,148($sp)
	move	$2,$3
	lw	$fp,144($sp)
	lw	$23,140($sp)
	lw	$22,136($sp)
	lw	$21,132($sp)
	lw	$20,128($sp)
	lw	$19,124($sp)
	lw	$18,120($sp)
	lw	$17,116($sp)
	lw	$16,112($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,152
	.set	macro
	.set	reorder

	.end	rtk_addNaptConnection
	.size	rtk_addNaptConnection, .-rtk_addNaptConnection
	.section	.text.udp_fragCache_init,"ax",@progbits
	.align	2
	.globl	udp_fragCache_init
	.set	nomips16
	.ent	udp_fragCache_init
	.type	udp_fragCache_init, @function
udp_fragCache_init:
	.frame	$sp,48,$31		# vars= 8, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L353 = .
$L354 = .
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lui	$2,%hi(malloc_sizes+4)
	move	$17,$4
	lw	$4,%lo(malloc_sizes+4)($2)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$16,%hi(udp_cache_table)

	move	$18,$2
	beq	$2,$0,$L355
	sw	$2,%lo(udp_cache_table)($16)

$L356 = .
	li	$4,12			# 0xc
	lui	$2,%hi(udp_cache_list_free)
	mult	$17,$4
	addiu	$3,$2,%lo(udp_cache_list_free)
	sw	$0,%lo(udp_cache_list_free)($2)
	sw	$3,4($3)
	lui	$2,%hi(max_udp_frag_entry)
	sw	$0,8($3)
	li	$5,32			# 0x20
	mflo	$4
	jal	__kmalloc
	sw	$17,%lo(max_udp_frag_entry)($2)

	move	$3,$0
	sw	$2,0($18)
	lw	$4,%lo(udp_cache_table)($16)
	lw	$4,0($4)
	bne	$4,$0,$L366
	li	$2,-1			# 0xffffffff

	j	$L358
	nop

$L360:
	lw	$5,%lo(udp_cache_table)($16)
	addiu	$3,$3,1
	lw	$5,0($5)
	mflo	$4
	addu	$5,$5,$4
	sw	$0,8($5)
	lw	$5,%lo(udp_cache_table)($16)
	lw	$5,0($5)
	addu	$5,$5,$4
	sw	$0,0($5)
	lw	$5,%lo(udp_cache_table)($16)
	lw	$5,0($5)
	addu	$4,$5,$4
	j	$L365
	sw	$4,4($4)

$L366:
	li	$2,12			# 0xc
$L365:
	slt	$4,$3,$17
	bne	$4,$0,$L360
	mult	$3,$2

	lui	$20,%hi(malloc_sizes)
	lui	$19,%hi(cache_timeout)
	lui	$16,%hi(udp_cache_list_free)
	addiu	$20,$20,%lo(malloc_sizes)
	addiu	$19,$19,%lo(cache_timeout)
	addiu	$16,$16,%lo(udp_cache_list_free)
	j	$L361
	move	$18,$0

$L362:
$L363 = .
	lw	$4,12($20)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	beq	$2,$0,$L355
	addiu	$4,$2,16

	move	$5,$0
	move	$6,$0
	addiu	$18,$18,1
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	sw	$2,32($2)
	addiu	$3,$2,48
	sw	$19,28($2)
	sw	$0,48($2)
	lw	$4,4($16)
	sw	$4,52($2)
	lw	$4,4($16)
	sw	$2,0($4)
	lw	$2,8($16)
	sw	$3,4($16)
	addiu	$2,$2,1
	sw	$2,8($16)
$L361:
	slt	$2,$18,$17
	bne	$2,$0,$L362
	li	$2,1			# 0x1

	j	$L358
	nop

$L355:
	move	$2,$0
$L358:
	lw	$31,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	udp_fragCache_init
	.size	udp_fragCache_init, .-udp_fragCache_init
	.local	napt_table_list_max
	.comm	napt_table_list_max,4,4
	.local	path_table_list_max
	.comm	path_table_list_max,4,4
	.local	max_udp_frag_entry
	.comm	max_udp_frag_entry,4,4
	.globl	route_list_inuse
	.section	.bss,"aw",@nobits
	.align	2
	.type	route_list_inuse, @object
	.size	route_list_inuse, 12
route_list_inuse:
	.space	12
	.globl	route_list_free
	.align	2
	.type	route_list_free, @object
	.size	route_list_free, 12
route_list_free:
	.space	12
	.globl	napt_list_inuse
	.align	2
	.type	napt_list_inuse, @object
	.size	napt_list_inuse, 12
napt_list_inuse:
	.space	12
	.globl	napt_list_free
	.align	2
	.type	napt_list_free, @object
	.size	napt_list_free, 12
napt_list_free:
	.space	12
	.globl	table_napt
	.align	2
	.type	table_napt, @object
	.size	table_napt, 4
table_napt:
	.space	4
	.globl	path_list_inuse
	.align	2
	.type	path_list_inuse, @object
	.size	path_list_inuse, 12
path_list_inuse:
	.space	12
	.globl	path_list_free
	.align	2
	.type	path_list_free, @object
	.size	path_list_free, 12
path_list_free:
	.space	12
	.globl	table_path
	.align	2
	.type	table_path, @object
	.size	table_path, 4
table_path:
	.space	4
	.globl	udp_cache_list_free
	.align	2
	.type	udp_cache_list_free, @object
	.size	udp_cache_list_free, 12
udp_cache_list_free:
	.space	12
	.globl	udp_cache_table
	.align	2
	.type	udp_cache_table, @object
	.size	udp_cache_table, 4
udp_cache_table:
	.space	4
	.ident	"GCC: (GNU) 4.4.5-1.5.5p4"
