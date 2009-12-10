#include <bert/buffer.h>

#include "test.h"
#include <string.h>

#define OUTPUT_SIZE	(BERT_CHUNK_SIZE * 4)

unsigned char data1[BERT_CHUNK_SIZE];
unsigned char data2[BERT_CHUNK_SIZE / 2];
unsigned char output[OUTPUT_SIZE];

bert_buffer_t buffer;

void test_read_length()
{
	unsigned int output_length = (BERT_CHUNK_SIZE + (BERT_CHUNK_SIZE / 2));
	size_t result;

	if ((result = bert_buffer_read(output,&buffer,OUTPUT_SIZE)) != output_length)
	{
		test_fail("bert_buffer_read only read %u bytes, expected %u",result,output_length);
	}
}

void test_read_contents()
{
	int i;

	for (i=0;i<BERT_CHUNK_SIZE;i++)
	{
		if (output[i] != 'A')
		{
			test_fail("bert_buffer_read return unexpected data");
		}
	}

	for (i=BERT_CHUNK_SIZE;i<(BERT_CHUNK_SIZE + (BERT_CHUNK_SIZE / 2));i++)
	{
		if (output[i] != 'B')
		{
			test_fail("bert_buffer_read return unexpected data");
		}
	}
}

void test_buffer_index()
{
	unsigned int expected_index = (BERT_CHUNK_SIZE / 2);

	if (buffer.index != expected_index)
	{
		test_fail("buffer index was set to %u, should be %u",buffer.index,expected_index);
	}
}

int main()
{

	bert_buffer_init(&buffer);

	memset(data1,'A',BERT_CHUNK_SIZE);
	bert_buffer_write(&buffer,data1,BERT_CHUNK_SIZE);

	memset(data2,'B',BERT_CHUNK_SIZE / 2);
	bert_buffer_write(&buffer,data2,BERT_CHUNK_SIZE / 2);

	test_read_length();
	test_read_contents();
	test_buffer_index();
	return 0;
}
