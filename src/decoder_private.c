#include "decoder_private.h"
#include <bert/types.h>
#include <bert/util.h>
#include <bert/errno.h>

#include <string.h>

inline uint8_t bert_decode_uint8(bert_decoder_t *decoder)
{
	uint8_t i;

	i = bert_read_uint8(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,1);
	return i;
}

inline int8_t bert_decode_int8(bert_decoder_t *decoder)
{
	int8_t i;

	i = bert_read_int8(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,1);
	return i;
}

inline uint16_t bert_decode_uint16(bert_decoder_t *decoder)
{
	uint16_t i = bert_read_uint16(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,2);
	return i;
}

inline uint32_t bert_decode_uint32(bert_decoder_t *decoder)
{
	uint32_t i = bert_read_uint32(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,4);
	return i;
}

inline int32_t bert_decode_int32(bert_decoder_t *decoder)
{
	int32_t i = bert_read_int32(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,4);
	return i;
}

inline bert_magic_t bert_decode_magic(bert_decoder_t *decoder)
{
	bert_magic_t m = bert_read_magic(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,1);
	return m;
}

inline int bert_decode_bytes(unsigned char *dest,bert_decoder_t *decoder,size_t length)
{
	unsigned int i;
	size_t chunk_length;

	for (i=0;i<length;i+=(BERT_SHORT_BUFFER / 2))
	{
		chunk_length = MIN((BERT_SHORT_BUFFER / 2), (length - i));
		BERT_DECODER_PULL(decoder,chunk_length);

		memcpy(dest+i,BERT_DECODER_PTR(decoder),sizeof(unsigned char)*chunk_length);
	}

	return BERT_SUCCESS;
}

int bert_decoder_pull(bert_decoder_t *decoder,size_t size)
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

fill_short_buffer:
	length = bert_buffer_read(decoder->short_buffer+decoder->short_length,&(decoder->buffer),empty_space);

	decoder->short_length += length;

	if ((decoder->short_length - decoder->short_index) < size)
	{
		return BERT_ERRNO_EMPTY;
	}
	return BERT_SUCCESS;
}
