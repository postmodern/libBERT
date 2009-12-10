#include "test.h"

void test_fail(const char *mesg,...)
{
	fprintf(stderr,mesg);
	putchar('\n');

	exit(-1);
}
