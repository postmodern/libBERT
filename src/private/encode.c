#include "encode.h"
#include "encoder.h"
#include "regex.h"
#include <bert/magic.h>
#include <bert/util.h>
#include <bert/errno.h>

#include <string.h>
#include <stdio.h>

int bert_encode_magic(bert_encoder_t *encoder,bert_magic_t magic)
{
	unsigned char buffer[1];

	bert_write_magic(buffer,magic);
	return bert_encoder_write(encoder,buffer,1);
}

int bert_encode_small_int(bert_encoder_t *encoder,uint8_t i)
{
	unsigned char buffer[2];

	bert_write_magic(buffer,BERT_SMALL_INT);
	bert_write_uint8(buffer+1,i);

	return bert_encoder_write(encoder,buffer,2);
}

int bert_encode_big_int(bert_encoder_t *encoder,uint32_t i)
{
	unsigned char buffer[5];

	bert_write_magic(buffer,BERT_INT);
	bert_write_uint32(buffer+1,i);

	return bert_encoder_write(encoder,buffer,5);
}

int bert_encode_int(bert_encoder_t *encoder,unsigned int i)
{
	if (i <= 0xff)
	{
		return bert_encode_small_int(encoder,i);
	}
	else if (i <= 0xffffffff)
	{
		return bert_encode_big_int(encoder,i);
	}

	return BERT_ERRNO_INVALID;
}

int bert_encode_float(bert_encoder_t *encoder,double d)
{
	size_t buffer_length = 1 + 31;
	unsigned char buffer[buffer_length + 1];

	bert_write_magic(buffer,BERT_FLOAT);

	memset(buffer+1,'\0',sizeof(unsigned char)*31);
	snprintf((char *)(buffer+1),31,"%15.15f",d);

	return bert_encoder_write(encoder,buffer,buffer_length);
}

int bert_encode_bignum(bert_encoder_t *encoder,int64_t integer)
{
	uint8_t sign = (integer < 0);
	size_t bytes_length = 0;

	uint64_t unsigned_integer = BERT_STRIP_SIGN((uint64_t)integer);
	unsigned int i;

	for (i=7;i>=0;i--)
	{
		if (unsigned_integer & (0xff << (i * 8)))
		{
			bytes_length = (i + 1);
			break;
		}
	}

	size_t buffer_length = 1;

	if (bytes_length > 4)
	{
		// 4 byte length field
		buffer_length += 4;
	}
	else
	{
		// 1 byte length field
		++buffer_length;
	}


	// 1 byte sign field
	++buffer_length;

	// bytes
	buffer_length += bytes_length;

	unsigned char buffer[buffer_length];

	if (bytes_length > 4)
	{
		bert_write_magic(buffer,BERT_LARGE_BIGNUM);
		bert_write_uint32(buffer,bytes_length);
	}
	else
	{
		bert_write_magic(buffer,BERT_SMALL_BIGNUM);
		bert_write_uint8(buffer,bytes_length);
	}

	bert_write_uint8(buffer,sign);

	for (i=0;i<bytes_length;i++)
	{
		buffer[i] = ((unsigned_integer & (0xff << (i * 8))) >> (i * 8));
	}

	return bert_encoder_write(encoder,buffer,buffer_length);
}

int bert_encode_atom(bert_encoder_t *encoder,const char *atom,size_t length)
{
	size_t buffer_length = 1 + 2 + length;
	unsigned char buffer[buffer_length];

	bert_write_magic(buffer,BERT_ATOM);
	bert_write_uint16(buffer+1,length);

	memcpy(buffer+1+2,atom,sizeof(unsigned char)*length);

	return bert_encoder_write(encoder,buffer,buffer_length);
}

int bert_encode_string(bert_encoder_t *encoder,const char *string,size_t length)
{
	size_t buffer_length = 1 + 4 + length;
	unsigned char buffer[buffer_length];

	bert_write_magic(buffer,BERT_STRING);
	bert_write_uint32(buffer+1,length);

	memcpy(buffer+1+4,string,sizeof(unsigned char)*length);

	return bert_encoder_write(encoder,buffer,buffer_length);
}

int bert_encode_bin(bert_encoder_t *encoder,const unsigned char *bin,size_t length)
{
	size_t buffer_length = 1 + 4 + length;
	unsigned char buffer[buffer_length];

	bert_write_magic(buffer,BERT_BIN);
	bert_write_uint32(buffer+1,length);

	memcpy(buffer+1+4,bin,sizeof(unsigned char)*length);

	return bert_encoder_write(encoder,buffer,buffer_length);
}

int bert_encode_tuple_header(bert_encoder_t *encoder,size_t elements)
{
	size_t buffer_length = 1 + 4;
	unsigned char buffer[buffer_length];

	if (elements <= 0xff)
	{
		buffer_length = 1 + 1;

		bert_write_magic(buffer,BERT_SMALL_TUPLE);
		bert_write_uint8(buffer+1,elements);
	}
	else if (elements <= 0xffffffff)
	{
		buffer_length = 1 + 4;

		bert_write_magic(buffer,BERT_LARGE_TUPLE);
		bert_write_uint32(buffer+1,elements);
	}
	else
	{
		return BERT_ERRNO_INVALID;
	}

	return bert_encoder_write(encoder,buffer,buffer_length);
}

