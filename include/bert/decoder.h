#ifndef _BERT_DECODER_H_
#define _BERT_DECODER_H_

#include <bert/data.h>

#include <sys/types.h>

struct bert_decoder
{
	const unsigned char *buffer;
	size_t length;

	unsigned int index;
	const unsigned char *ptr;
};
typedef struct bert_decoder bert_decoder_t;

extern void bert_decode_init(bert_decoder_t *decoder,const unsigned char *buffer,size_t length);
extern int bert_decode_data(bert_decoder_t *decoder,bert_data_t **data_ptr);

#endif
