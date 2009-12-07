#include <bert/buffer.h>

#include <malloc.h>
#include <string.h>

bert_buffer_t * bert_buffer_create()
{
	bert_buffer_t *new_buffer;

	if (!(new_buffer = malloc(sizeof(bert_buffer_t))))
	{
		return NULL;
	}

	new_buffer->chunk_length = 0;
	memset(new_buffer->chunk,0,sizeof(unsigned char)*BERT_BUFFER_CHUNK);

	new_buffer->prev = NULL;
	new_buffer->next = NULL;
	return new_buffer;
}

bert_buffer_t * bert_buffer_extend(bert_buffer_t *buffer,size_t length)
{
	unsigned int remaining = (BERT_BUFFER_CHUNK - buffer->chunk_length);

	if (length <= remaining)
	{
		// there is still room left in the buffer's chunk
		return buffer;
	}

	// adjust length for space left in the buffer's chunk
	unsigned int adjusted_length = (length - remaining);
	unsigned int chunks = (even_length / BERT_BUFFER_CHUNK);

	if (adjusted_length % BERT_BUFFER_CHUNK)
	{
		// add a chunk for any length left over
		++chunks;
	}

	bert_buffer_t *last_buffer = buffer;
	bert_buffer_t *new_buffer;
	unsigned int i;

	for (i=0;i<chunks;i++)
	{
		if (!(new_buffer = bert_buffer_create()))
		{
			last_buffer = buffer->next;

			// cleanup our new buffers
			while (last_buffer)
			{
				free(last_buffer);
				last_buffer = last_buffer->next;
			}

			// malloc failed
			return NULL;
		}

		last_buffer->next = new_buffer;
		new_buffer->prev = last_buffer;
		last_buffer = new_buffer;
	}

	return new_buffer;
}

void bert_buffer_destroy(bert_buffer_t *buffer)
{
	bert_buffer_t *prev = buffer;
	bert_buffer_t *last = NULL;

	while (prev)
	{
		last = prev;
		prev = prev->prev;

		free(last);
	}
}
