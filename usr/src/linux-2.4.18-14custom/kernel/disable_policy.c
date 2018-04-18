#include <linux/kernel.h>
#include <linux/slab.h>
#include <../include/linux/sched.h>

int sys_disable_policy(pid_t pid, int password){
	if(pid < 0) return -ESRCH;
	task_t* task = find_task_by_pid(pid);
	if(task == NULL) return -ESRCH;
	if(password != 234123) return -EINVAL;
	if(task->enabled == 0) return -EINVAL;
	kfree(task->log);
	task->enabled = 0;
	task->privilege = 0;
	return 0;
}
