#include <bert/list.h>
#include <bert/errno.h>

#include <stdlib.h>

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

int bert_list_append(bert_list_t *list,struct bert_data *data)
{
	bert_list_node_t *new_node;

	if (!(new_node = malloc(sizeof(bert_list_node_t))))
	{
		// malloc failed
		return BERT_ERRNO_MALLOC;
	}

	new_node->data = data;
	new_node->next = NULL;

	if (list->tail)
	{
		list->tail->next = new_node;
		list->tail = new_node;
	}
	else
	{
		list->head = new_node;
		list->tail = new_node;
	}

	return BERT_SUCCESS;
}

struct bert_data * bert_list_get(const bert_list_t *list,unsigned int index)
{
	bert_list_node_t *next_node = list->head;
	unsigned int i = 0;

	while (next_node && i < index)
	{
		next_node = next_node->next;
		++i;
	}

	if (!next_node)
	{
		return NULL;
	}

	return next_node->data;
}

int bert_list_set(bert_list_t *list,unsigned int index,struct bert_data *data)
{
	bert_list_node_t *next_node = list->head;
	unsigned int i = 0;

	while (next_node && i < index)
	{
		next_node = next_node->next;
		++i;
	}

	if (!next_node)
	{
		return 0;
	}

	bert_data_destroy(next_node->data);
	next_node->data = data;

	return 1;
}

size_t bert_list_length(const bert_list_t *list)
{
	bert_list_node_t *next_node = list->head;
	size_t length = 0;

	while (next_node)
	{
		next_node = next_node->next;
		++length;
	}

	return length;
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
