#ifndef _BERT_DECODER_H_
#define _BERT_DECODER_H_

#include <bert/data.h>
#include <bert/buffer.h>

#include <sys/types.h>

#define BERT_SHORT_BUFFER	BERT_BUFFER_CHUNK

struct bert_decoder
{
	bert_buffer_t *buffer_head;
	bert_buffer_t *buffer_tail;

	unsigned int chunk_index;
	const unsigned char *buffer_ptr;

	unsigned char short_buffer[BERT_SHORT_BUFFER];
};
typedef struct bert_decoder bert_decoder_t;

extern void bert_decode_init(bert_decoder_t *decoder,const unsigned char *buffer,size_t length);
extern int bert_decode_data(bert_decoder_t *decoder,bert_data_t **data_ptr);

#endif
