#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "arg.h"
#include "error.h"

unsigned flags;
signed char prec = 2;

char *argeval(int argc, char *argv[])
{
	char *buf = NULL;
	char **parg = NULL;

	while (--argc > 0 && (parg = (++argv)))
 	{		
		if (strcmp(*argv, "--help") == 0)
		{
			flags |= IS_HELP;
			return NULL;
		}

		if (!flags && **argv != '-')
		{
			flags |= IS_TEXT;
			buf = *argv;
			continue;
		}
		else if (flags && **argv != '-')
		{
			ERROR_MSG("wrong argument '%s'", *argv);
			flags |= IS_ERROR;
			return NULL;
		}

		while (*++(*argv))
		{
			switch(**argv)
			{
				case 'c': flags |= IS_CLIP; break;
				case 'v': flags |= IS_VERB; break;
				case 'e': flags |= IS_EXP; 	break;
				case 'h': flags |= IS_HELP; break;
				case 'f': 
						  if (!(*++parg))
						  {
							  ERROR_MSG("empty argument (supply path to file)");
							  flags |= IS_ERROR;
							  return NULL;
						  }

						  argc--;
						  buf = *parg;
						  flags |= IS_FILE;
						  break;
				case 'p': 
						  if (!(*++parg))
						  {
							  ERROR_MSG("empty argument (supply number for precision)");
							  flags |= IS_ERROR;
							  return NULL;
						  }

						  if (!(prec = atoi(*parg)) && !isdigit(**parg))
						  {
							  ERROR_MSG("not a number: '%s'", *parg);
							  flags |= IS_ERROR;
							  return NULL;
						  }
						  printf("%s", *parg);
						  argc--;
						  break;
				default:
						  ERROR_MSG("wrong argument: '%c'", **parg);
						  flags |= IS_ERROR;
						  return NULL;
			}
		}

		argv = parg;
	}

	return buf;
}

void help(void)
{
	printf("Usage:\n\tevalo [args?] [exp?]\n\n");

	printf("Options:\n");
	printf("\t-p [N]  \tSet precision length to N. It's unsigned variable. Max length is 127.\n");
	printf("\t-f <file>\tUse file as source\n");
	printf("\t-e\t\tSet scientific notaion\n");
	printf("\t-v\t\tShow full equation with answer\n");
	printf("\t-c\t\tCopy result to clipboard\n");
	printf("\t-h, --help\tShow help message\n\n");

	printf("Examples:\n");
	printf("\tevalo\n");
	printf("\tevalo \"12 + 22\"\n");
	printf("\tevalo 12+22\n");
	printf("\tevalo -f text.txt\n");
	printf("\tevalo -vcfp text.txt 12\n");
	printf("\tevalo -ecp 12 \"12 + 22\"\n");
}

#ifdef __linux__
void clip(double num)
{
	FILE *fp = popen("xclip -selection clipboard", "w");
	if (!fp)
	{
		ERROR_MSG("failed to open xclip");
		return;
	}

	fprintf(fp, flags & IS_CLIP ? "%.*e" : "%.*f", prec, num);
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
		ERROR_MSG("failed to open clipboard");
		exit(-1);
	}

	EmptyClipboard();

	size_t len = strlen(buf) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

	if (!hMem)
	{
		CloseClipboard();
		ERROR_MSG("failed to allocate memory");
		exit(-2);
	}

	memcpy(GlobalLock(hMem), buf, len);
	GlobalUnlock(hMem);

	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}
#endif
