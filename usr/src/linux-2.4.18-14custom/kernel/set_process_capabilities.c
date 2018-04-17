#include <linux/kernel.h>
#include <../include/linux/sched.h>

int sys_set_process_capabilities(pid_t pid, int new_level, int password){
	if(pid < 0) return ESRCH;
	task_t* task = find_task_by_pid(pid);
	if(task == NULL) return ESRCH;
	if(password != 234123) return EINVAL;
	if(new_level != 0 && new_level != 1 && new_level != 2) return EINVAL;
	if(task->enabled != 1) return EINVAL;
	task->privilege = new_level;
	return 0;
}
