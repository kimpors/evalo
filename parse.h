#pragma once
#include "token.h"

typedef struct node {
	Token token;
	struct node *left;
	struct node *right;
} Node;

Node *parse(void);
double evaluate(Node *root);
