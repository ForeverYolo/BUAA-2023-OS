#include <env.h>
#include <pmap.h>
#include <printk.h>
#include <trap.h>

extern void handle_int(void);
extern void handle_tlb(void);
extern void handle_sys(void);
extern void handle_mod(void);
extern void handle_reserved(void);
extern void handle_ov(void);

void (*exception_handlers[32])(void) = {
    [0 ... 31] = handle_reserved,
    [0] = handle_int,
    [2 ... 3] = handle_tlb,
    [12] = handle_ov,
#if !defined(LAB) || LAB >= 4
    [1] = handle_mod,
    [8] = handle_sys,
    //[12] = handle_ov,
#endif
};

/* Overview:
 *   The fallback handler when an unknown exception code is encountered.
 *   'genex.S' wraps this function in 'handle_reserved'.
 */
void do_reserved(struct Trapframe *tf) {
	print_tf(tf);
	panic("Unknown ExcCode %2d", (tf->cp0_cause >> 2) & 0x1f);
}

void do_ov(struct Trapframe *tf) {
	struct Trapframe tmp_tf = *tf;
	u_long EPC = tf->cp0_epc;
	//printk("EPC:%x\n",EPC);
	Pte * ppte;
	page_lookup(curenv->env_pgdir,EPC,&ppte);
	u_long* Ppa =(u_long*) (PTE_ADDR(*ppte)); //得到物理地址
	u_long* Vva =(u_long*) (KADDR((u_long)Ppa) | (EPC & 0xfff));
	//printk("Vva:%x\n",Vva);
	u_long instr = *(Vva);
	//printk("Instr:%x\n",instr);
	if ((instr & 0xff) == 0x20) //add
	{
		instr = ( instr & 0xffffff20 ) | 0x21;
		//printk("add: %x\n",instr);
		*Vva = instr;
		printk("add ov handled\n");		
	}
	else if ((instr & 0xff) == 0x22) //sub
	{
		instr = ( instr & 0xffffff22 ) | 0x23;
		*Vva = instr;
		printk("sub ov handled\n");
	}
	else if (instr & 0x20000000) //addi
	{
		u_long num_t = (instr & 0x001f0000) >> 16;
		//printk("t:%x\n",num_t);
	       	u_long num_s = (instr & 0x03e00000) >> 21;
		u_long num_i = instr & 0x0000ffff;
		//printk("s:%x\n",num_s);
		//printk("&s:%x\n",tf->regs[num_s]/2);
		//printk("num_i:%x\n",num_i);
		tf->regs[num_t] = (u_long)tf->regs[num_s]/(u_long)2 +(u_long)num_i/(u_long)2;
		tf->cp0_epc += 4;
		printk("addi ov handled\n");	
	}
	curenv->env_ov_cnt++;	
}
