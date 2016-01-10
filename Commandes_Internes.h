#ifndef COMINTERN_H
#define COMINTERN_H
#include "Shell.h"

void echo(Expression* e);

void date();

void pwd();

void cd(Expression *e);

void hostname();

void cmd_kill(Expression *e);

void cmd_exit();

void history();

void remote(Expression *e);

void remote_list();

void remote_add(Expression* e);

#endif
