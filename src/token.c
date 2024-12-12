#include "token.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int signcmp(char a, char b);

void pusht(Token *tok);
void pushn(double value);
void clear(void);

void push_temp(char ch);
char peep_temp(void);
Token *pop_temp(void);
int check_sign(char a);

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
	else if (isdigit(*s))
	{
		dest->type = NUMBER;
		dest->num = strtod(s, &s);
	}
	else if (check_sign(*s))
	{
		dest->type = WORD;
		strcpy(dest->word, s);
		s++;
	}
	else
	{
		dest->type = SIGN;
		dest->sign = *s;
		s++;
	}

	return s;
}

// Tokenize word.
// Produce stack of tokens
int tokenize(char *s, size_t lim)
{
	assert(s || *s);

	char *ps;
	Token res;
	Token *tok;
	Token prev;

	while ((ps = gettoken(s, &res, lim)))
	{
		if (res.type == NUMBER)
		{
			if (prev.type == NUMBER)
			{
				fprintf(stderr, "format error: '%.2f' can't be after '%.2f'\n", res.num, prev.num);
				clear();
				return -1;
			}

			pushn(res.num);
			prev = res;
		}
		else if (res.type == BRACKET)
		{
			if (prev.type == NUMBER && res.sign == '(')
			{
				fprintf(stderr, "format error: '%c' can't be after '%.2f'\n", res.sign, prev.num);
				clear();
				return -1;
			}

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

			prev = res;
		}
		else if (res.type == WORD)
		{
			fprintf(stderr, "format error: can't parse text: %s\n", res.word);
			clear();
			return -1;
		}
		else
		{
			if (prev.type == SIGN)
			{
				fprintf(stderr, "format error: '%c' can't be after '%c'\n", res.sign, prev.sign);
				clear();
				return -1;
			}

			if (signcmp(peep_temp(), res.sign) > 0)
			{
				while ((tok = pop_temp()))
				{
					pusht(tok);
				}
			}

			push_temp(res.sign);
			prev = res;
		}

		s = ps;
	}

	while ((tok = pop_temp()))
	{
		pusht(tok);
	}

	return 0;
}

int check_sign(char a)
{
	switch(a)
	{
		case '+': case '-':
		case '*': case '/':
		case '^':
			return 0;
	}

	return -1;
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

// Pop output buffer
Token *pop(void)
{
	if (oindex < 1) return NULL;
	return &out[--oindex];
}

// Pop temp buffer
Token *pop_temp(void)
{
	if (tindex < 1) return NULL;
	return &temp[--tindex];
}

// Clear all buffers
inline void clear(void)
{
	while (pop());
	while (pop_temp());
}
