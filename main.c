#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX 2048

static char sbuf[MAX];

void tokenize(char *s, size_t lim);

int main(void)
{
	while (fgets(sbuf, MAX, stdin))
	{
		tokenize(sbuf, TOKEN);
		show();
	}

	return 0;
}

// Return word from a line.
char *gettoken(char *s, size_t lim)
{
	if (!s || *s == '\0') return NULL;

	while (lim-- > 0 && *s == ' ') s++;
	char *ps = s;
	while (lim-- > 0 && *ps != ' ') ps++;

	*ps = '\0';
	return s;
}

// Tokenize word.
// Produce stack of tokens
void tokenize(char *s, size_t lim)
{
	assert(s || *s);

	char *ps;
	Token *tok;

	while ((ps = gettoken(s, lim)))
	{
		if (*s >= '0' && *s <= '9')
		{
			pushn(atoi(s));
		}
		else
		{
			if (signcmp(*s) < 0)
			{
				while ((tok = pop()))
				{
					pusht(tok);
				}
			}

			pushc(*s);
		}

		s = s + strlen(ps) + 1;
	}

	while ((tok = pop()))
	{
		pusht(tok);
	}
}
