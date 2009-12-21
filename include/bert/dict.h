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

/*
 * Allocates a new empty bert_dict_t.
 */
extern bert_dict_t * bert_dict_create();

/*
 * Appends the given key and value pair to the bert_dict_t.
 * Returns BERT_SUCCESS on success or BERT_ERRNO_MALLOC if malloc failed.
 */
extern int bert_dict_append(bert_dict_t *dict,struct bert_data *key,struct bert_data *value);

/*
 * Destroys a previously allocated bert_dict_t and it's contents.
 */
extern void bert_dict_destroy(bert_dict_t *dict);

#endif
