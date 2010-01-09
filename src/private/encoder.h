#ifndef _BERT_PRIVATE_ENCODER_H_
#define _BERT_PRIVATE_ENCODER_H_

#include <bert/encoder.h>

struct bert_encoder
{
	bert_mode mode;
	unsigned int wrote_magic;
	size_t total;

	union
	{
		int stream;

		struct
		{
			unsigned char *ptr;
			size_t length;

			unsigned int index;
		} buffer;

		struct
		{
			bert_write_func ptr;
			void *data;
		} callback;
	};
};

int bert_encoder_write(bert_encoder_t *encoder,const unsigned char *data,size_t length);

#endif
