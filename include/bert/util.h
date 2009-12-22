#ifndef _BERT_UTIL_H_
#define _BERT_UTIL_H_

#include <bert/types.h>

#include <stdint.h>

#define MIN(a,b)	((a) < (b) ? (a) : (b))

#define BERT_MAX_INT	((1 << 27) - 1)
#define BERT_MIN_INT	(-(1 << 27))

#define BERT_STRIP_SIGN(i)	((i) < 0 ? -(i) : (i))

/*
 * Reads a uint8_t from the given unsigned char pointer.
 */
extern inline uint8_t bert_read_uint8(const unsigned char *src);

/*
 * Reads a uint16_t from the given unsigned char pointer.
 */
extern inline uint16_t bert_read_uint16(const unsigned char *src);

/*
 * Reads a bert_magic_t from the given unsigned char pointer.
 */
extern inline bert_magic_t bert_read_magic(const unsigned char *src);

/*
 * Reads a uint32_t from the given unsigned char pointer.
 */
extern inline uint32_t bert_read_uint32(const unsigned char *src);

/*
 * Writes the given uint8_t to an unsigned char pointer.
 */
extern inline void bert_write_uint8(unsigned char *dest,uint8_t i);

/*
 * Writes the given uint16_t to an unsigned char pointer.
 */
extern inline void bert_write_uint16(unsigned char *dest,uint16_t i);

/*
 * Writes the given uint32_t to an unsigned char pointer.
 */
extern inline void bert_write_uint32(unsigned char *dest,uint32_t i);

/*
 * Writes the given bert_magic_t to an unsigned char pointer.
 */
extern inline void bert_write_magic(unsigned char *dest,bert_magic_t magic);

#endif
