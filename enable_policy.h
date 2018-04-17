#include <errno.h>
#include <sys/types.h>

int enable_policy(pid_t pid, int size, int password){
	int res;
	__asm__ (
		"int $0x80;"
		: "=a" (res)
		: "0" (243) , "b" (pid) , "c" (size), "d" (password)
		: "memory"
	);
	if (res < 0){
		errno = -res ;
		res = -1 ;
	}
	return res;
}
