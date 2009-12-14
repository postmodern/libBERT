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

	unsigned int i;

	for (i=2;i<6;i++)
	{
		if (output[i] != 0xff)
		{
			test_fail("output[%u] is 0x%x, expected 0x%x",output[i],0xff);
		}
	}
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
