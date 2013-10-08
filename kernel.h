#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>

#define STACK_SIZE 512 /* Size of task stacks in words */
#define HEAP_SIZE 512 /* Size of task heaps in BYTE */
#define TASK_LIMIT 8  /* Max number of tasks we can handle */
#define PIPE_BUF   64 /* Size of largest atomic pipe message */
#define PATH_MAX   32 /* Longest absolute path */
#define PIPE_LIMIT (TASK_LIMIT * 2)

#define PATHSERVER_FD (TASK_LIMIT + 3) 
	/* File descriptor of pipe to pathserver */

#define PRIORITY_DEFAULT 20
#define PRIORITY_LIMIT (PRIORITY_DEFAULT * 2 - 1)

enum TASK_STATUS{TASK_READY, TASK_WAIT_READ, TASK_WAIT_WRITE, TASK_WAIT_INTR, TASK_WAIT_TIME};

#define MAX_DESC 24

#define S_IFIFO 1
#define S_IMSGQ 2

#define O_CREAT 4

/* Stack struct of user thread, see "Exception entry and return" */
struct user_thread_stack {
	unsigned int r4;
	unsigned int r5;
	unsigned int r6;
	unsigned int r7;
	unsigned int r8;
	unsigned int r9;
	unsigned int r10;
	unsigned int fp;
	unsigned int _lr;	/* Back to system calls or return exception */
	unsigned int _r7;	/* Backup from isr */
	unsigned int r0;
	unsigned int r1;
	unsigned int r2;
	unsigned int r3;
	unsigned int ip;
	unsigned int lr;	/* Back to user thread code */
	unsigned int pc;
	unsigned int xpsr;
	unsigned int stack[STACK_SIZE - 18];
};

/* Task Control Block */
struct task_control_block {
	struct user_thread_stack *stack;
	/*Parent pid
	if no parent ppid is -2
	fork returns 0 to child process and child's pid to parent process, -1 if it fails.
	*/
	int ppid;
	int pid;
	int status;
	int priority;
	char description[MAX_DESC];

	struct task_control_block **prev;
	struct task_control_block  *next;
};

struct task_ctl_ptr{
	size_t *task_count;
	struct task_control_block *task_ptr;
};


#endif

