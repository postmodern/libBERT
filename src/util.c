#include <bert/util.h>

inline uint8_t bert_read_uint8(const unsigned char *src)
{
	return (uint8_t)(*src);
}

inline uint16_t bert_read_uint16(const unsigned char *src)
{
	return (uint16_t)((src[0] << 8) | src[1]);
}

inline bert_magic_t bert_read_magic(const unsigned char *src)
{
	return (bert_magic_t)(*src);
}

inline uint32_t bert_read_uint32(const unsigned char *src)
{
	return (uint32_t)((src[0] << 24 ) | (src[1] << 16) | (src[2] << 8) | src[3]);
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
