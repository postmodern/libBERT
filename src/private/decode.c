#include "decode.h"
#include "decoder.h"
#include "regex.h"

#include <bert/magic.h>
#include <bert/util.h>
#include <bert/errno.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int bert_decode_uint8(bert_decoder_t *decoder,uint8_t *i)
{
	BERT_DECODER_READ(decoder,1);

	*i = bert_read_uint8(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,1);
	return BERT_SUCCESS;
}

int bert_decode_uint16(bert_decoder_t *decoder,uint16_t *i)
{
	BERT_DECODER_READ(decoder,2);

	*i = bert_read_uint16(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,2);
	return BERT_SUCCESS;
}

int bert_decode_uint32(bert_decoder_t *decoder,uint32_t *i)
{
	BERT_DECODER_READ(decoder,4);

	*i = bert_read_uint32(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,4);
	return BERT_SUCCESS;
}

int bert_decode_magic(bert_decoder_t *decoder,bert_magic_t *magic)
{
	BERT_DECODER_READ(decoder,1);

	*magic = bert_read_magic(BERT_DECODER_PTR(decoder));

	BERT_DECODER_STEP(decoder,1);
	return BERT_SUCCESS;
}

int bert_decode_bytes(unsigned char *dest,bert_decoder_t *decoder,size_t length)
{
	unsigned int index = 0;
	size_t chunk_length;

	while (index < length)
	{
		chunk_length = MIN((BERT_SHORT_BUFFER / 2), (length - index));

		BERT_DECODER_READ(decoder,chunk_length);

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
	int result;
	uint8_t i;

	if ((result = bert_decode_uint8(decoder,&i)) != BERT_SUCCESS)
	{
		return result;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_int(i)))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_big_int(bert_decoder_t *decoder,bert_data_t **data)
{
	int result;
	uint32_t i;

	if ((result = bert_decode_uint32(decoder,&i)) != BERT_SUCCESS)
	{
		return result;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_int(i)))
	{
		return BERT_ERRNO_MALLOC;
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_float(bert_decoder_t *decoder,bert_data_t **data)
{
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
	if (size > sizeof(uint64_t))
	{
		return BERT_ERRNO_BIGNUM;
	}

	int result;
	uint8_t sign;

	if ((result = bert_decode_uint8(decoder,&sign)) != BERT_SUCCESS)
	{
		return result;
	}

	unsigned char bytes[size];

	if ((result = bert_decode_bytes(bytes,decoder,size)) != BERT_SUCCESS)
	{
		return result;
	}

	unsigned int i;
	uint64_t unsigned_integer = 0;

	for (i=0;i<size;i++)
	{
		unsigned_integer |= (((uint64_t)bytes[i]) << (i * 8));
	}

	int64_t signed_integer = BERT_STRIP_SIGN((int64_t)unsigned_integer);

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
	int result;
	uint8_t i;

	if ((result = bert_decode_uint8(decoder,&i)) != BERT_SUCCESS)
	{
		return result;
	}

	return bert_decode_bignum(decoder,data,i);
}

inline int bert_decode_big_bignum(bert_decoder_t *decoder,bert_data_t **data)
{
	int result;
	uint32_t i;

	if ((result = bert_decode_uint32(decoder,&i)) != BERT_SUCCESS)
	{
		return result;
	}

	return bert_decode_bignum(decoder,data,i);
}

inline int bert_decode_string(bert_decoder_t *decoder,bert_data_t **data)
{
	int result;
	bert_string_size_t size;

	if ((result = bert_decode_uint32(decoder,&size)) != BERT_SUCCESS)
	{
		return result;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_empty_string(size)))
	{
		return BERT_ERRNO_MALLOC;
	}

	if (size)
	{
		int result;

		if ((result = bert_decode_bytes((unsigned char *)(new_data->string.text),decoder,size)) != BERT_SUCCESS)
		{
			bert_data_destroy(new_data);
			return result;
		}
	}

	*data = new_data;
	return BERT_SUCCESS;
}

int bert_decode_time(bert_decoder_t *decoder,bert_data_t **data)
{
	bert_data_t *megaseconds;
	int result;
	
	if ((result = bert_decoder_pull(decoder,&megaseconds)) != 1)
	{
		bert_data_destroy(megaseconds);
		return result;
	}

	if (megaseconds->type != bert_data_int)
	{
		goto cleanup_megaseconds;
	}

	bert_data_t *seconds;

	if ((result = bert_decoder_pull(decoder,&seconds)) != 1)
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

	if ((result = bert_decoder_pull(decoder,&microseconds)) != 1)
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

	if ((result = bert_decoder_pull(decoder,&list_data)) != 1)
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

			if (next_tuple->tuple->length != 2)
			{
				// the tuple must have two elements
				bert_data_destroy(new_data);
				bert_data_destroy(list_data);
				return BERT_ERRNO_INVALID;
			}

			key_data = next_tuple->tuple->elements[0];
			value_data = next_tuple->tuple->elements[1];

			// hackish but removes the need to do a deep copy
			next_tuple->tuple->elements[0] = NULL;
			next_tuple->tuple->elements[1] = NULL;

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

	if ((result = bert_decoder_pull(decoder,&source)) != 1)
	{
		return result;
	}

	if (source->type != bert_data_bin)
	{
		bert_data_destroy(source);
		return BERT_ERRNO_INVALID;
	}

	bert_data_t *opt_list;

	if ((result = bert_decoder_pull(decoder,&opt_list)) != 1)
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
				if (next_opt->tuple->length != 2)
				{
					goto cleanup;
				}

				tuple_args = next_opt->tuple->elements;

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

	if ((result = bert_decoder_pull(decoder,&keyword)) != 1)
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
	int result;
	bert_atom_size_t size;

	if ((result = bert_decode_uint16(decoder,&size)) != BERT_SUCCESS)
	{
		return result;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_empty_atom(size)))
	{
		return BERT_ERRNO_MALLOC;
	}

	if (size)
	{
		int result;

		if ((result = bert_decode_bytes((unsigned char *)(new_data->atom.name),decoder,size)) != BERT_SUCCESS)
		{
			bert_data_destroy(new_data);
			return result;
		}
	}

	*data = new_data;
	return BERT_SUCCESS;
}

