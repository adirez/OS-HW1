#include "disable_policy.h"
#include "enable_policy.h"
#include "set_process_capabilities.h"

int main(int argc, char const *argv[])
{
	pid_t curpid = getpid();
	printf("curpid = %d\n", curpid);
	enable_policy(curpid, 5, 234123);
	set_process_capabilities(curpid, 0, 234123);

	if (fork() < 0) {
		printf("1howdy\n");
	} else {
		printf("1gowdy\n");
	}

	set_process_capabilities(curpid, 2, 234123);
	if (fork() == 0) {
		pid_t sonpid = getpid();
		if (disable_policy(sonpid, 234123) != 0) {
				printf("2howdy\n");
			} else {
				printf("2gowdy\n");
			}
	} else {
		if (enable_policy(curpid, 5, 234123) != 0) {
			printf("3howdy\n");
		} else {
			printf("3gowdy\n");
		}
	}

	wait();
	return 0;
}

