#include "arg.h"
#include "token.h"
#include "format.h"
#include "parse.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (arg_evaluate(argc - 1, argv)) return -1;
	tokenize(arg_text, TOKEN);
	char *arg = argv[argc - 1];
	char *eval = format(evaluate(parse()));

	printf("arg: %s\n", arg);
	printf("eval: %s\n", eval);
	printf("cmp: %d\n", strcmp(eval, "-> 2\n"));

	return strcmp(arg, eval);
}
