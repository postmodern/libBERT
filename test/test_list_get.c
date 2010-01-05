#include <bert/list.h>

#include "test.h"

#define EXPECTED_ONE 42
#define EXPECTED_TWO 69

bert_list_t *list;

void test_get_first()
{
	bert_data_t *data;

	if (!(data = bert_list_get(list,0)))
	{
		test_fail("bert_list_get did not get the first element");
	}

	if (data->type != bert_data_int)
	{
		test_fail("bert_list_get found the wrong first element");
	}

	if (data->integer != EXPECTED_ONE)
	{
		test_fail("bert_list_get found %d as the first element, expected %d",data->integer,EXPECTED_ONE);
	}
}

void test_get_last()
{
	bert_data_t *data;

	if (!(data = bert_list_get(list,1)))
	{
		test_fail("bert_list_get did not get the second element");
	}

	if (data->type != bert_data_int)
	{
		test_fail("bert_list_get found the wrong second element");
	}

	if (data->integer != EXPECTED_TWO)
	{
		test_fail("bert_list_get found %d as the first element, expected %d",data->integer,EXPECTED_TWO);
	}
}

void test_get_out_of_bounds()
{
	if (bert_list_get(list,2))
	{
		test_fail("bert_list_get did not return NULL for out of bounds requests");
	}
}

int main()
{
	if (!(list = bert_list_create()))
	{
		test_fail("malloc failed");
	}

	bert_data_t *data;

	if (!(data = bert_data_create_int(EXPECTED_ONE)))
	{
		test_fail("malloc failed");
	}

	bert_list_append(list,data);

	if (!(data = bert_data_create_int(EXPECTED_TWO)))
	{
		test_fail("malloc failed");
	}

	bert_list_append(list,data);

	test_get_first();
	test_get_last();
	test_get_out_of_bounds();

	bert_list_destroy(list);
	return 0;
}
