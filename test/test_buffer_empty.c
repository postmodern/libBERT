#include <bert/buffer.h>

#include "test.h"

bert_buffer_t buffer;

void test_before_write()
{
	if (!bert_buffer_empty(&buffer))
	{
		test_fail("bert_buffer should be empty by default");
	}
}

void test_after_write()
{
	if (bert_buffer_empty(&buffer))
	{
		test_fail("bert_buffer should not be empty after writting to it");
	}
}

int main()
{
	bert_buffer_init(&buffer);

	const unsigned char data[] = {'A'};

	test_before_write();
	bert_buffer_write(&buffer,data,1);
	test_after_write();
	return 0;
}
