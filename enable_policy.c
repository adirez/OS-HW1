#include "disable_policy.h"
#include "enable_policy.h"

int main(int argc, char const *argv[])
{
	if(disable_policy(1,234123) != 0){
		printf("1yes\n");
	} else {
		printf("1no\n");
	}
	if (enable_policy(1, 5, 23412) != 0)
	{
		printf("2yes\n");
	} else {
		printf("2no\n");
	}
	if(enable_policy(1, 5, 234123) == 0){
		printf("3yes\n");
	} else {
		printf("3no\n");
	}
	if (enable_policy(1, 5, 234123) != 0)
	{
		printf("4yes\n");
	} else {
		printf("4no\n");
	}
	if (disable_policy(1, 23412) != 0)
	{
		printf("5yes\n");
	} else {
		printf("5no\n");
	}
	if(disable_policy(1, 234123) == 0){
		printf("6yes\n");
	} else {
		printf("6no\n");
	}

	return 0;
}

