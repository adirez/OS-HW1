#include <errno.h>
#include <sys/types.h>

int disable_policy(pid_t pid, int password){
	int res;
	__asm__ (
		"int $0x80;"
		: "=a" (res)
		: "0" (244) , "b" (pid) , "c" (password)
		: "memory"
	);
	if (res < 0){
		errno = -res;
		res = -1;
	}
	return res;
}
