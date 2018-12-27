	.file	1 "filter.c"
	.section .mdebug.abi32
	.previous
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
	.globl	fast_path_cusRspTCPEndAck_func
	.data
	.align	2
	.type	fast_path_cusRspTCPEndAck_func, @object
	.size	fast_path_cusRspTCPEndAck_func, 4
fast_path_cusRspTCPEndAck_func:
	.word	0
	.globl	fast_path_cusRspTCPFinAck_func
	.align	2
	.type	fast_path_cusRspTCPFinAck_func, @object
	.size	fast_path_cusRspTCPFinAck_func, 4
fast_path_cusRspTCPFinAck_func:
	.word	0
	.globl	fast_path_cusRsp401_func
	.align	2
	.type	fast_path_cusRsp401_func, @object
	.size	fast_path_cusRsp401_func, 4
fast_path_cusRsp401_func:
	.word	0
	.globl	filter_tlb
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
	.align	2
	.type	res3, @object
	.size	res3, 4
res3:
	.word	0
	.section	.dram-l34-fwd,"aw",@progbits
	.type	url_count, @object
	.size	url_count, 1
url_count:
	.byte	0
	.align	2
	.type	none_dos_drop_pkt_cnt, @object
	.size	none_dos_drop_pkt_cnt, 4
none_dos_drop_pkt_cnt:
	.word	0
	.align	2
	.type	item, @object
	.size	item, 4
item:
	.word	0
	.data
	.align	2
	.type	item_backup, @object
	.size	item_backup, 4
item_backup:
	.word	0
	.align	2
	.type	block, @object
	.size	block, 4
block:
	.word	0
	.align	2
	.type	block_time, @object
	.size	block_time, 4
block_time:
	.word	0
	.align	2
	.type	block_count, @object
	.size	block_count, 4
block_count:
	.word	0
	.align	2
	.type	url_cache, @object
	.size	url_cache, 20
url_cache:
	.half	0
	.space	2
	.word	0
	.word	0
	.half	0
	.byte	0
	.byte	0
	.space	4
	.align	2
	.type	block_message, @object
	.size	block_message, 4
block_message:
	.word	0
	.align	2
	.type	res12, @object
	.size	res12, 4
res12:
	.word	0
	.align	2
	.type	log_enable, @object
	.size	log_enable, 4
log_enable:
	.word	0
	.rdata
	.align	2
$LC0:
	.ascii	"br0\000"
	.align	2
$LC1:
	.ascii	"ppp0\000"
	.align	2
$LC2:
	.ascii	"eth1\000"
	.align	2
$LC3:
	.ascii	"wlan0\000"
	.section	.text.need_filter,"ax",@progbits
	.align	2
	.globl	need_filter
	.ent	need_filter
	.type	need_filter, @function
