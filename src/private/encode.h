#ifndef _BERT_PRIVATE_ENCODE_H_
#define _BERT_PRIVATE_ENCODE_H_

#include <bert/encoder.h>
#include <bert/types.h>

#include <stdint.h>

int bert_encode_magic(bert_encoder_t *encoder,bert_magic_t magic);
int bert_encode_small_int(bert_encoder_t *encoder,uint8_t i);
int bert_encode_big_int(bert_encoder_t *encoder,uint32_t i);
int bert_encode_int(bert_encoder_t *encoder,unsigned int i);
int bert_encode_bignum(bert_encoder_t *encoder,int64_t i);
int bert_encode_float(bert_encoder_t *encoder,double d);
int bert_encode_atom(bert_encoder_t *encoder,const char *atom,size_t length);
int bert_encode_string(bert_encoder_t *encoder,const char *string,size_t length);
int bert_encode_bin(bert_encoder_t *encoder,const unsigned char *bin,size_t length);
int bert_encode_tuple_header(bert_encoder_t *encoder,size_t elements);
int bert_encode_tuple(bert_encoder_t *encoder,const bert_data_t **elements,size_t length);
int bert_encode_list_header(bert_encoder_t *encoder,size_t length);
int bert_encode_list(bert_encoder_t *encoder,const bert_list_t *list);

int bert_encode_complex_header(bert_encoder_t *encoder,const char *name,size_t elements);
int bert_encode_true(bert_encoder_t *encoder);
int bert_encode_false(bert_encoder_t *encoder);
int bert_encode_boolean(bert_encoder_t *encoder,unsigned int boolean);
int bert_encode_nil(bert_encoder_t *encoder);
int bert_encode_dict(bert_encoder_t *encoder,const bert_dict_t *dict);
int bert_encode_regex(bert_encoder_t *encoder,const char *source,size_t length,unsigned int options);
int bert_encode_time(bert_encoder_t *encoder,time_t timestamp);

#endif
