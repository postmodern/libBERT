#include <bert/util.h>
#include "data.h"

size_t bert_data_sizeof_int(int64_t i)
{
	if (i <= BERT_MAX_INT && i >= BERT_MIN_INT)
	{
		if (i <= 0xff)
		{
			return 1;
		}
		else
		{
			return 4;
		}
	}

	uint64_t unsigned_integer = BERT_STRIP_SIGN(i);
	size_t byte_count = 0;
	unsigned int j;

	for (j=sizeof(int64_t)-1;j>=0;j--)
	{
		if (unsigned_integer & (0xff << (j * 8)))
		{
			byte_count = (j + 1);
			break;
		}
	}

	size_t count = 0;

	if (byte_count > 0xff)
	{
		// byte length
		count += 4;
	}
	else
	{
		// byte length
		++count;
	}

	// signed byte
	++count;

	// additional bytes
	count += byte_count;

	return count;
}
