#include <stdio.h>
#include "arg.h"

void eprintf(double val)
{
	printf(flags & IS_EXP ? "-> %.*e\n" : "-> %.*f\n", prec, val);
}
