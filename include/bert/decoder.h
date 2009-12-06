#ifndef _BERT_DECODER_H_
#define _BERT_DECODER_H_

#include <sys/types.h>

struct bert_decoder
{
	const unsigned char *buffer;
	size_t buffer_length;

	unsigned int index;
};
typedef bert_decoder_t bert_decoder;

void bert_decode_init(bert_decoder_t *decoder,const unsigned char *buffer,size_t buffer_length);

#endif
