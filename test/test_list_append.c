#include <bert/list.h>

#include "test.h"
#include <string.h>

#define EXPECTED_ONE	42
#define EXPECTED_TWO	69

bert_list_t *list;

void test_one()
{
	bert_list_node_t *node = list->head;

	if (!node)
	{
		test_fail("bert_list_append should create the first node");
	}

	bert_data_t *data;

	if (!(data = node->data))
	{
		test_fail("bert_list_append should set the first data");
	}

	if (data->type != bert_data_int)
	{
		test_fail("bert_list_append appended the wrong first data");
	}

	if (data->integer != EXPECTED_ONE)
	{
		test_fail("first element is %d, expected %d",data->integer,EXPECTED_ONE);
	}
}

void test_two()
{
	bert_list_node_t *node = list->head->next;

	if (!node)
	{
		test_fail("bert_list_append should create the second node");
	}

	bert_data_t *data;

	if (!(data = node->data))
	{
		test_fail("bert_list_append should set the second data");
	}

	if (data->type != bert_data_int)
	{
		test_fail("bert_list_append appended the wrong second data");
	}

	if (data->integer != EXPECTED_TWO)
	{
		test_fail("first element is %d, expected %d",data->integer,EXPECTED_TWO);
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

	test_one();
	test_two();

	bert_list_destroy(list);
	return 0;
}
