#include <bert/encoder.h>
#include <bert/magic.h>
#include <bert/util.h>
#include <bert/errno.h>
#include "private/encode.h"

#include <malloc.h>

bert_encoder_t * bert_encoder_create()
{
	bert_encoder_t *new_encoder;

	if (!(new_encoder = malloc(sizeof(bert_encoder_t))))
	{
		// malloc failed
		return NULL;
	}

	new_encoder->mode = bert_mode_none;
	new_encoder->wrote_magic = 0;

	return new_encoder;
}

void bert_encoder_stream(bert_encoder_t *encoder,int stream)
{
	encoder->mode = bert_mode_stream;
	encoder->stream = stream;
}

void bert_encoder_buffer(bert_encoder_t *encoder,unsigned char *buffer,size_t length)
{
	encoder->mode = bert_mode_buffer;
	encoder->buffer.ptr = buffer;
	encoder->buffer.length = length;
}

void bert_encoder_callback(bert_encoder_t *encoder,bert_write_func callback,void *data)
{
	encoder->mode = bert_mode_callback;
	encoder->callback.ptr = callback;
	encoder->callback.data = data;
}

int bert_encoder_push(bert_encoder_t *encoder,const bert_data_t *data)
{
	if (!(encoder->wrote_magic))
	{
		int result;

		if ((result = bert_encode_magic(encoder,BERT_MAGIC)) != BERT_SUCCESS)
		{
			return result;
		}

		encoder->wrote_magic = 1;
	}

	switch (data->type)
	{
		case bert_data_int:
			if (data->integer <= BERT_MAX_INT && data->integer >= BERT_MIN_INT)
			{
				return bert_encode_int(encoder,data->integer);
			}
			else
			{
				return bert_encode_bignum(encoder,data->integer);
			}
		case bert_data_float:
			return bert_encode_float(encoder,data->floating_point);
		case bert_data_atom:
			return bert_encode_atom(encoder,data->atom.name,data->atom.length);
		case bert_data_string:
			return bert_encode_string(encoder,data->string.text,data->string.length);
		case bert_data_bin:
			return bert_encode_bin(encoder,data->bin.data,data->bin.length);
		case bert_data_tuple:
			return bert_encode_tuple(encoder,(const bert_data_t **)(data->tuple.elements),data->tuple.length);
		case bert_data_list:
			return bert_encode_list(encoder,data->list);
		case bert_data_nil:
			return bert_encode_nil(encoder);
		case bert_data_boolean:
			return bert_encode_boolean(encoder,data->boolean);
		case bert_data_dict:
			return bert_encode_dict(encoder,data->dict);
		case bert_data_regex:
			return bert_encode_regex(encoder,data->regex.source,data->regex.length,data->regex.options);
		case bert_data_time:
			return bert_encode_time(encoder,data->time);
		default:
			return BERT_ERRNO_INVALID;
	}

	return BERT_SUCCESS;
}

void bert_encoder_destroy(bert_encoder_t *encoder)
{
	free(encoder);
}
