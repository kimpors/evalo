#pragma once
#define IS_HELP		(1 << 0)
#define IS_TEXT		(1 << 1)
#define IS_FILE		(1 << 2)
#define IS_VERB		(1 << 3)
#define IS_CLIP		(1 << 4)
#define IS_EXP		(1 << 5)
#define IS_ERROR 	(1 << 6)

extern unsigned flags;
extern signed char prec;

void help(void);
void clip(double num);
char *argeval(int argc, char *argv[]);
