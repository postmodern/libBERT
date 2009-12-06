#include <bert/decoder.h>

void bert_decode_init(bert_decoder_t *decoder,const unsigned char *buffer,size_t buffer_length)
{
	decoder->index = 0;

	decoder->buffer = buffer;
	decoder->buffer_length = buffer_length;
}
