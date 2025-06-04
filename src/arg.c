#include "arg.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

bool ishelp = false;
bool istext = false;
bool isverb = false;
bool isfile = false;
bool isclip = false;
bool isexp = false;

char *arg_text = NULL;
signed char prec = 2;

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

int arg_evaluate(int argc, char *argv[])
{
	unsigned char index = 1;

	while (--argc > 0 && *(*++argv) == '-')
 	{		
		if (strcmp(*argv, "--help") == 0)
		{
			ishelp = true;
			continue;
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
					ishelp = true;
					break;
				case 't':
					if (isfile)
					{
						fprintf(stderr, "argument error: you can't use both -f and -t options\n");
						return -1;
					}

					if (!argv[index])
					{
						fprintf(stderr, "argument erro: empty arg\n");
						return -1;
					}

					istext = true;
					arg_text = argv[index++];
					argc--;
					break;
				case 'f':
					if (istext)
					{
						fprintf(stderr, "argument error: you can't use both -f and -t options\n");
						return -1;
					}

					if (!argv[index])
					{
						fprintf(stderr, "argument erro: empty arg\n");
						return -1;
					}

					isfile = true;
					arg_text = argv[index++];
					argc--;
					break;
				case 'p':
					if (!argv[index])
					{
						fprintf(stderr, "argument erro: empty arg\n");
						return -1;
					}

					prec = atoi(argv[index]);

					if (prec == 0 && (*argv[index] <= '0' || *argv[index] >= '9'))
					{
						fprintf(stderr, "argument error: not a number: %s\n", argv[index]);
						return -1;
					}

					if (prec < 0)
					{
						fprintf(stderr, "argument error: precision can't be negative number: %s\n", argv[index]);
						return -1;
					}

					index++;
					argc--;
					break;
				default:
					fprintf(stderr, "argument error: wrong argument: %c\n", **argv);
					return -1;
			}
		}
	}

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
