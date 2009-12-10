#include <bert/buffer.h>

#include "test.h"
#include <string.h>

#define DATA_SIZE	((BERT_CHUNK_SIZE * 2) + 20)

unsigned char data[DATA_SIZE];
bert_buffer_t buffer;

void test_head()
{
	if (!(buffer.head))
	{
		test_fail("bert_buffer_write did not allocate any chunks");
	}
}

void test_length()
{
	size_t length;

	if ((length = bert_buffer_length(&buffer)) != DATA_SIZE)
	{
		test_fail("bert_buffer_write wrote %u bytes, expected %u bytes",length,DATA_SIZE);
	}
}

void test_wrote_contents()
{
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
}

int main()
{
	memset(data,'A',DATA_SIZE);

	bert_buffer_init(&buffer);
	bert_buffer_write(&buffer,data,DATA_SIZE);

	test_head();
	test_length();
	test_wrote_contents();
	return 0;
}
