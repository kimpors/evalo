#include "token.h"
#include "parse.h"
#include "format.h"
#include "arg.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <winnt.h>
#endif

#define MAX 2048
static char sbuf[MAX];

int main(int argc, char *argv[])
{
	char *expr;
	FILE *fp = stdin;
	long double res;

	if ((expr = argeval(argc, argv)) == NULL && flags & IS_ERROR) return -1;

	if (flags & IS_HELP)
	{
		help();
		return 0;
	}

	if (flags & IS_TEXT)
	{
		if (tokenize(expr, TOKEN_MAX)) return -1;
		eprintf(res = eval(parse()));
		if (flags & IS_CLIP) clip(res);
		return 0;
	}

	if (flags & IS_FILE)
	{
		if (!(fp = fopen(expr, "r")))
		{
			fprintf(stderr, "can't open file: %s\n", expr);
			exit(-1);
		}
	}

	while (fgets(sbuf, MAX, fp))
	{
		if (!tokenize(sbuf, TOKEN_MAX))
		{
			eprintf(res = eval(parse()));
			if (flags & IS_CLIP) clip(res);
		}

		memset(sbuf, 0, sizeof(sbuf));
	}

	fclose(fp);
	return 0;
}
