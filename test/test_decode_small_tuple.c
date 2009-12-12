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

	if ((result = bert_decoder_pull(decoder,&data)) != 1)
	{
		test_fail(bert_strerror(result));
	}

	if (data->type != bert_data_tuple)
	{
		test_fail("bert_decoder_next did not decode a tuple");
	}

	size_t expected_length = 3;

	if (data->tuple.length != expected_length)
	{
		test_fail("bert_decoder_next decoded %u elements, expected %u",data->tuple.length,expected_length);
	}

	unsigned int i;

	for (i=0;i<expected_length;i++)
	{
		if (data->tuple.elements[i]->type != bert_data_int)
		{
			test_fail("bert_decode_next decoded a non-integer element at tuple index %u",i);
		}

		if (data->tuple.elements[i]->integer != i+1)
		{
			test_fail("bert_decoder_next decoded the integer %u at tuple index %u, expected %u",data->tuple.elements[i]->integer,i,i+1);
		}
	}
}

int main()
{
	int fd;

	if ((fd = open("files/small_tuple.bert",O_RDONLY)) == -1)
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
