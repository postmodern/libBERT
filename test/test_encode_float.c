#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"

#define EXPECTED_LENGTH 17
#define EXPECTED	"0.000000000050000"

#define OUTPUT_SIZE (1 + 1 + 31)

unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	if (output[1] != BERT_FLOAT)
	{
		test_fail("bert_encoder_push did not add the FLOAT magic byte");
	}

	test_strings((const char *)(output+2),EXPECTED,EXPECTED_LENGTH);
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);

	bert_data_t *data;

	if (!(data = bert_data_create_float(0.00000000005)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
