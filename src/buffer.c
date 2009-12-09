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
	return (!(buffer->head) || (buffer->head->length > 0))
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

	if (!next_chunk)
	{
		// no data in the buffer
		return 0;
	}

	size_t total_length = (length + buffer->index);

	if (total_length <= next_chunk->length)
	{
		// copy the requested length from the first chunk
		memcpy(dest,next_chunk->data+buffer->index,sizeof(unsigned char)*length);
		buffer->index += length;

		if (buffer->index >= next_chunk->length)
		{
			// reset the buffer head and index
			if (!(buffer->head = next_chunk->next))
			{
				buffer->tail = NULL;
			}
			bert_chunk_destroy(next_chunk);

			buffer->index = 0;
		}

		return length;
	}

	if (bert_buffer_length(buffer) < total_length)
	{
		// not enough data in the buffer
		return -1;
	}

	// calculate the front, middle and end lengths
	size_t front_length = (next_chunk->length - buffer->index);
	size_t middle_length = (length - front_length);
	size_t end_length = (middle_length % BERT_CHUNK_SIZE);
	middle_length -= end_length;

	// copy the front data
	memcpy(dest,next_chunk->data+buffer->index,sizeof(unsigned char)*front_length);

	// next chunk
	bert_chunk_t *last_chunk = next_chunk;
	next_chunk = next_chunk->next;

	// free the read chunk
	bert_chunk_destroy(last_chunk);

	unsigned int middle_chunks = (middle_length / BERT_CHUNK_SIZE);
	unsigned char *dest_ptr = (dest+front_length);
	unsigned int i;

	// copy the middle chunks
	for (i=0;i<middle_chunks;i++)
	{
		// copy a whole middle chunk completely
		memcpy(dest_ptr,next_chunk->data,sizeof(unsigned char)*BERT_CHUNK_SIZE);
		dest_ptr += BERT_CHUNK_SIZE;

		// next chunk
		last_chunk = next_chunk;
		next_chunk = next_chunk->next;

		// free the read chunk
		bert_chunk_destroy(last_chunk);
	}

	if (end_length)
	{
		// copy the end data
		memcpy(dest_ptr,next_chunk->data,sizeof(unsigned char)*end_length);
	}

	// reset the head and index
	buffer->head = next_chunk;
	buffer->index = end_length;
	return length;
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
