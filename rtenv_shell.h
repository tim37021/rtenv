#ifndef RTENV_SHELL_H
#define RTENV_SHELL_H

#include "kernel.h"

extern struct task_ctl_ptr tctlptr;

/*cmd arg in ps_command is garbage(because echo need it). still figuring it out*/
int ps_command(int fd, char *cmd);
int echo_command(int fd, char *cmd);
int help_command(int fd, char *cmd);

enum KeyName{BACKSPACE=127};

struct cmd_func_map {
	char *cmd;
	int (*cmd_func)(int, char *);
};

/*Not an elegant declaration, will fix it later*/
int (*cmd_map(const char *cmd))(int, char *);

#endif
