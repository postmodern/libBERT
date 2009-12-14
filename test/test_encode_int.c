#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"

unsigned char output[6];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	if (output[1] != BERT_INT)
	{
		test_fail("bert_encoder_push did not add the INT magic byte");
	}

	unsigned char bytes[] = {0xff, 0xff, 0xff, 0xff};

	test_bytes(output+2,bytes,4);
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,6);

	bert_data_t *data;

	if (!(data = bert_data_create_int(0xffffffff)))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
