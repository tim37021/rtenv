#ifndef RTENV_SHELL_H
#define RTENV_SHELL_H

#include "kernel.h"
#include "syscall.h"

#define MAX_CMD_LENGTH 32
#define MAX_CMD_ALIAS 10

extern struct task_ctl_ptr tctlptr;

/*cmd arg in ps_command is garbage(because echo need it). still figuring it out*/
int ps_command(int fd, char *cmd);
int echo_command(int fd, char *cmd);
int help_command(int fd, char *cmd);
int alias_command(int fd, char *cmd);
int malloctest_command(int fd, char *cmd);

int parse_cmd(char *buf);

enum KeyName{ESC=27, BACKSPACE=127};

typedef int (*PTR_CMD_FUNC_PROTO)(int, char *);

struct cmd_func_map {
	const char *cmd;
	PTR_CMD_FUNC_PROTO cmd_func;
	const char *desc;
};

struct alias_cmd{
	char cmd[MAX_CMD_LENGTH];
	char map_cmd[MAX_CMD_LENGTH];
};


struct cmd_func_map *get_cfm(int *count);
struct alias_cmd *get_cm();

/*Not an elegant declaration, will fix it later*/
PTR_CMD_FUNC_PROTO cmd_map(const char *cmd);
const char *alias_map(const char *);

#endif
