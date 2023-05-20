#include <asm/asm.h>
#include <stackframe.h>

.section .text.tlb_miss_entry
tlb_miss_entry:
	bgtz    sp, fast_tlb
	nop
	j 	exc_gen_entry
	nop
fast_tlb:
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

process_fast:
	SAVE_ALL
	mfc0    a0, CP0_BADVADDR
	mfc0    a1, CP0_ENTRYHI
	srl     a1, a1, 6
	andi    a1, a1, 0b111111
	jal     fast_tlb_refill
	mtc0    v0, CP0_ENTRYLO0
	nop
	tlbwr
	nop
	j       ret_from_exception
