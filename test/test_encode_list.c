#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

#define EXPECTED_LENGTH 256
#define OUTPUT_SIZE	(1 + 1 + 4 + ((1 + 1) * EXPECTED_LENGTH))

unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	if (output[1] != BERT_LIST)
	{
		test_fail("bert_encoder_push did not add the LIST magic byte");
	}

	if ((output[2] != 0x00) || (output[3] != 0x00) || (output[4] != 0x01) || (output[5] != 0x00))
	{
		test_fail("bert_encoder_push did not encode 256 as the list length");
	}

	unsigned int i;
	const unsigned char *output_ptr;

	for (i=0;i<EXPECTED_LENGTH;i++)
	{
		output_ptr = (output + 6 + (i*2));

		if (output_ptr[0] != BERT_SMALL_INT)
		{
			test_fail("bert_encoder_push did not encode a small int at index %u",i);
		}

		if (output_ptr[1] != i)
		{
			test_fail("bert_encoder_push encoded %u for the small int at index %u, expected %u",output_ptr[1],i,i);
		}
	}
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	bert_data_t *data;

	if (!(data = bert_data_create_list()))
	{
		test_fail("malloc failed");
	}

	bert_data_t *element;
	unsigned int i;

	for (i=0;i<EXPECTED_LENGTH;i++)
	{
		if (!(element = bert_data_create_int(i)))
		{
			test_fail("malloc failed");
		}

		bert_list_append(data->list,element);
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
