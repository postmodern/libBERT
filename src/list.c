#include <bert/list.h>

#include <malloc.h>

bert_list_t * bert_list_create()
{
	bert_list_t *new_list;

	if (!(new_list = malloc(sizeof(bert_list_t))))
	{
		return NULL;
	}

	new_list->head = NULL;
	new_list->tail = NULL;
	return new_list;
}

int bert_list_append(bert_list_t *list,bert_data_t *data)
{
	bert_list_node_t *new_node;

	if (!(new_node = bert_list_node_create(data)))
	{
		// malloc failed
		return -1;
	}

	if (list->head)
	{
		list->tail->next = new_node;
		list->tail = new_node;
	}
	else
	{
		list->head = new_node;
		list->tail = new_node;
	}

	return 0;
}

void bert_list_destroy(bert_list_t *list)
{
	if (!list)
	{
		return;
	}

	bert_list_node_t *last_node = NULL;
	bert_list_node_t *next_node = list->head;

	while (next_node)
	{
		last_node = next_node;
		next_node = next_node->next;

		if (last_node->data)
		{
			bert_data_destroy(last_node->data);
		}

		free(last_node);
	}

	free(list);
}
