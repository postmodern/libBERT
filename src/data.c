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

int bert_list_append(bert_data_t *list,bert_data_t *data)
{
	if (list->type != bert_data_list)
	{
		// not a list
		return -1;
	}

	bert_list_node_t *new_node;

	if (!(new_node = bert_list_node_create(data)))
	{
		// malloc failed
		return -1;
	}

	if (list->list.head)
	{
		list->list.tail->next = new_node;
		list->list.tail = new_node;
	}
	else
	{
		list->list.head = new_node;
		list->list.tail = new_node;
	}

	return 0;
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

bert_data_t * bert_data_create_null()
{
	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		return NULL;
	}

	new_data->type = bert_data_null;
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

bert_data_t * bert_data_create_float(float f)
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

bert_data_t * bert_data_create_atom(const char *name,size_t length)
{
	// +1 is for null terminating byte
	size_t new_length = length + 1;
	char *new_name;

	if (!(new_name = malloc(sizeof(char) * new_length)))
	{
		// malloc failed
		goto cleanup;
	}

	memcpy(new_name,name,sizeof(char) * length);

	// null terminate the name
	new_name[length] = '\0';

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
	// free the new_name
	free(new_name);
cleanup:
	// error
	return NULL;
}

bert_data_t * bert_data_create_string(const char *text,size_t length)
{
	// +1 is for null terminating byte
	size_t new_length = length + 1;
	char *new_text;

	if (!(new_text = malloc(sizeof(char) * new_length)))
	{
		// malloc failed
		goto cleanup;
	}

	memcpy(new_text,text,sizeof(char) * length);

	// null terminate the text
	new_text[length] = '\0';

	bert_data_t *new_data;

	if (!(new_data = bert_data_create()))
	{
		// malloc failed
		goto cleanup_text;
	}

	new_data->type = bert_data_string;
	new_data->string.length = length;
	new_data->string.text = new_text;
	return new_data;

cleanup_text:
	// free the new_text
	free(new_text);
cleanup:
	// error
	return NULL;
}

bert_data_t * bert_data_create_tupel(unsigned int length)
{
	bert_data_t **new_elements;

	if (!(new_elements = malloc(sizeof(bert_data_t *) * length)));
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

	new_data->type = bert_data_tupel;
	new_data->tupel.length = length;
	new_data->tupel.elements = new_elements;
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

	// be explicit about zeroing the pointers
	new_data->list.head = NULL;
	new_data->list.tail = NULL;
	return new_data;
}

bert_data_t * bert_data_create_bin(unsigned char *binary_data,size_t length)
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
		case bert_data_null:
		case bert_data_none:
			break;
		case bert_data_atom:
			free(data->atom.name);
			break;
		case bert_data_string:
			free(data->string.text);
			break;
		case bert_data_tupel:
			length = data->tupel.length;

			for (i=0; i<length; i++)
			{
				bert_data_destroy(data->tupel.elements[i]);
			}
			break;
		case bert_data_list:
			bert_list_node_destroy(data->list.head);
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
