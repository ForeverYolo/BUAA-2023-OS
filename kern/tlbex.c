#include <env.h>
#include <pmap.h>

#define mpt ((volatile Pte *)MVPT)
#define vpt ((volatile Pte *)UVPT)
static void passive_alloc(u_int va, Pde *pgdir, u_int asid) {
	struct Page *p = NULL;

	if (va < UTEMP) {
		panic("address too low");
	}

	if (va >= USTACKTOP && va < USTACKTOP + BY2PG) {
		panic("invalid memory");
	}

	if (va >= UENVS && va < UPAGES) {
		panic("envs zone");
	}

	if (va >= UPAGES && va < UVPT) {
		panic("pages zone");
	}

	if (va >= ULIM && va < KSEG2) {
		panic("kernel address");
	}

	panic_on(page_alloc(&p));
	panic_on(page_insert(pgdir, asid, p, PTE_ADDR(va), PTE_D));
}

/* Overview:
 *  Refill TLB.
 */
Pte _do_tlb_refill(u_long va, u_int asid) {
	Pte *pte;
	/* Hints:
	 *  Invoke 'page_lookup' repeatedly in a loop to find the page table entry 'pte' associated
	 *  with the virtual address 'va' in the current address space 'cur_pgdir'.
	 *
	 *  **While** 'page_lookup' returns 'NULL', indicating that the 'pte' could not be found,
	 *  allocate a new page using 'passive_alloc' until 'page_lookup' succeeds.
	 */
	/*int context = -1;
	asm("mfc0 %0, $4" : "=r"(context) :);
	int badVaddr = -1;
	asm("mfc0 %0, $8" : "=r"(badVaddr) :);
	//printk("Context : %08x\n",context);
	int TableNum = (context >> 2) & 0x7ffff;
	printk("=============================\n");
	printk("va : %08x\n", va);
	printk("badVaddr : %08x\n",badVaddr);
	printk("Context : %08x\n",context);
	printk("TableNum : %08x\n",TableNum);
	printk("mpt[TableNum] : %08x\n",&(mpt[TableNum]));
	//printk("mpt[0] : %08x\n",&(mpt[0]));
	//printk("mpt[1] : %08x\n",&(mpt[1]));
	while ( ((mpt[TableNum]) & PTE_V) == 0 )
       	{
		passive_alloc(va,cur_pgdir,asid);
	}
	return mpt[TableNum];*/ 	
	/* Exercise 2.9: Your code here. */
	while (page_lookup(cur_pgdir,va,&pte) == NULL)
	{	
		passive_alloc(va,cur_pgdir,asid);
	}
	return *pte;
}

Pte fast_tlb_refill(u_long va, u_int asid) {
	int context = -1;
	int memEntryHi = -1;
	asm("mfc0 %0, $10" : "=r"(memEntryHi) :);
	asm("mfc0 %0, $4" : "=r"(context) :);
	int badVaddr = -1;
	asm("mfc0 %0, $8" : "=r"(badVaddr) :);
	//printk("Context : %08x\n",context);
	//int TableNum = (context >> 2) & 0x7ffff;
	//printk("=============================\n");
	//printk("va : %08x\n", va);
	//printk("badVaddr : %08x\n",badVaddr);
	//printk("Context : %08x\n",context);
	//printk("TableNum : %08x\n",TableNum);
	//printk("mpt[TableNum] : %08x\n",&(mpt[TableNum]));
	//printk("mpt[0] : %08x\n",&(mpt[0]));
	//printk("mpt[1] : %08x\n",&(mpt[1]));
	Pte* pte = (Pte *)context;
	//printk("pte : %08x\n",*pte);
	//u_int* inst = (u_int *)(KADDR((*pte) & 0xffff000) + (va & 0xfff));
	//printk("inst : %08x\n",inst);
	//printk("inst : %08x\n",*inst);
	while ( ((*pte) & PTE_V) == 0 )
       	{
		//printk("Alloc ?????????????????????????????????????????????????????????????????????\n");
		passive_alloc(va,cur_pgdir,asid);
	}
	asm volatile("mtc0 %0, $10" : : "r"(memEntryHi));
	//printk("fast_tlb Done!\n");
	return *pte; 
}

#if !defined(LAB) || LAB >= 4
/* Overview:
 *   This is the TLB Mod exception handler in kernel.
 *   Our kernel allows user programs to handle TLB Mod exception in user mode, so we copy its
 *   context 'tf' into UXSTACK and modify the EPC to the registered user exception entry.
 *
 * Hints:
 *   'env_user_tlb_mod_entry' is the user space entry registered using
 *   'sys_set_user_tlb_mod_entry'.
 *
 *   The user entry should handle this TLB Mod exception and restore the context.
 */
void do_tlb_mod(struct Trapframe *tf) {
	struct Trapframe tmp_tf = *tf;

	if (tf->regs[29] < USTACKTOP || tf->regs[29] >= UXSTACKTOP) {
		tf->regs[29] = UXSTACKTOP;
	}
	tf->regs[29] -= sizeof(struct Trapframe);
	*(struct Trapframe *)tf->regs[29] = tmp_tf;

	if (curenv->env_user_tlb_mod_entry) {
		tf->regs[4] = tf->regs[29];
		tf->regs[29] -= sizeof(tf->regs[4]);
		// Hint: Set 'cp0_epc' in the context 'tf' to 'curenv->env_user_tlb_mod_entry'.
		/* Exercise 4.11: Your code here. */
		tf->cp0_epc = curenv->env_user_tlb_mod_entry;
	} else {
		panic("TLB Mod but no user handler registered");
	}
}
#endif
