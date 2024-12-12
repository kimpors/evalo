#pragma once
#include <stdlib.h>

#define TOKEN 	128
#define TOKENS 	512

typedef enum {
	NONE, WORD, NUMBER, SIGN, BRACKET
} TokenTypeE;

typedef struct {
	TokenTypeE type;
	union {
		char sign;
		double num;
		char word[TOKEN];
	};
} Token;

void show(void);
Token *pop(void);
int tokenize(char *s, size_t lim);
