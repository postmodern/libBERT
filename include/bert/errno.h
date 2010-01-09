#ifndef _BERT_ERRNO_H_
#define _BERT_ERRNO_H_

#define BERT_ERRNO_MAX		-9
#define BERT_ERRNO_BIGNUM	-8
#define BERT_ERRNO_MALLOC	-7
#define BERT_ERRNO_WRITE	-6
#define BERT_ERRNO_READ		-5
#define BERT_ERRNO_SHORT_WRITE	-4
#define BERT_ERRNO_SHORT_READ	-3
#define BERT_ERRNO_EMPTY	-2
#define BERT_ERRNO_INVALID	-1
#define BERT_SUCCESS		0

/*
 * Returns a string representation for the given error code.
 */
extern const char * bert_strerror(int code);

#endif
