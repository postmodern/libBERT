#ifndef _BERT_DECODE_H_
#define _BERT_DECODE_H_

#include <bert/decoder.h>

uint8_t bert_decode_uint8(bert_decoder_t *decoder);
uint16_t bert_decode_uint16(bert_decoder_t *decoder);
uint32_t bert_decode_uint32(bert_decoder_t *decoder);
bert_magic_t bert_decode_magic(bert_decoder_t *decoder);
int bert_decode_bytes(unsigned char *dest,bert_decoder_t *decoder,size_t length);

inline int bert_decode_nil(bert_decoder_t *decoder,bert_data_t **data);
inline int bert_decode_small_int(bert_decoder_t *decoder,bert_data_t **data);
inline int bert_decode_big_int(bert_decoder_t *decoder,bert_data_t **data);
inline int bert_decode_float(bert_decoder_t *decoder,bert_data_t **data);
int bert_decode_bignum(bert_decoder_t *decoder,bert_data_t **data,size_t size);
inline int bert_decode_small_bignum(bert_decoder_t *decoder,bert_data_t **data);
inline int bert_decode_big_bignum(bert_decoder_t *decoder,bert_data_t **data);
inline int bert_decode_string(bert_decoder_t *decoder,bert_data_t **data);
int bert_decode_time(bert_decoder_t *decoder,bert_data_t **data);
int bert_decode_dict(bert_decoder_t *decoder,bert_data_t **data);
int bert_decode_complex(bert_decoder_t *decoder,bert_data_t **data);
int bert_decode_atom(bert_decoder_t *decoder,bert_data_t **data);
inline int bert_decode_bin(bert_decoder_t *decoder,bert_data_t **data);
inline int bert_decode_tuple(bert_decoder_t *decoder,bert_data_t **data,size_t size);
inline int bert_decode_small_tuple(bert_decoder_t *decoder,bert_data_t **data);
inline int bert_decode_large_tuple(bert_decoder_t *decoder,bert_data_t **data);
int bert_decode_list(bert_decoder_t *decoder,bert_data_t **data);
int bert_decode_regex(bert_decoder_t *decoder,bert_data_t **data);

#endif
