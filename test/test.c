#include "test.h"
#include <bert/errno.h>

#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

void test_fail(const char *mesg,...)
{
	va_list args;

	va_start(args,mesg);
	vfprintf(stderr,mesg,args);
	va_end(args);

	putchar('\n');
	exit(-1);
}

int test_open_file(const char *path)
{
	int fd;

	if ((fd = open(path,O_RDONLY)) == -1)
	{
		test_fail(strerror(errno));
	}

	return fd;
}

bert_decoder_t * test_decoder()
{
	bert_decoder_t *decoder;

	if (!(decoder = bert_decoder_create()))
	{
		test_fail("malloc failed");
	}

	return decoder;
}

bert_encoder_t * test_encoder(unsigned char *buffer,size_t length)
{
	bert_encoder_t *encoder;

	if (!(encoder = bert_encoder_create()))
	{
		test_fail("malloc failed");
	}

	bert_encoder_buffer(encoder,buffer,length);
	return encoder;
}

void test_encoder_push(bert_encoder_t *encoder,const bert_data_t *data)
{
	int result;

	if ((result = bert_encoder_push(encoder,data)) != BERT_SUCCESS)
	{
		test_fail(bert_strerror(result));
	}

}
