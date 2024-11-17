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

void pushc(char ch);
void pushn(long value);
void pusht(Token *tok);
int signcmp(char a, char b);

char peep(void);
Token *outpop(void);
Token *pop(void);
void show(void);
void tokenize(char *s, size_t lim);
