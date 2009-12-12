#include "decoder.h"
#include "regex.h"

#include <bert/magic.h>
#include <bert/util.h>
#include <bert/errno.h>

#include <unistd.h>
#include <string.h>

int bert_decoder_read(bert_decoder_t *decoder,size_t size)
{
	size_t remaining_space = (decoder->short_length - decoder->short_index);

	if (size <= remaining_space)
	{
		// we still have enough data in the short buffer
		return BERT_SUCCESS;
	}

	size_t empty_space = BERT_DECODER_EMPTY(decoder);

	if (empty_space >= (BERT_SHORT_BUFFER / 2))
	{
		// fill the remaining space in the short buffer
		goto fill_short_buffer;
	}

	size_t unread_space = (decoder->short_length - decoder->short_index);

	if (unread_space)
	{
		// shift the other half of the short buffer down
		memcpy(decoder->short_buffer,decoder->short_buffer+decoder->short_index,sizeof(unsigned char)*unread_space);
	}

	decoder->short_length = unread_space;
	decoder->short_index = 0;
	empty_space = BERT_DECODER_EMPTY(decoder);

	ssize_t length;
	unsigned char *short_ptr;

fill_short_buffer:
	short_ptr = (decoder->short_buffer + decoder->short_length);

	switch (decoder->mode)
	{
		case bert_mode_stream:
			length = read(decoder->stream,short_ptr,sizeof(unsigned char)*empty_space);

			if (length < 0)
			{
				return BERT_ERRNO_READ;
			}
			break;
		case bert_mode_buffer:
			length = MIN((decoder->buffer.length - decoder->buffer.index),empty_space);

			memcpy(short_ptr,decoder->buffer.ptr,length);
			decoder->buffer.index += length;
			break;
		case bert_mode_callback:
			length = decoder->callback.ptr(short_ptr,empty_space,decoder->callback.data);

			if (length < 0)
			{
				return BERT_ERRNO_INVALID;
			}
			break;
		default:
			return BERT_ERRNO_INVALID;
	}

	if (!(length || remaining_space))
	{
		// no more data to read and the short buffer is empty
		return BERT_ERRNO_EMPTY;
	}

	decoder->short_length += length;

	if ((decoder->short_length - decoder->short_index) < size)
	{
		return BERT_ERRNO_SHORT;
	}
	return BERT_SUCCESS;
}
