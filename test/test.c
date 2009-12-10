#include <stdarg.h>

#include "test.h"

void test_fail(const char *mesg,...)
{
	va_list args;

	va_start(args,mesg);
	vfprintf(stderr,mesg,args);
	va_end(args);

	putchar('\n');
	exit(-1);
}
