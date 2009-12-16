#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"
#include <string.h>

#define EXPECTED_LENGTH 2
#define OUTPUT_SIZE	(1 + TEST_COMPLEX_HEADER_SIZE + 4 + 1 + 4 + ((1 + 1 + ((1 + 1) + (1 + 1))) * EXPECTED_LENGTH))

unsigned char output[OUTPUT_SIZE];

void test_output()
{
	if (output[0] != BERT_MAGIC)
	{
		test_fail("bert_encoder_push did not add the magic byte");
	}

	const unsigned char *data = test_complex_header(output+1,"dict");

	if (data[0] != BERT_LIST)
	{
		test_fail("bert_encoder_push did not encode the LIST magic byte");
	}

	if (data[4] != EXPECTED_LENGTH)
	{
		test_fail("bert_encoder_push encoded %u as the atom length, expected %u",data[4],EXPECTED_LENGTH);
	}

	unsigned int i;
	const unsigned char *tuple_ptr;

	for (i=0;i<2;i++)
	{
		tuple_ptr = (data + 5 + (((1 + 1) + (1 + 1) + (1 + 1)) * i));

		if (tuple_ptr[0] != BERT_SMALL_TUPLE)
		{
			test_fail("bert_encoder_push did not encode a SMALL_TUPLE for the key->value pair, at list index %u",i);
		}

		if (tuple_ptr[1] != 2)
		{
			test_fail("bert_encoder_push encoded %u as the length of the key->value tuple at list index %u, expected %u",tuple_ptr[1],i,2);
		}

		if (tuple_ptr[2] != BERT_SMALL_INT)
		{
			test_fail("bert_encoder_push did not encode a SMALL_INT as the key, at list index %u",i);
		}

		if (tuple_ptr[3] != (i + 1))
		{
			test_fail("bert_encoder_push encoded %u as the value at list index %u, expected %u",tuple_ptr[3],i,i+1);
		}

		if (tuple_ptr[4] != BERT_SMALL_INT)
		{
			test_fail("bert_encoder_push did not encode a SMALL_INT as the key, at list index %u",i);
		}

		if (tuple_ptr[5] != (i + 2))
		{
			test_fail("bert_encoder_push encoded %u as the value at list index %u, expected %u",tuple_ptr[5],i,i+2);
		}
	}
}

int main()
{
	bert_encoder_t *encoder = test_encoder(output,OUTPUT_SIZE);
	bert_data_t *data;

	if (!(data = bert_data_create_dict()))
	{
		test_fail("malloc failed");
	}

	bert_data_t *key;
	bert_data_t *value;
	unsigned int i;

	for (i=0;i<2;i++)
	{
		if (!(key = bert_data_create_int(i+1)))
		{
			test_fail("malloc failed");
		}

		if (!(value = bert_data_create_int(i+2)))
		{
			test_fail("malloc failed");
		}

		bert_dict_append(data->dict,key,value);
	}

	test_encoder_push(encoder,data);

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);

	test_output();
	return 0;
}
