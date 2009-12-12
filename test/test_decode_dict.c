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

	if (data->type != bert_data_dict)
	{
		test_fail("bert_decoder_next did not decode a dict");
	}

	bert_dict_node_t *next_node = data->dict->head;

	if (!next_node)
	{
		test_fail("bert_decoder_next failed to decode the first dict key->value pair");
	}

	if (next_node->next)
	{
		test_fail("bert_decoder_next decoded more than one dict key->value pair");
	}

	bert_data_t *key = next_node->key;

	if (key->type != bert_data_int)
	{
		test_fail("bert_decoder_next did not decode an integer as the first dict key");
	}

	unsigned int expected_key = 1;

	if (key->integer != expected_key)
	{
		test_fail("bert_decoder_next decoded %u as the first dict key, expected %u",key->integer,expected_key);
	}

	bert_data_t *value = next_node->value;

	if (value->type != bert_data_int)
	{
		test_fail("bert_decoder_next did not decode an integer as the first dict value");
	}

	unsigned int expected_value = 2;

	if (value->integer != expected_value)
	{
		test_fail("bert_decoder_next decoded %u as the first dict value, expected %u",value->integer,expected_value);
	}
}

int main()
{
	int fd;

	if ((fd = open("files/dict.bert",O_RDONLY)) == -1)
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
