#include "rtenv_shell.h"
#include "clib.h"
#include "kernel.h"

void ps_command(int fd){
	const char *status_desc[]={"READY", "WAIT_READ", "WAIT_WRITE", "WAIT_INTR", "WAIT_TIME"};

	int i;
	fprintf(fd, "\rPPID\tPID\tDescription\t\t\tPriority\tStatus\n");
	for(i=0; i<*tctlptr.task_count; ++i){
		fprintf(fd, "\r%s\t%d\t%s\t\t\t%d\t\t%s\n", tctlptr.task_ptr[i].ppid==-2?"N/A":itoa(tctlptr.task_ptr[i].ppid, 10), tctlptr.task_ptr[i].pid, tctlptr.task_ptr[i].description, tctlptr.task_ptr[i].priority, status_desc[tctlptr.task_ptr[i].status]);
	}
}
