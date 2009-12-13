#ifndef _TEST_H_
#define _TEST_H_

#include <bert/decoder.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void test_fail(const char *mesg,...);
int test_open_file(const char *path);
bert_decoder_t * test_decoder();

#endif
