#include <bert/decoder.h>
#include <bert/errno.h>

#include "test.h"
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define EXPECTED	-4294967296L

bert_decoder_t *decoder;

void test_read()
{
	bert_data_t *data;
	int result;

	if ((result = bert_decoder_pull(decoder,&data)) != 1)
	{
		test_fail(bert_strerror(result));
	}

	if (data->type != bert_data_int)
	{
		test_fail("bert_decoder_next did not decode an int");
	}

	if (data->integer != EXPECTED)
	{
		test_fail("bert_decoder_next decoded %lld, expected %lld",data->integer,EXPECTED);
	}
}

int main()
{
	int fd;

	decoder = bert_decoder_create();
	fd = test_open_file("files/small_bignum.bert");
	bert_decoder_stream(decoder,fd);

	test_read();

	bert_decoder_destroy(decoder);
	close(fd);
	return 0;
}
