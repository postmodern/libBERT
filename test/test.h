#ifndef _TEST_H_
#define _TEST_H_

#include <bert/decoder.h>
#include <bert/encoder.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void test_fail(const char *mesg,...);
int test_open_file(const char *path);
bert_decoder_t * test_decoder();

bert_encoder_t * test_encoder(unsigned char *buffer,size_t length);
void test_encoder_push(bert_encoder_t *encoder,const bert_data_t *data);

#endif
