#ifndef _BERT_BUFFER_H_
#define _BERT_BUFFER_H_

#include <sys/types.h>

#define BERT_CHUNK_SIZE	256

struct bert_chunk
{
	size_t length;
	unsigned char data[BERT_CHUNK_SIZE];

	struct bert_chunk *next;
};
typedef struct bert_chunk bert_chunk_t;

#define BERT_CHUNK_SPACE(chunk)	(BERT_CHUNK_SIZE - chunk->length)
#define BERT_CHUNK_PTR(chunk)	(chunk->data + chunk->length)

bert_chunk_t * bert_chunk_create();
inline void bert_chunk_destroy(bert_chunk_t *chunk);

struct bert_buffer
{
	bert_chunk_t *head;
	bert_chunk_t *tail;

	unsigned int index;
};
typedef struct bert_buffer bert_buffer_t;

#define BERT_BUFFER_EMPTY(buffer)	(!(buffer->head) || (buffer->head->length == 0))

inline void bert_buffer_init(bert_buffer_t *buffer);
size_t bert_buffer_length(const bert_buffer_t *buffer);
int bert_buffer_write(bert_buffer_t *buffer,const unsigned char *data,size_t length);
ssize_t bert_buffer_read(unsigned char *dest,bert_buffer_t *buffer,size_t length);
void bert_buffer_clear(bert_buffer_t *buffer);

#endif
