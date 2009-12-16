#include "test.h"
#include <bert/magic.h>
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

void test_bytes(const unsigned char *bytes,const unsigned char *expected,size_t expected_length)
{
	unsigned int i;

	for (i=0;i<expected_length;i++)
	{
		if (bytes[i] != expected[i])
		{
			test_fail("byte[%u] is 0x%x, expected 0x%x",bytes[i],expected[i]);
		}
	}
}

void test_strings(const char *string,const char *expected,size_t expected_length)
{
	unsigned int i;

	for (i=0;i<expected_length;i++)
	{
		if (string[i] != expected[i])
		{
			test_fail("string[%u] is %c, expected %c",string[i],expected[i]);
		}
	}
}

const unsigned char * test_complex_header(const unsigned char *ptr,const char *name)
{
	if (ptr[0] != BERT_SMALL_TUPLE)
	{
		test_fail("complex BERT data must be stored in a small tuple");
	}

	switch (ptr[1])
	{
		case 2:
		case 3:
		case 4:
		case 5:
			break;
		default:
			test_fail("invalid small tuple length %u for BERT complex data",ptr[1]);
	}

	if (ptr[2] != BERT_ATOM)
	{
		test_fail("BERT complex data does not contain the first 'bert' atom");
	}

	if (ptr[4] != 4)
	{
		test_fail("BERT complex keyword has length %u, expected %u",ptr[4],4);
	}

	test_strings((const char *)(ptr+5),"bert",4);

	if (ptr[9] != BERT_ATOM)
	{
		test_fail("BERT complex data does not contain the second '%s' atom",name);
	}

	size_t length = strlen(name);

	if (ptr[11] != length)
	{
		test_fail("second BERT complex keyword has length %u, expected %u",ptr[11],length);
	}

	return ptr+TEST_COMPLEX_HEADER_SIZE+length;
}
