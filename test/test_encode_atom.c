#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

unsigned char output[6];

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

	if (output[3] != 2)
	{
		test_fail("bert_encoder_push encoded %u as the atom length, expected %u",output[3],2);
	}

	const char *name = "id";

	if (memcmp(output+4,name,sizeof(unsigned char)*2))
	{
		test_fail("bert_encoder_push encoded %c%c, expected %s",output[4],output[5],name);
	}
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,6);
	bert_data_t *data;

	if (!(data = bert_data_create_atom("id")))
	{
		test_fail("malloc failed");
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
