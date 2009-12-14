#include <bert/data.h>

#include <malloc.h>
#include <string.h>

bert_list_node_t * bert_list_node_create(bert_data_t *data)
{
	bert_list_node_t *new_node;

	if (!(new_node = malloc(sizeof(bert_list_node_t))))
	{
		// malloc failed
		return NULL;
	}

	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void bert_list_node_destroy(bert_list_node_t *node)
{
	if (!(node))
	{
		return;
	}

	if (node->data)
	{
		bert_data_destroy(node->data);
	}

	if (node->next)
	{
		bert_list_node_destroy(node->next);
	}

	free(node);
}

bert_data_t * bert_data_create()
{
	bert_data_t *new_data;

	if (!(new_data = malloc(sizeof(bert_data_t))))
	{
		// malloc failed
		return NULL;
	}

	memset(new_data,0,sizeof(bert_data_t));

	// be explicit about setting the type
	new_data->type = bert_data_none;
	return new_data;
}

bert_data_t * bert_data_create_nil()
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_nil;
	return new_data;
}

bert_data_t * bert_data_create_true()
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_boolean;
	new_data->boolean = 1;
	return new_data;
}

bert_data_t * bert_data_create_false()
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_boolean;
	new_data->boolean = 0;
	return new_data;
}

bert_data_t * bert_data_create_int(int i)
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_int;
	new_data->integer = i;
	return new_data;
}

bert_data_t * bert_data_create_float(double f)
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_float;
	new_data->floating_point = f;
	return new_data;
}

bert_data_t * bert_data_create_empty_atom(bert_atom_size_t length)
{
	// +1 is for null terminating byte
	size_t new_length = length + 1;
	char *new_name;

	if (!(new_name = calloc(new_length,sizeof(char))))
	{
		// malloc failed
		goto cleanup;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		goto cleanup_name;
	}

	new_data->type = bert_data_atom;
	new_data->atom.length = length;
	new_data->atom.name = new_name;
	return new_data;

cleanup_name:
	free(new_name);
cleanup:
	return NULL;
}

bert_data_t * bert_data_create_atom(const char *name)
{
	bert_data_t *new_data;
	bert_atom_size_t length = strlen(name);

	if (!(new_data = bert_data_create_empty_atom(length)))
	{
		return NULL;
	}

	strncpy(new_data->atom.name,name,length);
	return new_data;
}

bert_data_t * bert_data_create_empty_string(bert_string_size_t length)
{
	size_t new_length = length + 1;
	char *new_text;

	if (!(new_text = calloc(new_length,sizeof(char))))
	{
		goto cleanup;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		goto cleanup_text;
	}

	new_data->type = bert_data_string;
	new_data->string.length = length;
	new_data->string.text = new_text;
	return new_data;

cleanup_text:
	free(new_text);
cleanup:
	return NULL;
}

bert_data_t * bert_data_create_string(const char *text)
{
	bert_data_t *new_data;
	bert_string_size_t length = strlen(text);

	if (!(new_data = bert_data_create(length)))
	{
		// malloc failed
		return NULL;
	}

	strncpy(new_data->string.text,text,length);
	return new_data;
}

bert_data_t * bert_data_create_tuple(bert_tuple_size_t length)
{
	bert_data_t **new_elements;

	if (!(new_elements = malloc(sizeof(bert_data_t *) * length)))
	{
		// malloc failed
		goto cleanup;
	}

	unsigned int i;

	// be explicit about zeroing the pointers
	for (i=0;i<length;i++)
	{
		new_elements[i] = NULL;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		goto cleanup_elements;
	}

	new_data->type = bert_data_tuple;
	new_data->tuple.length = length;
	new_data->tuple.elements = new_elements;
	return new_data;

cleanup_elements:
	free(new_elements);
cleanup:
	// error
	return NULL;
}

bert_data_t * bert_data_create_list()
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_list;
	new_data->list = bert_list_create();
	return new_data;
}

bert_data_t * bert_data_create_dict()
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_dict;
	new_data->dict = bert_dict_create();
	return new_data;
}

bert_data_t * bert_data_create_empty_bin(bert_bin_size_t length)
{
	unsigned char *new_bin;

	if (!(new_bin = calloc(length,sizeof(unsigned char))))
	{
		// malloc failed
		goto cleanup;
	}

	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		goto cleanup_bin;
	}

	new_data->type = bert_data_bin;
	new_data->bin.length = length;
	new_data->bin.data = new_bin;
	return new_data;

cleanup_bin:
	free(new_bin);
cleanup:
	return NULL;
}

bert_data_t * bert_data_create_bin(const unsigned char *binary_data,bert_bin_size_t length)
{
	unsigned char *new_binary_data;

	if (!(new_binary_data = malloc(sizeof(unsigned char) * length)))
	{
		// malloc failed
		goto cleanup;
	}

	memcpy(new_binary_data,binary_data,sizeof(unsigned char) * length);

	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		goto cleanup_binary_data;
	}

	new_data->type = bert_data_bin;
	new_data->bin.length = length;
	new_data->bin.data = new_binary_data;
	return new_data;

cleanup_binary_data:
	// free the new_binary_data
	free(new_binary_data);
cleanup:
	// error
	return NULL;
}

bert_data_t * bert_data_create_time(time_t timestamp)
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_time;
	new_data->time = timestamp;
	return new_data;
}

bert_data_t * bert_data_create_regex(const char *source,bert_regex_size_t length,int options)
{
	char *new_source;
	size_t new_length = length + 1;

	if (!(new_source = malloc(sizeof(char) * new_length)))
	{
		// malloc failed
		goto cleanup;
	}

	memcpy(new_source,source,sizeof(char) * length);
	new_source[length] = '\0';

	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		goto cleanup_source;
	}

	new_data->type = bert_data_regex;
	new_data->regex.length = length;
	new_data->regex.source = new_source;
	new_data->regex.options = options;
	return new_data;

cleanup_source:
	// free the new_source
	free(new_source);
cleanup:
	// error
	return NULL;
}

int bert_data_strequal(const bert_data_t *data,const char *str)
{
	void *data_ptr;
	size_t data_length;

	switch (data->type)
	{
		case bert_data_string:
			data_ptr = data->string.text;
			data_length = data->string.length;
			break;
		case bert_data_atom:
			data_ptr = data->atom.name;
			data_length = data->atom.length;
			break;
		case bert_data_bin:
			data_ptr = data->bin.data;
			data_length = data->bin.length;
			break;
		default:
			return 0;
	}

	size_t str_length = strlen(str);

	if (data_length != str_length)
	{
		return 0;
	}

	return memcmp(data_ptr,str,data_length) == 0;
}

void bert_data_destroy(bert_data_t *data)
{
	unsigned int i;
	size_t length;

	if (!data)
	{
		return;
	}

	switch (data->type)
	{
		case bert_data_int:
		case bert_data_float:
		case bert_data_nil:
		case bert_data_none:
			break;
		case bert_data_atom:
			free(data->atom.name);
			break;
		case bert_data_string:
			free(data->string.text);
			break;
		case bert_data_tuple:
			length = data->tuple.length;

			if (data->tuple.elements)
			{
				for (i=0;i<length;i++)
				{
					bert_data_destroy(data->tuple.elements[i]);
				}
			}
			break;
		case bert_data_list:
			bert_list_destroy(data->list);
			break;
		case bert_data_dict:
			bert_dict_destroy(data->dict);
			break;
		case bert_data_bin:
			free(data->bin.data);
			break;
		default:
			// should never get here
			break;
	}

	free(data);
}
