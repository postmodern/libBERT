#include <bert/errno.h>

#include <errno.h>
#include <string.h>

const char *bert_errors[] = {
	"unknown libBERT error code",
	"invalid BERT data",
	"no more data left",
	"not enough data available",
	"read error",
	"malloc failed"
};

const char * bert_strerror(int code)
{
	if (code >= 0 || code <= BERT_ERRNO_MAX)
	{
		return bert_errors[0];
	}

	if (code == BERT_ERRNO_READ)
	{
		return strerror(errno);
	}

	unsigned int index = (-code);

	return bert_errors[index];
}
