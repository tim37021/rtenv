#ifndef RTENV_SHELL_H
#define RTENV_SHELL_H

#include "kernel.h"

extern struct task_ctl_ptr tctlptr;

/*cmd arg in ps_command is garbage(because echo need it). still figuring it out*/
int ps_command(int fd, char *cmd);
int echo_command(int fd, char *cmd);
int help_command(int fd, char *cmd);

enum KeyName{BACKSPACE=127};

typedef int (*PTR_CMD_FUNC_PROTO)(int, char *);

struct cmd_func_map {
	const char *cmd;
	PTR_CMD_FUNC_PROTO cmd_func;
	const char *desc;
};

struct cmd_func_map *get_cfm(int *count);

/*Not an elegant declaration, will fix it later*/
PTR_CMD_FUNC_PROTO cmd_map(const char *cmd);

#endif
