#include <linux/kernel.h>
#include <sched.h>
#include <errno.h>

int disable_policy(pid_t pid, int password){
	if(pid < 0) return ESRCH;
	task_t* task = find_task_by_pid(pid);
	if(task == NULL) return ESRCH;
	if(password != 234123) return EINVAL;
	if(task->enabled == 0) return EINVAL;
	if(size < 0) return EINVAL;
	free(task->log);
	task->enabled = 0;
	task->privilege = 0;
	return 0;
}
