#include <stdio.h>
#include <string.h>

#include "format.h"
#include "parse.h"
#include "error.h"
#include "token.h"
#include "arg.h"

#ifdef _WIN32
#include <windows.h>
#include <winnt.h>
#endif

#define LINE_MAX 2048
static char sbuf[LINE_MAX];

int main(int argc, char *argv[])
{
	Token *tok = NULL;
	char *expr = NULL;
	FILE *fp = stdin;
	long double res;

	if (!(expr = argeval(argc, argv)) && flags & ARG_ERROR) return -1;

	if (flags & ARG_HELP)
	{
		help();
		return 0;
	}

	if (flags & ARG_TEXT)
	{
		if (tokenize(expr, TOKEN_MAX)) return -1;
		eprintf(res = eval(parse(tok)));
		if (flags & ARG_CLIP) clip(res);
		return 0;
	}

	if (flags & ARG_FILE)
	{
		if (!(fp = fopen(expr, "r")))
		{
			ERROR_MSG("can't open file: '%s'", expr);
			fclose(fp);
			return -1;
		}
	}

	while (fgets(sbuf, LINE_MAX, fp))
	{
		if (!(tok = tokenize(sbuf, TOKEN_MAX))) {
			eprintf(res = eval(parse(tok)));
			if (flags & ARG_CLIP) clip(res);
		}

		memset(sbuf, 0, sizeof(sbuf));
	}

	fclose(fp);
	return 0;
}