need_filter:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 16, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$16,32($sp)
	sw	$17,36($sp)
	sw	$31,40($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	lui	$16,%hi($LC0)
	.set	macro
	.set	reorder

	addiu	$4,$16,%lo($LC0)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	move	$17,$2
	.set	macro
	.set	reorder

	beq	$2,$0,$L2
	lw	$2,292($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$5,12($2)
$L60:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L2
	addiu	$2,$16,%lo($LC0)
	.set	macro
	.set	reorder

	addiu	$3,$5,35
#APP
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
#NO_APP
	beq	$4,$0,$L8
	lw	$5,0($5)
	j	$L60
$L8:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L66
	lui	$16,%hi($LC1)
	.set	macro
	.set	reorder

	lw	$3,20($5)
	lw	$2,16($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L1
	move	$4,$0
	.set	macro
	.set	reorder

$L2:
	lui	$16,%hi($LC1)
$L66:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC1)
	.set	macro
	.set	reorder

	beq	$2,$0,$L15
	lw	$2,292($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L28
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$5,12($2)
$L61:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L28
	addiu	$2,$16,%lo($LC1)
	.set	macro
	.set	reorder

	addiu	$3,$5,35
#APP
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
#NO_APP
	beq	$4,$0,$L21
	lw	$5,0($5)
	j	$L61
$L21:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L28
	move	$4,$0
	.set	macro
	.set	reorder

	lw	$3,16($17)
	lw	$2,16($5)
	j	$L65
$L15:
	lui	$16,%hi($LC2)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC2)
	.set	macro
	.set	reorder

	beq	$2,$0,$L29
	lw	$2,292($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L28
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	lw	$5,12($2)
$L62:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L28
	addiu	$2,$16,%lo($LC2)
	.set	macro
	.set	reorder

	addiu	$3,$5,35
#APP
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
#NO_APP
	beq	$4,$0,$L49
	lw	$5,0($5)
	j	$L62
$L29:
	lui	$16,%hi($LC3)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC3)
	.set	macro
	.set	reorder

	beq	$2,$0,$L28
	lw	$2,292($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L28
	sw	$2,28($sp)
	.set	macro
	.set	reorder

	lw	$5,12($2)
$L63:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L28
	addiu	$2,$16,%lo($LC3)
	.set	macro
	.set	reorder

	addiu	$3,$5,35
#APP
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
#NO_APP
	beq	$4,$0,$L49
	lw	$5,0($5)
	j	$L63
$L49:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L28
	move	$4,$0
	.set	macro
	.set	reorder

	lw	$3,16($17)
	lw	$2,20($5)
$L65:
	beq	$3,$2,$L1
$L28:
	li	$4,1			# 0x1
$L1:
	lw	$31,40($sp)
	move	$2,$4
	lw	$17,36($sp)
	lw	$16,32($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,48
	.set	macro
	.set	reorder

	.end	need_filter
	.section	.text.str2hexnum,"ax",@progbits
	.align	2
	.globl	str2hexnum
	.ent	str2hexnum
	.type	str2hexnum, @function
str2hexnum:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lbu	$3,0($4)
	move	$7,$0
$L80:
	beq	$3,$0,$L79
	andi	$5,$3,0x00ff

	addiu	$2,$5,-97
	addiu	$3,$5,-48
	sltu	$6,$2,6
	sltu	$2,$3,10
	beq	$2,$0,$L71
	addiu	$2,$5,-65

	j	$L70
	move	$5,$3

$L71:
	beq	$6,$0,$L73
	sltu	$2,$2,6

	j	$L70
	addiu	$5,$5,-87

$L73:
	bne	$2,$0,$L70
	addiu	$5,$5,-55

	j	$31
	li	$2,-1			# 0xffffffff

$L70:
	addiu	$4,$4,1
	sll	$2,$7,4
	lbu	$3,0($4)
	j	$L80
	addu	$7,$2,$5

$L79:
	j	$31
	move	$2,$7

	.set	macro
	.set	reorder
	.end	str2hexnum
	.section	.text.find_pattern,"ax",@progbits
	.align	2
	.ent	find_pattern
	.type	find_pattern, @function
find_pattern:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 0, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$fp,48($sp)
	sw	$21,36($sp)
	lw	$fp,80($sp)
	lw	$21,76($sp)
	sw	$23,44($sp)
	move	$23,$6
	sw	$22,40($sp)
	move	$22,$5
	sw	$20,32($sp)
	sw	$19,28($sp)
	move	$19,$4
	lb	$20,75($sp)
	sw	$18,24($sp)
	move	$18,$7
	sw	$17,20($sp)
	move	$17,$0
	sw	$16,16($sp)
	move	$16,$0
	sw	$0,0($fp)
	sw	$31,52($sp)
	sw	$0,0($21)
	addu	$4,$19,$17
$L108:
	li	$2,13			# 0xd
	lb	$5,0($4)
	bne	$5,$2,$L85
	andi	$3,$16,0x1

	beq	$3,$0,$L88
	addiu	$16,$16,1

	j	$L104
	move	$16,$0

$L85:
	li	$2,10			# 0xa
	bne	$5,$2,$L89
	addiu	$16,$16,1

	bne	$3,$0,$L107
	slt	$2,$16,4

$L89:
	j	$L104
	move	$16,$0

$L88:
	slt	$2,$16,4
$L107:
	beq	$2,$0,$L81
	move	$2,$0

$L104:
	move	$5,$23
	jal	memcmp
	move	$6,$18

	bne	$2,$0,$L84
	subu	$3,$22,$18

	addu	$2,$17,$18
	move	$5,$0
	sw	$2,0($21)
	move	$3,$2
	addu	$2,$19,$2
	lb	$2,0($2)
$L106:
	beq	$2,$20,$L103
	li	$2,1			# 0x1

	sltu	$2,$22,$3
	addiu	$3,$3,1
	addiu	$5,$5,1
	bne	$2,$0,$L83
	addu	$4,$19,$3

	lb	$2,0($4)
	j	$L106
	nop

$L103:
	j	$L81
	sw	$5,0($fp)

$L84:
	addiu	$17,$17,1
	sltu	$2,$3,$17
	beq	$2,$0,$L108
	addu	$4,$19,$17

$L83:
	move	$2,$0
$L81:
	lw	$31,52($sp)
	lw	$fp,48($sp)
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	find_pattern
	.rdata
	.align	2
$LC4:
	.ascii	"GET \000"
	.align	2
$LC5:
	.ascii	"Host: \000"
	.align	2
$LC6:
	.ascii	"%s\000"
	.section	.text.rtl_url_filter_fastpath,"ax",@progbits
	.align	2
	.globl	rtl_url_filter_fastpath
	.ent	rtl_url_filter_fastpath
	.type	rtl_url_filter_fastpath, @function
rtl_url_filter_fastpath:
	.set	nomips16
	.frame	$sp,88,$31		# vars= 24, regs= 8/0, args= 32, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$16,56($sp)
	move	$16,$4
	move	$4,$5
	sw	$22,80($sp)
	move	$22,$5
	sw	$31,84($sp)
	sw	$21,76($sp)
	sw	$20,72($sp)
	sw	$19,68($sp)
	sw	$18,64($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$17,60($sp)
	.set	macro
	.set	reorder

	sltu	$2,$2,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L109
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$16
	.set	macro
	.set	reorder

	lbu	$3,9($2)
	move	$16,$2
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L109
	move	$4,$0
	.set	macro
	.set	reorder

$L117:
$L158:
	lbu	$2,0($16)
	lui	$3,%hi(malloc_sizes+68)
	lw	$4,%lo(malloc_sizes+68)($3)
	li	$5,208			# 0xd0
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$2,$16,$2
	lw	$3,12($2)
	srl	$3,$3,28
	sll	$3,$3,2
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	addu	$18,$2,$3
	.set	macro
	.set	reorder

	move	$4,$0
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L109
	move	$17,$2
	.set	macro
	.set	reorder

	lbu	$2,0($16)
	lui	$21,%hi($LC4)
	lhu	$3,2($16)
	move	$4,$18
	andi	$2,$2,0xf
	addiu	$5,$21,%lo($LC4)
	sll	$2,$2,3
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcmp
	subu	$16,$3,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L167
	lui	$6,%hi($LC5)
	.set	macro
	.set	reorder

	li	$20,13			# 0xd
	addiu	$19,$sp,40
	addiu	$2,$sp,44
	addiu	$6,$6,%lo($LC5)
	move	$4,$18
	move	$5,$16
	li	$7,6			# 0x6
	sw	$2,24($sp)
	sw	$20,16($sp)
	.set	noreorder
	.set	nomacro
	jal	find_pattern
	sw	$19,20($sp)
	.set	macro
	.set	reorder

	beq	$2,$0,$L167
	lw	$2,40($sp)
	lw	$4,44($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L165
	addu	$3,$18,$2
	.set	macro
	.set	reorder

	move	$2,$17
#APP
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
#NO_APP
$L165:
	lw	$2,44($sp)
	move	$5,$16
	addiu	$6,$21,%lo($LC4)
	move	$4,$18
	addu	$2,$17,$2
	li	$7,4			# 0x4
	sb	$0,0($2)
	addiu	$2,$sp,48
	sw	$20,16($sp)
	sw	$19,20($sp)
	.set	noreorder
	.set	nomacro
	jal	find_pattern
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	beq	$2,$0,$L167
	lw	$2,48($sp)
	addiu	$5,$2,-9
	.set	noreorder
	.set	nomacro
	bgtz	$5,$L166
	sw	$5,48($sp)
	.set	macro
	.set	reorder

$L167:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L109
	move	$4,$0
	.set	macro
	.set	reorder

$L166:
	lw	$3,44($sp)
	lw	$2,40($sp)
	addu	$3,$17,$3
	addu	$2,$18,$2
#APP
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
#NO_APP
	lw	$2,48($sp)
	move	$4,$17
	lw	$3,44($sp)
	move	$19,$0
	addu	$2,$17,$2
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	strlen
	sb	$0,0($2)
	.set	macro
	.set	reorder

	move	$4,$22
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$16,$2
	.set	macro
	.set	reorder

	sltu	$16,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L171
	move	$18,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L170
	addu	$3,$17,$2
	.set	macro
	.set	reorder

$L181:
	.set	noreorder
	.set	nomacro
	j	$L171
	li	$19,1			# 0x1
	.set	macro
	.set	reorder

$L170:
	lb	$2,0($3)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L180
	move	$19,$0
	.set	macro
	.set	reorder

	move	$16,$3
$L177:
	addu	$4,$17,$19
	move	$5,$22
	move	$6,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$16,$16,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L181
	addiu	$19,$19,1
	.set	macro
	.set	reorder

	lb	$2,0($16)
	bne	$2,$0,$L177
$L180:
	move	$19,$0
$L171:
	lui	$4,%hi(log_info)
	lui	$6,%hi($LC6)
	addiu	$4,$4,%lo(log_info)
	addiu	$6,$6,%lo($LC6)
	li	$5,32			# 0x20
	.set	noreorder
	.set	nomacro
	jal	snprintf
	move	$7,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	sltu	$4,$0,$19
$L109:
	lw	$31,84($sp)
	move	$2,$4
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
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

	.end	rtl_url_filter_fastpath
	.section	.text.rtl_mac_filter_fastpath,"ax",@progbits
	.align	2
	.globl	rtl_mac_filter_fastpath
	.ent	rtl_mac_filter_fastpath
	.type	rtl_mac_filter_fastpath, @function
rtl_mac_filter_fastpath:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$19,28($sp)
	move	$19,$4
	sw	$18,24($sp)
	move	$18,$5
	sw	$17,20($sp)
	move	$17,$0
	sw	$31,32($sp)
	sw	$16,16($sp)
	addu	$2,$18,$17
$L192:
	lbu	$16,0($2)
	jal	rtl_skb_mac_header
	move	$4,$19

	addu	$2,$2,$17
	lbu	$2,6($2)
	bne	$16,$2,$L191
	xori	$2,$17,0x6

	addiu	$17,$17,1
	slt	$2,$17,6
	bne	$2,$0,$L192
	addu	$2,$18,$17

	xori	$2,$17,0x6
$L191:
	lw	$31,32($sp)
	sltu	$2,$2,1
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	rtl_mac_filter_fastpath
	.section	.text.rtl_ip_range_filter_fastpath,"ax",@progbits
	.align	2
	.ent	rtl_ip_range_filter_fastpath
	.type	rtl_ip_range_filter_fastpath, @function
rtl_ip_range_filter_fastpath:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$16,16($sp)
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	move	$16,$5

	lw	$5,12($2)
	lw	$2,0($16)
	sltu	$2,$5,$2
	bne	$2,$0,$L194
	li	$3,1			# 0x1

	lw	$2,4($16)
	sltu	$2,$2,$5
	beq	$2,$0,$L193
	nop

$L194:
	move	$3,$0
$L193:
	lw	$31,20($sp)
	move	$2,$3
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	rtl_ip_range_filter_fastpath
	.section	.text.rtl_schedule_filter_fastpath,"ax",@progbits
	.align	2
	.ent	rtl_schedule_filter_fastpath
	.type	rtl_schedule_filter_fastpath, @function
rtl_schedule_filter_fastpath:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	addiu	$4,$sp,16
	sw	$16,24($sp)
	sw	$31,28($sp)
	jal	do_gettimeofday
	move	$16,$5

	li	$6,-1851654144			# 0x91a20000
	lw	$2,16($sp)
	ori	$6,$6,0xb3c5
	li	$7,-2004353024			# 0x88880000
	li	$5,-1037172736			# 0xc22e0000
	mult	$2,$6
	ori	$7,$7,0x8889
	ori	$5,$5,0x4507
	sra	$3,$2,31
	li	$4,715784192			# 0x2aaa0000
	ori	$4,$4,0xaaab
	mfhi	$6
	mult	$2,$7
	addu	$6,$6,$2
	sra	$6,$6,11
	subu	$6,$6,$3
	mfhi	$8
	mult	$2,$5
	addu	$8,$8,$2
	sra	$8,$8,5
	subu	$8,$8,$3
	mfhi	$5
	mult	$6,$4
	addu	$5,$5,$2
	sra	$5,$5,16
	subu	$5,$5,$3
	li	$3,-1840709632			# 0x92490000
	addiu	$5,$5,4
	ori	$3,$3,0x2493
	mfhi	$4
	sra	$2,$6,31
	mult	$8,$7
	sra	$4,$4,2
	subu	$10,$4,$2
	sll	$2,$10,1
	sra	$4,$8,31
	addu	$2,$2,$10
	mfhi	$7
	sll	$2,$2,3
	mult	$5,$3
	addu	$7,$7,$8
	sra	$7,$7,5
	subu	$7,$7,$4
	subu	$10,$6,$2
	sll	$4,$7,4
	sra	$2,$5,31
	mfhi	$3
	subu	$4,$4,$7
	addu	$3,$3,$5
	sll	$4,$4,2
	sra	$3,$3,2
	subu	$7,$8,$4
	subu	$9,$3,$2
	sll	$2,$10,4
	sll	$3,$9,3
	subu	$2,$2,$10
	lw	$6,8($16)
	subu	$3,$3,$9
	sll	$2,$2,2
	subu	$9,$5,$3
	addu	$7,$2,$7
	li	$2,1			# 0x1
	sll	$2,$2,$9
	andi	$4,$6,0x80
	bne	$4,$0,$L197
	and	$3,$6,$2

	beq	$3,$0,$L195
	move	$2,$0

$L197:
	lw	$2,0($16)
	bne	$2,$0,$L200
	sltu	$3,$7,$2

	lw	$2,4($16)
	beq	$2,$0,$L199
	nop

	j	$L202
	sltu	$2,$2,$7

$L200:
	bne	$3,$0,$L195
	move	$2,$0

	lw	$2,4($16)
	sltu	$2,$2,$7
$L202:
	bne	$2,$0,$L196
	nop

$L199:
	j	$L195
	li	$2,1			# 0x1

$L196:
	move	$2,$0
$L195:
	lw	$31,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	rtl_schedule_filter_fastpath
	.section	.text.filter_table_head_init,"ax",@progbits
	.align	2
	.globl	filter_table_head_init
	.ent	filter_table_head_init
	.type	filter_table_head_init, @function
filter_table_head_init:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	move	$5,$0
	sw	$16,16($sp)
	lui	$16,%hi(table_list_head)
	addiu	$4,$16,%lo(table_list_head)
	sw	$31,20($sp)
	jal	memset
	li	$6,32			# 0x20

	addiu	$3,$16,%lo(table_list_head)
	li	$4,1			# 0x1
	lui	$6,%hi(table_list_head+8)
	sw	$4,24($3)
	addiu	$5,$6,%lo(table_list_head+8)
	sw	$3,%lo(table_list_head)($16)
	move	$2,$0
	lw	$31,20($sp)
	lw	$16,16($sp)
	addiu	$sp,$sp,24
	sw	$5,4($5)
	sw	$5,%lo(table_list_head+8)($6)
	sw	$3,4($3)
	sw	$0,16($3)
	sw	$0,20($3)
	j	$31
	sw	$0,28($3)

	.set	macro
	.set	reorder
	.end	filter_table_head_init
	.section	.text.filter_table_regist,"ax",@progbits
	.align	2
	.globl	filter_table_regist
	.ent	filter_table_regist
	.type	filter_table_regist, @function
filter_table_regist:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 8, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$17,28($sp)
	lui	$17,%hi(table_list_head)
	sw	$16,24($sp)
	addiu	$16,$17,%lo(table_list_head)
	lw	$2,24($16)
	sw	$19,36($sp)
	move	$19,$4
	sw	$18,32($sp)
	move	$18,$5
	bne	$2,$0,$L207
	sw	$31,40($sp)

	jal	filter_table_head_init
	nop

$L207:
$L212:
$L253:
	lui	$2,%hi(malloc_sizes+4)
	lw	$4,%lo(malloc_sizes+4)($2)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	li	$3,1			# 0x1
	move	$4,$2
	addiu	$5,$2,8
	sw	$3,24($4)
	move	$2,$0
	sw	$19,16($4)
	sw	$18,28($4)
	sw	$4,0($4)
	sw	$0,20($4)
	sw	$5,8($4)
	lw	$3,%lo(table_list_head)($17)
	sw	$5,4($5)
	sw	$3,0($4)
	sw	$4,4($3)
	sw	$16,4($4)
	lw	$3,24($16)
	lw	$31,40($sp)
	addiu	$3,$3,1
	sw	$4,%lo(table_list_head)($17)
	lw	$19,36($sp)
	sw	$3,24($16)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	filter_table_regist
	.rdata
	.align	2
$LC7:
	.ascii	"type=%d num=%d\n\000"
	.align	2
$LC15:
	.ascii	"unknow table type\n\000"
	.align	2
$LC8:
	.ascii	"index=%d\t\000"
	.align	2
$LC9:
	.ascii	"mac=%02x:%02x:%02x:%02x:%02x:%02x\n\000"
	.align	2
$LC10:
	.ascii	"index=%d\t\t\000"
	.align	2
$LC11:
	.ascii	"url=%s\n\000"
	.align	2
$LC12:
	.ascii	"start=%x\t\000"
	.align	2
$LC13:
	.ascii	"end=%x\t\000"
	.align	2
$LC14:
	.ascii	"day=%x\n\000"
	.align	2
$LC16:
	.ascii	"%s\n\000"
	.section	.text.filter_table_read_proc,"ax",@progbits
	.align	2
	.ent	filter_table_read_proc
	.type	filter_table_read_proc, @function
filter_table_read_proc:
	.set	nomips16
	.frame	$sp,88,$31		# vars= 16, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L265:
$L306:
	addiu	$sp,$sp,-88
	lui	$2,%hi(malloc_sizes)
	sw	$fp,80($sp)
	move	$fp,$4
	sw	$23,76($sp)
	move	$23,$7
	sw	$22,72($sp)
	move	$22,$6
	sw	$19,60($sp)
	sw	$16,48($sp)
	addiu	$16,$2,%lo(malloc_sizes)
	sw	$5,92($sp)
	sw	$31,84($sp)
	sw	$21,68($sp)
	sw	$20,64($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	lw	$4,52($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	move	$19,$2
	beq	$19,$0,$L260
	move	$2,$0

$L314:
$L355:
	lw	$4,44($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	bne	$2,$0,$L358
	move	$17,$2

	jal	kfree
	move	$4,$19

	j	$L260
	move	$2,$0

$L358:
	lui	$21,%hi(table_list_head)
	lw	$18,%lo(table_list_head)($21)
	addiu	$2,$21,%lo(table_list_head)
$L397:
	beq	$18,$2,$L385
	lui	$5,%hi($LC7)

	lw	$7,24($18)
	lw	$6,16($18)
	addiu	$5,$5,%lo($LC7)
	addiu	$7,$7,-1
	jal	sprintf
	move	$4,$19

	li	$2,3			# 0x3
	lw	$3,16($18)
	beq	$3,$2,$L368
	sltu	$2,$3,4

	beq	$2,$0,$L379
	li	$2,4			# 0x4

	li	$2,2			# 0x2
	beq	$3,$2,$L363
	lui	$5,%hi($LC15)

	j	$L398
	move	$4,$17

$L379:
	beq	$3,$2,$L373
	lui	$5,%hi($LC15)

	j	$L378
	move	$4,$17

$L363:
	lw	$16,8($18)
	addiu	$20,$18,8
$L393:
	beq	$16,$20,$L361
	lui	$5,%hi($LC8)

	lw	$6,20($16)
	addiu	$5,$5,%lo($LC8)
	jal	sprintf
	move	$4,$17

	move	$4,$19
	jal	strcat
	move	$5,$17

	lui	$5,%hi($LC9)
	lbu	$2,30($16)
	addiu	$5,$5,%lo($LC9)
	lbu	$6,28($16)
	move	$4,$17
	lbu	$7,29($16)
	sw	$2,16($sp)
	lbu	$2,31($16)
	sw	$2,20($sp)
	lbu	$2,32($16)
	sw	$2,24($sp)
	lbu	$2,33($16)
	jal	sprintf
	sw	$2,28($sp)

	move	$4,$19
	jal	strcat
	move	$5,$17

	lw	$16,0($16)
	j	$L393
	nop

$L368:
	lw	$16,8($18)
	addiu	$20,$18,8
$L394:
	beq	$16,$20,$L361
	lui	$5,%hi($LC10)

	lw	$6,20($16)
	addiu	$5,$5,%lo($LC10)
	jal	sprintf
	move	$4,$17

	move	$4,$19
	jal	strcat
	move	$5,$17

	lui	$5,%hi($LC11)
	addiu	$6,$16,28
	move	$4,$17
	jal	sprintf
	addiu	$5,$5,%lo($LC11)

	move	$4,$19
	jal	strcat
	move	$5,$17

	lw	$16,0($16)
	j	$L394
	nop

$L373:
	lw	$16,8($18)
	addiu	$20,$18,8
$L395:
	beq	$16,$20,$L361
	lui	$5,%hi($LC8)

	lw	$6,20($16)
	addiu	$5,$5,%lo($LC8)
	jal	sprintf
	move	$4,$17

	move	$4,$19
	jal	strcat
	move	$5,$17

	lui	$5,%hi($LC12)
	lw	$6,28($16)
	move	$4,$17
	jal	sprintf
	addiu	$5,$5,%lo($LC12)

	move	$4,$19
	jal	strcat
	move	$5,$17

	lui	$5,%hi($LC13)
	lw	$6,32($16)
	move	$4,$17
	jal	sprintf
	addiu	$5,$5,%lo($LC13)

	move	$4,$19
	jal	strcat
	move	$5,$17

	lui	$5,%hi($LC14)
	lw	$6,36($16)
	move	$4,$17
	jal	sprintf
	addiu	$5,$5,%lo($LC14)

	move	$4,$19
	jal	strcat
	move	$5,$17

	lw	$16,0($16)
	j	$L395
	nop

$L378:
$L398:
	jal	sprintf
	addiu	$5,$5,%lo($LC15)

	move	$4,$19
	jal	strcat
	move	$5,$17

$L361:
	lw	$18,0($18)
	j	$L397
	addiu	$2,$21,%lo(table_list_head)

$L385:
	lui	$5,%hi($LC16)
	move	$4,$fp
	addiu	$5,$5,%lo($LC16)
	jal	sprintf
	move	$6,$19

	move	$16,$2
	addu	$2,$22,$23
	slt	$2,$2,$16
	bne	$2,$0,$L381
	li	$3,1			# 0x1

	lw	$2,104($sp)
	sw	$3,0($2)
$L381:
	subu	$16,$16,$22
	addu	$3,$fp,$22
	slt	$2,$23,$16
	move	$4,$19
	movn	$16,$23,$2		#conditional move
	lw	$2,92($sp)
	sw	$3,0($2)
	slt	$2,$16,0
	jal	kfree
	movn	$16,$0,$2		#conditional move

	jal	kfree
	move	$4,$17

	move	$2,$16
$L260:
	lw	$31,84($sp)
	lw	$fp,80($sp)
	lw	$23,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,88

	.set	macro
	.set	reorder
	.end	filter_table_read_proc
	.section	.text.scan_table,"ax",@progbits
	.align	2
	.globl	scan_table
	.ent	scan_table
	.type	scan_table, @function
scan_table:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 0, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$23,44($sp)
	move	$23,$5
	sw	$22,40($sp)
	move	$22,$7
	sw	$20,32($sp)
	move	$20,$4
	sw	$19,28($sp)
	move	$19,$6
	sw	$18,24($sp)
	move	$18,$0
	sw	$31,48($sp)
	sw	$21,36($sp)
	sw	$17,20($sp)
	beq	$6,$0,$L435
	sw	$16,16($sp)

	lw	$2,28($6)
	beq	$2,$0,$L435
	addiu	$21,$6,8

	lw	$17,8($6)
	beq	$17,$21,$L399
	li	$5,-1			# 0xffffffff

$L428:
	lw	$3,16($17)
	andi	$2,$3,0x1
	bne	$2,$0,$L406
	move	$16,$17

	lw	$2,20($17)
	bne	$23,$2,$L404
	nop

$L406:
	andi	$2,$3,0x200
	beq	$2,$0,$L407
	andi	$2,$3,0x100

	andi	$2,$3,0x2
	lw	$6,0($22)
	beq	$2,$0,$L408
	nop

	beq	$6,$0,$L435
	move	$4,$20

	lw	$5,20($16)
	jal	scan_table
	move	$7,$6

	move	$18,$2
$L408:
	beq	$18,$0,$L404
	li	$5,1			# 0x1

	j	$L399
	nop

$L407:
	bne	$2,$0,$L414
	move	$4,$20

	lw	$2,28($19)
	jal	$2
	addiu	$5,$16,28

	beq	$2,$0,$L404
	nop

$L414:
	lw	$3,16($16)
	andi	$2,$3,0x2
	bne	$2,$0,$L415
	andi	$2,$3,0x1

	j	$L438
	li	$5,1			# 0x1

$L415:
	lw	$6,0($22)
	beq	$6,$0,$L435
	move	$4,$20

	lw	$5,20($16)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L404
	move	$18,$2

	lw	$3,16($16)
	li	$5,1			# 0x1
	andi	$2,$3,0x1
$L438:
	beq	$2,$0,$L399
	andi	$2,$3,0x8

	j	$L399
	sltu	$5,$2,1

$L404:
	lw	$17,0($17)
	bne	$17,$21,$L428
	nop

	beq	$16,$0,$L399
	li	$5,-1			# 0xffffffff

	lw	$3,16($16)
	andi	$2,$3,0x1
	beq	$2,$0,$L399
	move	$5,$0

	srl	$2,$3,3
	j	$L399
	andi	$5,$2,0x1

$L435:
	li	$5,-1			# 0xffffffff
$L399:
	lw	$31,48($sp)
	move	$2,$5
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	scan_table
	.rdata
	.align	2
$LC17:
	.ascii	"[ip_range_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\n\000"
	.align	2
$LC18:
	.ascii	"[mac_filter]%d.%d.%d.%d(%02x:%02x:%02x:%02x:%02x:%02x) ="
	.ascii	"==> %d.%d.%d.%d [Blocked]\n\000"
	.align	2
$LC19:
	.ascii	"[url_key_filter]%d.%d.%d.%d ==> %d.%d.%d.%d(%s)  [Blocke"
	.ascii	"d]\n\000"
	.align	2
$LC20:
	.ascii	"[schedule_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\n\000"
	.section	.text.log_fastFilter,"ax",@progbits
	.align	2
	.globl	log_fastFilter
	.ent	log_fastFilter
	.type	log_fastFilter, @function
log_fastFilter:
	.set	nomips16
	.frame	$sp,176,$31		# vars= 8, regs= 8/0, args= 136, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-176
	sw	$21,164($sp)
	move	$21,$5
	sw	$16,144($sp)
	move	$16,$4
	addiu	$4,$sp,136
	sw	$31,172($sp)
	sw	$22,168($sp)
	sw	$20,160($sp)
	sw	$19,156($sp)
	sw	$18,152($sp)
	jal	do_gettimeofday
	sw	$17,148($sp)

	jal	rtl_ip_hdr
	move	$4,$21

	move	$4,$21
	jal	rtl_eth_hdr
	move	$22,$2

	move	$3,$2
	li	$2,2			# 0x2
	beq	$16,$2,$L443
	slt	$2,$16,3

	beq	$2,$0,$L452
	li	$2,3			# 0x3

	li	$2,1			# 0x1
	beq	$16,$2,$L441
	nop

	j	$L440
	nop

$L452:
	beq	$16,$2,$L446
	li	$2,4			# 0x4

	beq	$16,$2,$L448
	nop

	j	$L440
	nop

$L441:
	beq	$22,$0,$L440
	lui	$4,%hi($LC17)

	lw	$5,12($22)
	j	$L454
	addiu	$4,$4,%lo($LC17)

$L443:
	beq	$3,$0,$L440
	nop

	beq	$22,$0,$L440
	nop

	jal	rtl_eth_hdr
	move	$4,$21

	move	$4,$21
	jal	rtl_eth_hdr
	move	$16,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$17,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$18,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$19,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$20,$2

	lui	$4,%hi($LC18)
	lw	$5,12($22)
	addiu	$4,$4,%lo($LC18)
	andi	$3,$5,0xff
	srl	$6,$5,16
	sw	$3,16($sp)
	srl	$7,$5,8
	lbu	$15,84($16)
	srl	$5,$5,24
	lbu	$9,85($16)
	andi	$6,$6,0x00ff
	lbu	$14,88($16)
	sll	$15,$15,24
	lbu	$8,89($16)
	sll	$9,$9,16
	lbu	$3,92($16)
	sll	$14,$14,24
	lbu	$12,93($16)
	sll	$8,$8,16
	lbu	$13,86($16)
	or	$9,$9,$15
	lbu	$10,90($16)
	or	$8,$8,$14
	lbu	$11,94($16)
	sll	$3,$3,24
	sll	$12,$12,16
	sll	$13,$13,8
	sll	$10,$10,8
	or	$12,$12,$3
	or	$13,$13,$9
	or	$10,$10,$8
	lbu	$9,87($16)
	sll	$11,$11,8
	lbu	$8,91($16)
	or	$11,$11,$12
	lbu	$3,95($16)
	or	$9,$9,$13
	or	$8,$8,$10
	andi	$7,$7,0x00ff
	or	$3,$3,$11
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	lbu	$3,96($16)
	sb	$3,32($sp)
	lbu	$3,97($16)
	sb	$3,33($sp)
	lbu	$15,98($17)
	lbu	$9,99($17)
	lbu	$14,102($17)
	sll	$15,$15,24
	lbu	$8,103($17)
	sll	$9,$9,16
	lbu	$3,106($17)
	sll	$14,$14,24
	lbu	$12,107($17)
	sll	$8,$8,16
	lbu	$13,100($17)
	or	$9,$9,$15
	lbu	$11,104($17)
	or	$8,$8,$14
	lbu	$10,108($17)
	sll	$3,$3,24
	sll	$12,$12,16
	sll	$13,$13,8
	sll	$11,$11,8
	or	$12,$12,$3
	or	$13,$13,$9
	or	$11,$11,$8
	lbu	$9,101($17)
	sll	$10,$10,8
	lbu	$8,105($17)
	or	$10,$10,$12
	lbu	$3,109($17)
	or	$9,$9,$13
	or	$8,$8,$11
	or	$3,$3,$10
	sw	$9,36($sp)
	sw	$8,40($sp)
	sw	$3,44($sp)
	lbu	$3,110($17)
	sb	$3,48($sp)
	lbu	$3,111($17)
	sb	$3,49($sp)
	lbu	$15,112($18)
	lbu	$9,113($18)
	lbu	$14,116($18)
	sll	$15,$15,24
	lbu	$8,117($18)
	sll	$9,$9,16
	lbu	$3,120($18)
	sll	$14,$14,24
	lbu	$12,121($18)
	sll	$8,$8,16
	lbu	$13,114($18)
	or	$9,$9,$15
	lbu	$11,118($18)
	or	$8,$8,$14
	lbu	$10,122($18)
	sll	$3,$3,24
	sll	$12,$12,16
	sll	$13,$13,8
	sll	$11,$11,8
	or	$12,$12,$3
	or	$13,$13,$9
	or	$11,$11,$8
	lbu	$9,115($18)
	sll	$10,$10,8
	lbu	$8,119($18)
	or	$10,$10,$12
	lbu	$3,123($18)
	or	$9,$9,$13
	or	$8,$8,$11
	or	$3,$3,$10
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$3,60($sp)
	lbu	$3,124($18)
	sb	$3,64($sp)
	lbu	$3,125($18)
	sb	$3,65($sp)
	lbu	$15,126($19)
	lbu	$9,127($19)
	lbu	$14,130($19)
	sll	$15,$15,24
	lbu	$8,131($19)
	sll	$9,$9,16
	lbu	$3,134($19)
	sll	$14,$14,24
	lbu	$12,135($19)
	sll	$8,$8,16
	lbu	$13,128($19)
	or	$9,$9,$15
	lbu	$11,132($19)
	or	$8,$8,$14
	lbu	$10,136($19)
	sll	$3,$3,24
	sll	$12,$12,16
	sll	$13,$13,8
	sll	$11,$11,8
	or	$12,$12,$3
	or	$13,$13,$9
	or	$11,$11,$8
	lbu	$9,129($19)
	sll	$10,$10,8
	lbu	$8,133($19)
	or	$10,$10,$12
	lbu	$3,137($19)
	or	$9,$9,$13
	or	$8,$8,$11
	or	$3,$3,$10
	sw	$9,68($sp)
	sw	$8,72($sp)
	sw	$3,76($sp)
	lbu	$3,138($19)
	sb	$3,80($sp)
	lbu	$3,139($19)
	sb	$3,81($sp)
	lbu	$15,140($20)
	lbu	$9,141($20)
	lbu	$14,144($20)
	sll	$15,$15,24
	lbu	$8,145($20)
	sll	$9,$9,16
	lbu	$3,148($20)
	sll	$14,$14,24
	lbu	$12,149($20)
	sll	$8,$8,16
	lbu	$13,142($20)
	or	$9,$9,$15
	lbu	$11,146($20)
	or	$8,$8,$14
	lbu	$10,150($20)
	sll	$3,$3,24
	sll	$12,$12,16
	sll	$13,$13,8
	sll	$11,$11,8
	or	$12,$12,$3
	or	$13,$13,$9
	or	$11,$11,$8
	lbu	$9,143($20)
	sll	$10,$10,8
	lbu	$8,147($20)
	or	$10,$10,$12
	lbu	$3,151($20)
	or	$9,$9,$13
	or	$8,$8,$11
	or	$3,$3,$10
	sw	$9,84($sp)
	sw	$8,88($sp)
	sw	$3,92($sp)
	lbu	$3,152($20)
	sb	$3,96($sp)
	lbu	$3,153($20)
	sb	$3,97($sp)
	lbu	$15,154($2)
	lbu	$9,155($2)
	lbu	$14,158($2)
	sll	$15,$15,24
	lbu	$8,159($2)
	sll	$9,$9,16
	lbu	$3,162($2)
	sll	$14,$14,24
	lbu	$12,163($2)
	sll	$8,$8,16
	lbu	$13,156($2)
	or	$9,$9,$15
	lbu	$11,160($2)
	or	$8,$8,$14
	lbu	$10,164($2)
	sll	$3,$3,24
	sll	$12,$12,16
	sll	$13,$13,8
	sll	$11,$11,8
	or	$12,$12,$3
	or	$13,$13,$9
	or	$11,$11,$8
	lbu	$9,157($2)
	sll	$10,$10,8
	lbu	$8,161($2)
	or	$10,$10,$12
	lbu	$3,165($2)
	or	$9,$9,$13
	or	$8,$8,$11
	or	$3,$3,$10
	sw	$9,100($sp)
	sw	$8,104($sp)
	sw	$3,108($sp)
	lbu	$3,166($2)
	sb	$3,112($sp)
	lbu	$2,167($2)
	sb	$2,113($sp)
	lbu	$2,16($22)
	sw	$2,116($sp)
	lbu	$2,17($22)
	sw	$2,120($sp)
	lbu	$2,18($22)
	sw	$2,124($sp)
	lbu	$2,19($22)
	jal	scrlog_printk
	sw	$2,128($sp)

	j	$L440
	nop

$L446:
	beq	$22,$0,$L440
	lui	$4,%hi($LC19)

	lw	$5,12($22)
	addiu	$4,$4,%lo($LC19)
	andi	$2,$5,0xff
	srl	$6,$5,16
	sw	$2,16($sp)
	srl	$7,$5,8
	lbu	$2,16($22)
	srl	$5,$5,24
	andi	$6,$6,0x00ff
	andi	$7,$7,0x00ff
	sw	$2,20($sp)
	lbu	$2,17($22)
	sw	$2,24($sp)
	lbu	$2,18($22)
	sw	$2,28($sp)
	lui	$2,%hi(log_info)
	lbu	$3,19($22)
	addiu	$2,$2,%lo(log_info)
	sw	$2,36($sp)
	j	$L453
	sw	$3,32($sp)

$L448:
	beq	$22,$0,$L440
	lui	$4,%hi($LC20)

	lw	$5,12($22)
	addiu	$4,$4,%lo($LC20)
$L454:
	andi	$2,$5,0xff
	srl	$6,$5,16
	sw	$2,16($sp)
	srl	$7,$5,8
	lbu	$2,16($22)
	srl	$5,$5,24
	andi	$6,$6,0x00ff
	andi	$7,$7,0x00ff
	sw	$2,20($sp)
	lbu	$2,17($22)
	sw	$2,24($sp)
	lbu	$2,18($22)
	sw	$2,28($sp)
	lbu	$2,19($22)
	sw	$2,32($sp)
$L453:
	jal	scrlog_printk
	nop

$L440:
	lw	$31,172($sp)
	move	$2,$0
	lw	$22,168($sp)
	lw	$21,164($sp)
	lw	$20,160($sp)
	lw	$19,156($sp)
	lw	$18,152($sp)
	lw	$17,148($sp)
	lw	$16,144($sp)
	j	$31
	addiu	$sp,$sp,176

	.set	macro
	.set	reorder
	.end	log_fastFilter
	.section	.text.do_filter,"ax",@progbits
	.align	2
	.globl	do_filter
	.ent	do_filter
	.type	do_filter, @function
do_filter:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$17,20($sp)
	lui	$17,%hi(table_list_head)
	sw	$16,16($sp)
	addiu	$2,$17,%lo(table_list_head)
	lw	$16,%lo(table_list_head)($17)
	sw	$18,24($sp)
	move	$18,$4
	beq	$16,$2,$L465
	sw	$31,28($sp)

	move	$4,$18
$L466:
	move	$6,$16
	move	$7,$16
	jal	scan_table
	move	$5,$0

	move	$3,$2
	li	$2,-1			# 0xffffffff
	beq	$3,$2,$L465
	addiu	$4,$17,%lo(table_list_head)

	li	$2,1			# 0x1
	bne	$3,$2,$L458
	lui	$2,%hi(log_enable)

	lw	$2,%lo(log_enable)($2)
	beq	$2,$0,$L462
	nop

	lw	$4,16($16)
	jal	log_fastFilter
	move	$5,$18

$L462:
	j	$L455
	li	$2,1			# 0x1

$L458:
	lw	$16,0($16)
	bne	$16,$4,$L466
	move	$4,$18

$L465:
	move	$2,$0
$L455:
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	do_filter
	.section	.text.FilterWeb_v2,"ax",@progbits
	.align	2
	.globl	FilterWeb_v2
	.ent	FilterWeb_v2
	.type	FilterWeb_v2, @function
FilterWeb_v2:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$16,16($sp)
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	move	$16,$4

	jal	need_filter
	move	$4,$16

	beq	$2,$0,$L469
	move	$4,$16

	lw	$31,20($sp)
	lw	$16,16($sp)
	j	do_filter
	addiu	$sp,$sp,24

$L469:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	FilterWeb_v2
	.rdata
	.align	2
$LC22:
	.ascii	"filter_table\000"
	.section	.text.filter_table_init,"ax",@progbits
	.align	2
	.globl	filter_table_init
	.ent	filter_table_init
	.type	filter_table_init, @function
filter_table_init:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,16($sp)
	jal	filter_table_head_init
	nop

	lui	$4,%hi($LC22)
	addiu	$4,$4,%lo($LC22)
	move	$5,$0
	jal	create_proc_entry
	move	$6,$0

	lui	$5,%hi(res12)
	move	$4,$2
	sw	$2,%lo(res12)($5)
	beq	$4,$0,$L471
	move	$2,$0

	lui	$3,%hi(filter_table_read_proc)
	addiu	$3,$3,%lo(filter_table_read_proc)
	sw	$3,64($4)
	lui	$3,%hi(filter_table_write_proc)
	lw	$4,%lo(res12)($5)
	addiu	$3,$3,%lo(filter_table_write_proc)
	sw	$3,68($4)
$L471:
	lw	$31,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_table_init
	.section	.text.filter_init_fastpath,"ax",@progbits
	.align	2
	.globl	filter_init_fastpath
	.ent	filter_init_fastpath
	.type	filter_init_fastpath, @function
filter_init_fastpath:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	li	$3,1			# 0x1
	lui	$2,%hi(filter_rule_num)
	sw	$31,16($sp)
	jal	filter_table_init
	sw	$3,%lo(filter_rule_num)($2)

	move	$2,$0
	lw	$31,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_init_fastpath
	.section	.text.filter_exit_fastpath,"ax",@progbits
	.align	2
	.globl	filter_exit_fastpath
	.ent	filter_exit_fastpath
	.type	filter_exit_fastpath, @function
filter_exit_fastpath:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	lui	$4,%hi($LC22)
	sw	$16,16($sp)
	lui	$16,%hi(res12)
	lw	$2,%lo(res12)($16)
	addiu	$4,$4,%lo($LC22)
	sw	$31,20($sp)
	beq	$2,$0,$L474
	move	$5,$2

	jal	remove_proc_entry
	nop

	sw	$0,%lo(res12)($16)
$L474:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_exit_fastpath
	.rdata
	.align	2
$LC23:
	.ascii	"dos_filter: alloc skb fail!\n\000"
	.section	.text.GenerateTCPENDACK,"ax",@progbits
	.align	2
	.ent	GenerateTCPENDACK
	.type	GenerateTCPENDACK, @function
GenerateTCPENDACK:
	.set	nomips16
	.frame	$sp,112,$31		# vars= 56, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$4,112($sp)
	lui	$4,%hi($LC0)
	addiu	$4,$4,%lo($LC0)
	sw	$31,108($sp)
	sw	$23,100($sp)
	sw	$20,88($sp)
	sw	$18,80($sp)
	sw	$16,72($sp)
	sw	$fp,104($sp)
	sw	$22,96($sp)
	sw	$21,92($sp)
	sw	$19,84($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	sw	$17,76($sp)
	.set	macro
	.set	reorder

	lw	$4,112($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	move	$5,$16
	move	$23,$2
	lbu	$2,0($2)
	addiu	$4,$23,12
	andi	$2,$2,0xf
	sll	$2,$2,2
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addu	$20,$23,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L475
	move	$18,$2
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
	bne	$2,$0,$L476
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	$3, 72($18)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($18)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	bne	$2,$0,$L475
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$18
	.set	macro
	.set	reorder

	j	$L475
$L476:
	lw	$4,112($sp)
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L482
	move	$22,$2
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	$3, 72($18)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($18)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	bne	$2,$0,$L483
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$18
	.set	macro
	.set	reorder

$L483:
	lui	$4,%hi($LC23)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC23)
	.set	macro
	.set	reorder

	j	$L475
$L482:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$22
	lbu	$5,0($2)
	andi	$5,$5,0xf
	sll	$5,$5,2
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	addiu	$5,$5,20
	.set	macro
	.set	reorder

	move	$4,$22
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$4,$sp,64
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_lock_from_hh
	move	$17,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$22
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	addiu	$16,$2,-16
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	move	$4,$16
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
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$22
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	$3, 72($18)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($18)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	bne	$2,$0,$L494
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$18
	.set	macro
	.set	reorder

$L494:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$4,$22
	.set	macro
	.set	reorder

	li	$19,268369920			# 0xfff0000
	lhu	$2,2($20)
	ori	$19,$19,0xffff
	lw	$3,52($sp)
	lui	$fp,%hi(url_cache)
	sh	$2,40($sp)
	li	$2,1342177280			# 0x50000000
	and	$3,$3,$19
	addiu	$21,$fp,%lo(url_cache)
	or	$3,$3,$2
	li	$2,-327680			# 0xfffb0000
	lw	$8,8($20)
	ori	$2,$2,0xffff
	lhu	$7,0($20)
	move	$4,$22
	lw	$5,4($20)
	addiu	$20,$sp,40
	sw	$3,52($sp)
	sb	$0,53($sp)
	addiu	$5,$5,1
	lw	$3,52($sp)
	lhu	$6,12($21)
	and	$3,$3,$2
	li	$2,1048576			# 0x100000
	or	$3,$3,$2
	li	$2,-131072			# 0xfffe0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,52($sp)
	sh	$7,42($sp)
	sw	$8,44($sp)
	sw	$5,48($sp)
	sh	$6,54($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$0,58($sp)
	.set	macro
	.set	reorder

	move	$6,$0
	addiu	$17,$2,-34
	move	$4,$20
	move	$5,$17
	andi	$17,$17,0xffff
	sh	$0,56($sp)
	lw	$18,12($23)
	lw	$16,16($23)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$17,$17,6
	.set	macro
	.set	reorder

	move	$4,$22
#APP
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$16, $18		
	sltu	$1, $16, $18	
	addu	$16, $1		
	addu	$16, $17		
	sltu	$1, $16, $17	
	addu	$16, $1		
	addu	$16, $2		
	sltu	$1, $16, $2	
	addu	$16, $1		
	.set	pop
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $16, 16	
	addu	$16, $1		
	sltu	$1, $16, $1	
	srl	$16, $16, 16	
	addu	$16, $1		
	xori	$16, 0xffff	
	.set	pop
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$16,56($sp)
	.set	macro
	.set	reorder

	move	$5,$20
	addiu	$4,$2,34
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,20			# 0x14
	.set	macro
	.set	reorder

	li	$2,1073741824			# 0x40000000
	lw	$3,16($sp)
	move	$5,$0
	lbu	$6,14($21)
	and	$3,$3,$19
	or	$3,$3,$2
	li	$2,-251723776			# 0xf0ff0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	li	$2,83886080			# 0x5000000
	or	$3,$3,$2
	sw	$3,16($sp)
	lw	$2,12($23)
	lw	$4,112($sp)
	sw	$2,32($sp)
	li	$2,6			# 0x6
	lw	$3,16($23)
	sb	$2,25($sp)
	sw	$3,28($sp)
	sb	$6,24($sp)
	sh	$0,22($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sb	$0,17($sp)
	.set	macro
	.set	reorder

	li	$2,40
	lhu	$3,%lo(url_cache)($fp)
	addiu	$6,$sp,16
	sh	$2,18($sp)
	addiu	$3,$3,1
	sh	$3,20($sp)
	lw	$3,16($sp)
	lw	$4,20($sp)
	srl	$2,$3,22
	addu	$5,$3,$4
	andi	$2,$2,0x3c
	sh	$0,26($sp)
	sltu	$4,$5,$4
	addu	$7,$6,$2
	addu	$5,$5,$4
	lw	$2,24($sp)
	addiu	$6,$sp,32
	lw	$3,28($sp)
	addu	$5,$5,$2
	sltu	$4,$5,$2
	addu	$5,$5,$4
	addu	$5,$5,$3
	sltu	$4,$5,$3
	addu	$5,$5,$4
$L498:
	lw	$2,0($6)
	addiu	$6,$6,4
	addu	$5,$5,$2
	sltu	$4,$5,$2
	.set	noreorder
	.set	nomacro
	bne	$6,$7,$L498
	addu	$5,$5,$4
	.set	macro
	.set	reorder

	move	$4,$22
#APP
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $5, 16	
	addu	$5, $1		
	sltu	$1, $5, $1	
	srl	$5, $5, 16	
	addu	$5, $1		
	xori	$5, 0xffff	
	.set	pop
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$5,26($sp)
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

	addiu	$2,$fp,%lo(url_cache)
	move	$4,$22
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$0,16($2)
	.set	macro
	.set	reorder

$L475:
	lw	$31,108($sp)
	move	$2,$0
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

	.end	GenerateTCPENDACK
	.section	.text.GenerateTCPFINACK,"ax",@progbits
	.align	2
	.ent	GenerateTCPFINACK
	.type	GenerateTCPFINACK, @function
GenerateTCPFINACK:
	.set	nomips16
	.frame	$sp,112,$31		# vars= 56, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$4,112($sp)
	lui	$4,%hi($LC0)
	addiu	$4,$4,%lo($LC0)
	sw	$31,108($sp)
	sw	$23,100($sp)
	sw	$20,88($sp)
	sw	$18,80($sp)
	sw	$16,72($sp)
	sw	$fp,104($sp)
	sw	$22,96($sp)
	sw	$21,92($sp)
	sw	$19,84($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	sw	$17,76($sp)
	.set	macro
	.set	reorder

	lw	$4,112($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	move	$5,$16
	move	$23,$2
	lbu	$2,0($2)
	addiu	$4,$23,12
	andi	$2,$2,0xf
	sll	$2,$2,2
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addu	$20,$23,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L503
	move	$18,$2
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
	bne	$2,$0,$L504
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	$3, 72($18)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($18)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	bne	$2,$0,$L503
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$18
	.set	macro
	.set	reorder

	j	$L503
$L504:
	lw	$4,112($sp)
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L510
	move	$22,$2
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	$3, 72($18)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($18)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	bne	$2,$0,$L511
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$18
	.set	macro
	.set	reorder

$L511:
	lui	$4,%hi($LC23)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC23)
	.set	macro
	.set	reorder

	j	$L503
$L510:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$22
	lbu	$5,0($2)
	andi	$5,$5,0xf
	sll	$5,$5,2
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	addiu	$5,$5,20
	.set	macro
	.set	reorder

	move	$4,$22
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$4,$sp,64
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_lock_from_hh
	move	$17,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$22
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	addiu	$16,$2,-16
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	move	$4,$16
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
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$22
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	$3, 72($18)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($18)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	bne	$2,$0,$L522
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$18
	.set	macro
	.set	reorder

$L522:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$4,$22
	.set	macro
	.set	reorder

	li	$19,268369920			# 0xfff0000
	lhu	$2,2($20)
	ori	$19,$19,0xffff
	lw	$3,52($sp)
	lui	$fp,%hi(url_cache)
	sh	$2,40($sp)
	li	$2,1342177280			# 0x50000000
	and	$3,$3,$19
	addiu	$21,$fp,%lo(url_cache)
	or	$3,$3,$2
	li	$2,-327680			# 0xfffb0000
	lw	$7,8($20)
	ori	$2,$2,0xffff
	lw	$8,4($20)
	move	$4,$22
	lhu	$6,0($20)
	addiu	$20,$sp,40
	sw	$3,52($sp)
	sb	$0,53($sp)
	lw	$3,52($sp)
	lhu	$5,12($21)
	and	$3,$3,$2
	li	$2,1048576			# 0x100000
	or	$3,$3,$2
	li	$2,65536			# 0x10000
	or	$3,$3,$2
	sw	$3,52($sp)
	sw	$7,44($sp)
	sw	$8,48($sp)
	sh	$6,42($sp)
	sh	$5,54($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$0,58($sp)
	.set	macro
	.set	reorder

	move	$6,$0
	addiu	$17,$2,-34
	move	$4,$20
	move	$5,$17
	andi	$17,$17,0xffff
	sh	$0,56($sp)
	lw	$18,12($23)
	lw	$16,16($23)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$17,$17,6
	.set	macro
	.set	reorder

	move	$4,$22
#APP
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$16, $18		
	sltu	$1, $16, $18	
	addu	$16, $1		
	addu	$16, $17		
	sltu	$1, $16, $17	
	addu	$16, $1		
	addu	$16, $2		
	sltu	$1, $16, $2	
	addu	$16, $1		
	.set	pop
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $16, 16	
	addu	$16, $1		
	sltu	$1, $16, $1	
	srl	$16, $16, 16	
	addu	$16, $1		
	xori	$16, 0xffff	
	.set	pop
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$16,56($sp)
	.set	macro
	.set	reorder

	move	$5,$20
	addiu	$4,$2,34
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,20			# 0x14
	.set	macro
	.set	reorder

	li	$2,1073741824			# 0x40000000
	lw	$3,16($sp)
	move	$5,$0
	lbu	$6,14($21)
	and	$3,$3,$19
	or	$3,$3,$2
	li	$2,-251723776			# 0xf0ff0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	li	$2,83886080			# 0x5000000
	or	$3,$3,$2
	sw	$3,16($sp)
	lw	$2,12($23)
	lw	$4,112($sp)
	sw	$2,32($sp)
	li	$2,6			# 0x6
	lw	$3,16($23)
	sb	$2,25($sp)
	sw	$3,28($sp)
	sb	$6,24($sp)
	sh	$0,22($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sb	$0,17($sp)
	.set	macro
	.set	reorder

	li	$2,40
	lhu	$3,%lo(url_cache)($fp)
	addiu	$6,$sp,16
	sh	$2,18($sp)
	addiu	$3,$3,1
	sh	$3,20($sp)
	lw	$3,16($sp)
	lw	$4,20($sp)
	srl	$2,$3,22
	addu	$5,$3,$4
	andi	$2,$2,0x3c
	sh	$0,26($sp)
	sltu	$4,$5,$4
	addu	$7,$6,$2
	addu	$5,$5,$4
	lw	$2,24($sp)
	addiu	$6,$sp,32
	lw	$3,28($sp)
	addu	$5,$5,$2
	sltu	$4,$5,$2
	addu	$5,$5,$4
	addu	$5,$5,$3
	sltu	$4,$5,$3
	addu	$5,$5,$4
$L526:
	lw	$2,0($6)
	addiu	$6,$6,4
	addu	$5,$5,$2
	sltu	$4,$5,$2
	.set	noreorder
	.set	nomacro
	bne	$6,$7,$L526
	addu	$5,$5,$4
	.set	macro
	.set	reorder

	move	$4,$22
#APP
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $5, 16	
	addu	$5, $1		
	sltu	$1, $5, $1	
	srl	$5, $5, 16	
	addu	$5, $1		
	xori	$5, 0xffff	
	.set	pop
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$5,26($sp)
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

	addiu	$3,$fp,%lo(url_cache)
	li	$2,1
	move	$4,$22
	sb	$2,16($3)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$0,15($3)
	.set	macro
	.set	reorder

$L503:
	lw	$31,108($sp)
	move	$2,$0
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

	.end	GenerateTCPFINACK
	.rdata
	.align	2
$LC24:
	.ascii	"HTTP/1.1 401 The web site is blocked by administrator\r\n"
	.ascii	"Server: Embedded HTTP Server 2.00\r\n"
	.ascii	"Connection: close\r\n\r\n"
	.ascii	"<HTML><HEAD><TITLE>401 The web site is blocked by admini"
	.ascii	"strator</TITLE></HEAD>\n"
	.ascii	"<BODY BGCOLOR=\"#ffffff\"><H4>401 The web site is blocke"
	.ascii	"d by administrator</H4></BODY></HTML>\n\000"
	.section	.text.GenerateHTTP401,"ax",@progbits
	.align	2
	.globl	GenerateHTTP401
	.ent	GenerateHTTP401
	.type	GenerateHTTP401, @function
GenerateHTTP401:
	.set	nomips16
	.frame	$sp,216,$31		# vars= 160, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-216
	sw	$4,216($sp)
	lui	$4,%hi($LC0)
	addiu	$4,$4,%lo($LC0)
	sw	$31,212($sp)
	sw	$fp,208($sp)
	sw	$21,196($sp)
	sw	$18,184($sp)
	sw	$16,176($sp)
	sw	$23,204($sp)
	sw	$22,200($sp)
	sw	$20,192($sp)
	sw	$19,188($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	sw	$17,180($sp)
	.set	macro
	.set	reorder

	lw	$4,216($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	move	$5,$16
	move	$fp,$2
	lbu	$2,0($2)
	addiu	$4,$fp,12
	andi	$2,$2,0xf
	sll	$2,$2,2
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addu	$18,$fp,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L531
	move	$21,$2
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
	bne	$2,$0,$L532
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	$3, 72($21)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($21)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	bne	$2,$0,$L531
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$21
	.set	macro
	.set	reorder

	j	$L531
$L532:
	lw	$4,216($sp)
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L587
	move	$23,$2
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	li	$19,268369920			# 0xfff0000
	.set	macro
	.set	reorder

	move	$4,$23
	lbu	$5,0($2)
	lui	$2,%hi(url_cache)
	addiu	$20,$2,%lo(url_cache)
	ori	$19,$19,0xffff
	andi	$5,$5,0xf
	addiu	$22,$sp,40
	sll	$5,$5,2
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	addiu	$5,$5,20
	.set	macro
	.set	reorder

	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$4,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$21
	.set	macro
	.set	reorder

	addiu	$4,$sp,168
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_lock_from_hh
	move	$17,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$0
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	addiu	$16,$2,-16
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	move	$4,$16
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
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$23
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

	lhu	$2,2($18)
	lw	$3,8($18)
	sh	$2,40($sp)
	lhu	$2,0($18)
	lw	$4,216($sp)
	lw	$16,4($18)
	sh	$2,42($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$3,44($sp)
	.set	macro
	.set	reorder

	li	$3,1342177280			# 0x50000000
	lw	$4,52($sp)
	addu	$2,$16,$2
	lbu	$6,0($fp)
	and	$4,$4,$19
	or	$4,$4,$3
	andi	$6,$6,0xf
	lw	$3,12($18)
	sll	$6,$6,2
	sw	$4,52($sp)
	move	$4,$23
	srl	$3,$3,28
	sb	$0,53($sp)
	sll	$3,$3,2
	lw	$5,52($sp)
	subu	$2,$2,$3
	li	$3,-327680			# 0xfffb0000
	subu	$2,$2,$6
	ori	$3,$3,0xffff
	and	$5,$5,$3
	li	$3,1048576			# 0x100000
	or	$5,$5,$3
	li	$3,-589824			# 0xfff70000
	lhu	$6,12($20)
	ori	$3,$3,0xffff
	and	$5,$5,$3
	sw	$5,52($sp)
	sw	$2,48($sp)
	sh	$6,54($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$0,58($sp)
	.set	macro
	.set	reorder

	move	$6,$0
	addiu	$2,$2,-34
	move	$4,$22
	sh	$0,56($sp)
	move	$5,$2
	lw	$18,12($fp)
	andi	$17,$2,0xffff
	lw	$16,16($fp)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$17,$17,6
	.set	macro
	.set	reorder

	move	$4,$23
#APP
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$16, $18		
	sltu	$1, $16, $18	
	addu	$16, $1		
	addu	$16, $17		
	sltu	$1, $16, $17	
	addu	$16, $1		
	addu	$16, $2		
	sltu	$1, $16, $2	
	addu	$16, $1		
	.set	pop
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $16, 16	
	addu	$16, $1		
	sltu	$1, $16, $1	
	srl	$16, $16, 16	
	addu	$16, $1		
	xori	$16, 0xffff	
	.set	pop
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$16,56($sp)
	.set	macro
	.set	reorder

	move	$5,$22
	addiu	$4,$2,34
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,20			# 0x14
	.set	macro
	.set	reorder

	li	$2,1073741824			# 0x40000000
	lw	$3,16($sp)
	addiu	$6,$sp,16
	sh	$0,22($sp)
	and	$3,$3,$19
	or	$3,$3,$2
	lui	$2,%hi(url_cache)
	lhu	$4,%lo(url_cache)($2)
	li	$2,-251723776			# 0xf0ff0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	li	$2,83886080			# 0x5000000
	or	$3,$3,$2
	addiu	$4,$4,1
	li	$2,40
	sw	$3,16($sp)
	sh	$4,20($sp)
	sh	$2,18($sp)
	li	$2,6			# 0x6
	sb	$0,17($sp)
	lw	$3,16($sp)
	lbu	$5,14($20)
	lw	$4,20($sp)
	sb	$5,24($sp)
	addu	$5,$3,$4
	sb	$2,25($sp)
	sltu	$4,$5,$4
	sh	$0,26($sp)
	srl	$2,$3,22
	addu	$5,$5,$4
	andi	$2,$2,0x3c
	lw	$3,12($fp)
	lw	$4,24($sp)
	sw	$3,32($sp)
	addu	$3,$6,$2
	addu	$5,$5,$4
	addiu	$6,$sp,32
	lw	$2,16($fp)
	sltu	$4,$5,$4
	addu	$5,$5,$4
	addu	$5,$5,$2
	sltu	$4,$5,$2
	addu	$5,$5,$4
	sw	$2,28($sp)
$L551:
	lw	$2,0($6)
	addiu	$6,$6,4
	addu	$5,$5,$2
	sltu	$4,$5,$2
	.set	noreorder
	.set	nomacro
	bne	$6,$3,$L551
	addu	$5,$5,$4
	.set	macro
	.set	reorder

	move	$4,$23
	lui	$18,%hi(block_message)
#APP
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $5, 16	
	addu	$5, $1		
	sltu	$1, $5, $1	
	srl	$5, $5, 16	
	addu	$5, $1		
	xori	$5, 0xffff	
	.set	pop
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$5,26($sp)
	.set	macro
	.set	reorder

	addiu	$5,$sp,16
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
	move	$4,$23
	.set	macro
	.set	reorder

	addiu	$4,$sp,64
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
	move	$4,$23
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	bne	$4,$0,$L583
	lui	$4,%hi($LC24)
	addiu	$4,$4,%lo($LC24)
$L583:
	jal	strlen
	li	$5,32			# 0x20
	lw	$4,216($sp)
	move	$6,$2
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L558
	move	$23,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L588
	lui	$4,%hi($LC23)
	.set	macro
	.set	reorder

$L587:
#APP
	   

	1:	ll	$3, 72($21)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($21)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	bne	$2,$0,$L559
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$21
	.set	macro
	.set	reorder

$L559:
	lui	$4,%hi($LC23)
$L588:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC23)
	.set	macro
	.set	reorder

	j	$L531
$L558:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$23
	lbu	$5,0($2)
	move	$20,$2
	andi	$5,$5,0xf
	sll	$2,$5,2
	addu	$fp,$20,$2
	lw	$2,12($fp)
	srl	$2,$2,28
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$5,2
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	bne	$4,$0,$L584
	lui	$4,%hi($LC24)
	addiu	$4,$4,%lo($LC24)
$L584:
	jal	strlen
	move	$4,$23
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$21
	.set	macro
	.set	reorder

	addiu	$4,$sp,172
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_lock_from_hh
	move	$17,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$23
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	addiu	$16,$2,-16
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	move	$4,$16
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
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$23
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	$3, 72($21)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($21)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L589
	move	$4,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$23
$L589:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lbu	$2,0($20)
	lw	$3,12($20)
	lw	$4,16($20)
	andi	$2,$2,0xf
	sll	$2,$2,2
	sw	$3,16($20)
	addu	$22,$20,$2
	sw	$4,12($20)
	lhu	$4,0($22)
	lhu	$3,2($22)
	lw	$2,12($22)
	sh	$3,0($22)
	sh	$4,2($22)
	srl	$2,$2,28
	lw	$4,%lo(block_message)($18)
	sll	$2,$2,2
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L573
	addu	$17,$22,$2
	.set	macro
	.set	reorder

	jal	strlen
	move	$4,$17
	lw	$5,%lo(block_message)($18)
	.set	noreorder
	.set	nomacro
	j	$L585
	addiu	$6,$2,1
	.set	macro
	.set	reorder

$L573:
	lui	$16,%hi($LC24)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC24)
	.set	macro
	.set	reorder

	move	$4,$17
	addiu	$5,$16,%lo($LC24)
	move	$6,$2
$L585:
	jal	memcpy
	lui	$3,%hi(url_cache)
	lw	$2,8($22)
	addiu	$19,$3,%lo(url_cache)
	lw	$4,216($sp)
	lw	$16,4($22)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,4($22)
	.set	macro
	.set	reorder

	lw	$3,12($fp)
	addu	$2,$16,$2
	lbu	$4,0($20)
	srl	$3,$3,28
	sll	$3,$3,2
	andi	$4,$4,0xf
	subu	$2,$2,$3
	sll	$4,$4,2
	subu	$2,$2,$4
	move	$4,$23
	sw	$2,8($22)
	lhu	$2,12($19)
	sb	$0,13($22)
	sh	$2,14($22)
	li	$2,-327680			# 0xfffb0000
	lw	$3,12($22)
	ori	$2,$2,0xffff
	sh	$0,18($22)
	and	$3,$3,$2
	li	$2,1048576			# 0x100000
	or	$3,$3,$2
	li	$2,524288			# 0x80000
	or	$3,$3,$2
	li	$2,-131072			# 0xfffe0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$3,12($22)
	.set	macro
	.set	reorder

	move	$6,$0
	lbu	$3,0($20)
	move	$4,$22
	sh	$0,16($22)
	andi	$3,$3,0xf
	sll	$3,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,-14
	move	$5,$2
	andi	$17,$2,0xffff
	lw	$18,12($20)
	lw	$16,16($20)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$17,$17,6
	.set	macro
	.set	reorder

	move	$4,$23
#APP
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$16, $18		
	sltu	$1, $16, $18	
	addu	$16, $1		
	addu	$16, $17		
	sltu	$1, $16, $17	
	addu	$16, $1		
	addu	$16, $2		
	sltu	$1, $16, $2	
	addu	$16, $1		
	.set	pop
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $16, 16	
	addu	$16, $1		
	sltu	$1, $16, $1	
	srl	$16, $16, 16	
	addu	$16, $1		
	xori	$16, 0xffff	
	.set	pop
#NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$16,16($22)
	.set	macro
	.set	reorder

	lui	$3,%hi(url_cache)
	addiu	$2,$2,-14
	addiu	$6,$20,16
	sh	$2,2($20)
	lbu	$2,14($19)
	sh	$0,6($20)
	sb	$2,8($20)
	lhu	$2,%lo(url_cache)($3)
	lw	$4,0($20)
	addiu	$2,$2,2
	sh	$2,4($20)
	srl	$2,$4,22
	lw	$3,4($20)
	andi	$2,$2,0x3c
	sh	$0,10($20)
	addu	$7,$20,$2
	addu	$4,$4,$3
	sltu	$5,$4,$3
	lw	$2,8($20)
	addu	$4,$4,$5
	lw	$3,12($20)
	addu	$4,$4,$2
	sltu	$5,$4,$2
	addu	$4,$4,$5
	addu	$4,$4,$3
	sltu	$5,$4,$3
	addu	$4,$4,$5
$L578:
	lw	$2,0($6)
	addiu	$6,$6,4
	addu	$4,$4,$2
	sltu	$5,$4,$2
	.set	noreorder
	.set	nomacro
	bne	$6,$7,$L578
	addu	$4,$4,$5
	.set	macro
	.set	reorder

	lui	$2,%hi(url_cache)
	addiu	$3,$2,%lo(url_cache)
	li	$2,1
#APP
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $4, 16	
	addu	$4, $1		
	sltu	$1, $4, $1	
	srl	$4, $4, 16	
	addu	$4, $1		
	xori	$4, 0xffff	
	.set	pop
#NO_APP
	sh	$4,10($20)
	move	$4,$23
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sb	$2,15($3)
	.set	macro
	.set	reorder

	addiu	$4,$sp,64
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
	move	$4,$23
	.set	macro
	.set	reorder

$L531:
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

	.end	GenerateHTTP401
	.section	.text.strtoint,"ax",@progbits
	.align	2
	.globl	strtoint
	.ent	strtoint
	.type	strtoint, @function
strtoint:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lbu	$3,0($4)
	move	$7,$0
$L598:
	beq	$3,$0,$L592
	sll	$3,$3,24

	sll	$2,$7,2
	sra	$3,$3,24
	addu	$2,$2,$7
	addiu	$5,$3,-48
	sll	$6,$2,1
	sltu	$2,$5,10
	beq	$2,$0,$L592
	addiu	$4,$4,1

	lbu	$3,0($4)
	j	$L598
	addu	$7,$6,$5

$L592:
	j	$31
	move	$2,$7

	.set	macro
	.set	reorder
	.end	strtoint
	.rdata
	.align	2
	.type	__func__.1, @object
	.size	__func__.1, 34
__func__.1:
	.ascii	"Add_Pattern_ACL_For_ContentFilter\000"
	.section	.text.Add_Pattern_ACL_For_ContentFilter,"ax",@progbits
	.align	2
	.globl	Add_Pattern_ACL_For_ContentFilter
	.ent	Add_Pattern_ACL_For_ContentFilter
	.type	Add_Pattern_ACL_For_ContentFilter, @function
Add_Pattern_ACL_For_ContentFilter:
	.set	nomips16
	.frame	$sp,80,$31		# vars= 56, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-80
	move	$5,$0
	li	$6,52			# 0x34
	addiu	$4,$sp,16
	sw	$31,76($sp)
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$16,72($sp)
	.set	macro
	.set	reorder

	li	$2,-125894656			# 0xf87f0000
	lw	$3,52($sp)
	ori	$2,$2,0xffff
	lui	$4,%hi($LC2)
	and	$3,$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xffff
	addiu	$4,$4,%lo($LC2)
	and	$3,$3,$2
	li	$2,7340032			# 0x700000
	or	$3,$3,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	sw	$3,52($sp)
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L599
	move	$4,$2
	.set	macro
	.set	reorder

	jal	rtl_get_ps_drv_netif_mapping_by_psdev
#APP
	   

	1:	ll	    $3, 452($16)		# atomic_sub		
       nop                                             
	subu	$3, 1					
	sc	    $3, 452($16)					
	beqz	$3, 2f					
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	addiu	$5,$2,12
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L599
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	jal	rtl865x_add_pattern_acl_for_contentFilter
$L599:
	lw	$31,76($sp)
	move	$2,$0
	lw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,80
	.set	macro
	.set	reorder

	.end	Add_Pattern_ACL_For_ContentFilter
	.section	.text.filter_item_regist,"ax",@progbits
	.align	2
	.globl	filter_item_regist
	.ent	filter_item_regist
	.type	filter_item_regist, @function
filter_item_regist:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	lui	$3,%hi(table_list_head)
	sw	$31,16($sp)
	addiu	$2,$3,%lo(table_list_head)
	lw	$6,%lo(table_list_head)($3)
	beq	$6,$2,$L605
	li	$2,-1			# 0xffffffff

$L613:
	lw	$2,16($6)
	bne	$2,$4,$L608
	addiu	$7,$3,%lo(table_list_head)

	lw	$2,8($6)
	addiu	$3,$6,8
	sw	$2,0($5)
	sw	$5,4($2)
	sw	$3,4($5)
	lw	$2,24($6)
	sw	$5,8($6)
	addiu	$3,$2,1
	li	$2,3			# 0x3
	bne	$4,$2,$L612
	sw	$3,24($6)

	li	$2,2			# 0x2
	bne	$3,$2,$L612
	nop

	jal	Add_Pattern_ACL_For_ContentFilter
	nop

$L612:
	j	$L605
	move	$2,$0

$L608:
	lw	$6,0($6)
	bne	$6,$7,$L613
	li	$2,-1			# 0xffffffff

$L605:
	lw	$31,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_item_regist
	.rdata
	.align	2
	.type	__func__.0, @object
	.size	__func__.0, 34
__func__.0:
	.ascii	"Del_Pattern_ACL_For_ContentFilter\000"
	.section	.text.Del_Pattern_ACL_For_ContentFilter,"ax",@progbits
	.align	2
	.globl	Del_Pattern_ACL_For_ContentFilter
	.ent	Del_Pattern_ACL_For_ContentFilter
	.type	Del_Pattern_ACL_For_ContentFilter, @function
Del_Pattern_ACL_For_ContentFilter:
	.set	nomips16
	.frame	$sp,80,$31		# vars= 56, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-80
	move	$5,$0
	li	$6,52			# 0x34
	addiu	$4,$sp,16
	sw	$31,76($sp)
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$16,72($sp)
	.set	macro
	.set	reorder

	li	$2,-125894656			# 0xf87f0000
	lw	$3,52($sp)
	ori	$2,$2,0xffff
	lui	$4,%hi($LC0)
	and	$3,$3,$2
	li	$2,25165824			# 0x1800000
	or	$3,$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xffff
	addiu	$4,$4,%lo($LC0)
	and	$3,$3,$2
	li	$2,268435456			# 0x10000000
	or	$3,$3,$2
	li	$2,-7405568			# 0xff8f0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	li	$2,6291456			# 0x600000
	or	$3,$3,$2
	lw	$2,36($sp)
	sw	$3,52($sp)
	ori	$2,$2,0x30
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	sw	$2,36($sp)
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L616
	move	$4,$2
	.set	macro
	.set	reorder

	jal	rtl_get_ps_drv_netif_mapping_by_psdev
#APP
	   

	1:	ll	    $3, 452($16)		# atomic_sub		
       nop                                             
	subu	$3, 1					
	sc	    $3, 452($16)					
	beqz	$3, 2f					
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	addiu	$5,$2,12
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L616
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	jal	rtl865x_del_pattern_acl_for_contentFilter
$L616:
	lw	$31,76($sp)
	move	$2,$0
	lw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,80
	.set	macro
	.set	reorder

	.end	Del_Pattern_ACL_For_ContentFilter
	.section	.text.filter_table_flush,"ax",@progbits
	.align	2
	.globl	filter_table_flush
	.ent	filter_table_flush
	.type	filter_table_flush, @function
filter_table_flush:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$19,28($sp)
	lui	$19,%hi(table_list_head)
	sw	$16,16($sp)
	addiu	$2,$19,%lo(table_list_head)
	lw	$16,%lo(table_list_head)($19)
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	lw	$20,0($16)
	beq	$16,$2,$L641
	li	$3,1			# 0x1

$L635:
	lw	$6,8($16)
	addiu	$18,$16,8
$L640:
	lw	$17,0($6)
	beq	$6,$18,$L639
	li	$2,1048576			# 0x100000

	lw	$5,4($6)
	lw	$3,0($6)
	ori	$2,$2,0x100
	move	$4,$6
	sw	$3,0($5)
	sw	$2,0($6)
	li	$2,2097152			# 0x200000
	ori	$2,$2,0x200
	sw	$5,4($3)
	jal	kfree
	sw	$2,4($6)

	move	$6,$17
	lw	$2,24($16)
	addiu	$2,$2,-1
	j	$L640
	sw	$2,24($16)

$L639:
	lw	$3,16($16)
	li	$2,3			# 0x3
	bne	$3,$2,$L632
	li	$2,1			# 0x1

	lw	$3,24($16)
	bne	$3,$2,$L632
	nop

	jal	Del_Pattern_ACL_For_ContentFilter
	nop

$L632:
	lw	$3,0($16)
	li	$2,1048576			# 0x100000
	lw	$5,4($16)
	ori	$2,$2,0x100
	move	$4,$16
	sw	$3,0($5)
	sw	$2,0($16)
	li	$2,2097152			# 0x200000
	ori	$2,$2,0x200
	sw	$5,4($3)
	sw	$2,4($16)
	jal	kfree
	move	$16,$20

	addiu	$3,$19,%lo(table_list_head)
	lw	$2,24($3)
	lw	$20,0($20)
	addiu	$2,$2,-1
	bne	$16,$3,$L635
	sw	$2,24($3)

	li	$3,1			# 0x1
$L641:
	lui	$4,%hi(filter_rule_num)
	lw	$31,36($sp)
	move	$2,$0
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	addiu	$sp,$sp,40
	sw	$3,%lo(filter_rule_num)($4)
	lui	$3,%hi(log_enable)
	j	$31
	sw	$0,%lo(log_enable)($3)

	.set	macro
	.set	reorder
	.end	filter_table_flush
	.rdata
	.align	2
$LC27:
	.ascii	"init\000"
	.align	2
$LC29:
	.ascii	"flush\000"
	.align	2
$LC28:
	.ascii	" \000"
	.align	2
$LC30:
	.ascii	"enable_log\000"
	.align	2
$LC31:
	.ascii	"add:\000"
	.align	2
$LC32:
	.ascii	":\000"
	.align	2
$LC33:
	.ascii	";\000"
	.align	2
$LC34:
	.ascii	"#\000"
	.align	2
$LC36:
	.ascii	"-\000"
	.section	.text.filter_table_write_proc,"ax",@progbits
	.align	2
	.ent	filter_table_write_proc
	.type	filter_table_write_proc, @function
filter_table_write_proc:
	.set	nomips16
	.frame	$sp,96,$31		# vars= 48, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
$L647:
$L688:
	addiu	$sp,$sp,-96
	lui	$2,%hi(malloc_sizes+60)
	sw	$20,80($sp)
	move	$20,$6
	sw	$19,76($sp)
	sw	$17,68($sp)
	move	$17,$5
	sw	$31,92($sp)
	sw	$22,88($sp)
	sw	$21,84($sp)
	sw	$18,72($sp)
	sw	$16,64($sp)
	lw	$4,%lo(malloc_sizes+60)($2)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L642
	li	$2,-14			# 0xfffffff2
	.set	macro
	.set	reorder

	lui	$16,%hi(get_info_from_usr_space)
	addiu	$4,$16,%lo(get_info_from_usr_space)
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$19,40($sp)
	.set	macro
	.set	reorder

	addiu	$4,$16,%lo(get_info_from_usr_space)
	move	$6,$2
	.set	noreorder
	.set	nomacro
	jal	memset
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L956
	addu	$3,$17,$20
	.set	macro
	.set	reorder

	addiu	$7,$16,%lo(get_info_from_usr_space)
	or	$3,$17,$3
	lw	$2,24($28)
	or	$3,$3,$20
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L693
	move	$6,$20
	.set	macro
	.set	reorder

	move	$5,$17
	move	$4,$7
#APP
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
#NO_APP
$L693:
	bne	$6,$0,$L956
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L962
	move	$4,$7
	.set	macro
	.set	reorder

	move	$4,$19
	move	$3,$7
	move	$2,$20
#APP
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$2,1
	sb	$1,($4)
	beqz	$1,2f
	addiu	$4,1
	bnez	$2,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
#NO_APP
	move	$4,$7
$L962:
	.set	noreorder
	.set	nomacro
	jal	strlen
	lui	$16,%hi($LC27)
	.set	macro
	.set	reorder

	addiu	$4,$16,%lo($LC27)
	addu	$2,$19,$2
	.set	noreorder
	.set	nomacro
	jal	strlen
	sb	$0,-1($2)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC27)
	lw	$4,40($sp)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L702
	lui	$16,%hi($LC29)
	.set	macro
	.set	reorder

	lui	$16,%hi($LC28)
	addiu	$4,$sp,40
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$16,%lo($LC28)
	.set	macro
	.set	reorder

	sw	$2,48($sp)
	addiu	$5,$16,%lo($LC28)
$L963:
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$4,$sp,40
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,48($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi(filter_tlb)
	sll	$5,$2,3
	addiu	$3,$3,%lo(filter_tlb)
	addu	$5,$5,$3
	lw	$5,4($5)
	.set	noreorder
	.set	nomacro
	jal	filter_table_regist
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$2,40($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L963
	addiu	$5,$16,%lo($LC28)
	.set	macro
	.set	reorder

	j	$L960
$L702:
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC29)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC29)
	lw	$4,40($sp)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L707
	lui	$16,%hi($LC30)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	filter_table_flush
	move	$4,$0
	.set	macro
	.set	reorder

	j	$L960
$L707:
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC30)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC30)
	lw	$4,40($sp)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L709
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(log_enable)
	.set	noreorder
	.set	nomacro
	j	$L960
	sw	$3,%lo(log_enable)($2)
	.set	macro
	.set	reorder

$L709:
	lui	$16,%hi($LC31)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC31)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC31)
	lw	$4,40($sp)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L960
	lui	$5,%hi($LC32)
	.set	macro
	.set	reorder

	addiu	$4,$sp,40
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC32)
	.set	macro
	.set	reorder

	sw	$2,44($sp)
	lui	$5,%hi($LC33)
$L970:
	addiu	$4,$sp,40
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC33)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L960
	sw	$2,44($sp)
	.set	macro
	.set	reorder

	lui	$5,%hi($LC28)
	move	$4,$2
	addiu	$5,$5,%lo($LC28)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L964
	lui	$22,%hi($LC34)
	.set	macro
	.set	reorder

	lw	$4,44($sp)
	jal	strlen
	li	$3,1			# 0x1
	beq	$2,$3,$L960
$L964:
	addiu	$4,$sp,44
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$22,%lo($LC34)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,48($sp)
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffff
	beq	$2,$3,$L960
	addiu	$4,$sp,44
$L969:
	addiu	$5,$22,%lo($LC34)
	.set	noreorder
	.set	nomacro
	jal	strsep
	lui	$18,%hi($LC28)
	.set	macro
	.set	reorder

	addiu	$5,$18,%lo($LC28)
	addiu	$4,$sp,48
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$2,48($sp)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	addiu	$4,$sp,48
	addiu	$5,$18,%lo($LC28)
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	move	$21,$2
	li	$2,2			# 0x2
	.set	noreorder
	.set	nomacro
	beq	$16,$2,$L778
	slt	$2,$16,3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L955
	li	$2,3			# 0x3
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$16,$2,$L965
	lui	$2,%hi(malloc_sizes+20)
	.set	macro
	.set	reorder

	j	$L720
$L955:
	.set	noreorder
	.set	nomacro
	beq	$16,$2,$L835
	li	$2,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$16,$2,$L966
	lui	$2,%hi(malloc_sizes+20)
	.set	macro
	.set	reorder

	j	$L720
$L723:
$L730:
$L771:
$L965:
	lw	$4,%lo(malloc_sizes+20)($2)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	addiu	$5,$18,%lo($LC28)
	move	$16,$2
	addiu	$4,$sp,48
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L960
	addiu	$2,$2,8
	.set	macro
	.set	reorder

	sw	$16,4($16)
	sw	$2,8($16)
	sw	$2,4($2)
	lui	$2,%hi(filter_rule_num)
	sw	$16,0($16)
	sw	$0,24($16)
	lw	$2,%lo(filter_rule_num)($2)
	sw	$21,16($16)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$2,20($16)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	li	$6,4			# 0x4
	addiu	$4,$16,28
	addiu	$5,$sp,52
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,52($sp)
	.set	macro
	.set	reorder

	addiu	$5,$18,%lo($LC28)
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$4,$sp,48
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	addiu	$4,$16,32
	addiu	$5,$sp,52
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,52($sp)
	.set	macro
	.set	reorder

	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	filter_item_regist
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	lw	$2,48($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L723
	lui	$2,%hi(malloc_sizes+20)
	.set	macro
	.set	reorder

	j	$L720
$L778:
$L783:
$L824:
	lui	$2,%hi(malloc_sizes+20)
	lw	$4,%lo(malloc_sizes+20)($2)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L960
	move	$17,$2
	.set	macro
	.set	reorder

	addiu	$2,$2,8
	addiu	$5,$18,%lo($LC28)
	sw	$17,4($17)
	addiu	$4,$sp,48
	sw	$2,4($2)
	move	$18,$0
	sw	$2,8($17)
	lui	$2,%hi(filter_rule_num)
	sw	$17,0($17)
	sw	$0,24($17)
	lw	$2,%lo(filter_rule_num)($2)
	sw	$21,16($17)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$2,20($17)
	.set	macro
	.set	reorder

	sw	$2,56($sp)
	lui	$5,%hi($LC36)
$L967:
	addiu	$4,$sp,56
	addiu	$5,$5,%lo($LC36)
	.set	noreorder
	.set	nomacro
	jal	strsep
	addu	$16,$18,$17
	.set	macro
	.set	reorder

	addiu	$18,$18,1
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	sb	$2,28($16)
	lw	$2,56($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L967
	lui	$5,%hi($LC36)
	.set	macro
	.set	reorder

	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	filter_item_regist
	li	$4,2			# 0x2
	.set	macro
	.set	reorder

	j	$L720
$L835:
$L842:
$L883:
	lui	$2,%hi(malloc_sizes+20)
$L968:
	lw	$4,%lo(malloc_sizes+20)($2)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	addiu	$5,$18,%lo($LC28)
	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L960
	addiu	$4,$sp,48
	.set	macro
	.set	reorder

	jal	strsep
	sw	$2,56($sp)
	lb	$2,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L837
	addiu	$3,$16,8
	.set	macro
	.set	reorder

	sw	$16,4($16)
	lui	$2,%hi(filter_rule_num)
	sw	$3,4($3)
	sw	$16,0($16)
	sw	$3,8($16)
	sw	$0,24($16)
	lw	$2,%lo(filter_rule_num)($2)
	sw	$21,16($16)
	sw	$2,20($16)
	lw	$4,56($sp)
	jal	strlen
	sltu	$2,$2,41
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L892
	li	$5,39			# 0x27
	.set	macro
	.set	reorder

	lw	$4,56($sp)
	jal	strlen
	move	$5,$2
$L892:
	lw	$4,56($sp)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L894
	addiu	$3,$16,28
	.set	macro
	.set	reorder

	move	$2,$5
#APP
	.set	noreorder
	.set	noat
1:	lbu	$1,($4)
	subu	$2,1
	sb	$1,($3)
	beqz	$1,2f
	addiu	$3,1
	bnez	$2,1b
	addiu	$4,1
2:
	.set	at
	.set	reorder
#NO_APP
$L894:
	addu	$2,$5,$16
	li	$4,3			# 0x3
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	filter_item_regist
	sb	$0,28($2)
	.set	macro
	.set	reorder

$L837:
	lw	$2,48($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L968
	lui	$2,%hi(malloc_sizes+20)
	.set	macro
	.set	reorder

	j	$L720
$L896:
$L903:
$L944:
$L966:
	li	$5,208			# 0xd0
	lw	$4,%lo(malloc_sizes+20)($2)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	lui	$17,%hi($LC28)
	.set	macro
	.set	reorder

	addiu	$5,$17,%lo($LC28)
	move	$16,$2
	addiu	$4,$sp,48
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L960
	addiu	$2,$2,8
	.set	macro
	.set	reorder

	sw	$16,4($16)
	sw	$2,8($16)
	sw	$2,4($2)
	lui	$2,%hi(filter_rule_num)
	sw	$16,0($16)
	sw	$0,24($16)
	lw	$2,%lo(filter_rule_num)($2)
	sw	$21,16($16)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$2,20($16)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	li	$6,4			# 0x4
	addiu	$4,$16,28
	addiu	$5,$sp,60
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,60($sp)
	.set	macro
	.set	reorder

	addiu	$4,$sp,48
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$17,%lo($LC28)
	.set	macro
	.set	reorder

	move	$4,$2
	sw	$2,56($sp)
	lw	$2,60($sp)
	bne	$2,$0,$L951
	jal	str2hexnum
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L961
	li	$2,1440			# 0x5a0
	.set	macro
	.set	reorder

$L951:
	lw	$4,56($sp)
	jal	str2hexnum
$L961:
	li	$6,4			# 0x4
	addiu	$4,$16,32
	addiu	$5,$sp,60
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,60($sp)
	.set	macro
	.set	reorder

	addiu	$5,$17,%lo($LC28)
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$4,$sp,48
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	addiu	$4,$16,36
	addiu	$5,$sp,60
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L953
	sw	$2,60($sp)
	.set	macro
	.set	reorder

	li	$2,255			# 0xff
	sw	$2,60($sp)
$L953:
	jal	memcpy
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	filter_item_regist
	li	$4,4			# 0x4
	.set	macro
	.set	reorder

	lw	$2,48($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L896
	lui	$2,%hi(malloc_sizes+20)
	.set	macro
	.set	reorder

$L720:
	lw	$2,44($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L969
	addiu	$4,$sp,44
	.set	macro
	.set	reorder

	lui	$2,%hi(filter_rule_num)
	lw	$3,%lo(filter_rule_num)($2)
	addiu	$3,$3,1
	sw	$3,%lo(filter_rule_num)($2)
	lw	$2,40($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L970
	lui	$5,%hi($LC33)
	.set	macro
	.set	reorder

$L960:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L642
	move	$2,$20
	.set	macro
	.set	reorder

$L956:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$19
	.set	macro
	.set	reorder

	li	$2,-14			# 0xfffffff2
$L642:
	lw	$31,92($sp)
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
	addiu	$sp,$sp,96
	.set	macro
	.set	reorder

	.end	filter_table_write_proc
	.section	.text.dos_pkt_locate,"ax",@progbits
	.align	2
	.ent	dos_pkt_locate
	.type	dos_pkt_locate, @function
dos_pkt_locate:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lw	$6,12($4)
	lui	$2,%hi(ConnectedIp)
	addiu	$2,$2,%lo(ConnectedIp)
	andi	$3,$6,0xff
	andi	$5,$6,0x3ff
	sll	$3,$3,2
	sll	$4,$5,3
	addu	$3,$3,$2
	addu	$4,$4,$5
	lui	$2,%hi(dos_pkt)
	sll	$4,$4,2
	lw	$3,0($3)
	addiu	$2,$2,%lo(dos_pkt)
	bne	$6,$3,$L972
	addu	$4,$4,$2

	lw	$2,24($4)
	bne	$2,$6,$L978
	move	$5,$0

	j	$L971
	sb	$0,0($4)

$L972:
	lw	$2,24($4)
$L978:
	beq	$2,$6,$L977
	move	$5,$0

	lb	$2,0($4)
	bne	$2,$0,$L971
	nop

	lui	$2,%hi(cur_p_pkt)
	li	$5,1			# 0x1
	sw	$4,%lo(cur_p_pkt)($2)
	li	$2,1
	sw	$6,24($4)
	j	$L971
	sb	$2,0($4)

$L977:
	lb	$3,0($4)
	li	$2,1			# 0x1
	bne	$3,$2,$L971
	nop

	lui	$2,%hi(cur_p_pkt)
	li	$5,1			# 0x1
	sw	$4,%lo(cur_p_pkt)($2)
$L971:
	j	$31
	move	$2,$5

	.set	macro
	.set	reorder
	.end	dos_pkt_locate
	.rdata
	.align	2
$LC37:
	.ascii	"DoS: Whole System SYN Flood Attack\n\000"
	.align	2
$LC38:
	.ascii	"DoS: Whole System FIN Flood Attack\n\000"
	.align	2
$LC39:
	.ascii	"DoS: Whole System UDP Flood Attack\n\000"
	.align	2
$LC40:
	.ascii	"DoS: Whole System ICMP Flood Attack\n\000"
	.align	2
$LC41:
	.ascii	"DoS: Per-source SYN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\n\000"
	.align	2
$LC42:
	.ascii	"DoS: Per-source FIN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\n\000"
	.align	2
$LC43:
	.ascii	"DoS: Per-source UDP Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\n\000"
	.align	2
$LC44:
	.ascii	"DoS: Per-source ICMP Flood Attack source=%u.%u.%u.%u des"
	.ascii	"tination=%u.%u.%u.%u\n\000"
	.align	2
$LC45:
	.ascii	"DoS: Port Scan Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\n\000"
	.align	2
$LC46:
	.ascii	"DoS: Tcp Scan Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\n\000"
	.align	2
$LC47:
	.ascii	"DoS: Tcp SYN With Data Attack source=%u.%u.%u.%u destina"
	.ascii	"tion=%u.%u.%u.%u\n\000"
	.align	2
$LC48:
	.ascii	"DoS: IP Land Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\n\000"
	.align	2
$LC49:
	.ascii	"DoS: UdpEchoChargen Attack source=%u.%u.%u.%u destinatio"
	.ascii	"n=%u.%u.%u.%u\n\000"
	.align	2
$LC50:
	.ascii	"DoS: UdpBomb Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\n\000"
	.align	2
$LC51:
	.ascii	"DoS: PingOfDeath Attack source=%u.%u.%u.%u destination=%"
	.ascii	"u.%u.%u.%u\n\000"
	.align	2
$LC52:
	.ascii	"DoS: IcmpSmurf Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\n\000"
	.align	2
$LC53:
	.ascii	"DoS: IpSpoof Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\n\000"
	.align	2
$LC54:
	.ascii	"DoS: TearDrop Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\n\000"
	.section	.text.dos_timer_fn,"ax",@progbits
	.align	2
	.ent	dos_timer_fn
	.type	dos_timer_fn, @function
dos_timer_fn:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 0, regs= 3/0, args= 40, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	lui	$6,%hi(item)
	sw	$31,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lw	$5,%lo(item)($6)
	beq	$5,$0,$L980
	lui	$2,%hi(block_time)

	lui	$4,%hi(block_count)
	lw	$3,%lo(block_time)($2)
	lw	$2,%lo(block_count)($4)
	slt	$2,$2,$3
	bne	$2,$0,$L1029
	lui	$2,%hi(block)

	sw	$0,%lo(block)($2)
	sw	$0,%lo(block_count)($4)
	lui	$2,%hi(block)
$L1029:
	lw	$3,%lo(block)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L1030
	andi	$2,$5,0x1e

	lw	$2,%lo(block_count)($4)
	addiu	$2,$2,1
	sw	$2,%lo(block_count)($4)
	andi	$2,$5,0x1e
$L1030:
	move	$8,$0
	move	$9,$0
	move	$10,$0
	beq	$2,$0,$L996
	move	$12,$0

	lw	$3,%lo(item)($6)
	lui	$2,%hi(dos_pkt)
	addiu	$5,$2,%lo(dos_pkt)
	li	$6,1			# 0x1
	andi	$15,$3,0x10
	andi	$14,$3,0x2
	andi	$13,$3,0x4
	andi	$11,$3,0x8
	li	$7,1023			# 0x3ff
$L991:
	lbu	$4,0($5)
	sll	$2,$4,24
	sra	$2,$2,24
	bne	$2,$6,$L1031
	sll	$2,$4,24

	beq	$14,$0,$L1032
	sra	$2,$2,24

	lw	$2,4($5)
	slt	$3,$0,$2
	addu	$2,$8,$2
	movn	$8,$2,$3		#conditional move  
	sll	$2,$4,24
$L1031:
	sra	$2,$2,24
$L1032:
	bne	$2,$6,$L1033
	sll	$2,$4,24

	beq	$13,$0,$L1034
	sra	$2,$2,24

	lw	$2,8($5)
	slt	$3,$0,$2
	addu	$2,$9,$2
	movn	$9,$2,$3		#conditional move  
	sll	$2,$4,24
$L1033:
	sra	$2,$2,24
$L1034:
	bne	$2,$6,$L1035
	sll	$2,$4,24

	beq	$11,$0,$L1036
	sra	$2,$2,24

	lw	$2,12($5)
	slt	$3,$0,$2
	addu	$2,$10,$2
	movn	$10,$2,$3		#conditional move  
	sll	$2,$4,24
$L1035:
	sra	$2,$2,24
$L1036:
	bne	$2,$6,$L986
	nop

	beq	$15,$0,$L986
	nop

	lw	$2,16($5)
	slt	$3,$0,$2
	addu	$2,$12,$2
	movn	$12,$2,$3		#conditional move  
$L986:
	addiu	$7,$7,-1
	bgez	$7,$L991
	addiu	$5,$5,36

	lui	$2,%hi(whole_syn_threshold)
	lw	$3,%lo(whole_syn_threshold)($2)
	slt	$2,$3,$8
	beq	$2,$0,$L1037
	lui	$2,%hi(whole_fin_threshold)

	lui	$2,%hi(item)
	lw	$2,%lo(item)($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L1037
	lui	$2,%hi(whole_fin_threshold)

	blez	$3,$L1037
	lui	$3,%hi(LogFlag)

	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x2
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(whole_fin_threshold)
$L1037:
	lw	$3,%lo(whole_fin_threshold)($2)
	slt	$2,$3,$9
	beq	$2,$0,$L1038
	lui	$2,%hi(whole_udp_threshold)

	lui	$2,%hi(item)
	lw	$2,%lo(item)($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L1038
	lui	$2,%hi(whole_udp_threshold)

	blez	$3,$L1038
	lui	$3,%hi(LogFlag)

	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x4
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(whole_udp_threshold)
$L1038:
	lw	$3,%lo(whole_udp_threshold)($2)
	slt	$2,$3,$10
	beq	$2,$0,$L1039
	lui	$2,%hi(whole_icmp_threshold)

	lui	$2,%hi(item)
	lw	$2,%lo(item)($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L1039
	lui	$2,%hi(whole_icmp_threshold)

	blez	$3,$L1039
	lui	$3,%hi(LogFlag)

	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x8
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(whole_icmp_threshold)
$L1039:
	lw	$3,%lo(whole_icmp_threshold)($2)
	slt	$2,$3,$12
	beq	$2,$0,$L1040
	lui	$17,%hi(LogFlag)

	lui	$2,%hi(item)
	lw	$2,%lo(item)($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L1040
	nop

	blez	$3,$L1040
	lui	$3,%hi(LogFlag)

	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x10
	sw	$2,%lo(LogFlag)($3)
$L996:
	lui	$17,%hi(LogFlag)
$L1040:
	lw	$16,%lo(LogFlag)($17)
	andi	$2,$16,0x2
	beq	$2,$0,$L1041
	andi	$2,$16,0x4

	lui	$4,%hi($LC37)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC37)

	andi	$2,$16,0x4
$L1041:
	beq	$2,$0,$L1042
	andi	$2,$16,0x8

	lui	$4,%hi($LC38)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC38)

	andi	$2,$16,0x8
$L1042:
	beq	$2,$0,$L1043
	andi	$2,$16,0x10

	lui	$4,%hi($LC39)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC39)

	andi	$2,$16,0x10
$L1043:
	beq	$2,$0,$L1044
	andi	$2,$16,0x20

	lui	$4,%hi($LC40)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC40)

	andi	$2,$16,0x20
$L1044:
	beq	$2,$0,$L1045
	andi	$2,$16,0x40

	lui	$2,%hi(attack_saddr+4)
	lui	$4,%hi($LC41)
	lbu	$5,%lo(attack_saddr+4)($2)
	lui	$2,%hi(attack_saddr+5)
	lbu	$6,%lo(attack_saddr+5)($2)
	lui	$2,%hi(attack_saddr+6)
	lbu	$7,%lo(attack_saddr+6)($2)
	lui	$2,%hi(attack_saddr+7)
	lbu	$10,%lo(attack_saddr+7)($2)
	lui	$2,%hi(attack_daddr+4)
	lbu	$9,%lo(attack_daddr+4)($2)
	lui	$2,%hi(attack_daddr+5)
	lbu	$8,%lo(attack_daddr+5)($2)
	lui	$2,%hi(attack_daddr+6)
	lbu	$3,%lo(attack_daddr+6)($2)
	lui	$2,%hi(attack_daddr+7)
	lbu	$2,%lo(attack_daddr+7)($2)
	addiu	$4,$4,%lo($LC41)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x40
$L1045:
	beq	$2,$0,$L1046
	andi	$2,$16,0x80

	lui	$2,%hi(attack_saddr+8)
	lui	$4,%hi($LC42)
	lbu	$5,%lo(attack_saddr+8)($2)
	lui	$2,%hi(attack_saddr+9)
	lbu	$6,%lo(attack_saddr+9)($2)
	lui	$2,%hi(attack_saddr+10)
	lbu	$7,%lo(attack_saddr+10)($2)
	lui	$2,%hi(attack_saddr+11)
	lbu	$10,%lo(attack_saddr+11)($2)
	lui	$2,%hi(attack_daddr+8)
	lbu	$9,%lo(attack_daddr+8)($2)
	lui	$2,%hi(attack_daddr+9)
	lbu	$8,%lo(attack_daddr+9)($2)
	lui	$2,%hi(attack_daddr+10)
	lbu	$3,%lo(attack_daddr+10)($2)
	lui	$2,%hi(attack_daddr+11)
	lbu	$2,%lo(attack_daddr+11)($2)
	addiu	$4,$4,%lo($LC42)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x80
$L1046:
	beq	$2,$0,$L1047
	andi	$2,$16,0x100

	lui	$2,%hi(attack_saddr+12)
	lui	$4,%hi($LC43)
	lbu	$5,%lo(attack_saddr+12)($2)
	lui	$2,%hi(attack_saddr+13)
	lbu	$6,%lo(attack_saddr+13)($2)
	lui	$2,%hi(attack_saddr+14)
	lbu	$7,%lo(attack_saddr+14)($2)
	lui	$2,%hi(attack_saddr+15)
	lbu	$10,%lo(attack_saddr+15)($2)
	lui	$2,%hi(attack_daddr+12)
	lbu	$9,%lo(attack_daddr+12)($2)
	lui	$2,%hi(attack_daddr+13)
	lbu	$8,%lo(attack_daddr+13)($2)
	lui	$2,%hi(attack_daddr+14)
	lbu	$3,%lo(attack_daddr+14)($2)
	lui	$2,%hi(attack_daddr+15)
	lbu	$2,%lo(attack_daddr+15)($2)
	addiu	$4,$4,%lo($LC43)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x100
$L1047:
	beq	$2,$0,$L1048
	andi	$2,$16,0x200

	lui	$2,%hi(attack_saddr+16)
	lui	$4,%hi($LC44)
	lbu	$5,%lo(attack_saddr+16)($2)
	lui	$2,%hi(attack_saddr+17)
	lbu	$6,%lo(attack_saddr+17)($2)
	lui	$2,%hi(attack_saddr+18)
	lbu	$7,%lo(attack_saddr+18)($2)
	lui	$2,%hi(attack_saddr+19)
	lbu	$10,%lo(attack_saddr+19)($2)
	lui	$2,%hi(attack_daddr+16)
	lbu	$9,%lo(attack_daddr+16)($2)
	lui	$2,%hi(attack_daddr+17)
	lbu	$8,%lo(attack_daddr+17)($2)
	lui	$2,%hi(attack_daddr+18)
	lbu	$3,%lo(attack_daddr+18)($2)
	lui	$2,%hi(attack_daddr+19)
	lbu	$2,%lo(attack_daddr+19)($2)
	addiu	$4,$4,%lo($LC44)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x200
$L1048:
	beq	$2,$0,$L1049
	andi	$2,$16,0x8000

	lui	$2,%hi(attack_saddr+20)
	lui	$4,%hi($LC45)
	lbu	$5,%lo(attack_saddr+20)($2)
	lui	$2,%hi(attack_saddr+21)
	lbu	$6,%lo(attack_saddr+21)($2)
	lui	$2,%hi(attack_saddr+22)
	lbu	$7,%lo(attack_saddr+22)($2)
	lui	$2,%hi(attack_saddr+23)
	lbu	$10,%lo(attack_saddr+23)($2)
	lui	$2,%hi(attack_daddr+20)
	lbu	$9,%lo(attack_daddr+20)($2)
	lui	$2,%hi(attack_daddr+21)
	lbu	$8,%lo(attack_daddr+21)($2)
	lui	$2,%hi(attack_daddr+22)
	lbu	$3,%lo(attack_daddr+22)($2)
	lui	$2,%hi(attack_daddr+23)
	lbu	$2,%lo(attack_daddr+23)($2)
	addiu	$4,$4,%lo($LC45)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x8000
$L1049:
	beq	$2,$0,$L1050
	li	$2,65536			# 0x10000

	lui	$2,%hi(attack_saddr+44)
	lui	$4,%hi($LC46)
	lbu	$5,%lo(attack_saddr+44)($2)
	lui	$2,%hi(attack_saddr+45)
	lbu	$6,%lo(attack_saddr+45)($2)
	lui	$2,%hi(attack_saddr+46)
	lbu	$7,%lo(attack_saddr+46)($2)
	lui	$2,%hi(attack_saddr+47)
	lbu	$10,%lo(attack_saddr+47)($2)
	lui	$2,%hi(attack_daddr+44)
	lbu	$9,%lo(attack_daddr+44)($2)
	lui	$2,%hi(attack_daddr+45)
	lbu	$8,%lo(attack_daddr+45)($2)
	lui	$2,%hi(attack_daddr+46)
	lbu	$3,%lo(attack_daddr+46)($2)
	lui	$2,%hi(attack_daddr+47)
	lbu	$2,%lo(attack_daddr+47)($2)
	addiu	$4,$4,%lo($LC46)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	li	$2,65536			# 0x10000
$L1050:
	and	$2,$16,$2
	beq	$2,$0,$L1051
	andi	$2,$16,0x800

	lui	$2,%hi(attack_saddr+48)
	lui	$4,%hi($LC47)
	lbu	$5,%lo(attack_saddr+48)($2)
	lui	$2,%hi(attack_saddr+49)
	lbu	$6,%lo(attack_saddr+49)($2)
	lui	$2,%hi(attack_saddr+50)
	lbu	$7,%lo(attack_saddr+50)($2)
	lui	$2,%hi(attack_saddr+51)
	lbu	$10,%lo(attack_saddr+51)($2)
	lui	$2,%hi(attack_daddr+48)
	lbu	$9,%lo(attack_daddr+48)($2)
	lui	$2,%hi(attack_daddr+49)
	lbu	$8,%lo(attack_daddr+49)($2)
	lui	$2,%hi(attack_daddr+50)
	lbu	$3,%lo(attack_daddr+50)($2)
	lui	$2,%hi(attack_daddr+51)
	lbu	$2,%lo(attack_daddr+51)($2)
	addiu	$4,$4,%lo($LC47)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x800
$L1051:
	beq	$2,$0,$L1052
	li	$2,262144			# 0x40000

	lui	$2,%hi(attack_saddr+28)
	lui	$4,%hi($LC48)
	lbu	$5,%lo(attack_saddr+28)($2)
	lui	$2,%hi(attack_saddr+29)
	lbu	$6,%lo(attack_saddr+29)($2)
	lui	$2,%hi(attack_saddr+30)
	lbu	$7,%lo(attack_saddr+30)($2)
	lui	$2,%hi(attack_saddr+31)
	lbu	$10,%lo(attack_saddr+31)($2)
	lui	$2,%hi(attack_daddr+28)
	lbu	$9,%lo(attack_daddr+28)($2)
	lui	$2,%hi(attack_daddr+29)
	lbu	$8,%lo(attack_daddr+29)($2)
	lui	$2,%hi(attack_daddr+30)
	lbu	$3,%lo(attack_daddr+30)($2)
	lui	$2,%hi(attack_daddr+31)
	lbu	$2,%lo(attack_daddr+31)($2)
	addiu	$4,$4,%lo($LC48)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	li	$2,262144			# 0x40000
$L1052:
	and	$2,$16,$2
	beq	$2,$0,$L1053
	li	$2,131072			# 0x20000

	lui	$2,%hi(attack_saddr+56)
	lui	$4,%hi($LC49)
	lbu	$5,%lo(attack_saddr+56)($2)
	lui	$2,%hi(attack_saddr+57)
	lbu	$6,%lo(attack_saddr+57)($2)
	lui	$2,%hi(attack_saddr+58)
	lbu	$7,%lo(attack_saddr+58)($2)
	lui	$2,%hi(attack_saddr+59)
	lbu	$10,%lo(attack_saddr+59)($2)
	lui	$2,%hi(attack_daddr+56)
	lbu	$9,%lo(attack_daddr+56)($2)
	lui	$2,%hi(attack_daddr+57)
	lbu	$8,%lo(attack_daddr+57)($2)
	lui	$2,%hi(attack_daddr+58)
	lbu	$3,%lo(attack_daddr+58)($2)
	lui	$2,%hi(attack_daddr+59)
	lbu	$2,%lo(attack_daddr+59)($2)
	addiu	$4,$4,%lo($LC49)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	li	$2,131072			# 0x20000
$L1053:
	and	$2,$16,$2
	beq	$2,$0,$L1054
	andi	$2,$16,0x4000

	lui	$2,%hi(attack_saddr+52)
	lui	$4,%hi($LC50)
	lbu	$5,%lo(attack_saddr+52)($2)
	lui	$2,%hi(attack_saddr+53)
	lbu	$6,%lo(attack_saddr+53)($2)
	lui	$2,%hi(attack_saddr+54)
	lbu	$7,%lo(attack_saddr+54)($2)
	lui	$2,%hi(attack_saddr+55)
	lbu	$10,%lo(attack_saddr+55)($2)
	lui	$2,%hi(attack_daddr+52)
	lbu	$9,%lo(attack_daddr+52)($2)
	lui	$2,%hi(attack_daddr+53)
	lbu	$8,%lo(attack_daddr+53)($2)
	lui	$2,%hi(attack_daddr+54)
	lbu	$3,%lo(attack_daddr+54)($2)
	lui	$2,%hi(attack_daddr+55)
	lbu	$2,%lo(attack_daddr+55)($2)
	addiu	$4,$4,%lo($LC50)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x4000
$L1054:
	beq	$2,$0,$L1055
	andi	$2,$16,0x400

	lui	$2,%hi(attack_saddr+40)
	lui	$4,%hi($LC51)
	lbu	$5,%lo(attack_saddr+40)($2)
	lui	$2,%hi(attack_saddr+41)
	lbu	$6,%lo(attack_saddr+41)($2)
	lui	$2,%hi(attack_saddr+42)
	lbu	$7,%lo(attack_saddr+42)($2)
	lui	$2,%hi(attack_saddr+43)
	lbu	$10,%lo(attack_saddr+43)($2)
	lui	$2,%hi(attack_daddr+40)
	lbu	$9,%lo(attack_daddr+40)($2)
	lui	$2,%hi(attack_daddr+41)
	lbu	$8,%lo(attack_daddr+41)($2)
	lui	$2,%hi(attack_daddr+42)
	lbu	$3,%lo(attack_daddr+42)($2)
	lui	$2,%hi(attack_daddr+43)
	lbu	$2,%lo(attack_daddr+43)($2)
	addiu	$4,$4,%lo($LC51)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x400
$L1055:
	beq	$2,$0,$L1056
	andi	$2,$16,0x1000

	lui	$2,%hi(attack_saddr+24)
	lui	$4,%hi($LC52)
	lbu	$5,%lo(attack_saddr+24)($2)
	lui	$2,%hi(attack_saddr+25)
	lbu	$6,%lo(attack_saddr+25)($2)
	lui	$2,%hi(attack_saddr+26)
	lbu	$7,%lo(attack_saddr+26)($2)
	lui	$2,%hi(attack_saddr+27)
	lbu	$10,%lo(attack_saddr+27)($2)
	lui	$2,%hi(attack_daddr+24)
	lbu	$9,%lo(attack_daddr+24)($2)
	lui	$2,%hi(attack_daddr+25)
	lbu	$8,%lo(attack_daddr+25)($2)
	lui	$2,%hi(attack_daddr+26)
	lbu	$3,%lo(attack_daddr+26)($2)
	lui	$2,%hi(attack_daddr+27)
	lbu	$2,%lo(attack_daddr+27)($2)
	addiu	$4,$4,%lo($LC52)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x1000
$L1056:
	beq	$2,$0,$L1057
	andi	$2,$16,0x2000

	lui	$2,%hi(attack_saddr+32)
	lui	$4,%hi($LC53)
	lbu	$5,%lo(attack_saddr+32)($2)
	lui	$2,%hi(attack_saddr+33)
	lbu	$6,%lo(attack_saddr+33)($2)
	lui	$2,%hi(attack_saddr+34)
	lbu	$7,%lo(attack_saddr+34)($2)
	lui	$2,%hi(attack_saddr+35)
	lbu	$10,%lo(attack_saddr+35)($2)
	lui	$2,%hi(attack_daddr+32)
	lbu	$9,%lo(attack_daddr+32)($2)
	lui	$2,%hi(attack_daddr+33)
	lbu	$8,%lo(attack_daddr+33)($2)
	lui	$2,%hi(attack_daddr+34)
	lbu	$3,%lo(attack_daddr+34)($2)
	lui	$2,%hi(attack_daddr+35)
	lbu	$2,%lo(attack_daddr+35)($2)
	addiu	$4,$4,%lo($LC53)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x2000
$L1057:
	beq	$2,$0,$L1014
	nop

	lui	$2,%hi(attack_saddr+36)
	lui	$4,%hi($LC54)
	lbu	$5,%lo(attack_saddr+36)($2)
	lui	$2,%hi(attack_saddr+37)
	lbu	$6,%lo(attack_saddr+37)($2)
	lui	$2,%hi(attack_saddr+38)
	lbu	$7,%lo(attack_saddr+38)($2)
	lui	$2,%hi(attack_saddr+39)
	lbu	$10,%lo(attack_saddr+39)($2)
	lui	$2,%hi(attack_daddr+36)
	lbu	$9,%lo(attack_daddr+36)($2)
	lui	$2,%hi(attack_daddr+37)
	lbu	$8,%lo(attack_daddr+37)($2)
	lui	$2,%hi(attack_daddr+38)
	lbu	$3,%lo(attack_daddr+38)($2)
	lui	$2,%hi(attack_daddr+39)
	lbu	$2,%lo(attack_daddr+39)($2)
	addiu	$4,$4,%lo($LC54)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

$L1014:
	lui	$2,%hi(dos_pkt)
	li	$3,1023			# 0x3ff
	sw	$0,%lo(LogFlag)($17)
	addiu	$2,$2,%lo(dos_pkt)
$L1019:
	addiu	$3,$3,-1
	sb	$0,0($2)
	sw	$0,24($2)
	sw	$0,4($2)
	sw	$0,8($2)
	sw	$0,12($2)
	sw	$0,16($2)
	sw	$0,20($2)
	bgez	$3,$L1019
	addiu	$2,$2,36

$L980:
	lui	$6,%hi(none_dos_drop_pkt_cnt)
	lw	$2,%lo(none_dos_drop_pkt_cnt)($6)
	sltu	$2,$2,3001
	bne	$2,$0,$L1021
	lui	$4,%hi(item_backup)

	lui	$4,%hi(item)
	lw	$3,%lo(item)($4)
	beq	$3,$0,$L1058
	lui	$2,%hi(jiffies)

	lui	$2,%hi(item_backup)
	sw	$3,%lo(item_backup)($2)
	j	$L1023
	sw	$0,%lo(item)($4)

$L1021:
	lw	$3,%lo(item_backup)($4)
	beq	$3,$0,$L1058
	lui	$2,%hi(jiffies)

	lui	$2,%hi(item)
	sw	$3,%lo(item)($2)
	sw	$0,%lo(item_backup)($4)
$L1023:
	lui	$2,%hi(jiffies)
$L1058:
	lui	$4,%hi(dos_timer)
	lw	$5,%lo(jiffies)($2)
	addiu	$4,$4,%lo(dos_timer)
	lw	$31,48($sp)
	lw	$17,44($sp)
	addiu	$5,$5,100
	lw	$16,40($sp)
	addiu	$sp,$sp,56
	j	mod_timer
	sw	$0,%lo(none_dos_drop_pkt_cnt)($6)

	.set	macro
	.set	reorder
	.end	dos_timer_fn
	.section	.text.dos_read_proc,"ax",@progbits
	.align	2
	.ent	dos_read_proc
	.type	dos_read_proc, @function
dos_read_proc:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$19,28($sp)
	move	$19,$5
	sw	$17,20($sp)
	lui	$5,%hi($LC16)
	move	$17,$6
	lui	$6,%hi(dos_flag)
	addiu	$5,$5,%lo($LC16)
	addiu	$6,$6,%lo(dos_flag)
	sw	$18,24($sp)
	move	$18,$4
	sw	$16,16($sp)
	move	$16,$7
	sw	$31,32($sp)
	jal	sprintf
	addu	$18,$18,$17

	move	$4,$2
	addu	$2,$17,$16
	slt	$2,$2,$4
	subu	$4,$4,$17
	slt	$3,$16,$4
	movn	$4,$16,$3		#conditional move
	bne	$2,$0,$L1060
	slt	$5,$4,0

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L1060:
	sw	$18,0($19)
	move	$2,$0
	lw	$31,32($sp)
	movz	$2,$4,$5		#conditional move  
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	dos_read_proc
	.section	.text.strtok,"ax",@progbits
	.align	2
	.globl	strtok
	.ent	strtok
	.type	strtok, @function
strtok:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	move	$3,$0
	sw	$17,20($sp)
	move	$17,$5
	sw	$16,16($sp)
	move	$16,$4
	bne	$4,$0,$L1065
	sw	$31,24($sp)

	lui	$2,%hi(___strtok)
	lw	$16,%lo(___strtok)($2)
	beq	$16,$0,$L1063
	nop

$L1065:
	jal	strspn
	move	$4,$16

	move	$5,$17
	addu	$16,$16,$2
	move	$3,$0
	lb	$2,0($16)
	bne	$2,$0,$L1066
	move	$4,$16

	lui	$2,%hi(___strtok)
	j	$L1063
	sw	$0,%lo(___strtok)($2)

$L1066:
	jal	strpbrk
	nop

	beq	$2,$0,$L1067
	move	$3,$2

	lb	$2,0($2)
	beq	$2,$0,$L1068
	lui	$2,%hi(___strtok)

	sb	$0,0($3)
	addiu	$3,$3,1
$L1067:
	lui	$2,%hi(___strtok)
$L1068:
	sw	$3,%lo(___strtok)($2)
	move	$3,$16
$L1063:
	lw	$31,24($sp)
	move	$2,$3
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	strtok
	.section	.text.dos_write_proc,"ax",@progbits
	.align	2
	.ent	dos_write_proc
	.type	dos_write_proc, @function
dos_write_proc:
	.set	nomips16
	.frame	$sp,120,$31		# vars= 80, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-120
	sltu	$2,$6,2
	sw	$19,108($sp)
	li	$3,-14			# 0xfffffff2
	sw	$18,104($sp)
	move	$19,$6
	sw	$17,100($sp)
	li	$18,1			# 0x1
	sw	$31,112($sp)
	addiu	$17,$sp,16
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1069
	sw	$16,96($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L1071
	addiu	$3,$5,80
	.set	macro
	.set	reorder

	lw	$2,24($28)
	or	$3,$5,$3
	and	$2,$2,$3
	lui	$3,%hi(dos_flag)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1071
	addiu	$3,$3,%lo(dos_flag)
	.set	macro
	.set	reorder

	move	$4,$3
	li	$6,80			# 0x50
#APP
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L1071
	li	$2,80			# 0x50
	.set	macro
	.set	reorder

	move	$4,$17
#APP
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$2,1
	sb	$1,($4)
	beqz	$1,2f
	addiu	$4,1
	bnez	$2,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	j	$L1100
	lui	$5,%hi($LC28)
	.set	macro
	.set	reorder

$L1099:
	move	$4,$16
	move	$5,$0
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	move	$17,$0
	.set	macro
	.set	reorder

	addiu	$3,$18,-1
	move	$4,$2
	sltu	$2,$3,13
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1101
	addiu	$2,$18,1
	.set	macro
	.set	reorder

	sll	$2,$3,2
	lui	$3,%hi($L1098)
	addiu	$3,$3,%lo($L1098)
	addu	$2,$2,$3
	lw	$2,0($2)
	j	$2
	.rdata
	.align	2
$L1098:
	.word	$L1084
	.word	$L1085
	.word	$L1086
	.word	$L1087
	.word	$L1088
	.word	$L1089
	.word	$L1090
	.word	$L1091
	.word	$L1092
	.word	$L1093
	.word	$L1094
	.word	$L1095
	.word	$L1096
	.section	.text.dos_write_proc
$L1084:
	lui	$2,%hi(op_mode)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sb	$4,%lo(op_mode)($2)
	.set	macro
	.set	reorder

$L1085:
	move	$4,$16
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lui	$3,%hi(lan_addr)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$2,%lo(lan_addr)($3)
	.set	macro
	.set	reorder

$L1086:
	move	$4,$16
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
	j	$L1083
	sw	$2,%lo(lan_mask)($3)
	.set	macro
	.set	reorder

$L1087:
	lui	$2,%hi(item)
	sw	$4,%lo(item)($2)
	lui	$2,%hi(item_backup)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$0,%lo(item_backup)($2)
	.set	macro
	.set	reorder

$L1088:
	lui	$2,%hi(whole_syn_threshold)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$4,%lo(whole_syn_threshold)($2)
	.set	macro
	.set	reorder

$L1089:
	lui	$2,%hi(whole_fin_threshold)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$4,%lo(whole_fin_threshold)($2)
	.set	macro
	.set	reorder

$L1090:
	lui	$2,%hi(whole_udp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$4,%lo(whole_udp_threshold)($2)
	.set	macro
	.set	reorder

$L1091:
	lui	$2,%hi(whole_icmp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$4,%lo(whole_icmp_threshold)($2)
	.set	macro
	.set	reorder

$L1092:
	lui	$2,%hi(per_syn_threshold)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$4,%lo(per_syn_threshold)($2)
	.set	macro
	.set	reorder

$L1093:
	lui	$2,%hi(per_fin_threshold)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$4,%lo(per_fin_threshold)($2)
	.set	macro
	.set	reorder

$L1094:
	lui	$2,%hi(per_udp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$4,%lo(per_udp_threshold)($2)
	.set	macro
	.set	reorder

$L1095:
	lui	$2,%hi(per_icmp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L1083
	sw	$4,%lo(per_icmp_threshold)($2)
	.set	macro
	.set	reorder

$L1096:
	lui	$2,%hi(block_time)
	sw	$4,%lo(block_time)($2)
$L1083:
	addiu	$2,$18,1
$L1101:
	andi	$18,$2,0x00ff
	lui	$5,%hi($LC28)
$L1100:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	strtok
	addiu	$5,$5,%lo($LC28)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1099
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1069
	move	$3,$19
	.set	macro
	.set	reorder

$L1071:
	li	$3,-14			# 0xfffffff2
$L1069:
	lw	$31,112($sp)
	move	$2,$3
	lw	$19,108($sp)
	lw	$18,104($sp)
	lw	$17,100($sp)
	lw	$16,96($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,120
	.set	macro
	.set	reorder

	.end	dos_write_proc
	.section	.text.filter_addconnect,"ax",@progbits
	.align	2
	.globl	filter_addconnect
	.ent	filter_addconnect
	.type	filter_addconnect, @function
filter_addconnect:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	andi	$3,$4,0xff
	lui	$2,%hi(ConnectedIp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$3,$3,$2
	j	$31
	sw	$4,0($3)

	.set	macro
	.set	reorder
	.end	filter_addconnect
	.section	.text.filter_delconnect,"ax",@progbits
	.align	2
	.globl	filter_delconnect
	.ent	filter_delconnect
	.type	filter_delconnect, @function
filter_delconnect:
	.set	nomips16
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
	.section	.text.filter_checkConnect,"ax",@progbits
	.align	2
	.globl	filter_checkConnect
	.ent	filter_checkConnect
	.type	filter_checkConnect, @function
filter_checkConnect:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	andi	$3,$4,0xff
	lui	$2,%hi(ConnectedIp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$3,$3,$2
	lw	$3,0($3)
	beq	$3,$0,$L1106
	move	$5,$0

	beq	$3,$4,$L1105
	nop

$L1106:
	li	$5,1			# 0x1
$L1105:
	j	$31
	move	$2,$5

	.set	macro
	.set	reorder
	.end	filter_checkConnect
	.section	.text.unregister_customRspStr,"ax",@progbits
	.align	2
	.globl	unregister_customRspStr
	.ent	unregister_customRspStr
	.type	unregister_customRspStr, @function
unregister_customRspStr:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$16,16($sp)
	lui	$16,%hi(block_message)
	lw	$2,%lo(block_message)($16)
	sw	$31,20($sp)
	beq	$2,$0,$L1107
	move	$4,$2

	jal	kfree
	nop

	sw	$0,%lo(block_message)($16)
$L1107:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	unregister_customRspStr
	.section	.text.register_customRspStr,"ax",@progbits
	.align	2
	.globl	register_customRspStr
	.ent	register_customRspStr
	.type	register_customRspStr, @function
register_customRspStr:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 16, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$16,32($sp)
	move	$16,$4
	sw	$17,36($sp)
	sw	$31,40($sp)
	jal	unregister_customRspStr
	lui	$17,%hi($LC24)

	beq	$16,$0,$L1110
	move	$4,$16

$L1115:
$L1156:
	jal	strlen
	nop

	li	$5,208			# 0xd0
	jal	__kmalloc
	addiu	$4,$2,1

	lui	$5,%hi($LC6)
	move	$4,$2
	addiu	$5,$5,%lo($LC6)
	j	$L1211
	move	$6,$16

$L1110:
$L1165:
$L1206:
	jal	strlen
	addiu	$4,$17,%lo($LC24)

	li	$5,208			# 0xd0
	jal	__kmalloc
	move	$4,$2

	lui	$5,%hi($LC6)
	move	$4,$2
	addiu	$5,$5,%lo($LC6)
	addiu	$6,$17,%lo($LC24)
$L1211:
	lui	$3,%hi(block_message)
	beq	$2,$0,$L1109
	sw	$2,%lo(block_message)($3)

	jal	sprintf
	nop

$L1109:
	lw	$31,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	register_customRspStr
	.section	.text.register_customRspHook,"ax",@progbits
	.align	2
	.globl	register_customRspHook
	.ent	register_customRspHook
	.type	register_customRspHook, @function
register_customRspHook:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	beq	$4,$0,$L1213
	lui	$3,%hi(fast_path_cusRsp401_func)

	j	$L1214
	sw	$4,%lo(fast_path_cusRsp401_func)($3)

$L1213:
	lui	$2,%hi(GenerateHTTP401)
	addiu	$2,$2,%lo(GenerateHTTP401)
	sw	$2,%lo(fast_path_cusRsp401_func)($3)
$L1214:
	beq	$5,$0,$L1215
	lui	$3,%hi(fast_path_cusRspTCPFinAck_func)

	j	$L1216
	sw	$5,%lo(fast_path_cusRspTCPFinAck_func)($3)

$L1215:
	lui	$2,%hi(GenerateTCPFINACK)
	addiu	$2,$2,%lo(GenerateTCPFINACK)
	sw	$2,%lo(fast_path_cusRspTCPFinAck_func)($3)
$L1216:
	beq	$6,$0,$L1217
	lui	$3,%hi(fast_path_cusRspTCPEndAck_func)

	j	$31
	sw	$6,%lo(fast_path_cusRspTCPEndAck_func)($3)

$L1217:
	lui	$2,%hi(GenerateTCPENDACK)
	addiu	$2,$2,%lo(GenerateTCPENDACK)
	j	$31
	sw	$2,%lo(fast_path_cusRspTCPEndAck_func)($3)

	.set	macro
	.set	reorder
	.end	register_customRspHook
	.section	.text.unregister_customRspHook,"ax",@progbits
	.align	2
	.globl	unregister_customRspHook
	.ent	unregister_customRspHook
	.type	unregister_customRspHook, @function
unregister_customRspHook:
	.set	nomips16
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
	.section	.text.filter_enter,"ax",@progbits
	.align	2
	.globl	filter_enter
	.ent	filter_enter
	.type	filter_enter, @function
filter_enter:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 0, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$21,36($sp)
	lui	$21,%hi(item)
	lw	$2,%lo(item)($21)
	sw	$23,44($sp)
	move	$23,$4
	sw	$31,48($sp)
	sw	$22,40($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	bne	$2,$0,$L1221
	sw	$16,16($sp)

	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	bne	$2,$0,$L1325
	lui	$3,%hi(none_dos_drop_pkt_cnt)

$L1221:
	jal	rtl_get_skb_type
	move	$4,$23

	bne	$2,$0,$L1220
	li	$4,4			# 0x4

	jal	rtl_get_skb_protocol
	move	$4,$23

	li	$3,2048			# 0x800
	bne	$2,$3,$L1220
	li	$4,4			# 0x4

	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	bne	$2,$0,$L1224
	nop

	jal	FilterWeb_v2
	move	$4,$23

	beq	$2,$0,$L1225
	lui	$3,%hi(fast_path_cusRsp401_func)

	lw	$2,%lo(fast_path_cusRsp401_func)($3)
	j	$L1324
	nop

$L1225:
	lw	$2,%lo(fast_path_cusRsp401_func)($3)
	beq	$2,$0,$L1224
	lui	$2,%hi(url_cache)

	li	$4,1			# 0x1
	addiu	$3,$2,%lo(url_cache)
	lbu	$2,15($3)
	bne	$2,$4,$L1230
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)

	lw	$2,%lo(fast_path_cusRspTCPFinAck_func)($2)
	j	$L1324
	nop

$L1230:
	lbu	$2,16($3)
	bne	$2,$4,$L1224
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)

	lw	$2,%lo(fast_path_cusRspTCPEndAck_func)($2)
$L1324:
	beq	$2,$0,$L1234
	nop

	jal	$2
	move	$4,$23

$L1234:
	j	$L1220
	move	$4,$0

$L1224:
	lw	$2,%lo(item)($21)
	andi	$2,$2,0x1
	beq	$2,$0,$L1235
	lui	$22,%hi(item)

	move	$4,$23
	jal	rtl_ip_hdr
	lui	$20,%hi(wan_ppp_dev)

	lui	$4,%hi($LC1)
	move	$16,$2
	addiu	$4,$4,%lo($LC1)
	lbu	$2,0($2)
	lui	$19,%hi(wan_dev)
	move	$17,$0
	andi	$2,$2,0xf
	sll	$2,$2,2
	jal	rtl_get_dev_by_name
	addu	$18,$16,$2

	lui	$4,%hi($LC2)
	addiu	$4,$4,%lo($LC2)
	jal	rtl_get_dev_by_name
	sw	$2,%lo(wan_ppp_dev)($20)

	move	$4,$23
	jal	rtl_get_skb_dev
	sw	$2,%lo(wan_dev)($19)

	beq	$2,$0,$L1235
	nop

	jal	rtl_get_skb_dev
	move	$4,$23

	lw	$3,%lo(wan_dev)($19)
	beq	$2,$3,$L1238
	nop

	jal	rtl_get_skb_dev
	move	$4,$23

	lw	$3,%lo(wan_ppp_dev)($20)
	bne	$2,$3,$L1235
	nop

$L1238:
	lw	$2,%lo(item)($22)
	andi	$2,$2,0x3fe
	beq	$2,$0,$L1326
	lui	$6,%hi(item)

	jal	dos_pkt_locate
	move	$4,$16

	beq	$2,$0,$L1326
	lui	$6,%hi(item)

	lw	$6,%lo(item)($22)
	andi	$2,$6,0x22
	beq	$2,$0,$L1241
	lui	$3,%hi(cur_p_pkt)

	lbu	$2,9($16)
	lw	$5,%lo(cur_p_pkt)($3)
	li	$3,6			# 0x6
	bne	$2,$3,$L1242
	li	$3,2			# 0x2

	lbu	$2,13($18)
	andi	$2,$2,0x3f
	bne	$2,$3,$L1242
	lui	$7,%hi(block)

	li	$2,1			# 0x1
	lw	$3,%lo(block)($7)
	bne	$3,$2,$L1243
	lui	$2,%hi(attack_saddr+4)

	lw	$3,%lo(attack_saddr+4)($2)
	lw	$2,12($16)
	beq	$3,$2,$L1244
	li	$4,-1			# 0xffffffff

$L1243:
	lw	$2,4($5)
	lui	$3,%hi(per_syn_threshold)
	addiu	$2,$2,1
	sw	$2,4($5)
	lw	$3,%lo(per_syn_threshold)($3)
	slt	$2,$3,$2
	beq	$2,$0,$L1242
	andi	$2,$6,0x20

	beq	$2,$0,$L1244
	move	$4,$0

	blez	$3,$L1244
	lui	$2,%hi(attack_saddr+4)

	lw	$3,12($16)
	lui	$5,%hi(LogFlag)
	sw	$3,%lo(attack_saddr+4)($2)
	lui	$2,%hi(attack_daddr+4)
	lw	$4,16($16)
	lw	$3,%lo(LogFlag)($5)
	sw	$4,%lo(attack_daddr+4)($2)
	li	$2,1			# 0x1
	ori	$3,$3,0x20
	sw	$2,%lo(block)($7)
	sw	$3,%lo(LogFlag)($5)
$L1242:
	move	$4,$0
$L1244:
	move	$17,$4
$L1241:
	lw	$6,%lo(item)($21)
	andi	$2,$6,0x44
	beq	$2,$0,$L1327
	andi	$2,$6,0x88

	lui	$3,%hi(cur_p_pkt)
	lbu	$2,9($16)
	lw	$5,%lo(cur_p_pkt)($3)
	li	$3,6			# 0x6
	bne	$2,$3,$L1249
	move	$4,$0

	lbu	$2,13($18)
	andi	$3,$2,0x3f
	li	$2,1			# 0x1
	bne	$3,$2,$L1249
	lui	$7,%hi(block)

	lw	$2,%lo(block)($7)
	bne	$2,$3,$L1248
	lui	$2,%hi(attack_saddr+8)

	lw	$3,%lo(attack_saddr+8)($2)
	lw	$2,12($16)
	beq	$3,$2,$L1249
	li	$4,-1			# 0xffffffff

$L1248:
	lw	$2,8($5)
	lui	$3,%hi(per_fin_threshold)
	addiu	$2,$2,1
	sw	$2,8($5)
	lw	$3,%lo(per_fin_threshold)($3)
	slt	$2,$3,$2
	beq	$2,$0,$L1247
	andi	$2,$6,0x40

	beq	$2,$0,$L1249
	move	$4,$0

	blez	$3,$L1249
	lui	$2,%hi(attack_saddr+8)

	lw	$3,12($16)
	lui	$5,%hi(LogFlag)
	sw	$3,%lo(attack_saddr+8)($2)
	lui	$2,%hi(attack_daddr+8)
	lw	$4,16($16)
	lw	$3,%lo(LogFlag)($5)
	sw	$4,%lo(attack_daddr+8)($2)
	li	$2,1			# 0x1
	ori	$3,$3,0x40
	sw	$2,%lo(block)($7)
	sw	$3,%lo(LogFlag)($5)
$L1247:
	move	$4,$0
$L1249:
	or	$17,$17,$4
	lw	$6,%lo(item)($21)
	andi	$2,$6,0x88
$L1327:
	beq	$2,$0,$L1251
	lui	$3,%hi(cur_p_pkt)

	lbu	$2,9($16)
	lw	$5,%lo(cur_p_pkt)($3)
	li	$3,17			# 0x11
	bne	$2,$3,$L1254
	move	$4,$0

	lui	$7,%hi(block)
	li	$2,1			# 0x1
	lw	$3,%lo(block)($7)
	bne	$3,$2,$L1253
	lui	$2,%hi(attack_saddr+12)

	lw	$3,%lo(attack_saddr+12)($2)
	lw	$2,12($16)
	beq	$3,$2,$L1254
	li	$4,-1			# 0xffffffff

$L1253:
	lw	$2,12($5)
	lui	$3,%hi(per_udp_threshold)
	addiu	$2,$2,1
	sw	$2,12($5)
	lw	$3,%lo(per_udp_threshold)($3)
	slt	$2,$3,$2
	beq	$2,$0,$L1252
	andi	$2,$6,0x80

	beq	$2,$0,$L1254
	move	$4,$0

	blez	$3,$L1254
	lui	$2,%hi(attack_saddr+12)

	lw	$3,12($16)
	lui	$5,%hi(LogFlag)
	sw	$3,%lo(attack_saddr+12)($2)
	lui	$2,%hi(attack_daddr+12)
	lw	$4,16($16)
	lw	$3,%lo(LogFlag)($5)
	sw	$4,%lo(attack_daddr+12)($2)
	li	$2,1			# 0x1
	ori	$3,$3,0x80
	sw	$2,%lo(block)($7)
	sw	$3,%lo(LogFlag)($5)
$L1252:
	move	$4,$0
$L1254:
	or	$17,$17,$4
$L1251:
	lw	$6,%lo(item)($21)
	andi	$2,$6,0x110
	beq	$2,$0,$L1256
	lui	$2,%hi(cur_p_pkt)

	lbu	$3,9($16)
	lw	$5,%lo(cur_p_pkt)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L1259
	move	$4,$0

	lui	$7,%hi(block)
	lw	$2,%lo(block)($7)
	bne	$2,$3,$L1258
	lui	$2,%hi(attack_saddr+16)

	lw	$3,%lo(attack_saddr+16)($2)
	lw	$2,12($16)
	beq	$3,$2,$L1259
	li	$4,-1			# 0xffffffff

$L1258:
	lw	$2,16($5)
	lui	$3,%hi(per_icmp_threshold)
	addiu	$2,$2,1
	sw	$2,16($5)
	lw	$3,%lo(per_icmp_threshold)($3)
	slt	$2,$3,$2
	beq	$2,$0,$L1257
	andi	$2,$6,0x100

	beq	$2,$0,$L1259
	move	$4,$0

	blez	$3,$L1259
	lui	$2,%hi(attack_saddr+16)

	lw	$3,12($16)
	lui	$5,%hi(LogFlag)
	sw	$3,%lo(attack_saddr+16)($2)
	lui	$2,%hi(attack_daddr+16)
	lw	$4,16($16)
	lw	$3,%lo(LogFlag)($5)
	sw	$4,%lo(attack_daddr+16)($2)
	li	$2,1			# 0x1
	ori	$3,$3,0x100
	sw	$2,%lo(block)($7)
	sw	$3,%lo(LogFlag)($5)
$L1257:
	move	$4,$0
$L1259:
	or	$17,$17,$4
$L1256:
	lw	$2,%lo(item)($21)
	andi	$2,$2,0x200
	beq	$2,$0,$L1328
	lui	$2,%hi(item)

	lui	$3,%hi(cur_p_pkt)
	lbu	$2,9($16)
	lw	$4,%lo(cur_p_pkt)($3)
	li	$3,6			# 0x6
	bne	$2,$3,$L1262
	nop

	lhu	$5,32($4)
	andi	$3,$5,0xffff
	beq	$3,$0,$L1263
	nop

	lhu	$2,2($18)
	beq	$3,$2,$L1263
	nop

	lw	$2,20($4)
	addiu	$2,$2,1
	sw	$2,20($4)
$L1263:
	bne	$5,$0,$L1262
	nop

	lhu	$2,2($18)
	sh	$2,32($4)
$L1262:
	lbu	$3,9($16)
	li	$2,17			# 0x11
	bne	$3,$2,$L1265
	nop

	lhu	$5,32($4)
	andi	$3,$5,0xffff
	beq	$3,$0,$L1266
	nop

	lhu	$2,2($18)
	beq	$3,$2,$L1266
	nop

	lw	$2,20($4)
	addiu	$2,$2,1
	sw	$2,20($4)
$L1266:
	bne	$5,$0,$L1265
	nop

	lhu	$2,2($18)
	sh	$2,32($4)
$L1265:
	lw	$2,%lo(item)($21)
	li	$3,8388608			# 0x800000
	and	$2,$2,$3
	beq	$2,$0,$L1268
	nop

	lw	$2,20($4)
	slt	$2,$2,11
	bne	$2,$0,$L1268
	lui	$3,%hi(LogFlag)

	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x200
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+20)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+20)($2)
	lui	$2,%hi(attack_daddr+20)
	lw	$3,16($16)
	sw	$3,%lo(attack_daddr+20)($2)
$L1268:
	lw	$2,%lo(item)($21)
	li	$3,8388608			# 0x800000
	and	$2,$2,$3
	bne	$2,$0,$L1328
	lui	$2,%hi(item)

	lw	$2,20($4)
	slt	$2,$2,201
	bne	$2,$0,$L1328
	lui	$2,%hi(item)

	lui	$3,%hi(LogFlag)
	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x200
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+20)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+20)($2)
	lui	$2,%hi(attack_daddr+20)
	lw	$3,16($16)
	sw	$3,%lo(attack_daddr+20)($2)
	lui	$2,%hi(item)
$L1328:
	li	$3,4194304			# 0x400000
	lw	$2,%lo(item)($2)
	and	$2,$2,$3
	movz	$17,$0,$2		#conditional move
	lui	$6,%hi(item)
$L1326:
	lw	$2,%lo(item)($6)
	andi	$2,$2,0x8000
	beq	$2,$0,$L1272
	li	$2,6			# 0x6

	lbu	$3,9($16)
	bne	$3,$2,$L1275
	move	$4,$0

	lbu	$2,13($18)
	lui	$3,%hi(_tcpDosScanBitmap)
	addiu	$3,$3,%lo(_tcpDosScanBitmap)
	andi	$2,$2,0x3f
	addu	$2,$2,$3
	lb	$2,0($2)
	beq	$2,$0,$L1273
	lui	$3,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x8000
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+44)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+44)($2)
	lui	$2,%hi(attack_daddr+44)
	lw	$3,16($16)
	sw	$3,%lo(attack_daddr+44)($2)
$L1273:
$L1275:
	or	$17,$17,$4
$L1272:
	lw	$2,%lo(item)($6)
	li	$5,65536			# 0x10000
	and	$2,$2,$5
	beq	$2,$0,$L1276
	li	$2,6			# 0x6

	lbu	$3,9($16)
	bne	$3,$2,$L1279
	move	$4,$0

	lbu	$2,13($18)
	li	$3,2			# 0x2
	andi	$2,$2,0x3f
	bne	$2,$3,$L1279
	nop

	lbu	$2,0($16)
	lw	$3,12($18)
	andi	$2,$2,0xf
	lhu	$4,2($16)
	sll	$2,$2,2
	srl	$3,$3,28
	subu	$4,$4,$2
	sll	$3,$3,2
	bne	$4,$3,$L1329
	lui	$3,%hi(LogFlag)

	lhu	$2,6($16)
	andi	$2,$2,0x2000
	beq	$2,$0,$L1277
	nop

$L1329:
	li	$4,-1			# 0xffffffff
	lw	$2,%lo(LogFlag)($3)
	or	$2,$2,$5
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+48)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+48)($2)
	lui	$2,%hi(attack_daddr+48)
	lw	$3,16($16)
	j	$L1279
	sw	$3,%lo(attack_daddr+48)($2)

$L1277:
	move	$4,$0
$L1279:
	or	$17,$17,$4
$L1276:
	lw	$2,%lo(item)($6)
	andi	$2,$2,0x800
	beq	$2,$0,$L1281
	nop

	lw	$3,12($16)
	lw	$2,16($16)
	bne	$3,$2,$L1283
	move	$4,$0

	lui	$3,%hi(LogFlag)
	li	$4,-1			# 0xffffffff
	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x800
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+28)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+28)($2)
	lui	$2,%hi(attack_daddr+28)
	lw	$3,16($16)
	sw	$3,%lo(attack_daddr+28)($2)
$L1283:
	or	$17,$17,$4
$L1281:
	lw	$2,%lo(item)($6)
	li	$3,262144			# 0x40000
	and	$2,$2,$3
	beq	$2,$0,$L1284
	li	$2,17			# 0x11

	lbu	$3,9($16)
	bne	$3,$2,$L1288
	move	$5,$0

	lhu	$2,2($18)
	li	$5,7			# 0x7
	beq	$2,$5,$L1330
	lui	$4,%hi(LogFlag)

	beq	$2,$3,$L1287
	li	$4,19			# 0x13

	beq	$2,$4,$L1287
	nop

	lhu	$2,0($18)
	beq	$2,$5,$L1287
	nop

	beq	$2,$3,$L1287
	nop

	bne	$2,$4,$L1285
	nop

$L1287:
	lui	$4,%hi(LogFlag)
$L1330:
	li	$3,262144			# 0x40000
	lw	$2,%lo(LogFlag)($4)
	li	$5,-1			# 0xffffffff
	or	$2,$2,$3
	sw	$2,%lo(LogFlag)($4)
	lui	$2,%hi(attack_saddr+56)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+56)($2)
	lui	$2,%hi(attack_daddr+56)
	lw	$3,16($16)
	j	$L1288
	sw	$3,%lo(attack_daddr+56)($2)

$L1285:
	move	$5,$0
$L1288:
	or	$17,$17,$5
$L1284:
	lw	$2,%lo(item)($6)
	li	$5,131072			# 0x20000
	and	$2,$2,$5
	beq	$2,$0,$L1289
	li	$2,17			# 0x11

	lbu	$3,9($16)
	bne	$3,$2,$L1293
	move	$4,$0

	lhu	$2,6($16)
	andi	$2,$2,0x3fff
	bne	$2,$0,$L1293
	nop

	lbu	$2,0($16)
	lhu	$4,2($16)
	andi	$2,$2,0xf
	sll	$2,$2,2
	lhu	$3,4($18)
	subu	$4,$4,$2
	slt	$3,$3,$4
	beq	$3,$0,$L1290
	lui	$3,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$2,%lo(LogFlag)($3)
	or	$2,$2,$5
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+52)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+52)($2)
	lui	$2,%hi(attack_daddr+52)
	lw	$3,16($16)
	j	$L1293
	sw	$3,%lo(attack_daddr+52)($2)

$L1290:
	move	$4,$0
$L1293:
	or	$17,$17,$4
$L1289:
	lw	$2,%lo(item)($6)
	andi	$2,$2,0x4000
	beq	$2,$0,$L1294
	nop

	lhu	$3,6($16)
	andi	$2,$3,0x2000
	bne	$2,$0,$L1297
	move	$4,$0

	andi	$3,$3,0x1fff
	beq	$3,$0,$L1297
	sll	$3,$3,3

	lbu	$2,0($16)
	lhu	$4,2($16)
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$3,$3,$4
	subu	$3,$3,$2
	li	$2,65535			# 0xffff
	sltu	$2,$2,$3
	beq	$2,$0,$L1295
	lui	$3,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x4000
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+40)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+40)($2)
	lui	$2,%hi(attack_daddr+40)
	lw	$3,16($16)
	j	$L1297
	sw	$3,%lo(attack_daddr+40)($2)

$L1295:
	move	$4,$0
$L1297:
	or	$17,$17,$4
$L1294:
	lw	$2,%lo(item)($6)
	andi	$2,$2,0x400
	beq	$2,$0,$L1298
	nop

	lbu	$2,0($16)
	lbu	$3,9($16)
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$4,$16,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L1299
	li	$2,8			# 0x8

	lbu	$3,0($4)
	bne	$3,$2,$L1301
	move	$4,$0

	lui	$2,%hi(lan_mask)
	lw	$4,%lo(lan_mask)($2)
	lui	$2,%hi(lan_addr)
	lw	$3,%lo(lan_addr)($2)
	lw	$2,12($16)
	and	$3,$3,$4
	and	$2,$2,$4
	bne	$2,$3,$L1299
	lui	$3,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x400
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+24)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+24)($2)
	lui	$2,%hi(attack_daddr+24)
	lw	$3,16($16)
	j	$L1301
	sw	$3,%lo(attack_daddr+24)($2)

$L1299:
	move	$4,$0
$L1301:
	or	$17,$17,$4
$L1298:
	lw	$2,%lo(item)($6)
	andi	$2,$2,0x1000
	beq	$2,$0,$L1302
	lui	$2,%hi(lan_mask)

	lw	$4,%lo(lan_mask)($2)
	lui	$2,%hi(lan_addr)
	lw	$3,%lo(lan_addr)($2)
	lw	$2,12($16)
	and	$3,$3,$4
	and	$2,$2,$4
	bne	$2,$3,$L1304
	move	$4,$0

	lbu	$3,9($16)
	li	$2,1			# 0x1
	beq	$3,$2,$L1304
	lui	$3,%hi(LogFlag)

	li	$4,-1			# 0xffffffff
	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x1000
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+32)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+32)($2)
	lui	$2,%hi(attack_daddr+32)
	lw	$3,16($16)
	sw	$3,%lo(attack_daddr+32)($2)
$L1304:
	or	$17,$17,$4
$L1302:
	lw	$2,%lo(item)($6)
	andi	$2,$2,0x2000
	beq	$2,$0,$L1237
	nop

	jal	dos_pkt_locate
	move	$4,$16

	beq	$2,$0,$L1308
	move	$4,$0

	lui	$2,%hi(cur_p_pkt)
	lw	$5,%lo(cur_p_pkt)($2)
	lhu	$7,4($16)
	lhu	$3,28($5)
	andi	$2,$7,0xffff
	beq	$2,$3,$L1309
	nop

	bne	$3,$0,$L1308
	nop

$L1309:
	lhu	$4,6($16)
	andi	$2,$4,0x3fff
	beq	$2,$0,$L1310
	andi	$2,$4,0x1fff

	lhu	$6,30($5)
	sll	$2,$2,3
	andi	$3,$6,0xffff
	slt	$2,$2,$3
	bne	$2,$0,$L1311
	andi	$2,$4,0x2000

	bne	$2,$0,$L1312
	nop

	sh	$0,30($5)
	j	$L1310
	sh	$0,28($5)

$L1312:
	sh	$7,28($5)
	lbu	$2,0($16)
	lhu	$3,2($16)
	andi	$2,$2,0xf
	addu	$3,$6,$3
	sll	$2,$2,2
	subu	$3,$3,$2
	j	$L1310
	sh	$3,30($5)

$L1311:
	lui	$3,%hi(LogFlag)
	li	$4,-1			# 0xffffffff
	lw	$2,%lo(LogFlag)($3)
	ori	$2,$2,0x2000
	sw	$2,%lo(LogFlag)($3)
	lui	$2,%hi(attack_saddr+36)
	lw	$3,12($16)
	sw	$3,%lo(attack_saddr+36)($2)
	lui	$2,%hi(attack_daddr+36)
	lw	$3,16($16)
	j	$L1308
	sw	$3,%lo(attack_daddr+36)($2)

$L1310:
	move	$4,$0
$L1308:
	or	$17,$17,$4
$L1237:
	bne	$17,$0,$L1220
	move	$4,$0

$L1235:
	jal	rtl_ip_hdr
	move	$4,$23

	lbu	$3,9($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L1220
	li	$4,4			# 0x4

	lui	$3,%hi(none_dos_drop_pkt_cnt)
$L1325:
	li	$4,1			# 0x1
	lw	$2,%lo(none_dos_drop_pkt_cnt)($3)
	addiu	$2,$2,1
	sw	$2,%lo(none_dos_drop_pkt_cnt)($3)
$L1220:
	lw	$31,48($sp)
	move	$2,$4
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	filter_enter
	.rdata
	.align	2
$LC55:
	.ascii	"enable_dos\000"
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	filter_init
	.ent	filter_init
	.type	filter_init, @function
filter_init:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(item)
	lui	$4,%hi($LC55)
	sw	$0,%lo(item)($2)
	lui	$2,%hi(none_dos_drop_pkt_cnt)
	addiu	$sp,$sp,-32
	addiu	$4,$4,%lo($LC55)
	move	$5,$0
	move	$6,$0
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
	lui	$2,%hi(url_count)
	sw	$17,20($sp)
	lui	$17,%hi(dos_timer)
	sw	$16,16($sp)
	lui	$16,%hi(_tcpDosScanBitmap)
	sw	$31,24($sp)
	jal	create_proc_entry
	sb	$0,%lo(url_count)($2)

	lui	$7,%hi(res3)
	move	$3,$2
	move	$6,$0
	addiu	$4,$17,%lo(dos_timer)
	move	$5,$0
	beq	$2,$0,$L1332
	sw	$2,%lo(res3)($7)

	lui	$2,%hi(dos_read_proc)
	addiu	$2,$2,%lo(dos_read_proc)
	sw	$2,64($3)
	lui	$2,%hi(dos_write_proc)
	lw	$3,%lo(res3)($7)
	addiu	$2,$2,%lo(dos_write_proc)
	sw	$2,68($3)
$L1332:
	jal	init_timer_key
	nop

	lui	$4,%hi(jiffies)
	lw	$2,%lo(jiffies)($4)
	addiu	$3,$17,%lo(dos_timer)
	lw	$5,%lo(jiffies)($4)
	move	$4,$3
	addiu	$2,$2,100
	sw	$2,8($3)
	lui	$2,%hi(dos_timer_fn)
	addiu	$2,$2,%lo(dos_timer_fn)
	addiu	$5,$5,100
	sw	$2,12($3)
	jal	mod_timer
	sw	$0,16($3)

	addiu	$4,$16,%lo(_tcpDosScanBitmap)
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	addiu	$3,$16,%lo(_tcpDosScanBitmap)
	li	$2,1
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
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	filter_init
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	filter_exit
	.ent	filter_exit
	.type	filter_exit, @function
filter_exit:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	lui	$4,%hi($LC55)
	sw	$16,16($sp)
	lui	$16,%hi(res3)
	sw	$31,20($sp)
	addiu	$4,$4,%lo($LC55)
	lw	$2,%lo(res3)($16)
	beq	$2,$0,$L1334
	move	$5,$2

	jal	remove_proc_entry
	nop

	sw	$0,%lo(res3)($16)
$L1334:
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
	.globl	table_list_head
	.section	.bss
	.align	2
	.type	table_list_head, @object
	.size	table_list_head, 32
table_list_head:
	.space	32
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
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
