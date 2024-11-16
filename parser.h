#pragma once
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
int signcmp(char sign);

char peep(void);
Token *pop(void);
void show(void);
