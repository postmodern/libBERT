#include <bert/buffer.h>

#include "test.h"
#include <string.h>

#define DATA_SIZE	1024
#define OUTPUT_SIZE	(BERT_CHUNK_SIZE + 64)

unsigned char data[DATA_SIZE];
unsigned char output[OUTPUT_SIZE];

bert_buffer_t buffer;

void test_read_length()
{
	size_t result;

	if ((result = bert_buffer_read(output,&buffer,OUTPUT_SIZE)) != OUTPUT_SIZE)
	{
		test_fail("bert_buffer_read only read %u bytes, expected %u",result,OUTPUT_SIZE);
	}
}

void test_read_contents()
{
	unsigned int i;

	for (i=0;i<OUTPUT_SIZE;i++)
	{
		if (data[i] != output[i])
		{
			test_fail("output[%u] is %c, expected %c",i,output[i],data[i]);
		}
	}
}

void test_buffer_index()
{
	unsigned int expected_index = (OUTPUT_SIZE % BERT_CHUNK_SIZE);

	if (buffer.index != expected_index)
	{
		test_fail("buffer index was set to %u, expected %u",buffer.index,expected_index);
	}
}

int main()
{
	bert_buffer_init(&buffer);
	memset(data,'A',DATA_SIZE);

	bert_buffer_write(&buffer,data,DATA_SIZE);

	test_read_length();
	test_read_contents();
	test_buffer_index();
	return 0;
}
