#include "arg.h"
#include <stdbool.h>

int main(int argc, char *argv[])
{
	argeval(argc, argv);
	return (flags & IS_ERROR);
}
