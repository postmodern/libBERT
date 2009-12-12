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

	if (data->type != bert_data_regex)
	{
		test_fail("bert_decoder_next did not decode a boolean");
	}

	const char *expected_source = "hello\\s*world";

	if (strcmp(data->regex.source,expected_source))
	{
		test_fail("bert_decoder_next decoded regex source %s, expected %s",data->regex.source,expected_source);
	}

	unsigned int expected_options = BERT_REGEX_CASELESS;

	if (data->regex.options != expected_options)
	{
		test_fail("bert_decoder_next decoded regex options %u, expected %u",expected_options);
	}
}

int main()
{
	int fd;

	if ((fd = open("files/regex.bert",O_RDONLY)) == -1)
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
