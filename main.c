#include "token.h"
#include "parse.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define MAX 2048

static char sbuf[MAX];

int main(void)
{
	while (fgets(sbuf, MAX, stdin))
	{
		tokenize(sbuf, TOKEN);
		printf("res: %ld\n", evaluate(parse()));
		memset(sbuf, 0, sizeof(sbuf));
	}

	return 0;
}

