#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/errno.h>

#include "test.h"

unsigned char output[2];

int main()
{
	bert_encoder_t *encoder = test_encoder(output,2);
	bert_data_t *data;

	if (!(data = bert_data_create_int(0)))
	{
		test_fail("malloc failed");
	}

	int result;

	if ((result = bert_encoder_push(encoder,data)) != BERT_ERRNO_SHORT)
	{
		test_fail("bert_encoder_push returned %d, expected BERT_ERRNO_SHORT",result);
	}

	bert_data_destroy(data);
	bert_encoder_destroy(encoder);
	return 0;
}
