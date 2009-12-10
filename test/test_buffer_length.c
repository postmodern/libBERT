#include <bert/buffer.h>

#include "test.h"

int main()
{
	bert_buffer_t buffer;

	bert_buffer_init(&buffer);

	if (bert_buffer_length(&buffer))
	{
		test_fail("bert_buffer should have a length of 0 by default");
	}

	const unsigned char data[] = {'A', 'A'};

	bert_buffer_write(&buffer,data,2);

	size_t length;

	if ((length = bert_buffer_length(&buffer)) != 2)
	{
		test_fail("bert_buffer has length %u, expected 2",length);
	}

	return 0;
}
