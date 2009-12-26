#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

#define EXPECTED_LENGTH (4 + ((1 + 4) * 3))
#define EXPECTED	"time"
#define EXPECTED_MACRO_SECS	100
#define EXPECTED_SECS	21

#define OUTPUT_SIZE	(1 + TEST_COMPLEX_HEADER_SIZE + EXPECTED_LENGTH)

unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	const unsigned char *data = test_complex_header(output+1,EXPECTED);

	if (data[0] != BERT_SMALL_INT)
	{
		test_fail("bert_encoder_push did not encode the first BERT int");
	}

	if (data[1] != EXPECTED_MACRO_SECS)
	{
		test_fail("bert_encoder_push wrote %u as the first BERT int, expected %u",data[1],EXPECTED_MACRO_SECS);
	}

	if (data[2] != BERT_SMALL_INT)
	{
		test_fail("bert_encoder_push did not encode the second BERT int");
	}

	if (data[3] != EXPECTED_SECS)
	{
		test_fail("bert_encoder_push wrote %u as the first BERT int, expected %u",data[3],EXPECTED_SECS);
	}

	if (data[4] != BERT_SMALL_INT)
	{
		test_fail("bert_encoder_push did not encode the third BERT int");
	}

	if (data[5])
	{
		test_fail("bert_encoder_push wrote did not write %u as the third BERT int",0);
	}
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	time_t t1 = (time_t)((EXPECTED_MACRO_SECS * 1000000) + EXPECTED_SECS);
	bert_data_t *data;

	if (!(data = bert_data_create_time(t1)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
