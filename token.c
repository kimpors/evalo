#include "token.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int signcmp(char a, char b);

void pushn(long value);
void pusht(Token *tok);

void push_temp(char ch);
char peep_temp(void);
Token *pop_temp(void);

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
			if (signcmp(peep_temp(), *s) > 0)
			{
				while ((tok = pop_temp()))
				{
					pusht(tok);
				}
			}

			push_temp(*s);
		}

		s = s + strlen(ps) + 1;
	}

	while ((tok = pop_temp()))
	{
		pusht(tok);
	}
}

int sign_priority(char a)
{
	switch (a)
	{
		case '+':
		case '-':
			return 0;

		case '*':
		case '/':
			return 1;
	}

	return 0;
}

// Compare sign priority.
inline int signcmp(char a, char b)
{
	return sign_priority(a) - sign_priority(b);
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

// Pop output buffer
Token *pop(void)
{
	if (oindex < 1) return NULL;
	return &out[--oindex];
}

// Push sign to temp buffer
void push_temp(char ch)
{
	assert(tindex < TOKENS - 1);
	temp[tindex].type = SIGN;
	temp[tindex++].data = (void *)(long)ch;
}

// Peep temp buffer
char peep_temp(void)
{
	if (tindex < 1) return '\0';
	return (long)temp[tindex - 1].data;
}

// Pop temp buffer
Token *pop_temp(void)
{
	if (tindex < 1) return NULL;
	return &temp[--tindex];
}
