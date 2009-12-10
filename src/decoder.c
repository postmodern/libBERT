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
	return (((decoder->short_length - decoder->short_index) == 0) && bert_buffer_empty(&(decoder->buffer)));
}

int bert_decoder_push(bert_decoder_t *decoder,const unsigned char *data,size_t length)
{
	return bert_buffer_write(&(decoder->buffer),data,length);
}

int bert_decoder_next(bert_decoder_t *decoder,bert_data_t **data)
{
	if (bert_decoder_empty(decoder))
	{
		return 0;
	}

	BERT_DECODER_PULL(decoder,1);

	bert_magic_t magic = bert_decode_magic(decoder);

	// skip the BERT MAGIC start byte
	if (magic == BERT_MAGIC)
	{
		BERT_DECODER_PULL(decoder,1);

		magic = bert_decode_magic(decoder);
	}

	int result = 0;

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
			// TODO: implement float decoding
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
	bert_buffer_clear(&(decoder->buffer));
	free(decoder);
}
