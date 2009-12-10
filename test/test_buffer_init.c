#include <bert/buffer.h>
#include <string.h>

#include "test.h"

bert_buffer_t buffer;

void test_head()
{
	if (buffer.head)
	{
		test_fail("bert_buffer_init should reset the head pointer");
	}
}

void test_tail()
{
	if (buffer.tail)
	{
		test_fail("bert_buffer_init should reset the tail pointer");
	}
}

void test_index()
{
	if (buffer.index)
	{
		test_fail("bert_buffer_init should reset the index");
	}
}

int main()
{
	bert_buffer_init(&buffer);

	test_head();
	test_tail();
	test_index();
	return 0;
}
