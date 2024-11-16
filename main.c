#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 2048
#define TOKEN 128

static char buf[MAX];

char *gettoken(char *s, size_t lim);

int main(void)
{
	char *pbuf = buf;
	char *ps;

	while (fgets(pbuf, MAX, stdin))
	{
		while ((ps = gettoken(pbuf, TOKEN)))
		{
			printf("token: %s\n", ps);
			pbuf = pbuf + strlen(ps) + 1;
		}
	}

	return 0;
}

char *gettoken(char *s, size_t lim)
{
	if (!s || *s == '\0') return NULL;

	while (lim-- > 0 && *s == ' ') s++;
	char *ps = s;
	while (lim-- > 0 && *ps != ' ') ps++;

	*ps = '\0';
	return s;
}
