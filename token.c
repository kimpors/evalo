#include "token.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
			if (signcmp(peep(), *s) > 0)
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

// Compare sign priority.
// 0 priority: '*', '/'.
// 1 priority: '+', ''-'
int signcmp(char a, char b)
{
	if (a == b) return 0;

	int a1 = 0;
	int b1 = 0;

	switch (a)
	{
		case '+':
		case '-':
			a1 = 0;
			break;
		case '*':
		case '/':
			a1 = 1;
			break;
	}

	switch (b)
	{
		case '+':
		case '-':
			b1 = 0;
			break;
		case '*':
		case '/':
			b1 = 1;
			break;
	}

	return a1 - b1;
}

static size_t oindex;
static Token out[TOKENS];

static size_t tindex;
static Token temp[TOKENS];

// Show debug information
void show(void)
{
	for (size_t i = 0; i < oindex; i++)
	{
		printf("type: %s\n", out[i].type == SIGN ? "sign" : "number");

		if (out[i].type == SIGN)
		{
			printf("data: %c\n", (char)(long)out[i].data);
		}
		else
		{
			printf("data: %ld\n", (long)out[i].data);
		}
	}
}

// Push number to output buffer
void pushn(long value)
{
	assert(oindex < TOKENS - 1);
	out[oindex].type = NUMBER;
	out[oindex++].data = (void *)(long)value;
}

// Push token to output buffer
void pusht(Token *tok)
{
	assert(oindex < TOKENS - 1);
	out[oindex].type = tok->type;
	out[oindex++].data = tok->data;
}

// Push sign to temp buffer
void pushc(char ch)
{
	assert(tindex < TOKENS - 1);
	temp[tindex].type = SIGN;
	temp[tindex++].data = (void *)(long)ch;
}

// Peep temp buffer
char peep(void)
{
	if (tindex < 1) return '\0';
	return (long)temp[tindex - 1].data;
}

// Pop temp buffer
Token *pop(void)
{
	if (tindex < 1) return NULL;
	return &temp[--tindex];
}

Token *outpop(void)
{
	if (oindex < 1) return NULL;
	return &out[--oindex];
}
