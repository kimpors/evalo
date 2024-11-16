#include "parser.h"
#include <stdio.h>
#include <assert.h>

static size_t oindex;
static Token out[TOKENS];

static size_t tindex;
static Token temp[TOKENS];


void pushn(long value)
{
	assert(oindex < TOKENS - 1);
	out[oindex].type = NUMBER;
	out[oindex++].data = (void *)(long)value;
}

void pusht(Token *tok)
{
	assert(oindex < TOKENS - 1);
	out[oindex].type = tok->type;
	out[oindex++].data = tok->data;
}

void pushc(char ch)
{
	assert(tindex < TOKENS - 1);
	temp[tindex].type = SIGN;
	temp[tindex++].data = (void *)(long)ch;
}

char peep(void)
{
	if (tindex < 1) return '\0';
	return (long)temp[tindex - 1].data;
}

Token *pop(void)
{
	if (tindex < 1) return NULL;
	return &temp[--tindex];
}

int signcmp(char sign)
{
	if (sign == peep()) return 0;

	size_t len = 4;
	int a1 = 0, a2 = 0;
	char table[] = { '*', '+', '/', '-'};

	while (len-- > 0)
	{
		if (table[len] == sign)
		{
			a1 = table[len] % 2;
		}
		else if (table[len] == peep())
		{
			a2 = table[len] % 2;
		}
	}

	return a1 - a2;
}

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
