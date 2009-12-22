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
	else
	{
		// byte length + signed byte
		size_t count = 1 + 1;

		uint64_t unsigned_integer = BERT_STRIP_SIGN(i);
		unsigned int j;

		for (j=sizeof(int64_t)-1;j>=0;j--)
		{
			if (unsigned_integer & (0xff << (j * 8)))
			{
				count += (j + 1);
				break;
			}
		}

		return count;
	}
}
