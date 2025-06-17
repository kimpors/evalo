#pragma once
#define ARG_HELP	(1 << 0)
#define ARG_TEXT	(1 << 1)
#define ARG_FILE	(1 << 2)
#define ARG_VERB	(1 << 3)
#define ARG_CLIP	(1 << 4)
#define ARG_EXP		(1 << 5)
#define ARG_ERROR 	(1 << 6)

extern unsigned flags;
extern signed char prec;

void help(void);
void clip(double num);
char *argeval(int argc, char *argv[]);
