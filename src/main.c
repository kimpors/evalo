#include "token.h"
#include "parse.h"
#include "format.h"
#include "arg.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define MAX 1024
static char sbuf[MAX];

void help(void);
char *trim(char *s);
void clip(double num);

int main(int argc, char *argv[])
{
	if (arg_evaluate(argc, argv)) return -1;

	if (ishelp)
	{
		help();
		return 0;
	}

	long double res;
	FILE *fp = stdin;

	if (istext)
	{
		if (tokenize(arg_text, TOKEN)) return -2;
		printf("%s", format_eval(res = evaluate(parse())));
		if (isclip) clip(res);
		return 0;
	}

	if (isfile)
	{
		if (!(fp = fopen(arg_text, "r")))
		{
			fprintf(stderr, "can't open file: %s\n", arg_text);
			exit(-1);
		}
	}

	while (fgets(sbuf, MAX, fp))
	{
		if (!tokenize(sbuf, TOKEN))
		{
			printf("%s\n", format_eval(res = evaluate(parse())));
			if (isclip) clip(res);
		}
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
	printf("\t-p [N]  \tSet precision length to N. Max length is 127\n");
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

