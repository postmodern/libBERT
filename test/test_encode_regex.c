#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

#define EXPECTED_LENGTH 13
#define EXPECTED	"hello\\s*world"
#define OUTPUT_SIZE	(1 + TEST_COMPLEX_HEADER_SIZE + 5 + 1 + 4 + EXPECTED_LENGTH + 1 + 4)

unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	const unsigned char *data = test_complex_header(output+1,"regex");

	if (data[0] != BERT_BIN)
	{
		test_fail("bert_encoder_push did not encode the BIN magic byte");
	}

	if (data[4] != EXPECTED_LENGTH)
	{
		test_fail("bert_encoder_push encoded %u as the source length, expected %u",data[4],EXPECTED_LENGTH);
	}

	test_strings((const char *)(data+5),EXPECTED,EXPECTED_LENGTH);

	if (data[5+EXPECTED_LENGTH] != BERT_LIST)
	{
		test_fail("bert_encoder_push did not encode the LIST magic byte, for the regex options");
	}

	if (data[5+EXPECTED_LENGTH+3] != 0)
	{
		test_fail("bert_encoder_push encoded %u as the regex option list length, expected %u",data[5+EXPECTED_LENGTH+3],0);
	}
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	bert_data_t *data;

	if (!(data = bert_data_create_regex(EXPECTED,EXPECTED_LENGTH,0)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
