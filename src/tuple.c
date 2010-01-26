#include <bert/tuple.h>
#include <bert/data.h>

#include <stdlib.h>

bert_tuple_t * bert_tuple_create(bert_tuple_size_t length)
{
	struct bert_data **new_elements;

	if (!(new_elements = calloc(length,sizeof(struct bert_data_t *))))
	{
		// malloc failed
		goto cleanup;
	}

	bert_tuple_t *new_tuple;

	if (!(new_tuple = malloc(sizeof(bert_tuple_t))))
	{
		// malloc failed
		goto cleanup_elements;
	}

	new_tuple->length = length;
	new_tuple->elements = new_elements;
	return new_tuple;

cleanup_elements:
	free(new_elements);
cleanup:
	// error
	return NULL;
}

struct bert_data * bert_tuple_get(const bert_tuple_t *tuple,unsigned int index)
{
	if (index >= tuple->length)
	{
		// not found
		return NULL;
	}

	return tuple->elements[index];
}

int bert_tuple_set(bert_tuple_t *tuple,unsigned int index,struct bert_data *data)
{
	if (index >= tuple->length)
	{
		return 0;
	}

	if (tuple->elements[index])
	{
		bert_data_destroy(tuple->elements[index]);
	}

	tuple->elements[index] = data;
	return 1;
}

void bert_tuple_destroy(bert_tuple_t *tuple)
{
	bert_tuple_size_t length = tuple->length;
	unsigned int i;

	for (i=0;i<length;i++)
	{
		bert_data_destroy(tuple->elements[i]);
	}

	free(tuple->elements);
	free(tuple);
}
