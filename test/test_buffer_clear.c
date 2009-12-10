#include <bert/buffer.h>
#include <string.h>

#include "test.h"

#define DATA_SIZE	1024

bert_buffer_t buffer;

void test_head()
{
	if (buffer.head)
	{
		test_fail("bert_buffer_clear should reset the head pointer");
	}
}

void test_tail()
{
	if (buffer.tail)
	{
		test_fail("bert_buffer_clear should reset the tail pointer");
	}
}

void test_index()
{
	if (buffer.index)
	{
		test_fail("bert_buffer_clear should reset the index");
	}
}

int main()
{
	bert_buffer_t buffer;
	unsigned char data[DATA_SIZE];

	memset(data,'A',DATA_SIZE);
	bert_buffer_init(&buffer);

	bert_buffer_write(&buffer,data,DATA_SIZE);
	bert_buffer_clear(&buffer);

	test_head();
	test_tail();
	test_index();
	return 0;
}
