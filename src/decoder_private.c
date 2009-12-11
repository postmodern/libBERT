#include "decoder_private.h"
#include "regex_private.h"
#include <bert/magic.h>
#include <bert/util.h>
#include <bert/errno.h>

#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

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
	unsigned int index = 0;
	size_t chunk_length;

	while (index < length)
	{
		chunk_length = MIN((BERT_SHORT_BUFFER / 2), (length - index));

		BERT_DECODER_PULL(decoder,chunk_length);

		memcpy(dest+index,BERT_DECODER_PTR(decoder),sizeof(unsigned char)*chunk_length);
		index += chunk_length;

		BERT_DECODER_STEP(decoder,chunk_length);
	}

	return BERT_SUCCESS;
}

inline int bert_decode_nil(bert_decoder_t *decoder,bert_data_t **data)
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create_nil()))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_small_int(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,1);

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_int(bert_decode_uint8(decoder))))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_big_int(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,4);

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_int(bert_decode_uint32(decoder))))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_float(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,31);

	char float_buffer[32];
	int result;

	memset(float_buffer,'\0',sizeof(unsigned char)*32);

	if ((result = bert_decode_bytes((unsigned char *)float_buffer,decoder,31)) != BERT_SUCCESS)
	{
		return result;
	}

	double floating_point;

	if (sscanf(float_buffer,"%lf",&floating_point) != 1)
	{
		return BERT_ERRNO_INVALID;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_float(floating_point)))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

int bert_decode_bignum(bert_decoder_t *decoder,bert_data_t **data,size_t size)
{
	BERT_DECODER_PULL(decoder,1 + size);

	uint8_t sign = bert_decode_uint8(decoder);

	const unsigned char *ptr = BERT_DECODER_PTR(decoder);

	uint32_t unsigned_integer = 0;
	uint8_t b;
	uint8_t i;

	for (i=0;i<size;i++)
	{
		b = (uint8_t)(ptr[i]);
		unsigned_integer = ((unsigned_integer << 8) | b);
	}

	BERT_DECODER_STEP(decoder,size);

	int32_t signed_integer = (int32_t)unsigned_integer;

	if (sign)
	{
		signed_integer = -(signed_integer);
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_int(signed_integer)))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_small_bignum(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,1);

	return bert_decode_bignum(decoder,data,bert_decode_uint8(decoder));
}

inline int bert_decode_big_bignum(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,4);

	return bert_decode_bignum(decoder,data,bert_decode_uint32(decoder));
}

inline int bert_decode_string(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,4);

	bert_string_size_t size = bert_decode_uint32(decoder);

	char string_buffer[size];

	if (size)
	{
		int result;

		if ((result = bert_decode_bytes((unsigned char *)string_buffer,decoder,size)) != BERT_SUCCESS)
		{
			return result;
		}
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_string(string_buffer,size)))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

int bert_decode_time(bert_decoder_t *decoder,bert_data_t **data)
{
	bert_data_t *megaseconds;
	int result;
	
	if ((result = bert_decoder_next(decoder,&megaseconds)) != 1)
	{
		bert_data_destroy(megaseconds);
		return result;
	}

	if (megaseconds->type != bert_data_int)
	{
		goto cleanup_megaseconds;
	}

	bert_data_t *seconds;

	if ((result = bert_decoder_next(decoder,&seconds)) != 1)
	{
		bert_data_destroy(seconds);
		bert_data_destroy(megaseconds);
		return result;
	}

	if (seconds->type != bert_data_int)
	{
		goto cleanup_seconds;
	}

	bert_data_t *microseconds;

	if ((result = bert_decoder_next(decoder,&microseconds)) != 1)
	{
		bert_data_destroy(microseconds);
		bert_data_destroy(seconds);
		bert_data_destroy(megaseconds);
		return result;
	}

	if (seconds->type != bert_data_int)
	{
		goto cleanup_microseconds;
	}

	time_t timestamp = ((megaseconds->integer * 1000000) + seconds->integer + (microseconds->integer / 1000000));
	bert_data_t *new_data;

	bert_data_destroy(microseconds);
	bert_data_destroy(seconds);
	bert_data_destroy(megaseconds);

	if (!(new_data = bert_data_create_time(timestamp)))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;

cleanup_microseconds:
	bert_data_destroy(microseconds);
cleanup_seconds:
	bert_data_destroy(seconds);
cleanup_megaseconds:
	bert_data_destroy(megaseconds);
	return BERT_ERRNO_INVALID;
}

