#ifndef _BERT_DICT_H_
#define _BERT_DICT_H_

#include <bert/data.h>

struct bert_data;

struct bert_dict_node
{
	struct bert_data *key;
	struct bert_data *value;

	struct bert_dict_node *next;
};
typedef struct bert_dict_node bert_dict_node_t;

struct bert_dict
{
	struct bert_dict_node *head;
	struct bert_dict_node *tail;
};
typedef struct bert_dict bert_dict_t;

extern bert_dict_t * bert_dict_create();
extern bert_dict_append(bert_dict_t *dict,struct bert_data *key,struct bert_data *value);
extern bert_dict_set(bert_dict_t *dict,struct bert_data *key,struct bert_data *value);
extern void bert_dict_destroy(bert_dict_t *dict);

#endif
