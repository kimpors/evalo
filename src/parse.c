#include "parse.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

Node *create(Token tok);

Node *parse(Token *tok)
{
	//Token *tok = NULL;
	if (!(tok = pop())) return NULL;

	if (tok->type == NUMBER)
	{
		return create(*tok);
	}

	Node *node = create(*tok);
	node->left = parse(tok);
	node->right = parse(tok);
	return node;
}

double eval(Node *root)
{
	if (!root) return 0;

	if (root->token.type == NUMBER)
	{
		return root->token.num;
	}

	double left = eval(root->left);
	double right = eval(root->right);

	switch (root->token.sign)
	{
		case '+': return left + right;
		case '-': return right - left;
		case '*': return left * right;
		case '/': return right / left;
		case '^': return pow(right, left);
	}

	return -1;
}

static size_t index;
static Node buf[NODE_MAX];

Node *create(Token tok)
{
	if (index > NODE_MAX - 1) return NULL;

	buf[index].token = tok;
	buf[index].left = NULL;
	buf[index].right = NULL;
	return &buf[index++];
}
