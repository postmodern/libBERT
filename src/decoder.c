#include <bert/decoder.h>
#include <bert/util.h>
#include <bert/dict.h>
#include <bert/magic.h>
#include <bert/errno.h>
#include <time.h>
#include <malloc.h>
#include <string.h>

#include "decoder_private.h"

bert_decoder_t * bert_decoder_create()
{
	bert_decoder_t *new_decoder;

	if (!(new_decoder = malloc(sizeof(bert_decoder_t))))
	{
		// malloc failed
		return NULL;
	}

	bert_buffer_init(&(new_decoder->buffer));

	new_decoder->short_length = 0;
	new_decoder->short_index = 0;
	return new_decoder;
}

int bert_decoder_push(bert_decoder_t *decoder,const unsigned char *data,size_t length)
{
	return bert_buffer_write(&(decoder->buffer),data,length);
}

void bert_decoder_destroy(bert_decoder_t *decoder)
{
	bert_buffer_clear(&(decoder->buffer));
	free(decoder);
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
	// 3 magic bytes and uint32s
	BERT_DECODER_PULL(decoder,3 * (1 + 4));

	// must be an integer
	if (bert_decode_magic(decoder) != BERT_INT)
	{
		return BERT_ERRNO_INVALID;
	}

	uint32_t megaseconds = bert_decode_uint32(decoder);

	// must be an integer
	if (bert_decode_magic(decoder) != BERT_INT)
	{
		return BERT_ERRNO_INVALID;
	}

	uint32_t seconds = bert_decode_uint32(decoder);

	// must be an integer
	if (bert_decode_magic(decoder) != BERT_INT)
	{
		return BERT_ERRNO_INVALID;
	}

	uint32_t microseconds = bert_decode_uint32(decoder);

	time_t timestamp = (megaseconds * 1000000) + seconds + (microseconds / 1000000);
	bert_data_t *new_data;

	if (!(new_data = bert_data_create_time(timestamp)))
	{
		return BERT_ERRNO_MALLOC;
	}

	return BERT_SUCCESS;
}

int bert_decode_dict(bert_decoder_t *decoder,bert_data_t **data)
{
	bert_data_t *list_data;
	int result;

	if ((result = bert_decoder_next(decoder,&list_data)) != 1)
	{
		return result;
	}

	// eat the nil byte
	bert_decode_uint8(decoder);

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

			if ((result = bert_decoder_next(decoder,&key_data)) != 1)
			{
				bert_data_destroy(new_data);
				bert_data_destroy(list_data);
				return result;
			}

			if ((result = bert_decoder_next(decoder,&value_data)) != 1)
			{
				bert_data_destroy(key_data);
				bert_data_destroy(new_data);
				bert_data_destroy(list_data);
				return result;
			}

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

int bert_decode_complex(bert_decoder_t *decoder,bert_data_t **data)
{
	BERT_DECODER_PULL(decoder,2);

	bert_atom_size_t size = bert_decode_uint16(decoder);

	BERT_DECODER_PULL(decoder,size);

	bert_data_t *new_data;
	const char *ptr = (const char *)BERT_DECODER_PTR(decoder);

	if ((size == 3) && (strncmp(ptr,"nil",3) == 0))
	{
		new_data = bert_data_create_nil();
	}
	else if ((size == 4) && (strncmp(ptr,"true",4) == 0))
	{
		new_data = bert_data_create_true();
	}
	else if ((size == 5) && (strncmp(ptr,"false",5) == 0))
	{
		new_data = bert_data_create_false();
	}
	else if ((size == 4) && (strncmp(ptr,"time",4) == 0))
	{
		return bert_decode_time(decoder,data);
	}
	else if ((size = 4) && (strncmp(ptr,"dict",4) == 0))
	{
		return bert_decode_dict(decoder,data);
	}
	else
	{
		return BERT_ERRNO_INVALID;
	}

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

	if ((size == 4) && (memcmp(atom_buffer,"bert",4) == 0))
	{
		return bert_decode_complex(decoder,data);
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

inline int bert_decode_tuple(bert_decoder_t *decoder,bert_data_t **data,size_t size)
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create_tuple(size)))
	{
		return BERT_ERRNO_MALLOC;
	}

	unsigned int i;
	int result;

	for (i=0;i<size;i++)
	{
		if ((result = bert_decoder_next(decoder,new_data->tuple.elements+i)) != 1)
		{
			bert_data_destroy(new_data);
			return result;
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

inline int bert_decode_big_tuple(bert_decoder_t *decoder,bert_data_t **data)
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

	bert_data_t *list_element;
	unsigned int i;
	int result;

	for (i=0;i<list_size;i++)
	{
		if ((result = bert_decoder_next(decoder,&list_element)) != 1)
		{
			bert_data_destroy(new_data);
			return result;
		}

		if (bert_list_append(new_data->list,list_element) == BERT_ERRNO_MALLOC)
		{
			bert_data_destroy(list_element);
			bert_data_destroy(new_data);
			return BERT_ERRNO_MALLOC;
		}
	}

	*data = new_data;
	return BERT_SUCCESS;
}

int bert_decoder_empty(const bert_decoder_t *decoder)
{
	return (((decoder->short_length - decoder->short_index) == 0) && bert_buffer_empty(&(decoder->buffer)));
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
			result = bert_decode_big_tuple(decoder,data);
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
