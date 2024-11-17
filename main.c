#include "token.h"
#include "parse.h"
#include <stdio.h>
#include <assert.h>

#define MAX 2048

static char sbuf[MAX];

int main(void)
{
	Token *tok;

	while (fgets(sbuf, MAX, stdin))
	{
		tokenize(sbuf, TOKEN);
		printf("res: %ld\n", evaluate(parse()));
	}

	return 0;
}

