#include <bert/buffer.h>

#include <string.h>
#include <stdio.h>

#include "test.h"

int main()
{
	bert_buffer_t buffer;

	bert_buffer_init(&buffer);

	if (!bert_buffer_empty(&buffer))
	{
		test_fail("bert_buffer should be empty by default");
	}

	const unsigned char data[] = {'A'};

	bert_buffer_write(&buffer,data,sizeof(data));

	if (bert_buffer_empty(&buffer))
	{
		test_fail("bert_buffer should not be empty after writting to it");
	}

	return 0;
}
