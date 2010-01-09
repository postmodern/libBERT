#ifndef _BERT_PRIVATE_DECODER_H_
#define _BERT_PRIVATE_DECODER_H_

#include <bert/decoder.h>

#define BERT_DECODER_EMPTY(decoder)	(BERT_SHORT_BUFFER - decoder->short_length)
#define BERT_DECODER_STEP(decoder,i)	(decoder->short_index += i)
#define BERT_DECODER_PTR(decoder)	(decoder->short_buffer + decoder->short_index)
#define BERT_DECODER_READ(decoder,i)	switch (bert_decoder_read(decoder,i)) { \
						case BERT_ERRNO_EMPTY: \
						case BERT_ERRNO_SHORT_READ: \
							return BERT_ERRNO_SHORT_READ; \
						case BERT_ERRNO_READ: \
							return BERT_ERRNO_READ; \
						case BERT_ERRNO_INVALID: \
							return BERT_ERRNO_INVALID; \
					}

struct bert_decoder
{
	bert_mode mode;
	size_t total;

	union
	{
		int stream;

		struct
		{
			bert_read_func ptr;
			void *data;
		} callback;
		
		struct
		{
			const unsigned char *ptr;
			size_t length;

			unsigned int index;
		} buffer;
	};

	size_t short_length;
	unsigned int short_index;

	unsigned char short_buffer[BERT_SHORT_BUFFER];
};

int bert_decoder_read(bert_decoder_t *decoder,size_t size);

#endif
