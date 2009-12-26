#include <bert/errno.h>

#include <errno.h>
#include <string.h>

const char *bert_errors[] = {
	"unknown libBERT error code",
	"invalid BERT data",
	"no more data left",
	"no more space to write data",
	"not enough data available",
	"read error",
	"write error",
	"malloc failed"
};

const char * bert_strerror(int code)
{
	if (code >= 0 || code <= BERT_ERRNO_MAX)
	{
		return bert_errors[0];
	}

	if (code == BERT_ERRNO_READ || code == BERT_ERRNO_WRITE)
	{
		return strerror(errno);
	}

	unsigned int index = (-code) + 1;

	return bert_errors[index];
}
