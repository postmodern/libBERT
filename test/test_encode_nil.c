#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

#define EXPECTED_LENGTH 3
#define EXPECTED	"nil"
#define OUTPUT_SIZE	(1 + TEST_COMPLEX_HEADER_SIZE + 1 + 2 + EXPECTED_LENGTH)

unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	const unsigned char *data = test_complex_header(output+1);

	if (data[0] != BERT_ATOM)
	{
		test_fail("bert_encoder_push did not encode the 'true' atom");
	}

	if (data[2] != EXPECTED_LENGTH)
	{
		test_fail("bert_encoder_push encoded %u as the atom length, expected %u",data[1],EXPECTED_LENGTH);
	}

	test_strings((const char *)(data+3),EXPECTED,EXPECTED_LENGTH);
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	bert_data_t *data;

	if (!(data = bert_data_create_nil()))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
