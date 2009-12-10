#include <bert/buffer.h>

#include <stdio.h>
#include <string.h>

#include "test.h"

int main()
{
	unsigned char chunk[4];
	bert_buffer_t buffer;

	memset(chunk,'A',sizeof(chunk));

	bert_buffer_init(&buffer);

	bert_buffer_write(&buffer,chunk,sizeof(chunk));

	if (!(buffer.head))
	{
		test_fail("bert_buffer_write did not allocate any chunks");
	}

	if (buffer.head->length != 4)
	{
		test_fail("bert_buffer_write wrote %u bytes instead of %u bytes",buffer.head->length,sizeof(chunk));
	}

	const unsigned char *result = buffer.head->data;

	if (memcmp(result,"AAAA",sizeof(unsigned char)*4))
	{
		test_fail("bert_buffer_write wrote %c%c%c%c instead of AAAA",result[0],result[1],result[2],result[3]);
	}

	return 0;
}
