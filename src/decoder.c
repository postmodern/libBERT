#include <bert/decoder.h>
#include <bert/util.h>
#include <bert/dict.h>
#include <bert/magic.h>
#include <bert/errno.h>
#include <time.h>
#include <malloc.h>
#include <string.h>

#include "decoder_private.h"

bert_decoder_t * bert_decoder_stream(int fd)
{
	bert_decoder_t *new_decoder;

	if (!(new_decoder = bert_decoder_create()))
	{
		return NULL;
	}

	new_decoder->mode = bert_decoder_streaming;
	new_decoder->fd = fd;
	return new_decoder;
}

bert_decoder_t * bert_decoder_buffer()
{
	bert_decoder_t *new_decoder;

	if (!(new_decoder = bert_decoder_create()))
	{
		return NULL;
	}

	new_decoder->mode = bert_decoder_buffered;
	bert_buffer_init(&(new_decoder->buffer));
	return new_decoder;
}

int bert_decoder_empty(const bert_decoder_t *decoder)
{
	if ((decoder->short_length - decoder->short_index) > 0)
	{
		return 0;
	}
	    
	switch (decoder->mode)
	{
		case bert_decoder_buffered:
			return bert_buffer_empty(&(decoder->buffer));
		case bert_decoder_streaming:
		default:
			return 1;
	}
}

int bert_decoder_push(bert_decoder_t *decoder,const unsigned char *data,size_t length)
{
	if (decoder->mode != bert_decoder_buffered)
	{
		return BERT_ERRNO_INVALID;
	}

	return bert_buffer_write(&(decoder->buffer),data,length);
}

int bert_decoder_next(bert_decoder_t *decoder,bert_data_t **data)
{
	int result;
	
	switch ((result = bert_decoder_pull(decoder,1)))
	{
		case BERT_SUCCESS:
			break;
		case BERT_ERRNO_EMPTY:
			return 0;
		default:
			return result;
	}

	bert_magic_t magic = bert_decode_magic(decoder);

	// skip the BERT MAGIC start byte
	if (magic == BERT_MAGIC)
	{
		BERT_DECODER_PULL(decoder,1);

		magic = bert_decode_magic(decoder);
	}

	// decode primative data first
	switch (magic)
	{
		case BERT_NIL:
			result = bert_decode_nil(decoder,data);
			break;
		case BERT_SMALL_INT:
			result = bert_decode_small_int(decoder,data);
			break;
		case BERT_INT:
			result = bert_decode_big_int(decoder,data);
			break;
		case BERT_SMALL_BIGNUM:
			result = bert_decode_small_bignum(decoder,data);
			break;
		case BERT_LARGE_BIGNUM:
			result = bert_decode_big_bignum(decoder,data);
			break;
		case BERT_FLOAT:
			result = bert_decode_float(decoder,data);
			break;
		case BERT_ATOM:
			result = bert_decode_atom(decoder,data);
			break;
		case BERT_STRING:
			result = bert_decode_string(decoder,data);
			break;
		case BERT_BIN:
			result = bert_decode_bin(decoder,data);
			break;
		case BERT_SMALL_TUPLE:
			result = bert_decode_small_tuple(decoder,data);
			break;
		case BERT_LARGE_TUPLE:
			result = bert_decode_large_tuple(decoder,data);
			break;
		case BERT_LIST:
			result = bert_decode_list(decoder,data);
			break;
		default:
			return BERT_ERRNO_INVALID;
	}

	if (result != BERT_SUCCESS)
	{
		return result;
	}

	return 1;
}

void bert_decoder_destroy(bert_decoder_t *decoder)
{
	if (decoder->mode == bert_decoder_buffered)
	{
		bert_buffer_clear(&(decoder->buffer));
	}

	free(decoder);
}
