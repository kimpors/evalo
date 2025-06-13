#pragma once
#include "token.h"
#define TOKEN_MAX 512

typedef struct node {
	Token token;
	struct node *left;
	struct node *right;
} Node;

Node *parse(void);
double eval(Node *root);
