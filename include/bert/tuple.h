#ifndef _BERT_TUPLE_H_
#define _BERT_TUPLE_H_

#include <bert/types.h>

struct bert_data;

typedef struct bert_tuple
{
	bert_tuple_size_t length;
	struct bert_data **elements;
} bert_tuple_t;

/*
 * Allocates a new empty bert_tuple_t of the given length.
 */
bert_tuple_t * bert_tuple_create(bert_tuple_size_t length);

/*
 * Retrieves the data at the given index within the tuple.
 */
struct bert_data * bert_tuple_get(const bert_tuple_t *tuple,unsigned int index);

/*
 * Sets the data at the given index within the tuple.
 */
int bert_tuple_set(bert_tuple_t *tuple,unsigned int index,struct bert_data *data);

/*
 * Destroys a previously allocated bert_tuple_t.
 */
void bert_tuple_destroy(bert_tuple_t *tuple);

#endif
