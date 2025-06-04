#include "arg.h"
#include <ctype.h>
#include <stdio.h>

#define LEN 2048
static char sbuf[LEN];

char *trim(char *s);

char *format(double val)
{
	if (isverb) sprintf(sbuf, isexp ? "%s -> %.*e\n" : "%s -> %.*f\n", arg_text, prec, val);
	else sprintf(sbuf, isexp ? "-> %.*e\n" : "-> %.*f\n", prec, val);
	return sbuf;
}

char *trim(char *s)
{
	while (isblank(*s)) s++;

	char *ps = s;
	while (*ps != '\0') ps++;
	ps--;
	while (isblank(*ps)) ps--;
	*ps = '\0';

	return s;
}
