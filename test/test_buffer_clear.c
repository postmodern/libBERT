#include <bert/buffer.h>
#include <string.h>

#include "test.h"

#define DATA_SIZE	1024

int main()
{
	bert_buffer_t buffer;
	unsigned char data[DATA_SIZE];

	memset(data,'A',DATA_SIZE);
	bert_buffer_init(&buffer);

	bert_buffer_write(&buffer,data,DATA_SIZE);
	bert_buffer_clear(&buffer);

	if (buffer.head)
	{
		test_fail("bert_buffer_clear should reset the head pointer");
	}

	if (buffer.tail)
	{
		test_fail("bert_buffer_clear should reset the tail pointer");
	}

	if (buffer.index)
	{
		test_fail("bert_buffer_clear should reset the index");
	}

	return 0;
}
