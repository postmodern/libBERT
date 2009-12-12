#ifndef _BERT_UTIL_H_
#define _BERT_UTIL_H_

#include <bert/types.h>

#include <stdint.h>

#define MIN(a,b)	((a) < (b) ? (a) : (b))

#define BERT_MAX_INT	((1 << 27) - 1)
#define BERT_MIN_INT	(-(1 << 27))

inline uint8_t bert_read_uint8(const unsigned char *src);
inline uint16_t bert_read_uint16(const unsigned char *src);
inline bert_magic_t bert_read_magic(const unsigned char *src);
inline uint32_t bert_read_uint32(const unsigned char *src);

inline void bert_write_uint8(unsigned char *dest,uint8_t i);
inline void bert_write_uint16(unsigned char *dest,uint16_t i);
inline void bert_write_uint32(unsigned char *dest,uint32_t i);
inline void bert_write_magic(unsigned char *dest,bert_magic_t magic);

#endif
