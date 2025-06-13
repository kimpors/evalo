#include "arg.h"
#include <stdio.h>

void eprintf(double val)
{
	printf(flags & IS_EXP ? "-> %.*e\n" : "-> %.*f\n", prec, val);
}
