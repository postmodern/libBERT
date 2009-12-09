#ifndef _BERT_DECODER_PRIVATE_H_
#define _BERT_DECODER_PRIVATE_H_

#include <bert/decoder.h>

#define BERT_DECODER_EMPTY(decoder)	(BERT_SHORT_BUFFER - decoder->short_length)
#define BERT_DECODER_STEP(decoder,i)	(decoder->short_index += i)
#define BERT_DECODER_PTR(decoder)	(decoder->short_buffer + decoder->short_index)
#define BERT_DECODER_PULL(decoder,i)	switch (bert_decoder_pull(decoder,i)) { \
						case BERT_ERRNO_MALLOC: \
							return BERT_ERRNO_MALLOC; \
						case BERT_ERRNO_SHORT: \
						case BERT_ERRNO_EMPTY: \
							return BERT_ERRNO_SHORT; \
						case BERT_ERRNO_INVALID: \
							return BERT_ERRNO_INVALID; \
					}

inline uint8_t bert_decode_uint8(bert_decoder_t *decoder);
inline int8_t bert_decode_int8(bert_decoder_t *decoder);
inline uint16_t bert_decode_uint16(bert_decoder_t *decoder);
inline uint32_t bert_decode_uint32(bert_decoder_t *decoder);
inline int32_t bert_decode_int32(bert_decoder_t *decoder);
inline bert_magic_t bert_decode_magic(bert_decoder_t *decoder);
inline int bert_decode_bytes(unsigned char *dest,bert_decoder_t *decoder,size_t length);

int bert_decoder_pull(bert_decoder_t *decoder,size_t size);

#endif
