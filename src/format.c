#include <stdio.h>
#include "arg.h"

void eprintf(double val)
{
	printf(flags & ARG_EXP ? "-> %.*e\n" : "-> %.*f\n", prec, val);
}
