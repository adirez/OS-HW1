#include <errno.h>
#include <sys/types.h>

int set_process_capabilities(pid_t pid, int new_level ,int password){
	int res;
	__asm__ (
		"int $0x80;"
		: "=a" (res)
		: "0" (245) , "b" (pid) , "c" (new_level), "d" (password)
		: "memory"
	);
	if (res < 0){
		errno = -res;
		res = -1;
	}
	return res;
}
