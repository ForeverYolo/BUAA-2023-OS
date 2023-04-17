#include <env.h>
#include <pmap.h>
#include <printk.h>

/* Overview:
 *   Implement a round-robin scheduling to select a runnable env and schedule it using 'env_run'.
 *
 * Post-Condition:
 *   If 'yield' is set (non-zero), 'curenv' should not be scheduled again unless it is the only
 *   runnable env.
 *
 * Hints:
 *   1. The variable 'count' used for counting slices should be defined as 'static'.
 *   2. Use variable 'env_sched_list', which contains and only contains all runnable envs.
 *   3. You shouldn't use any 'return' statement because this function is 'noreturn'.
 */
void schedule(int yield) {
	static int count = 0; // remaining time slices of current env
	struct Env *e = curenv;
	static int user_time[5];
	int avaiable_user[5];
	for (int i = 0 ;i < 5;i++)
	{
		avaiable_user[i] = 0;
	}
	int avaiable_count = 0;
	struct Env * nowEnv;
	TAILQ_FOREACH(nowEnv,&env_sched_list,env_sched_link)
	{
		avaiable_user[nowEnv->env_user]++;
	}
	

	/* We always decrease the 'count' by 1.
	 *
	 * If 'yield' is set, or 'count' has been decreased to 0, or 'e' (previous 'curenv') is
	 * 'NULL', or 'e' is not runnable, then we pick up a new env from 'env_sched_list' (list of
	 * all runnable envs), set 'count' to its priority, and schedule it with 'env_run'. **Panic
	 * if that list is empty**.
	 *
	 * (Note that if 'e' is still a runnable env, we should move it to the tail of
	 * 'env_sched_list' before picking up another env from its head, or we will schedule the
	 * head env repeatedly.)
	 *
	 * Otherwise, we simply schedule 'e' again.
	 *
	 * You may want to use macros below:
	 *   'TAILQ_FIRST', 'TAILQ_REMOVE', 'TAILQ_INSERT_TAIL'
	 */
	/* Exercise 3.12: Your code here. */
	//printk("2\n");
	//if ( count <= 0 || e == NULL || e->env_status != ENV_RUNNABLE || yield )
	//{
	//	if ( e != NULL )
	//	{
	//		TAILQ_REMOVE(&env_sched_list,e,env_sched_link);
	//		if (e->env_status == ENV_RUNNABLE)
	//	       	{
	//			TAILQ_INSERT_TAIL(&env_sched_list,e,env_sched_link);
	//		}
	//	}
	//	if ( !TAILQ_EMPTY(&env_sched_list) )
	//	{
	//		//printk("1");
	//		e = TAILQ_FIRST(&env_sched_list);
	//		count = e->env_pri;
	//	} 
	//	else
	//	{
	//		panic("schedule: no runnable envs");
	//	}		
	//}
	//count--;
	//env_run(e);
	if ( count <= 0 || e == NULL || e->env_status != ENV_RUNNABLE || yield )
	{
		if ( e != NULL && e->env_status == ENV_RUNNABLE )
		{
			TAILQ_REMOVE(&env_sched_list,e,env_sched_link);
			TAILQ_INSERT_TAIL(&env_sched_list,e,env_sched_link);
			user_time[e->env_user] += e->env_pri;
		}
		if ( !TAILQ_EMPTY(&env_sched_list) )
		{
			//printk("1");
			int targetUser = 100;
			int targetTimeUsed = -1;
			for (int i = 0 ; i < 5 ; i++ )
			{
				if ( avaiable_user[i] !=0 )
				{
					if ( user_time[i] < targetTimeUsed || targetTimeUsed == -1 )
					{
						targetUser = i;
						targetTimeUsed = user_time[i];
					}
					else if (user_time[i] == targetTimeUsed)
					{
						if ( i < targetUser)
						{
							targetUser = i;
						}
					}
				}
			}
			TAILQ_FOREACH(nowEnv,&env_sched_list,env_sched_link)
			{
				if (nowEnv -> env_user == targetUser )
				{
					e = nowEnv;
					break;
				}
			}
			count = e->env_pri;
		} 
		else
		{
			panic("schedule: no runnable envs");
		}		
	}
	count--;
	env_run(e);
	/*if (yield || count <= 0 || e == NULL || e->env_status != ENV_RUNNABLE) {
		if ( e != NULL && e->env_status == ENV_RUNNABLE )
		{
			TAILQ_REMOVE(&env_sched_list, e, env_sched_link);
			TAILQ_INSERT_TAIL(&env_sched_list, e, env_sched_link);
		}
		e = TAILQ_FIRST(&env_sched_list);
		if (e == NULL) {
			panic("Error!");
		}
		count = e->env_pri;
		env_run(e);
	} else {
		count--;
	}*/
	/*if(yield != 0 || count == 0 || e == NULL || e->env_status != ENV_RUNNABLE) {
		if (TAILQ_EMPTY(&env_sched_list)) {
			panic("error in schedule");
		}
		if (e != NULL) {
			if (e->env_status == ENV_RUNNABLE) {
				TAILQ_INSERT_TAIL(&env_sched_list, e, env_sched_link);
			}
		}
		e = TAILQ_FIRST(&env_sched_list);
		if (e == NULL) {
			panic("error in schedule");
		}
		TAILQ_REMOVE(&env_sched_list, e, env_sched_link);
		count = e->env_pri;
	}
	count--;
	env_run(e);*/
}
