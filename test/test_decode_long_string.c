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

	if (data->type != bert_data_string)
	{
		test_fail("bert_decoder_next did not decode a string");
	}

	size_t expected_length = 1025;

	if (data->string.length != expected_length)
	{
		test_fail("bert_decoder_next decoded %u bytes, expected %u",data->string.length,expected_length);
	}

	unsigned int i;

	for (i=0;i<expected_length-1;i++)
	{
		if (data->string.text[i] != 'A')
		{
			test_fail("data->string.text[%u] is %c, expected %c",i,data->string.text[i],'A');
		}
	}

	if (data->string.text[expected_length-1] != 'B')
	{
		test_fail("data->string.text[%u] is %c, expected %c",expected_length-1,data->string.text[expected_length-1],'B');
	}
}

int main()
{
	int fd;

	decoder = bert_decoder_create();

	fd = test_open_file("files/long_string.bert");
	bert_decoder_stream(decoder,fd);

	test_read();

	bert_decoder_destroy(decoder);
	close(fd);
	return 0;
}
