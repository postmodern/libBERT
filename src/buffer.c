#include <bert/buffer.h>
#include <bert/errno.h>

#include <malloc.h>
#include <string.h>
#include <math.h>

bert_chunk_t * bert_chunk_create()
{
	bert_chunk_t *new_chunk;

	if (!(new_chunk = malloc(sizeof(bert_chunk_t))))
	{
		// malloc failed
		return NULL;
	}

	new_chunk->length = 0;
	memset(new_chunk->data,0,sizeof(unsigned char)*BERT_CHUNK_SIZE);

	new_chunk->next = NULL;
	new_chunk->prev = NULL;
	return new_chunk;
}

void bert_chunk_destroy(bert_chunk_t *chunk)
{
	free(chunk);
}

void bert_buffer_init(bert_buffer_t *buffer)
{
	buffer->head = NULL;
	buffer->tail = NULL;
}

size_t bert_buffer_length(const bert_buffer_t *buffer)
{
	if (!BERT_BUFFER_EMPTY(buffer))
	{
		return 0;
	}

	const bert_chunk_t *next_chunk = buffer->head;
	size_t s;

	while (next_chunk)
	{
		s += next_chunk->length;
		next_chunk = next_chunk->next;
	}

	return s;
}

int bert_buffer_write(bert_buffer_t *buffer,const unsigned char *data,size_t length)
{
	if (!(buffer->tail))
	{
		bert_chunk_t *new_chunk;
		
		if (!(new_chunk = bert_chunk_create()))
		{
			return BERT_ERRNO_MALLOC;
		}

		buffer->head = new_chunk;
		buffer->tail = new_chunk;
	}

	bert_chunk_t *start_chunk = buffer->tail;
	bert_chunk_t *last_chunk = start_chunk;
	bert_chunk_t *next_chunk;

	size_t remaining = BERT_CHUNK_SPACE(start_chunk);
	unsigned int chunks = (int)ceil((length - remaining) / (double)BERT_CHUNK_SIZE);

	unsigned int i;

	for (i=0;i<chunks;i++)
	{
		if (!(next_chunk = bert_chunk_create()))
		{
			next_chunk = start_chunk->next;

			// cleanup our new buffer chunks
			while (next_chunk)
			{
				bert_chunk_destroy(next_chunk);
				next_chunk = next_chunk->next;
			}

			// malloc failed
			return BERT_ERRNO_MALLOC;
		}

		last_chunk->next = next_chunk;
		next_chunk->prev = last_chunk;
		last_chunk = next_chunk;
	}

	next_chunk = start_chunk;
	i = 0;

	while (next_chunk && (i < length))
	{
		remaining = BERT_CHUNK_SPACE(next_chunk);
		memcpy(BERT_CHUNK_PTR(next_chunk),data+i,sizeof(unsigned char) * remaining);

		next_chunk = next_chunk->next;
		i += remaining;
	}

	return BERT_SUCCESS;
}

void bert_buffer_clear(bert_buffer_t *buffer)
{
	bert_chunk_t *next_chunk = buffer->head;
	bert_chunk_t *last_chunk;

	while (next_chunk)
	{
		last_chunk = next_chunk;

		bert_chunk_destroy(next_chunk);
		next_chunk = last_chunk->next;
	}

	buffer->head = NULL;
	buffer->tail = NULL;
}