int bert_decode_dict(bert_decoder_t *decoder,bert_data_t **data)
{
	bert_data_t *list_data;
	int result;

	if ((result = bert_decoder_next(decoder,&list_data)) != 1)
	{
		return result;
	}

	if (list_data->type != bert_data_nil && list_data->type != bert_data_list)
	{
		// dicts terms must contain either a nil or a list
		bert_data_destroy(list_data);
		return BERT_ERRNO_INVALID;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_dict()))
	{
		bert_data_destroy(list_data);
		return BERT_ERRNO_MALLOC;
	}

	if (list_data->type == bert_data_list)
	{
		bert_list_node_t *next_node = list_data->list->head;
		bert_data_t *next_tuple;
		bert_data_t *key_data;
		bert_data_t *value_data;

		while (next_node)
		{
			next_tuple = next_node->data;

			if (next_tuple->type != bert_data_tuple)
			{
				// the list must contain tuples
				bert_data_destroy(new_data);
				bert_data_destroy(list_data);
				return BERT_ERRNO_INVALID;
			}

			if (next_tuple->tuple.length != 2)
			{
				// the tuple must have two elements
				bert_data_destroy(new_data);
				bert_data_destroy(list_data);
				return BERT_ERRNO_INVALID;
			}

			key_data = next_tuple->tuple.elements[0];
			value_data = next_tuple->tuple.elements[1];

			// hackish but removes the need to do a deep copy
			next_tuple->tuple.elements[0] = NULL;
			next_tuple->tuple.elements[1] = NULL;

			// append the key -> value pair to the dict
			if (bert_dict_append(new_data->dict,key_data,value_data) == BERT_ERRNO_MALLOC)
			{
				bert_data_destroy(value_data);
				bert_data_destroy(key_data);
				bert_data_destroy(new_data);
				bert_data_destroy(list_data);
				return BERT_ERRNO_MALLOC;
			}

			next_node = next_node->next;
		}
	}

	bert_data_destroy(list_data);

	*data = new_data;
	return BERT_SUCCESS;
}

int bert_decode_regex(bert_decoder_t *decoder,bert_data_t **data)
{
	bert_data_t *source;
	int result;

	if ((result = bert_decoder_next(decoder,&source)) != 1)
	{
		return result;
	}

	if (source->type != bert_data_bin)
	{
		bert_data_destroy(source);
		return BERT_ERRNO_INVALID;
	}

	bert_data_t *opt_list;

	if ((result = bert_decoder_next(decoder,&opt_list)) != 1)
	{
		bert_data_destroy(source);
		return result;
	}

	if (opt_list->type != bert_data_list)
	{
		goto cleanup;
	}

	bert_list_node_t *next_node = opt_list->list->head;
	bert_data_t *next_opt;
	bert_data_t **tuple_args;
	int options = 0;

	while (next_node)
	{
		next_opt = next_node->data;

		switch (next_opt->type)
		{
			case bert_data_atom:
				options |= bert_regex_optmask(next_opt->atom.name);
				break;
			case bert_data_tuple:
				if (next_opt->tuple.length != 2)
				{
					goto cleanup;
				}

				tuple_args = next_opt->tuple.elements;

				if (tuple_args[0]->type != bert_data_atom)
				{
					goto cleanup;
				}

				if (!bert_data_strequal(tuple_args[0],"newline"))
				{
					goto cleanup;
				}

				if (tuple_args[1]->type != bert_data_atom)
				{
					goto cleanup;
				}

				if (bert_data_strequal(tuple_args[1],"cr"))
				{
					options |= BERT_REGEX_NEWLINE_CR;
				}
				else if (bert_data_strequal(tuple_args[1],"cr"))
				{
					options |= BERT_REGEX_NEWLINE_LF;
				}
				else if (bert_data_strequal(tuple_args[1],"crlf"))
				{
					options |= BERT_REGEX_NEWLINE_CRLF;
				}
				else if (bert_data_strequal(tuple_args[1],"anycrlf"))
				{
					options |= BERT_REGEX_NEWLINE_ANYCRLF;
				}
				else if (bert_data_strequal(tuple_args[1],"any"))
				{
					options |= BERT_REGEX_NEWLINE_ANY;
				}
				else
				{
					goto cleanup;
				}
				break;
			default:
				goto cleanup;
		}

		next_node = next_node->next;
	}

	bert_data_destroy(opt_list);

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_regex((char *)source->bin.data,source->bin.length,options)))
	{
		bert_data_destroy(source);
		return BERT_ERRNO_MALLOC;
	}

	bert_data_destroy(source);

	*data = new_data;
	return BERT_SUCCESS;

