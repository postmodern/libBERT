#include <bert/decoder.h>
#include <bert/util.h>
#include <bert/dict.h>
#include <bert/magic.h>
#include <bert/errno.h>
#include <time.h>
#include <malloc.h>
#include <string.h>

#include "private/decoder.h"
#include "private/decode.h"

bert_decoder_t * bert_decoder_create()
{
	bert_decoder_t *new_decoder;

	if (!(new_decoder = malloc(sizeof(bert_decoder_t))))
	{
		// malloc failed
		return NULL;
	}

	new_decoder->mode = bert_mode_none;

	new_decoder->short_length = 0;
	new_decoder->short_index = 0;

	memset(new_decoder->short_buffer,0,sizeof(unsigned char)*BERT_SHORT_BUFFER);
	return new_decoder;
}

void bert_decoder_stream(bert_decoder_t *decoder,int fd)
{
	decoder->mode = bert_mode_stream;
	decoder->stream = fd;
}

void bert_decoder_callback(bert_decoder_t *decoder,bert_read_func callback,void *data)
{
	decoder->mode = bert_mode_callback;
	decoder->callback.ptr = callback;
	decoder->callback.data = data;
}

void bert_decoder_buffer(bert_decoder_t *decoder,const unsigned char *buffer,size_t length)
{
	decoder->mode = bert_mode_buffer;
	decoder->buffer.ptr = buffer;
	decoder->buffer.length = length;
	decoder->buffer.index = 0;
}

int bert_decoder_next(bert_decoder_t *decoder,bert_data_t **data)
{
	int result;
	
	switch ((result = bert_decoder_read(decoder,1)))
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
		BERT_DECODER_READ(decoder,1);

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
	free(decoder);
}
