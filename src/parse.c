#include <stdio.h>
#include <math.h>
#include "parse.h"
#include "error.h"
#include "token.h"

Node *create(Token tok);

Node *parse(Token *tok)
{
	if (!(tok = pop())) return NULL;

	if (tok->type == NUMBER 	||
		tok->type == CONSTANT	||
		tok->type == VARIABLE)
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

	Token tok;
	double left, right;

	switch ((tok = root->token).type)
	{
		case NUMBER: 	return tok.num;
		case CONSTANT: 	return clookup(tok.word);
		case VARIABLE:	return lookup(tok.word);
		default:
						left = eval(root->left);
						right = eval(root->right);
						break;
	}

	switch (tok.sign)
	{
		case '+': return left + right;
		case '-': return right - left;
		case '*': return left * right;
		case '/': return right / left;
		case '^': return pow(right, left);
		case '=': 
				  tok = root->right->token;
				  if (tok.type != VARIABLE)
				  {
					  WARN_MSG("left side doesn't contain variable type");
					  return 0;
				  }

				  install(tok.word, left);
				  return lookup(tok.word);
	}

	return -1;
}

// struct node_
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
