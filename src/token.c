#include "token.h"
#include "error.h"

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
		dest->type = NONE;
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
	Token *tok = NULL;
	Token prev = {0};

	while ((ps = gettoken(s, &res, lim)))
	{
		if (res.type == NUMBER)
		{
			if (prev.type == NUMBER)
			{
				ERROR_MSG("'%.2f' can't be after '%.2f'", res.num, prev.num);
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
				ERROR_MSG("'%c' can't be after '%.2f'", res.sign, prev.num);
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
		else if (res.type == NONE)
		{
			if (strcmp(res.word, "quit\n") == 0 ||
				strcmp(res.word, "q\n") == 0 ||
				strcmp(res.word, "exit\n") == 0 )
			{
				exit(-1);
			}

			ERROR_MSG("can't parse text: '%s'", res.word)
			clear();
			return -1;
		}
		else
		{
			if (prev.type == SIGN)
			{
				ERROR_MSG("'%c' can't be after '%c'", res.sign, prev.sign);
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
static Token out[TOKEN_MAX];

static size_t tindex;
static Token temp[TOKEN_MAX];

// Push number to output buffer
void pushn(double value)
{
	assert(oindex < TOKEN_MAX - 1);
	out[oindex].type = NUMBER;
	out[oindex++].num = value;
}

// Push token to output buffer
void pusht(Token *tok)
{
	assert(oindex < TOKEN_MAX - 1);
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
	assert(tindex < TOKEN_MAX - 1);
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
