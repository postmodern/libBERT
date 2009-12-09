#include <bert/buffer.h>
#include <bert/util.h>
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
	return new_chunk;
}

inline void bert_chunk_destroy(bert_chunk_t *chunk)
{
	free(chunk);
}

void bert_buffer_init(bert_buffer_t *buffer)
{
	buffer->head = NULL;
	buffer->tail = NULL;
}

int bert_buffer_empty(const bert_buffer_t *buffer)
{
	return (!(buffer->head) || (buffer->head->length == 0));
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
	size_t remaining = BERT_CHUNK_SPACE(start_chunk);

	if (length <= remaining)
	{
		memcpy(start_chunk->data+start_chunk->length,data,sizeof(unsigned char)*length);
		start_chunk->length += length;
		return BERT_SUCCESS;
	}

	unsigned int chunks = (int)ceil((length - remaining) / (double)BERT_CHUNK_SIZE);
	bert_chunk_t *last_chunk = start_chunk;
	bert_chunk_t *next_chunk;
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

ssize_t bert_buffer_read(unsigned char *dest,bert_buffer_t *buffer,size_t length)
{
	bert_chunk_t *next_chunk = buffer->head;
	bert_chunk_t *last_chunk;

	if (!next_chunk)
	{
		// no data in the buffer
		return 0;
	}

	// set the index
	size_t index = buffer->index;
	size_t num_read = 0;
	size_t chunk_length;

	do
	{
		// calculate the minimum length to be read
		chunk_length = MIN((next_chunk->length - index), (length - num_read));
		memcpy(dest+num_read,next_chunk->data+index,sizeof(unsigned char)*chunk_length);

		// update the index
		num_read += chunk_length;
		index = ((num_read + buffer->index) % BERT_CHUNK_SIZE);

		// get the next chunk
		last_chunk = next_chunk;
		next_chunk = next_chunk->next;

		// destroy the last chunk
		bert_chunk_destroy(last_chunk);
	} while (next_chunk && (num_read < length));

	// reset the head and index
	if (!(buffer->head = next_chunk))
	{
		buffer->tail = NULL;
	}
	buffer->index = index;

	return num_read;
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

	bert_buffer_init(buffer);
}
