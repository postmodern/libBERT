#ifndef _BERT_UTIL_H_
#define _BERT_UTIL_H_

#include <bert/types.h>

#include <stdint.h>

#define MIN(a,b)	((a) < (b) ? (a) : (b))

inline uint8_t bert_read_uint8(const unsigned char *ptr);
inline uint16_t bert_read_uint16(const unsigned char *ptr);
inline bert_magic_t bert_read_magic(const unsigned char *ptr);
inline uint32_t bert_read_uint32(const unsigned char *ptr);

inline void bert_write_uint8(unsigned char *dest,uint8_t i);
inline void bert_write_uint16(unsigned char *dest,uint16_t i);
inline void bert_write_uint32(unsigned char *dest,uint32_t i);
inline void bert_write_magic(unsigned char *dest,bert_magic_t magic);

#endif
