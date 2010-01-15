#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

#define EXPECTED_LENGTH 2
#define OUTPUT_SIZE	(1 + 1 + 1 + ((1 + 1) * EXPECTED_LENGTH))

unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	if (output[1] != BERT_SMALL_TUPLE)
	{
		test_fail("bert_encoder_push did not add the SMALL_TUPLE magic byte");
	}

	if (output[2] != EXPECTED_LENGTH)
	{
		test_fail("bert_encoder_push encoded %u as the tuple length, expected %u",output[2],EXPECTED_LENGTH);
	}

	if (output[3] != BERT_SMALL_INT)
	{
		test_fail("bert_encoder_push failed to encode the first small int of the tuple");
	}

	if (output[4] != 1)
	{
		test_fail("bert_encoder_push wrote %u as the first small int, expected %u",output[4],1);
	}

	if (output[5] != BERT_SMALL_INT)
	{
		test_fail("bert_encoder_push failed to encode the second small int of the tuple");
	}

	if (output[6] != 2)
	{
		test_fail("bert_encoder_push wrote %u as the second small int, expected %u",output[6],2);
	}
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	bert_data_t *data;

	if (!(data = bert_data_create_tuple(EXPECTED_LENGTH)))
	{
		test_fail("malloc failed");
	}

	if (!(data->tuple->elements[0] = bert_data_create_int(1)))
	{
		test_fail("malloc failed");
	}

	if (!(data->tuple->elements[1] = bert_data_create_int(2)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
