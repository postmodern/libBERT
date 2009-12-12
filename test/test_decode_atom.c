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

	if (data->type != bert_data_atom)
	{
		test_fail("bert_decoder_next did not decode an atom");
	}

	const char *expected = "id";
	size_t expected_length = strlen(expected);

	if (data->atom.length != expected_length)
	{
		test_fail("bert_decoder_next decoded %u bytes, expected %u",data->atom.length,expected_length);
	}

	if (memcmp(data->atom.name,expected,expected_length))
	{
		test_fail("bert_decoder_next decoded %s, expected %s",data->atom.name,expected);
	}
}

int main()
{
	int fd;

	if ((fd = open("files/atom.bert",O_RDONLY)) == -1)
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
