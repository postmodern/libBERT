#include <bert/util.h>

inline uint8_t bert_read_uint8(const unsigned char *ptr)
{
	return (uint8_t)(*ptr);
}

inline uint16_t bert_read_uint16(const unsigned char *ptr)
{
	return (uint16_t)((ptr[0] << 8) | ptr[1]);
}

inline bert_magic_t bert_read_magic(const unsigned char *ptr)
{
	return (bert_magic_t)(*ptr);
}

inline uint32_t bert_read_uint32(const unsigned char *ptr)
{
	return (uint32_t)((ptr[0] << 24 ) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3]);
}

inline void bert_write_uint8(unsigned char *dest,uint8_t i)
{
	dest[0] = i;
}

inline void bert_write_uint16(unsigned char *dest,uint16_t i)
{
	dest[0] = ((i & 0xff00) >> 8);
	dest[1] = (i & 0xff);
}

inline void bert_write_uint32(unsigned char *dest,uint32_t i)
{
	dest[0] = ((i & 0xff000000) >> 24);
	dest[1] = ((i & 0xff0000) >> 16);
	dest[2] = ((i & 0xff00) >> 8);
	dest[3] = (i & 0x00ff);
}

inline void bert_write_magic(unsigned char *dest,bert_magic_t magic)
{
	bert_write_uint8(dest,magic);
}
