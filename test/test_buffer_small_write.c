#include <bert/buffer.h>

#include <stdio.h>
#include <string.h>

int main()
{
	unsigned char chunk[4];
	bert_buffer_t buffer;

	memset(chunk,'A',sizeof(chunk));

	bert_buffer_init(&buffer);

	bert_buffer_write(&buffer,chunk,sizeof(chunk));

	if (!(buffer.head))
	{
		fprintf(stderr,"bert_buffer_write did not allocate any chunks\n");
		return -1;
	}

	if (buffer.head->length != 4)
	{
		fprintf(stderr,"bert_buffer_write wrote %u bytes instead of %u bytes\n",buffer.head->length,sizeof(chunk));
		return -1;
	}

	const unsigned char *result = buffer.head->data;

	if (memcmp(result,"AAAA",sizeof(unsigned char)*4))
	{
		fprintf(stderr,"bert_buffer_write wrote %c%c%c%c instead of AAAA\n",result[0],result[1],result[2],result[3]);
		return -1;
	}

	return 0;
}
