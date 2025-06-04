#include "token.h"
#include "parse.h"
#include "format.h"
#include "arg.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#include <winnt.h>
#endif

#define MAX 2048
static char sbuf[MAX];

int main(int argc, char *argv[])
{
	if (arg_evaluate(argc, argv)) return -1;

	if (ishelp)
	{
		help();
		return 0;
	}

	long double res;
	FILE *fp = stdin;

	if (istext)
	{
		if (tokenize(arg_text, TOKEN)) return -2;
		printf("%s", format(res = evaluate(parse())));
		if (isclip) clip(res);
		return 0;
	}

	if (isfile)
	{
		if (!(fp = fopen(arg_text, "r")))
		{
			fprintf(stderr, "can't open file: %s\n", arg_text);
			exit(-1);
		}
	}

	while (fgets(sbuf, MAX, fp))
	{
		if (!tokenize(sbuf, TOKEN))
		{
			printf("%s\n", format(res = evaluate(parse())));
			if (isclip) clip(res);
		}
		memset(sbuf, 0, sizeof(sbuf));
	}

	fclose(fp);
	return 0;
}
