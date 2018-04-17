#include <linux/kernel.h>
#include <sched.h>
#include <errno.h>

int enable_policy(pid_t pid, int size, int password){
	if(pid < 0) return ESRCH;
	task_t* task = find_task_by_pid(pid);
	if(task == NULL) return ESRCH;
	if(password != 234123) return EINVAL;
	if(task->enabled != 0) return EINVAL;
	if(size < 0) return EINVAL;
	task->log = malloc(size*sizeof(forbidden_activity_info));
	if(task->log == NULL) return ENOMEM;
	task->enabled = 1;
	task->privilege = 2;
	return 0;
}
