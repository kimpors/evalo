#pragma once
#include <stdlib.h>

#define WORD_MAX 	32
#define TOKEN_MAX 	512

typedef enum {
	NONE, NUMBER, SIGN, BRACKET, VARIABLE, CONSTANT
} TokenTypeE;

typedef struct {
	TokenTypeE type;
	union {
		char sign;
		double num;
		char word[WORD_MAX];
	};
} Token;

Token *pop(void);
Token *tokenize(char *s, size_t lim);

long double lookup(char *s);
long double clookup(char *s);
void install(char *s, long double value);
