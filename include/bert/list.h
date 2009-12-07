#ifndef _BERT_LIST_H_
#define _BERT_LIST_H_

#include <bert/data.h>

struct bert_data;

struct bert_list_node
{
	struct bert_data *data;

	struct bert_list_node *next;
};
typedef struct bert_list_node bert_list_node_t;

struct bert_list
{
	struct bert_list_node *head;
	struct bert_list_node *tail;
};
typedef struct bert_list bert_list_t;

extern bert_list_t * bert_list_create();
extern bert_list_append(bert_list_t *list,struct bert_data *data);
extern void bert_list_destroy(bert_list_t *list);

#endif
