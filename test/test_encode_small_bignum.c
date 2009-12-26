#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"

#define EXPECTED_LENGTH	5
#define EXPECTED	0x01

#define OUTPUT_SIZE (1 + 1 + 1 + 1 + 5)

unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	if (output[1] != BERT_SMALL_BIGNUM)
	{
		test_fail("bert_encoder_push did not add the SMALL_BIGNUM magic byte, wrote %u",output[1]);
	}

	if (output[2] != EXPECTED_LENGTH)
	{
		test_fail("bert_encoder_push wrote %u as the bignum length, expected %u",output[2],EXPECTED_LENGTH);
	}

	if (output[3] != 1)
	{
		test_fail("bert_encoder_push did not write the signed byte");
	}

	unsigned char bytes[] = {0x00, 0x00, 0x00, 0x00, EXPECTED};

	test_bytes(output+4,bytes,5);
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	bert_data_t *data;
	int64_t i = -(((uint64_t)EXPECTED) << 32);

	if (!(data = bert_data_create_int(i)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
