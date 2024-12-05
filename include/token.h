#pragma once
#include <stdlib.h>

#define TOKEN 	128
#define TOKENS 	512

typedef enum {
	NUMBER, SIGN, BRACKET
} TokenTypeE;

typedef struct {
	TokenTypeE type;
	union {
		char sign;
		double num;
	};
} Token;

void show(void);
Token *pop(void);
void tokenize(char *s, size_t lim);
