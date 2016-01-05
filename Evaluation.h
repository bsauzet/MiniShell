#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "Shell.h"

extern void command_pipe(Expression *e);

extern void redirection_out(Expression *e, int append);

extern void redirection_in(Expression *e);

extern void subshell(Expression *e);

extern int evaluer_expr(Expression *e);

#endif
