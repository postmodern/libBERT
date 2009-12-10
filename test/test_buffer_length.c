#include <bert/buffer.h>

#include "test.h"

bert_buffer_t buffer;

void test_before_write()
{
	if (bert_buffer_length(&buffer))
	{
		test_fail("bert_buffer should have a length of 0 by default");
	}
}

void test_after_write()
{
	size_t length;

	if ((length = bert_buffer_length(&buffer)) != 2)
	{
		test_fail("bert_buffer has length %u, expected 2",length);
	}
}

int main()
{
	bert_buffer_init(&buffer);

	const unsigned char data[] = {'A', 'A'};

	test_before_write();
	bert_buffer_write(&buffer,data,2);
	test_after_write();
	return 0;
}
