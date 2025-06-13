#include "arg.h"
#include "token.h"
#include "format.h"
#include "parse.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *value;
	if ((value = argeval(argc - 1, argv)) == NULL) return -1;
	tokenize(value, TOKEN);
	char *arg = argv[argc - 1];
	char *res = format(eval(parse()));

	printf("arg: %s\n", arg);
	printf("eval: %s\n", res);
	printf("cmp: %d\n", strcmp(res, "-> 2\n"));

	return strcmp(arg, res);
}
