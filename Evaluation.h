#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "Shell.h"

extern void command_pipe(Expression *e);

void redirection_out(Expression *e, int append);

extern int evaluer_expr(Expression *e);

#endif
