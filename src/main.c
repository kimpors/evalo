#include "token.h"
#include "parse.h"
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define MAX 2048
static char sbuf[MAX];

static bool istext = false;
static bool isverb = false;
static bool isfile = false;
static bool isexp = false;
static bool isclip = false;
static unsigned char prec = 2;

void help(void);
char *trim(char *s);
void clip(double num);

int main(int argc, char *argv[])
{
	char *ps;
	double res;
	FILE *fp = stdin;
	unsigned char index = 1;

	while (--argc > 0 && *(*++argv) == '-')
 	{		
		if (strcmp(*argv, "--help") == 0)
		{
			help();
			return 0;
		}

		while (*++(*argv))
		{
			switch(**argv)
			{
				case 'c':
					isclip = true;
					break;
				case 'v':
					isverb = true;
					break;
				case 'e':
					isexp = true;
					break;
				case 'h':
					help();
					return 0;
				case 't':
					if (isfile)
					{
						fprintf(stderr, "you can't use both -f and -t options\n");
						return -1;
					}

					istext = true;
					ps = argv[index++];
					argc--;
					break;
				case 'f':
					if (istext)
					{
						fprintf(stderr, "you can't use both -f and -t options\n");
						return -1;
					}

					isfile = true;
					ps = argv[index++];
					argc--;
					break;
				case 'p':
					prec = atoi(argv[index++]);

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
	}

	if (istext)
	{
		tokenize(ps, TOKEN);
		if (isverb) printf("%s ", ps);
		printf(isexp ? "-> %.*e\n\n" : "-> %.*f\n", prec, res = evaluate(parse()));
		if (isclip) clip(res);
		return 0;
	}

	if (isfile)
	{
		if (!(fp = fopen(ps, "r")))
		{
			fprintf(stderr, "can't open file: %s\n", ps);
			exit(-1);
		}
	}

	while (fgets(sbuf, MAX, fp))
	{
		tokenize(sbuf, TOKEN);

		if (isverb) printf("%s ", trim(sbuf));

		if (isfile)
		{
			printf(isexp ? "-> %.*e\n" : "-> %.*f\n", prec, res = evaluate(parse()));
		}
		else
		{
			printf(isexp ? "-> %.*e\n\n" : "-> %.*f\n\n", prec, res = evaluate(parse()));
		}

		if (isclip) clip(res);
		memset(sbuf, 0, sizeof(sbuf));
	}

	fclose(fp);
	return 0;
}


#ifdef __linux__
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
#elif _WIN32
#include <windows.h>
void clip(double num)
{
	char buf[255];
	sprintf_s(buf, sizeof(buf), "%.*f", prec, num);

	if (!OpenClipboard(NULL))
	{
		fprintf(stderr, "Failed to open clipboard\n");
		exit(-1);
	}

	EmptyClipboard();

	size_t len = strlen(buf) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

	if (!hMem)
	{
		CloseClipboard();
		fprintf(stderr, "Failed to allocate memory\n");
		exit(-2);
	}

	memcpy(GlobalLock(hMem), buf, len);
	GlobalUnlock(hMem);

	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}
#endif

void help(void)
{
	printf("Usage:\n\tevalo [args?]\n\n");

	printf("Options:\n");
	printf("\t-t [EXP]\tUse EXP from args\n");
	printf("\t-p [N]  \tSet precision length to N. Max length is 255\n");
	printf("\t-f <file>\tUse file as source\n");
	printf("\t-e\t\tSet scientific notaion\n");
	printf("\t-v\t\tShow full equation with answer\n");
	printf("\t-c\t\tCopy result to clipboard\n");
	printf("\t-h, --help\tShow help message\n\n");

	printf("Examples:\n");
	printf("\tevalo\n");
	printf("\tevalo -t \"12 + 22\"\n");
	printf("\tevalo -t 12+22\n");
	printf("\tevalo -f text.txt\n");
	printf("\tevalo -vcfp text.txt 12\n");
	printf("\tevalo -ecpt 12 \"12 + 22\"\n");
}

char *trim(char *s)
{
	while (isblank(*s)) s++;

	char *ps = s;
	while (*ps != '\0') ps++;
	ps--;
	while (isblank(*ps)) ps--;
	*ps = '\0';

	return s;
}
