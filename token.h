#pragma once
#include <stdlib.h>

#define TOKEN 	128
#define TOKENS 	512

typedef enum {
	NUMBER, SIGN
} TokenType;

typedef struct {
	void *data;
	TokenType type;
} Token;

void show(void);
Token *pop(void);
void tokenize(char *s, size_t lim);
