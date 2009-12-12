#ifndef _BERT_ERRNO_H_
#define _BERT_ERRNO_H_

#define BERT_ERRNO_MAX		-7
#define BERT_ERRNO_MALLOC	-6
#define BERT_ERRNO_WRITE	-5
#define BERT_ERRNO_READ		-4
#define BERT_ERRNO_SHORT	-3
#define BERT_ERRNO_EMPTY	-2
#define BERT_ERRNO_INVALID	-1
#define BERT_SUCCESS		0

const char * bert_strerror(int code);

#endif
