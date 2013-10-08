#include "rtenv_shell.h"
#include "clib.h"
#include "kernel.h"

int parse_cmd(char *buf){
        int i;
        for(i=0;buf[i]!='\0'&&buf[i]!=' '; ++i);
        buf[i]='\0';
        return i;
}


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

int alias_command(int fd, char *arg){
	static int cmd_map_count=0;

	int len=parse_cmd(arg);
	if(cmd_map_count>=MAX_CMD_ALIAS)
		fprintf(fd,"\rReach the limit of alias.\n");
	else{
		struct alias_cmd *cm=get_cm();
		memcpy(cm[cmd_map_count].cmd, arg, len+1);
		/*This line does not work correctly if ENABLE_ASM_OPT*/
		/*Instead i use my version of memcpy.*/
		memcpy(cm[cmd_map_count].map_cmd, arg+len+1, strlen(arg+len+1)+1);
		++cmd_map_count;
	}

	arg[len+1]='\0';
	return 0;
}

struct alias_cmd *get_cm(){
	static struct alias_cmd cm[MAX_CMD_ALIAS]={0};
	return cm;
}

struct cmd_func_map *get_cfm(int *count){
	static struct cmd_func_map cfm[]={
		{.cmd="ps", .cmd_func=ps_command, .desc="List process"}
		,{.cmd="echo", .cmd_func=echo_command, .desc="Print a string"}
		,{.cmd="alias", .cmd_func=alias_command, .desc="Alias a command"}
		,{.cmd="malloctest", .cmd_func=malloctest_command, .desc="Test malloc function"}
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


const char *alias_map(const char *cmd){
	int i;
	struct alias_cmd *cm=get_cm();
	for(i=0; i<MAX_CMD_ALIAS; ++i){
		if(strcmp(cm[i].cmd, cmd)==0)
			return cm[i].map_cmd;
	}
	return NULL;
}

#define alloc_and_check(fd, n, p) if(((p)=malloc(n))==NULL) \
	fprintf(fd, "\rmalloc fails to allocate %d bytes for me\n", n);

int malloctest_command(int fd, char *cmd){
	fprintf(fd, "\rAllocate 100 bytes...\n");
	unsigned char *p, *q;
	alloc_and_check(fd, 100, p);
	
	int i;
	fprintf(fd, "\r");
	for(i=0;i<100;++i){
		fprintf(fd, "%x ", p[i]);
	}
	fprintf(fd, "\n");
	
	alloc_and_check(fd, 100, q);
	
	fprintf(fd, "\rAfter freeing p...\n");
	free(p);

	alloc_and_check(fd, 100, q);
	fprintf(fd, "\rSuccessful!\n");
	free(q);
}
