#include <bert/buffer.h>
#include <string.h>

#include "test.h"

int main()
{
	bert_buffer_t buffer;

	bert_buffer_init(&buffer);

	if (buffer.head)
	{
		test_fail("bert_buffer_init should reset the head pointer");
	}

	if (buffer.tail)
	{
		test_fail("bert_buffer_init should reset the tail pointer");
	}

	if (buffer.index)
	{
		test_fail("bert_buffer_init should reset the index");
	}

	return 0;
}
