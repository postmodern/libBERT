#ifndef _BERT_MAGIC_H_
#define _BERT_MAGIC_H_

#include <sys/types.h>

typedef uint8_t bert_magic_t;

#define BERT_SMALL_INT		((bert_magic_t) 97)
#define BERT_INT		((bert_magic_t) 98)
#define BERT_SMALL_BIGNUM	((bert_magic_t) 110)
#define BERT_LARGE_BIGNUM	((bert_magic_t) 111)
#define BERT_FLOAT		((bert_magic_t) 99)
#define BERT_ATOM		((bert_magic_t) 100)
#define BERT_SMALL_TUPLE	((bert_magic_t) 104)
#define BERT_LARGE_TUPLE	((bert_magic_t) 105)
#define BERT_NULL		((bert_magic_t) 106)
#define BERT_STRING		((bert_magic_t) 107)
#define BERT_LIST		((bert_magic_t) 108)
#define BERT_BIN		((bert_magic_t) 109)
#define BERT_FUN		((bert_magic_t) 117)
#define BERT_NEW_FUN		((bert_magic_t) 112)
#define BERT_MAGIC		((bert_magic_t) 131)

#define BERT_MAX_INT	((1 << 27) - 1)
#define BERT_MIN_INT	(-(1 << 27))

#endif
