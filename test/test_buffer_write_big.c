#include <bert/buffer.h>

#include <stdio.h>
#include <string.h>

#include "test.h"

#define DATA_SIZE	1024

int main()
{
	unsigned char data[DATA_SIZE];
	bert_buffer_t buffer;

	memset(data,'A',DATA_SIZE);

	bert_buffer_init(&buffer);

	bert_buffer_write(&buffer,data,DATA_SIZE);

	if (!(buffer.head))
	{
		test_fail("bert_buffer_write did not allocate any chunks");
	}

	size_t length;

	if ((length = bert_buffer_length(&buffer)) != DATA_SIZE)
	{
		test_fail("bert_buffer_write only wrote %u bytes, expected %u bytes",length,DATA_SIZE);
	}

	const bert_chunk_t *chunk = buffer.head;
	unsigned int count = 0;
	unsigned int i;

	while (chunk)
	{
		for (i=0;i<chunk->length;i++)
		{
			if (chunk->data[i] != 'A')
			{
				test_fail("chunk[%u][%u] is %c, expected %c",count,i,chunk->data[i],'A');
			}
		}

		++count;
		chunk = chunk->next;
	}

	return 0;
}
