#include "token.h"
#include "parse.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define MAX 2048

static char sbuf[MAX];

static bool istext = false;
static bool isfile = false;
static bool isexp = false;
static bool isclip = false;
static unsigned char prec = 2;

void help(void);
void clip(double num);

int main(int argc, char *argv[])
{
	char *ps;
	double res;
	FILE *fp = stdin;

	while (--argc > 0 && (*++argv)[0] == '-')
 	{		
		if (strcmp(*argv, "--help") == 0)
		{
			help();
			return 0;
		}

		switch((*argv)[1])
		{
			case 'c':
				isclip = true;
				break;
			case 'h':
				help();
				return 0;
			case 'e':
				isexp = true;
				break;
			case 't':
				if (isfile)
				{
					fprintf(stderr, "you can't use both -f and -t options\n");
					return -1;
				}

				istext = true;
				ps = *++argv;
				argc--;
				break;
			case 'f':
				if (istext)
				{
					fprintf(stderr, "you can't use both -f and -t options\n");
					return -1;
				}

				ps = *++argv;
				argc--;
				break;
			case 'p':
				prec = atoi(*++argv);

				if (prec < 0)
				{
					fprintf(stderr, "precision can't be negative number\n");
					prec = 2;
				}

				argc--;
				break;
			default:
				fprintf(stderr, "wrong argument: %s\n", *argv);
				return -1;

		}
	}

	if (istext)
	{
		tokenize(ps, TOKEN);
		printf(isexp ? "-> %.*e\n\n" : "-> %.*f\n", prec, res = evaluate(parse()));
		if (isclip) clip(res);
		return 0;
	}

	if (isfile)
	{
		fp = fopen(ps, "r");

		if (!fp)
		{
			fprintf(stderr, "can't open file: %s\n", ps);
			exit(-1);
		}
	}

	while (fgets(sbuf, MAX, fp))
	{
		tokenize(sbuf, TOKEN);
		printf(isexp ? "-> %.*e\n\n" : "-> %.*f\n\n", prec, res = evaluate(parse()));

		if (isclip) clip(res);
		memset(sbuf, 0, sizeof(sbuf));
	}

	fclose(fp);
	return 0;
}

void clip(double num)
{
	FILE *fp = popen("xclip -selection clipboard", "w");
	if (!fp)
	{
		fprintf(stderr, "failed to open xlip\n");
		return;
	}

	fprintf(fp, isexp ? "%.*e" : "%.*f", prec, num);
	pclose(fp);
}

void help(void)
{
	printf("Usage:\n\tevalo [args?]\n\n");

	printf("Options:\n");
	printf("\t-t [EXP]\tUse EXP from args\n");
	printf("\t-p [N]  \tSet precision length to N. Max length is 255\n");
	printf("\t-f <file>\tUse file as source\n");
	printf("\t-e\t\tSet scientific notaion\n");
	printf("\t-c\t\tCopy result to clipboard\n");
	printf("\t-h, --help\tShow help message\n\n");

	printf("Examples:\n");
	printf("\tevalo\n");
	printf("\tevalo -t \"12 + 22\"\n");
	printf("\tevalo -t 12+22\n");
	printf("\tevalo -f text.txt\n");
}
