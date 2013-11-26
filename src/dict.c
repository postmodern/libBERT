#include <bert/dict.h>
#include <bert/errno.h>

#include <stdlib.h>

bert_dict_t * bert_dict_create()
{
	bert_dict_t *new_dict;

	if (!(new_dict = malloc(sizeof(bert_dict_t))))
	{
		return NULL;
	}

	new_dict->head = NULL;
	new_dict->tail = NULL;
	return new_dict;
}

int bert_dict_append(bert_dict_t *dict,bert_data_t *key,bert_data_t *value)
{
	bert_dict_node_t *new_node;

	if (!(new_node = malloc(sizeof(bert_dict_node_t))))
	{
		return BERT_ERRNO_MALLOC;
	}

	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;

	if (dict->tail)
	{
		dict->tail->next = new_node;
		dict->tail = new_node;
	}
	else
	{
		dict->head = new_node;
		dict->tail = new_node;
	}

	return BERT_SUCCESS;
}

void bert_dict_destroy(bert_dict_t *dict)
{
	bert_dict_node_t *last_node = NULL;
	bert_dict_node_t *next_node = dict->head;

	while (next_node)
	{
		last_node = next_node;
		next_node = next_node->next;

		bert_data_destroy(last_node->key);
		bert_data_destroy(last_node->value);

		free(last_node);
	}

	free(dict);
}
