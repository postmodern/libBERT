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

	if (data->type != bert_data_list)
	{
		test_fail("bert_decoder_next did not decode a list");
	}

	bert_list_node_t *next_node = data->list->head;
	unsigned int i = 1;
	unsigned int length = 0;

	while (next_node)
	{
		if (next_node->data->type != bert_data_int)
		{
			test_fail("bert_decoder_next decoded a non-integer for list index %u",length);
		}

		if (next_node->data->integer != i)
		{
			test_fail("bert_decoder_next decoded integer %u for list index %u, expected %u",next_node->data->integer,length,i);
		}

		++i;
		++length;

		next_node = next_node->next;
	}

	unsigned int expected_length = 3;

	if (length != expected_length)
	{
		test_fail("bert_decoder_next decoded %u list elements, expected %u",length,expected_length);
	}
}

int main()
{
	int fd;

	decoder = bert_decoder_create();

	fd = test_open_file("files/list.bert");
	bert_decoder_stream(decoder,fd);

	test_read();

	bert_decoder_destroy(decoder);
	close(fd);
	return 0;
}
