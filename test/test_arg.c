#include "arg.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	return argeval(argc, argv) == NULL;
}
