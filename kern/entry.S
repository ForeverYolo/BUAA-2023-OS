#include <asm/asm.h>
#include <stackframe.h>


.section .text.tlb_miss_entry
tlb_miss_entry:
	j 	process_fast
	nop

.section .text.exc_gen_entry
exc_gen_entry:
	SAVE_ALL
	mfc0 t0, CP0_CAUSE
	andi t0, 0x7c
	lw t0, exception_handlers(t0)
	jr t0
	nop


arr:
	.space 52
process_fast:
	la k0,arr

#保存现场
	mfc0	k1,CP0_EPC
	sw	k1,0(k0)
	mfc0	k1,CP0_BADVADDR
	sw	k1,4(k0)
	mfc0	k1,CP0_ENTRYHI
	sw	k1,8(k0)
	mfc0	k1,CP0_CONTEXT
	sw	k1,12(k0)
#保存现场

#记录次数
	lw	k1,16(k0)
	addi	k1,k1,1
	sw	k1,16(k0)
#记录次数

#取Context
	mfc0	k1,CP0_CONTEXT
#取Context

#保存Context寄存器的值
	move	k0,k1
#保存Context寄存器的值

#读Context(TLB重入)
	lw k1,0(k1)
#读Context(TLB重入)

check_one:
#有效位判别
	la	k1,arr
	lw	k1,12(k1)
	move 	k0,k1
	lw	k1,0(k1)
	andi	k1,k1,0x200
	bnez 	k1,success_alloc
	nop
#有效位判别

#给passive_alloc传参并跳转
failed_alloc:
	la	k1,arr
	sw	sp,20(k1)
	sw	v0,24(k1)
	sw	v1,28(k1)
	sw	s8,32(k1)
	sw	a0,36(k1)
	sw	a1,40(k1)
	sw	a2,44(k1)
	sw	a3,48(k1)

	li      sp, KSTACKTOP

	la	k1,arr
	lw	k1,4(k1)
	move	a0,k1		#BadVaddr

	lui	a1,0x8018
	lw	a1,12324(a1)

	la	k1,arr
	lw	k1,8(k1)	#EntryHi
	move    a2, k1
	srl     a2, a2, 6
	andi    a2, a2, 0b111111

	jal	passive_alloc
	nop

	la	k1,arr
	lw	sp,20(k1)
	lw	v0,24(k1)
	lw	v1,28(k1)
	lw	s8,32(k1)
	lw	a0,36(k1)
	lw	a1,40(k1)
	lw	a2,44(k1)
	lw	a3,48(k1)
#给passive_alloc传参并跳转

#判断passive_alloc 成功与否
check_two:
	la	k1,arr
	lw	k0,12(k1)
	move	k1,k0
	lw	k1,0(k1)
	andi	k1,k1,0x200
	beqz	k1,failed_alloc
	nop
#判断passive_alloc 成功与否

#最终重填
success_alloc:
	move	k1,k0
	lw	k1,0(k1)
	mtc0	k1,CP0_ENTRYLO0
	la	k0,arr
	lw	k0,8(k0)
	mtc0	k0,CP0_ENTRYHI
	nop
	tlbwr
	nop
#最终重填

#恢复现场
	la	k1,arr
	lw	k0,0(k1)

	# sw	zero,0(k1)
	# sw	zero,4(k1)
	# sw	zero,8(k1)
	# sw	zero,12(k1)
.set noreorder
	jr 	k0
	rfe
.set reorder
#恢复现场
