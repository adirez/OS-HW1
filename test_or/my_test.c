#include "hw1_syscalls.h"
#include "test_utilities.h"
#include <limits.h>
#include <stdlib.h>

int ret_status(int status){
	return (status>>8) & 0xff;
}

bool enable_test(){
	int this_pid = getpid();

	int res= enable_policy(-1,4,234123);
	ASSERT_TEST(res==-1 && errno==ESRCH);
	
	res= enable_policy(999999,4,234123);
	ASSERT_TEST(res==-1 && errno==ESRCH);

	res= enable_policy(this_pid,4,999999);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res= enable_policy(this_pid,-4,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res= enable_policy(this_pid,10,234123);
	ASSERT_TEST(res==0);

	res= enable_policy(this_pid,10,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res= disable_policy(this_pid,234123);
	ASSERT_TEST(res==0);

	int parent=getppid();
	ASSERT_TEST(parent>=0 && parent!=this_pid);

	res= enable_policy(parent,10,234123);
	ASSERT_TEST(res==0);

	res= enable_policy(parent,10,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);
	
	res= disable_policy(parent,234123);
	ASSERT_TEST(res==0);

	// ZERO
	res= enable_policy(this_pid,0,234123);
	ASSERT_TEST(res==0);
	
	res= disable_policy(this_pid,234123);
	ASSERT_TEST(res==0);

	return true;
}

bool disable_test(){
	int this_pid = getpid();

	int res=enable_policy(this_pid,10,234123);
	ASSERT_TEST(res==0);

	res=enable_policy(this_pid,10,234123);
	ASSERT_TEST(res==-1);

	res= disable_policy(-1,234123);
	ASSERT_TEST(res==-1 && errno==ESRCH);

	res= disable_policy(9999,234123);
	ASSERT_TEST(res==-1 && errno==ESRCH);

	res= disable_policy(this_pid,99999);
	ASSERT_TEST(res==-1 && EINVAL);

	res= disable_policy(this_pid,234123);
	ASSERT_TEST(res==0);

	res= disable_policy(this_pid,234123);
	ASSERT_TEST(res==-1 && EINVAL);


	int parent=getppid();
	ASSERT_TEST(parent>=0 && parent!=this_pid);

	res=enable_policy(parent,10,234123);
	ASSERT_TEST(res==0);

	res=enable_policy(parent,10,234123);
	ASSERT_TEST(res==-1);

	res= disable_policy(-1,234123);
	ASSERT_TEST(res==-1 && errno==ESRCH);

	res= disable_policy(9999,234123);
	ASSERT_TEST(res==-1 && errno==ESRCH);

	res= disable_policy(parent,99999);
	ASSERT_TEST(res==-1 && EINVAL);

	res= disable_policy(parent,234123);
	ASSERT_TEST(res==0);

	res= disable_policy(parent,234123);
	ASSERT_TEST(res==-1 && EINVAL);

	return true;
}

bool set_capa_test(){
	int this_pid = getpid();

	int res=enable_policy(this_pid,10,234123);
	ASSERT_TEST(res==0);

	res=set_process_capabilities(-1,1,234123);
	ASSERT_TEST(res==-1 && errno==ESRCH);

	res=set_process_capabilities(9999,1,234123);
	ASSERT_TEST(res==-1 && errno==ESRCH);

	res=set_process_capabilities(this_pid,-1,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res=set_process_capabilities(this_pid,3,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res=set_process_capabilities(this_pid,555,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res=set_process_capabilities(this_pid,1,9999);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res=set_process_capabilities(this_pid,1,234123);
	ASSERT_TEST(res==0);

	res=disable_policy(this_pid,234123);
	ASSERT_TEST(res==0);

	res=set_process_capabilities(this_pid,1,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	int parent=getppid();
	ASSERT_TEST(parent>=0 && parent!=this_pid);

	res=enable_policy(parent,10,234123);
	ASSERT_TEST(res==0);

	res=set_process_capabilities(parent,-1,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res=set_process_capabilities(parent,3,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res=set_process_capabilities(parent,555,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res=set_process_capabilities(parent,1,9999);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	res=set_process_capabilities(parent,0,234123);
	ASSERT_TEST(res==0);

	res=disable_policy(parent,234123);
	ASSERT_TEST(res==0);

	res=set_process_capabilities(parent,1,234123);
	ASSERT_TEST(res==-1 && errno==EINVAL);

	return true;
}

bool not_priv_fork_test(){
	int this_pid = getpid();
	
	int child = fork();
	ASSERT_TEST(child!=-1);
	if (child==0){
		exit(0);
	}
	ASSERT_TEST(child>0);

	int res=enable_policy(this_pid,10,234123);
	ASSERT_TEST(res==0);

	child = fork();
	ASSERT_TEST(child!=-1);
	if (child==0){
		exit(0);
	}
	ASSERT_TEST(child>0);

	res=set_process_capabilities(this_pid,1,234123);
	ASSERT_TEST(res==0);

	child = fork();
	ASSERT_TEST(child==-1);	
	if (child==0){
		exit(0);
	}

	res=set_process_capabilities(this_pid,0,234123);
	ASSERT_TEST(res==0);

	child = fork();
	ASSERT_TEST(child==-1);	
	if (child==0){
		exit(0);
	}

	res=set_process_capabilities(this_pid,2,234123);
	ASSERT_TEST(res==0);

	child = fork();
	ASSERT_TEST(child!=-1);	
	if (child==0){
		exit(0);
	}
	ASSERT_TEST(child>0);

	res=set_process_capabilities(this_pid,0,234123);
	ASSERT_TEST(res==0);

	child = fork();
	ASSERT_TEST(child==-1);	
	if (child==0){
		exit(0);
	}

	res=disable_policy(this_pid,234123);
	ASSERT_TEST(res==0);

	child = fork();
	ASSERT_TEST(child!=-1);	
	if (child==0){
		exit(0);
	}
	ASSERT_TEST(child>0);

	return true;
}

bool not_priv_yield_test(){
	int this_pid = getpid();
	while(wait(NULL)!=-1){};
	ASSERT_TEST(disable_policy(this_pid,234123) == -1 && errno == EINVAL);
	ASSERT_TEST(wait(NULL) == -1 && errno == ECHILD);
	ASSERT_TEST(enable_policy(this_pid,10,234123) == 0);
	ASSERT_TEST(set_process_capabilities(this_pid,0,234123) == 0);
	ASSERT_TEST(sched_yield()==-1);

	ASSERT_TEST(set_process_capabilities(this_pid,2,234123) == 0);
	int child=fork();
	ASSERT_TEST(child!=-1);
	if(child==0){
		ASSERT_TEST(enable_policy(getpid(),10,234123)==0);
		ASSERT_TEST(set_process_capabilities(getpid(),1,234123) == 0);
		ASSERT_TEST(sched_yield()==0);
		exit(0);
	}
	ASSERT_TEST(child>0);
	wait(NULL);

	ASSERT_TEST(set_process_capabilities(this_pid,2,234123) == 0);
	child=fork();
	ASSERT_TEST(child!=-1);
	if(child==0){
		ASSERT_TEST(enable_policy(getpid(),10,234123)==0);
		ASSERT_TEST(set_process_capabilities(getpid(),0,234123) == 0);
		ASSERT_TEST(sched_yield()==-1);
		exit(0);
	}
	ASSERT_TEST(child>0);
	wait(NULL);

	ASSERT_TEST(set_process_capabilities(this_pid,2,234123) == 0);
	child=fork();
	ASSERT_TEST(child!=-1);
	if(child==0){
		ASSERT_TEST(enable_policy(getpid(),10,234123)==0);
		ASSERT_TEST(set_process_capabilities(getpid(),2,234123) == 0);
		ASSERT_TEST(sched_yield()==0);
		exit(0);
	}
	ASSERT_TEST(child>0);
	wait(NULL);

	return true;
}

bool not_priv_wait_test(){
	int this_pid = getpid();
	while(wait(NULL)!=-1){};
	ASSERT_TEST(wait(NULL) == -1 && errno == ECHILD);	
	ASSERT_TEST(set_process_capabilities(this_pid,2,234123)==0);
	int child=fork();
	ASSERT_TEST(child!=-1);
	if(child==0){
		exit(111);
	}
	ASSERT_TEST(child>0);
	int res=0;
	wait(&res);
	res=ret_status(res);
	ASSERT_TEST(res==111);

	child=fork();
	ASSERT_TEST(set_process_capabilities(this_pid,0,234123)==0);
	ASSERT_TEST(child!=-1);
	if(child==0){
		exit(222);
	}
	ASSERT_TEST(child>0);
	res=0;
	ASSERT_TEST(wait(&res)==-1);
	ASSERT_TEST(set_process_capabilities(this_pid,1,234123)==0);
	ASSERT_TEST(wait(&res)==child);
	res=ret_status(res);
	ASSERT_TEST(res==222);
	ASSERT_TEST(set_process_capabilities(this_pid,2,234123)==0);
	this_pid = getpid();
	child=fork();
	errno=0;
	if (child!=0){
		ASSERT_TEST(set_process_capabilities(this_pid,0,234123)==0);
	}
	ASSERT_TEST(child!=-1);
	if(child==0){
		exit(255);
	}
	ASSERT_TEST(child>0);
	res=0;
	ASSERT_TEST(wait(&res)==-1);
	ASSERT_TEST(disable_policy(this_pid,234123)==0);
	ASSERT_TEST(wait(&res)==child);
	res=ret_status(res);
	ASSERT_TEST(res==255);

	return true;
}

bool get_test(){
	int this_pid = getpid();
	struct forbidden_activity_info arr[10];
	arr[0].syscall_req_level=-1;
	arr[0].proc_level=-1;
	arr[0].time=-1;
	ASSERT_TEST(enable_policy(this_pid,10,234123)==0);

	ASSERT_TEST(get_process_log(-1,0,arr)==-1 && errno==ESRCH);
	ASSERT_TEST(get_process_log(9999,0,arr)==-1 && errno==ESRCH);
	ASSERT_TEST(get_process_log(this_pid,1,arr)==-1 && errno==EINVAL);
	ASSERT_TEST(get_process_log(this_pid,-1,arr)==-1 && errno==EINVAL);
	disable_policy(getppid(),234123);
	ASSERT_TEST(get_process_log(getppid(),0,arr)==-1 && errno==EINVAL);
	ASSERT_TEST(get_process_log(this_pid,0,arr)==0);
	ASSERT_TEST(arr[0].syscall_req_level==-1);
	ASSERT_TEST(arr[0].proc_level==-1);
	ASSERT_TEST(arr[0].time==-1);

	ASSERT_TEST(set_process_capabilities(this_pid,0,234123)==0);
	int i=0;
	for (i=0; i<5;i++){
		ASSERT_TEST(set_process_capabilities(this_pid,1,234123)==0);
		ASSERT_TEST(fork()==-1);
		ASSERT_TEST(set_process_capabilities(this_pid,0,234123)==0);
		ASSERT_TEST(wait(NULL)==-1);
		if (i!=4)
			ASSERT_TEST(get_process_log(this_pid,2*(i+1)+1,arr)==-1 && errno==EINVAL);
	}
	ASSERT_TEST(get_process_log(this_pid,0,arr)==0);
	ASSERT_TEST(get_process_log(this_pid,11,arr)==-1 && errno==EINVAL);

	ASSERT_TEST(get_process_log(this_pid,10,arr)==0);
	for (i=0; i<10;i++){
		if (i%2==0){
			ASSERT_TEST(arr[i].syscall_req_level==2);
			ASSERT_TEST(arr[i].proc_level==1);
		}else{
			ASSERT_TEST(arr[i].syscall_req_level==1);
			ASSERT_TEST(arr[i].proc_level==0);
		}
	}

	for (i=0; i<5;i++){
		ASSERT_TEST(set_process_capabilities(this_pid,1,234123)==0);
		ASSERT_TEST(fork()==-1);
		ASSERT_TEST(set_process_capabilities(this_pid,0,234123)==0);
		ASSERT_TEST(wait(NULL)==-1);
	}
	ASSERT_TEST(get_process_log(this_pid,10,arr)==0);
	for (i=0; i<5;i++){
		if (i%2==0){
			ASSERT_TEST(arr[i].syscall_req_level==2);
			ASSERT_TEST(arr[i].proc_level==1);
		}else{
			ASSERT_TEST(arr[i].syscall_req_level==1);
			ASSERT_TEST(arr[i].proc_level==0);
		}
	}
	/*ASSERT_TEST(get_process_log(this_pid,5,arr)==0);
	for (i=0; i<5;i++){
		if (i%2==1){
			ASSERT_TEST(arr[i].syscall_req_level==2);
			ASSERT_TEST(arr[i].proc_level==1);
		}else{
			ASSERT_TEST(arr[i].syscall_req_level==1);
			ASSERT_TEST(arr[i].proc_level==0);
		}
	}*/

	return true;
}

int main(){
	if (enable_test()==true)
		printf("enable_test passed\n");
	else
		printf("enable_test failed\n");

	if (disable_test()==true)
		printf("disable_test passed\n");
	else
		printf("disable_test failed\n");

	if (not_priv_fork_test()==true)
		printf("not_priv_fork_test passed\n");
	else
		printf("not_priv_fork_test failed\n");

	if (not_priv_yield_test()==true)
		printf("not_priv_yield_test passed\n");
	else
		printf("not_priv_yield_test failed\n");

	if (not_priv_wait_test()==true)
		printf("not_priv_wait_test passed\n");
	else
		printf("not_priv_wait_test failed\n");

	if (get_test()==true)
		printf("get_test passed\n");
	else
		printf("get_test failed\n");

	return 0;
}
