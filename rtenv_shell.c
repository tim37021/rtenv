#include "rtenv_shell.h"
#include "clib.h"
#include "kernel.h"

int ps_command(int fd, char *arg){
	const char *status_desc[]={"READY", "WAIT_READ", "WAIT_WRITE", "WAIT_INTR", "WAIT_TIME"};

	int i;
	fprintf(fd, "\rPPID\tPID\tDescription\t\t\tPriority\tStatus\n");
	for(i=0; i<*tctlptr.task_count; ++i){
		fprintf(fd, "\r%s\t%d\t%s\t\t\t%d\t\t%s\n", tctlptr.task_ptr[i].ppid==-2?"N/A":itoa(tctlptr.task_ptr[i].ppid, 10), tctlptr.task_ptr[i].pid, tctlptr.task_ptr[i].description, tctlptr.task_ptr[i].priority, status_desc[tctlptr.task_ptr[i].status]);
	}
	return *tctlptr.task_count;
}


int echo_command(int fd, char *arg){
	fprintf(fd, "\r%s\n", arg);
	/*SET ARG 0 to \0 is important*/
	arg[0]='\0';
	return 0;
}
int help_command(int fd, char *arg){
	int count;
	struct cmd_func_map *cfm=get_cfm(&count);
	for(int i=0; i<count; ++i){
		fprintf(fd, "\r%s - %s\n", cfm[i].cmd, cfm[i].desc);
	}
	return 0;
}

struct cmd_func_map *get_cfm(int *count){
	static struct cmd_func_map cfm[]={
		{.cmd="ps", .cmd_func=ps_command, .desc="List process"}
		,{.cmd="echo", .cmd_func=echo_command, .desc="Print a string"}
		,{.cmd="help", .cmd_func=help_command, .desc="help"}
	};
	*count=sizeof(cfm)/sizeof(cfm[0]);
	return cfm;
}

PTR_CMD_FUNC_PROTO cmd_map(const char *cmd){


	int i, count;
	struct cmd_func_map *cfm=get_cfm(&count);
	for(i=0; i<count; ++i){
		if(strcmp(cfm[i].cmd, cmd)==0)
			return cfm[i].cmd_func;
	}
	return NULL;
}
