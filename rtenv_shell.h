#ifndef RTENV_SHELL_H
#define RTENV_SHELL_H

#include "kernel.h"

extern struct task_ctl_ptr tctlptr;

void ps_command(int);

enum KeyName{BACKSPACE=127};

#endif
