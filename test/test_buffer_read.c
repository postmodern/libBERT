#include <bert/buffer.h>

#include <string.h>
#include <stdio.h>

#include "test.h"

int main()
{
	unsigned char data[BERT_CHUNK_SIZE];
	bert_buffer_t buffer;

	bert_buffer_init(&buffer);

	memset(data,'A',sizeof(data));
	bert_buffer_write(&buffer,data,sizeof(data));

	memset(data,'B',sizeof(data));
	bert_buffer_write(&buffer,data,sizeof(data));

	unsigned char output[sizeof(data)* 2];
	size_t result;

	if ((result = bert_buffer_read(output,&buffer,sizeof(output))) != sizeof(output))
	{
		test_fail("bert_buffer_read only read %u bytes",result);
	}

	int i;

	for (i=0;i<sizeof(data);i++)
	{
		if (output[i] != 'A')
		{
			test_fail("bert_buffer_read return unexpected data");
		}
	}

	for (i=sizeof(data);i<sizeof(output);i++)
	{
		if (output[i] != 'B')
		{
			test_fail("bert_buffer_read return unexpected data");
		}
	}

	return 0;
}
