#include <bert/errno.h>

const char *bert_errors[] = {
	"unknown bert error code",
	"no more data left",
	"not enough data available",
	"malloc failed"
};

const char * bert_strerror(int errno)
{
	if (errno >= 0 || errno <= BERT_ERRNO_MAX)
	{
		return bert_errors[0];
	}

	unsigned int index = (-errno) - 1;

	return bert_errors[index];
}