inline int bert_decode_bin(bert_decoder_t *decoder,bert_data_t **data)
{
	int result;
	bert_bin_size_t size;

	if ((result = bert_decode_uint32(decoder,&size)) != BERT_SUCCESS)
	{
		return result;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_empty_bin(size)))
	{
		return BERT_ERRNO_MALLOC;
	}

	if (size)
	{
		int result;

		if ((result = bert_decode_bytes(new_data->bin.data,decoder,size)) != BERT_SUCCESS)
		{
			bert_data_destroy(new_data);
			return result;
		}
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

		if ((result = bert_decoder_pull(decoder,&first)) != 1)
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

		new_data->tuple->elements[0] = first;

		unsigned int i;

		for (i=1;i<size;i++)
		{
			if ((result = bert_decoder_pull(decoder,new_data->tuple->elements+i)) != 1)
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
	int result;
	uint8_t size;

	if ((result = bert_decode_uint8(decoder,&size)) != BERT_SUCCESS)
	{
		return result;
	}

	return bert_decode_tuple(decoder,data,size);
}

inline int bert_decode_large_tuple(bert_decoder_t *decoder,bert_data_t **data)
{
	int result;
	uint32_t size;

	if ((result = bert_decode_uint32(decoder,&size)) != BERT_SUCCESS)
	{
		return result;
	}

	return bert_decode_tuple(decoder,data,size);
}

int bert_decode_list(bert_decoder_t *decoder,bert_data_t **data)
{
	int result;
	bert_list_size_t size;

	if ((result = bert_decode_uint32(decoder,&size)) != BERT_SUCCESS)
	{
		return result;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create_list()))
	{
		return BERT_ERRNO_MALLOC;
	}

	bert_data_t *element;
	unsigned int i;

	for (i=0;i<size;i++)
	{
		if ((result = bert_decoder_pull(decoder,&element)) != 1)
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

	BERT_DECODER_READ(decoder,1);
	BERT_DECODER_STEP(decoder,1);

	*data = new_data;
	return BERT_SUCCESS;
}