int bert_encode_tuple(bert_encoder_t *encoder,const bert_data_t **elements,size_t length)
{
	int result;

	if ((result = bert_encode_tuple_header(encoder,length)) != BERT_SUCCESS)
	{
		return result;
	}

	unsigned int i;

	for (i=0;i<length;i++)
	{
		if ((result = bert_encoder_push(encoder,elements[i])) != BERT_SUCCESS)
		{
			return result;
		}
	}

	return BERT_SUCCESS;
}

int bert_encode_list_header(bert_encoder_t *encoder,size_t length)
{
	size_t buffer_length = 1 + 4;
	unsigned char buffer[buffer_length];

	bert_write_magic(buffer,BERT_LIST);
	bert_write_uint32(buffer+1,length);

	return bert_encoder_write(encoder,buffer,buffer_length);
}

int bert_encode_list(bert_encoder_t *encoder,const bert_list_t *list)
{
	bert_list_node_t *next_node = list->head;
	size_t length = 0;

	while (next_node)
	{
		++length;
		next_node = next_node->next;
	}

	int result;

	if ((result = bert_encode_list_header(encoder,length)) != BERT_SUCCESS)
	{
		return result;
	}

	next_node = list->head;

	while (next_node)
	{
		if ((result = bert_encoder_push(encoder,next_node->data)) != BERT_SUCCESS)
		{
			return result;
		}

		next_node = next_node->next;
	}

	return BERT_SUCCESS;
}

int bert_encode_complex_header(bert_encoder_t *encoder,const char *name,size_t elements)
{
	int result;

	if ((result = bert_encode_tuple_header(encoder,elements + 2)) != BERT_SUCCESS)
	{
		return result;
	}

	if ((result = bert_encode_atom(encoder,"bert",4)) != BERT_SUCCESS)
	{
		return result;
	}

	if ((result = bert_encode_atom(encoder,name,strlen(name))) != BERT_SUCCESS)
	{
		return result;
	}

	return BERT_SUCCESS;
}

int bert_encode_nil(bert_encoder_t *encoder)
{
	return bert_encode_complex_header(encoder,"nil",0);
}

int bert_encode_true(bert_encoder_t *encoder)
{
	return bert_encode_complex_header(encoder,"true",0);
}

int bert_encode_boolean(bert_encoder_t *encoder,unsigned int boolean)
{
	if (boolean == 1)
	{
		return bert_encode_true(encoder);
	}
	else if (boolean == 0)
	{
		return bert_encode_false(encoder);
	}

	return BERT_ERRNO_INVALID;
}

int bert_encode_false(bert_encoder_t *encoder)
{
	return bert_encode_complex_header(encoder,"false",0);
}

int bert_encode_dict(bert_encoder_t *encoder,const bert_dict_t *dict)
{
	bert_dict_node_t *next_node = dict->head;
	size_t length = 0;

	while (next_node)
	{
		++length;
		next_node = next_node->next;
	}

	int result;

	if ((result = bert_encode_complex_header(encoder,"dict",1)) != BERT_SUCCESS)
	{
		return result;
	}

	if ((result = bert_encode_list_header(encoder,length)) != BERT_SUCCESS)
	{
		return result;
	}

	next_node = dict->head;

	while (next_node)
	{
		if ((result = bert_encode_tuple_header(encoder,2)) != BERT_SUCCESS)
		{
			return result;
		}

		if ((result = bert_encoder_push(encoder,next_node->key)) != BERT_SUCCESS)
		{
			return result;
		}

		if ((result = bert_encoder_push(encoder,next_node->value)) != BERT_SUCCESS)
		{
			return result;
		}

		next_node = next_node->next;
	}

	return BERT_SUCCESS;
}

int bert_encode_time(bert_encoder_t *encoder,time_t timestamp)
{
	int result;

	if ((result = bert_encode_complex_header(encoder,"time",3)) != BERT_SUCCESS)
	{
		return result;
	}

	unsigned int megaseconds = (timestamp / 1000000);
	unsigned int seconds = (timestamp % 1000000);

	if ((result = bert_encode_int(encoder,megaseconds)) != BERT_SUCCESS)
	{
		return result;
	}

	if ((result = bert_encode_int(encoder,seconds)) != BERT_SUCCESS)
	{
		return result;
	}

	if ((result = bert_encode_int(encoder,0)) != BERT_SUCCESS)
	{
		return result;
	}

	return BERT_SUCCESS;
}

int bert_encode_regex(bert_encoder_t *encoder,const char *source,size_t length,unsigned int options)
{
	int result;

	if ((result = bert_encode_complex_header(encoder,"regex",2)) != BERT_SUCCESS)
	{
		return result;
	}

	if ((result = bert_encode_bin(encoder,(const unsigned char *)source,length)) != BERT_SUCCESS)
	{
		return result;
	}

	size_t opts_length = 0;
	unsigned int i;

	for (i=0;i<sizeof(int);i++)
	{
		if (options & (0x1 << i))
		{
			++opts_length;
		}
	}

	if ((result = bert_encode_list_header(encoder,opts_length)) != BERT_SUCCESS)
	{
		return result;
	}

	const char *opt_name;

	for (i=0;i<sizeof(int);i++)
	{
		if ((opt_name = bert_regex_optname(options & (0x1 << i))))
		{
			if ((result = bert_encode_atom(encoder,opt_name,strlen(opt_name))) != BERT_SUCCESS)
			{
				return result;
			}
		}
	}

	return BERT_SUCCESS;
}
