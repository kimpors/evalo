#include <stdbool.h>

extern bool ishelp;
extern bool istext;
extern bool isverb;
extern bool isfile;
extern bool isclip;
extern bool isexp;

extern char *arg_text;
extern signed char prec;

void help(void);
void clip(double num);
int arg_evaluate(int argc, char *argv[]);
