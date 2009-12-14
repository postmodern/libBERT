#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

unsigned char output[6];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	if (output[1] != BERT_ATOM)
	{
		test_fail("bert_encoder_push did not add the SMALL_INT magic byte");
	}

	size_t expected_length = 2;

	if (output[3] != expected_length)
	{
		test_fail("bert_encoder_push encoded %u as the atom length, expected %u",output[3],expected_length);
	}

	const char *expected = "id";

	test_strings((const char *)(output+4),expected,expected_length);
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,6);
	bert_data_t *data;

	if (!(data = bert_data_create_atom("id")))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
