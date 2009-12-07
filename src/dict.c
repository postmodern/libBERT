#include <bert/dict.h>

#include <malloc.h>

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

		free(next_node);
	}

	free(dict);
}
