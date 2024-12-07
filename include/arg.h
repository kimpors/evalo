#include <stdbool.h>

extern bool ishelp;
extern bool istext;
extern bool isverb;
extern bool isfile;
extern bool isclip;
extern bool isexp;

extern char *arg_text;
extern unsigned char prec;

int arg_evaluate(int argc, char *argv[]);
