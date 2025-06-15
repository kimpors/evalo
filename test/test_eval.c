#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "parse.h"

#define EPSILON 1e-2

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("wrong args\n");
		return -1;
	}

	Token *tok = tokenize(argv[1], TOKEN_MAX);
	double res = eval(parse(tok));
	double arg = atof(argv[2]);

	if (isinf(res))
	{
		return !(res == arg);
	}

	return !(fabs(arg - res) < EPSILON);
}
