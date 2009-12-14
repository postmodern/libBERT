#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

#define EXPECTED_LENGTH	11
#define EXPECTED	"hello\0world"

#define OUTPUT_SIZE	(1 + 1 + 4 + EXPECTED_LENGTH)
unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	if (output[1] != BERT_BIN)
	{
		test_fail("bert_encoder_push did not add the SMALL_BIN magic byte");
	}

	if (output[5] != EXPECTED_LENGTH)
	{
		test_fail("bert_encoder_push encoded %u as the atom length, expected %u",output[5],EXPECTED_LENGTH);
	}

	test_bytes(output+6,(const unsigned char *)EXPECTED,EXPECTED_LENGTH);
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	bert_data_t *data;

	if (!(data = bert_data_create_bin((const unsigned char *)(EXPECTED),EXPECTED_LENGTH)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
