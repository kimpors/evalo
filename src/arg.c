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
