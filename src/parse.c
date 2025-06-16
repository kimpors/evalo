#include <stdio.h>
#include <math.h>
#include "parse.h"
#include "error.h"
#include "token.h"

Node *create(Token tok);

Node *parse(Token *tok)
{
	if (!(tok = pop())) return NULL;

	if (tok->type == NUMBER || tok->type == VARIABLE)
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
	else if (root->token.type == VARIABLE)
	{
		return lookup(root->token.word);
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
		case '=': 
				  if (root->right->token.type != VARIABLE)
				  {
					  WARN_MSG("left side doesn't contain variable type");
					  return 0;
				  }

				  install(root->right->token.word, left);
				  return lookup(root->right->token.word);
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
