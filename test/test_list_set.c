#include <bert/list.h>

#include "test.h"

#define EXPECTED_BEFORE 42
#define EXPECTED_AFTER 69

bert_list_t *list;

void test_set()
{
	bert_data_t *data;

	if (!(data = bert_data_create_int(EXPECTED_AFTER)))
	{
		test_fail("malloc failed");
	}

	if (!bert_list_set(list,0,data))
	{
		test_fail("bert_list_set could not find the first element");
	}

	bert_data_t *first = list->head->data;

	if (first->type != bert_data_int)
	{
		test_fail("bert_list_set did not set an integer");
	}

	if (first->integer != EXPECTED_AFTER)
	{
		test_fail("bert_list_set set %d as the first element, expected %d",first->integer,EXPECTED_AFTER);
	}
}

void test_set_out_of_bounds()
{
	bert_data_t *data;

	if (!(data = bert_data_create_int(EXPECTED_AFTER)))
	{
		test_fail("malloc failed");
	}

	if (bert_list_set(list,2,data))
	{
		test_fail("bert_list_set did not return 0 when setting out of bounds elements");
	}

	bert_data_destroy(data);
}

int main()
{
	if (!(list = bert_list_create()))
	{
		test_fail("malloc failed");
	}

	bert_data_t *data;

	if (!(data = bert_data_create_int(EXPECTED_BEFORE)))
	{
		test_fail("malloc failed");
	}

	bert_list_append(list,data);

	test_set();
	test_set_out_of_bounds();

	bert_list_destroy(list);
	return 0;
}
