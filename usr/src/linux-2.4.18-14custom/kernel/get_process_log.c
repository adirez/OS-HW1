#include <linux/kernel.h>
#include <linux/slab.h>
#include <../include/linux/sched.h>

int sys_get_process_log(pid_t pid, int size, struct forbidden_activity_info* user_mem){
	int i;
	if(pid < 0) return -ESRCH;
	task_t* task = find_task_by_pid(pid);
	if(task == NULL) return -ESRCH;
	if(size > task->count) return -EINVAL;
	if(task->enabled == 0) return -EINVAL;
	if(size < 0) return -EINVAL;
	for (i = 0; i < size; ++i)
	{
		user_mem[i] = task->log[i];
		task->log[i] = task->log[size + i];
	}
	task->count = task->count - size;
	return 0;
}
