#ifndef _BERT_DECODER_PRIVATE_H_
#define _BERT_DECODER_PRIVATE_H_

#include <bert/decoder.h>

#define BERT_DECODER_EMPTY(decoder)	(BERT_SHORT_BUFFER - decoder->short_length)
#define BERT_DECODER_STEP(decoder,i)	(decoder->short_index += i)
#define BERT_DECODER_PTR(decoder)	(decoder->short_buffer + decoder->short_index)
#define BERT_DECODER_READ(decoder,i)	switch (bert_decoder_read(decoder,i)) { \
						case BERT_ERRNO_EMPTY: \
						case BERT_ERRNO_SHORT: \
							return BERT_ERRNO_SHORT; \
						case BERT_ERRNO_READ: \
							return BERT_ERRNO_READ; \
						case BERT_ERRNO_INVALID: \
							return BERT_ERRNO_INVALID; \
					}

int bert_decoder_read(bert_decoder_t *decoder,size_t size);

#endif
