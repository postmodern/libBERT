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

	if (data->type != bert_data_float)
	{
		test_fail("bert_decoder_next did not decode a float");
	}

	double expected = 0.0000000005;

	if (data->floating_point != expected)
	{
		test_fail("bert_decoder_next decoded %u, expected %u",data->integer,expected);
	}
}

int main()
{
	int fd;

	decoder = bert_decoder_create();

	fd = test_open_file("files/float.bert");
	bert_decoder_stream(decoder,fd);

	test_read();

	bert_decoder_destroy(decoder);
	close(fd);
	return 0;
}
