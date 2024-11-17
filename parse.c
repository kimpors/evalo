#include "parse.h"
#include <stdio.h>
#include <assert.h>

#define MAX 2048

Node *create(Token tok);

Node *parse(void)
{
	Token *tok;
	assert((tok = outpop()));

	if (tok->type == NUMBER)
	{
		return create(*tok);
	}

	Node *node = create(*tok);
	node->left = parse();
	node->right = parse();
	return node;
}

long evaluate(Node *root)
{
	if (!root) return 0;

	if (root->token.type == NUMBER)
	{
		return (long)root->token.data;
	}

	long left = evaluate(root->left);
	long right = evaluate(root->right);

	switch ((char)(long)root->token.data)
	{
		case '+': return left + right;
		case '-': return right - left;
		case '*': return left * right;
		case '/': return right / left;
	}

	return -1;
}

static size_t index;
static Node buf[MAX];

Node *create(Token tok)
{
	if (index > MAX - 1) return NULL;
	buf[index].token = tok;
	return &buf[index++];
}
