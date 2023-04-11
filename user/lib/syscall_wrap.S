#include <asm/asm.h>

LEAF(msyscall)
	// Just use 'syscall' instruction and return.
	nop
	syscall
	nop
	jr ra
	nop
	/* Exercise 4.1: Your code here. */

END(msyscall)
