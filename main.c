#include "token.h"
#include <stdio.h>
#include <assert.h>

#define MAX 2048

static char sbuf[MAX];

int main(void)
{
	while (fgets(sbuf, MAX, stdin))
	{
		tokenize(sbuf, TOKEN);
		show();
	}

	return 0;
}

