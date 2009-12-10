#include <bert/buffer.h>

#include "test.h"
#include <string.h>

#define DATA_SIZE	1024
#define OUTPUT_SIZE	(BERT_CHUNK_SIZE + 64)

int main()
{
	unsigned char data[DATA_SIZE];
	bert_buffer_t buffer;

	bert_buffer_init(&buffer);
	memset(data,'A',DATA_SIZE);

	bert_buffer_write(&buffer,data,DATA_SIZE);

	unsigned char output[OUTPUT_SIZE];
	size_t result;

	if ((result = bert_buffer_read(output,&buffer,OUTPUT_SIZE)) != OUTPUT_SIZE)
	{
		test_fail("bert_buffer_read only read %u bytes, expected %u",result,OUTPUT_SIZE);
	}

	unsigned int i;

	for (i=0;i<OUTPUT_SIZE;i++)
	{
		if (data[i] != output[i])
		{
			test_fail("output[%u] is %c, expected %c",i,output[i],data[i]);
		}
	}

	return 0;
}
