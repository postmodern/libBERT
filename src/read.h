#ifndef _BERT_READ_H_
#define _BERT_READ_H_

#include <bert/types.h>

#include <sys/types.h>
#include <stdint.h>

inline uint8_t bert_read_uint8(const unsigned char *ptr);
inline int8_t bert_read_int8(const unsigned char *ptr);
inline uint16_t bert_read_uint16(const unsigned char *ptr);
inline int16_t bert_read_int16(const unsigned char *ptr);
inline bert_magic_t bert_read_magic(const unsigned char *ptr);
inline uint32_t bert_read_uint32(const unsigned char *ptr);
inline int32_t bert_read_int32(const unsigned char *ptr);
inline void bert_read_string(char *dest,unsigned char *ptr,size_t length);
inline void bert_read_binary(unsigned char *dest,unsigned char *ptr,size_t length);

#endif
