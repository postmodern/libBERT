#include <bert/buffer.h>

#include <string.h>
#include <stdio.h>

#include "test.h"

#define DATA_SIZE	4

int main()
{
	unsigned char data[4];
	bert_buffer_t buffer;

	bert_buffer_init(&buffer);

	memset(data,'A',DATA_SIZE);

	unsigned int i;

	for (i=0;i<((BERT_CHUNK_SIZE / DATA_SIZE) * 2);i++)
	{
		bert_buffer_write(&buffer,data,DATA_SIZE);
	}

	unsigned char output[DATA_SIZE];
	size_t result;

	if ((result = bert_buffer_read(output,&buffer,DATA_SIZE)) != DATA_SIZE)
	{
		test_fail("bert_buffer_read only read %u bytes, expected %u",result,DATA_SIZE);
	}

	if (memcmp(output,data,DATA_SIZE))
	{
		test_fail("bert_buffer_read return %c%c%c%c, expected AAAA",output[0],output[1],output[2],output[3]);
	}

	return 0;
}
