#include <errno.h>
#include <sys/types.h>

struct forbidden_activity_info{
	int syscall_req_level;
	int proc_level;
	int time;	
};

int get_process_log(pid_t pid, int size, struct forbidden_activity_info* user_mem){
	int res;
	__asm__ (
		"int $0x80;"
		: "=a" (res)
		: "0" (246) , "b" (pid) , "c" (size), "d" (user_mem)
		: "memory"
	);
	if (res < 0){
		errno = -res;
		res = -1;
	}
	return res;
}
