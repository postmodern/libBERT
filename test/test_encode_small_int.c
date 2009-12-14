#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"

unsigned char output[3];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	if (output[1] != BERT_SMALL_INT)
	{
		test_fail("bert_encoder_push did not add the SMALL_INT magic byte");
	}

	if (output[2] != 127)
	{
	}
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,3);

	bert_data_t *data;

	if (!(data = bert_data_create_int(127)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
