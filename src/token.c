#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <math.h>

#include "token.h"
#include "error.h"

int signcmp(char a, char b);

void pusht(Token *tok);
void pushn(double value);
void clear(void);

void push_temp(char ch);
char peep_temp(void);
Token *pop_temp(void);
int isign(char a);

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
	else if (isign(*s))
	{
		dest->type = SIGN;
		dest->sign = *s;
		s++;
	}
	else 
	{
		char *ps = s;
		while (lim-- > 0 && isalpha(*s))
		{
			dest->word[s - ps] = *s;
			s++;
		}

		dest->word[s - ps] = '\0';
		dest->type = clookup(dest->word)
			? CONSTANT : VARIABLE;
	}

	return s;
}

// Tokenize word.
// Produce stack of tokens
Token *tokenize(char *s, size_t lim)
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
				WARN_MSG("'%.2f' can't be after '%.2f'", res.num, prev.num);
				clear();
				return NULL;
			}

			pushn(res.num);
			prev = res;
		}
		else if (res.type == BRACKET)
		{
			if (prev.type == NUMBER && res.sign == '(')
			{
				WARN_MSG("'%c' can't be after '%.2f'", res.sign, prev.num);
				clear();
				return NULL;
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
		else if (res.type == VARIABLE || res.type == CONSTANT)
		{
			pusht(&res);
			prev = res;
		}
		else if (res.type == SIGN)
		{
			if (prev.type == SIGN)
			{
				WARN_MSG("'%c' can't be after '%c'", res.sign, prev.sign);
				clear();
				return NULL;
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

	return tok;
}

int isign(char a)
{
	switch(a)
	{
		case '+': case '-':
		case '*': case '/':
		case '^': case '=':
			return 1;
	}

	return 0;
}

int signprior(char a)
{
	switch (a)
	{
		case '=':
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
	return signprior(a) - signprior(b);
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
	else if (tok->type == VARIABLE)
	{
		strcpy(out[oindex++].word, tok->word);
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
void clear(void)
{
	oindex = 0;
	tindex = 0;

	memset(out, 0, sizeof(out));
	memset(temp, 0, sizeof(temp));
}

#define HASH 4096
static long double vars[HASH];
static long double cons[HASH];

unsigned hash(char *s)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *s++))
	{
		hash = ((hash << 5) + hash) + c;
	}

	return hash % HASH;
}

void init_cons(void)
{
	cons[hash("pi")] = M_PI;
	cons[hash("e")] = M_E;
	cons[hash("inf")] = INFINITY;
	cons[hash("nan")] = NAN;
}

void install(char *s, long double value)
{
	vars[hash(s)] = value;
}

long double lookup(char *s)
{
	return vars[hash(s)];
}

long double clookup(char *s)
{
	if (!cons[hash(s)]) init_cons();
	return cons[hash(s)];
}
