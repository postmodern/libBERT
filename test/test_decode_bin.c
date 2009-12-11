#include <bert/decoder.h>
#include <bert/errno.h>

#include "test.h"
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

bert_decoder_t *decoder;

void test_read()
{
	bert_data_t *data;
	int result;

	if ((result = bert_decoder_next(decoder,&data)) != 1)
	{
		test_fail(bert_strerror(result));
	}

	if (data->type != bert_data_bin)
	{
		test_fail("bert_decoder_next did not decode bin data");
	}

	const unsigned char *expected = (unsigned char *)"hello\0world";
	size_t expected_length = 5 + 1 + 5;

	if (data->bin.length != expected_length)
	{
		test_fail("bert_decoder_next decoded %u bytes, expected %u",data->bin.length,expected_length);
	}

	if (memcmp(data->bin.data,expected,expected_length))
	{
		test_fail("bert_decode_next decoded the wrong bin data");
	}
}

int main()
{
	int fd;

	if ((fd = open("files/bin.bert",O_RDONLY)) == -1)
	{
		test_fail(strerror(errno));
	}

	if (!(decoder = bert_decoder_create()))
	{
		test_fail("malloc failed");
	}

	bert_decoder_stream(decoder,fd);

	test_read();

	bert_decoder_destroy(decoder);
	close(fd);
	return 0;
}
