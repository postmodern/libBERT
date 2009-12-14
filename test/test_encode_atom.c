#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

#define EXPECTED_LENGTH 2
#define EXPECTED	"id"
#define OUTPUT_SIZE	(1 + 1 + 2 + EXPECTED_LENGTH)

unsigned char output[OUTPUT_SIZE];

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

	if (output[3] != EXPECTED_LENGTH)
	{
		test_fail("bert_encoder_push encoded %u as the atom length, expected %u",output[3],EXPECTED_LENGTH);
	}

	test_strings((const char *)(output+4),EXPECTED,EXPECTED_LENGTH);
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	bert_data_t *data;

	if (!(data = bert_data_create_atom(EXPECTED)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