cleanup:
	bert_data_destroy(opt_list);
	bert_data_destroy(source);
	return BERT_ERRNO_INVALID;
}

int bert_decode_complex(bert_decoder_t *decoder,bert_data_t **data)
{
	bert_data_t *keyword;
	int result;

	if ((result = bert_decoder_next(decoder,&keyword)) != 1)
	{
		return result;
	}

	if (keyword->type != bert_data_atom)
	{
		return BERT_ERRNO_INVALID;
	}

	bert_data_t *new_data = NULL;

	if (bert_data_strequal(keyword,"nil"))
	{
		new_data = bert_data_create_nil();
	}
	else if (bert_data_strequal(keyword,"true"))
	{
		new_data = bert_data_create_true();
	}
	else if (bert_data_strequal(keyword,"false"))
	{
		new_data = bert_data_create_false();
	}
	else if (bert_data_strequal(keyword,"time"))
	{
		bert_data_destroy(keyword);
		return bert_decode_time(decoder,data);
	}
	else if (bert_data_strequal(keyword,"dict"))
	{
		bert_data_destroy(keyword);
		return bert_decode_dict(decoder,data);
	}
	else if (bert_data_strequal(keyword,"regex"))
	{
		bert_data_destroy(keyword);
		return bert_decode_regex(decoder,data);
	}
	else
	{
		bert_data_destroy(keyword);
		return BERT_ERRNO_INVALID;
	}

	bert_data_destroy(keyword);

	if (!new_data)
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

int bert_decode_atom(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,2);

	bert_atom_size_t size = bert_decode_uint16(decoder);

	char atom_buffer[size];

	if (size)
	{
		int result;

		if ((result = bert_decode_bytes((unsigned char *)atom_buffer,decoder,size)) != BERT_SUCCESS)
		{
			return result;
		}
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_atom(atom_buffer,size)))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_bin(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,4);

	bert_bin_size_t size = bert_decode_uint32(decoder);

	unsigned char bin_buffer[size];

	if (size)
	{
		int result;

		if ((result = bert_decode_bytes(bin_buffer,decoder,size)) != BERT_SUCCESS)
		{
			return result;
		}
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_bin(bin_buffer,size)))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

int bert_decode_tuple(bert_decoder_t *decoder,bert_data_t **data,size_t size)
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create_tuple(size)))
	{
		return BERT_ERRNO_MALLOC;
	}

	if (size)
	{
		bert_data_t *first;
		int result;

		if ((result = bert_decoder_next(decoder,&first)) != 1)
		{
			bert_data_destroy(new_data);
			return result;
		}

		if ((first->type == bert_data_atom) && bert_data_strequal(first,"bert"))
		{
			bert_data_destroy(first);
			bert_data_destroy(new_data);
			return bert_decode_complex(decoder,data);
		}

		new_data->tuple.elements[0] = first;

		unsigned int i;

		for (i=1;i<size;i++)
		{
			if ((result = bert_decoder_next(decoder,new_data->tuple.elements+i)) != 1)
			{
				bert_data_destroy(new_data);
				return result;
			}
		}
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_small_tuple(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,1);

	return bert_decode_tuple(decoder,data,bert_decode_uint8(decoder));
}

inline int bert_decode_large_tuple(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,4);

	return bert_decode_tuple(decoder,data,bert_decode_uint32(decoder));
}

int bert_decode_list(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,4);

	bert_list_size_t list_size = bert_decode_uint32(decoder);
	bert_data_t *new_data;

	if (!(new_data = bert_data_create_list()))
	{
		return BERT_ERRNO_MALLOC;
	}

	bert_data_t *element;
	unsigned int i;
	int result;

	for (i=0;i<list_size;i++)
	{
		if ((result = bert_decoder_next(decoder,&element)) != 1)
		{
			bert_data_destroy(new_data);
			return result;
		}

		if (bert_list_append(new_data->list,element) == BERT_ERRNO_MALLOC)
		{
			bert_data_destroy(element);
			bert_data_destroy(new_data);
			return BERT_ERRNO_MALLOC;
		}
	}

	BERT_DECODER_PULL(decoder,1);
	BERT_DECODER_STEP(decoder,1);

	*data = new_data;
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
