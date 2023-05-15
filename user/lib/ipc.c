// User-level IPC library routines

#include <env.h>
#include <lib.h>
#include <mmu.h>

// Send val to whom.  This function keeps trying until
// it succeeds.  It should panic() on any error other than
// -E_IPC_NOT_RECV.
//
// Hint: use syscall_yield() to be CPU-friendly.
void ipc_send(u_int whom, u_int val, const void *srcva, u_int perm) {
	int r;
	while ((r = syscall_ipc_try_send(whom, val, srcva, perm)) == -E_IPC_NOT_RECV) {
		syscall_yield();
	}
	user_assert(r == 0);
}

// Receive a value.  Return the value and store the caller's envid
// in *whom.
//
// Hint: use env to discover the value and who sent it.
u_int ipc_recv(u_int *whom, void *dstva, u_int *perm) {
	int r = syscall_ipc_recv(dstva);
	if (r != 0) {
		user_panic("syscall_ipc_recv err: %d", r);
	}

	if (whom) {
		*whom = env->env_ipc_from;
	}

	if (perm) {
		*perm = env->env_ipc_perm;
	}

	return env->env_ipc_value;
}


u_int get_time(u_int * us) {
	u_int Time_Address = 0x15000000;
	u_int Trigger = 0;
	u_int Now_Time = 0;
	panic_on(syscall_write_dev( (void *)&Trigger , Time_Address , 4 ));
	panic_on(syscall_read_dev( (void *)&Now_Time , Time_Address + 16 , 4 ));
	panic_on(syscall_read_dev( (void *)us, Time_Address + 0x20, 4));
	return Now_Time;
}


void usleep(u_int us) {
	u_int now_us = 0;
	u_int * now_us_pointer = &now_us;
	u_int enter_time = get_time(now_us_pointer);
	double enter_time_d = (double) enter_time;
	double us_d = (double) us;
	while(1) {
		u_int now_time = get_time(now_us_pointer);
		double now_time_d = (double)now_time;
		double now_us_d = (double)now_us;
		if (now_time_d + now_us_d / 1000000 >= enter_time_d + us_d / 1000000 ) {
			return;
		} else {
			syscall_yield();
		}
	}
}


