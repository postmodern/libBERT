#ifndef _BERT_BUFFER_H_
#define _BERT_BUFFER_H_

#include <sys/types.h>

#define BERT_BUFFER_CHUNK	256

struct bert_buffer
{
	size_t chunk_length;
	unsigned char chunk[BERT_BUFFER_CHUNK];

	struct bert_buffer *prev;
	struct bert_buffer *next;
};
typedef struct bert_buffer bert_buffer_t;

#define BERT_BUFFER_LEFT(buffer)	(BERT_BUFFER_CHUNK - buffer->chunk_length)

bert_buffer_t * bert_buffer_create();
bert_buffer_t * bert_buffer_extend(bert_buffer_t *buffer,size_t length);
bert_buffer_t * bert_buffer_write(bert_buffer_t *buffer,const unsigned char *data,size_t length);
void bert_buffer_destroy(bert_buffer_t *buffer);

#endif
