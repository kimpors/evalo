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

	tokenize(argv[1], TOKEN);
	double res = eval(parse());
	double arg = atof(argv[2]);

	if (isinf(res))
	{
		return !(res == arg);
	}

	return !(fabs(arg - res) < EPSILON);
}
