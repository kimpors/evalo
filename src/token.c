#include "token.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int signcmp(char a, char b);

void pusht(Token *tok);
void pushn(double value);

void push_temp(char ch);
char peep_temp(void);
Token *pop_temp(void);

char *gettoken(char *s, Token *dest, size_t lim)
{
	if (!s) return NULL;

	while (lim-- > 0 && *s == ' ') s++;
	if (*s == '\n' || *s == '\0') return NULL; 

	if (*s == '(' || *s == ')')
	{
		dest->type = BRACKET;
		dest->sign = *s;
		s++;
	}
	else if (!isdigit(*s) && *s != '.')
	{
		dest->type = SIGN;
		dest->sign = *s;
		s++;
	}
	else
	{
		dest->type = NUMBER;
		dest->num = strtod(s, &s);
	}

	return s;
}

// Tokenize word.
// Produce stack of tokens
void tokenize(char *s, size_t lim)
{
	assert(s || *s);

	char *ps;
	Token res;
	Token *tok;

	while ((ps = gettoken(s, &res, lim)))
	{
		if (res.type == NUMBER)
		{
			pushn(res.num);
		}
		else if (res.type == BRACKET)
		{
			if (res.sign == '(')
			{
				push_temp(res.sign);
			}
			else if (res.sign == ')')
			{
				while ((tok = pop_temp()) && tok->sign != '(')
				{
					pusht(tok);
				}
			}
		}
		else
		{
			if (signcmp(peep_temp(), res.sign) > 0)
			{
				while ((tok = pop_temp()))
				{
					pusht(tok);
				}
			}

			push_temp(res.sign);
		}

		s = ps;
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
			printf("data: %c\n", out[i].sign);
		}
		else
		{
			printf("data: %f\n", out[i].num);
		}
	}
}

// Push number to output buffer
void pushn(double value)
{
	assert(oindex < TOKENS - 1);
	out[oindex].type = NUMBER;
	out[oindex++].num = value;
}

// Push token to output buffer
void pusht(Token *tok)
{
	assert(oindex < TOKENS - 1);
	out[oindex].type = tok->type;

	if (tok->type == SIGN)
	{
		out[oindex++].sign = tok->sign;
	}
	else
	{
		out[oindex++].num = tok->num;
	}
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
	temp[tindex++].sign = ch;
}

// Peep temp buffer
char peep_temp(void)
{
	if (tindex < 1) return '\0';
	return temp[tindex - 1].sign;
}

// Pop temp buffer
Token *pop_temp(void)
{
	if (tindex < 1) return NULL;
	return &temp[--tindex];
}
