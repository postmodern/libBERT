#include <bert/encoder.h>

#include <malloc.h>

bert_encoder_t * bert_encoder_create()
{
	bert_encoder_t *new_encoder;

	if (!(new_encoder = malloc(sizeof(bert_encoder_t))))
	{
		// malloc failed
		return NULL;
	}

	new_encoder->mode = bert_mode_none;
	new_encoder->wrote_magic = 0;

	return new_encoder;
}

void bert_encoder_stream(bert_encoder_t *encoder,int stream)
{
	encoder->mode = bert_mode_stream;
	encoder->stream = stream;
}

void bert_encoder_buffer(bert_encoder_t *encoder,unsigned char *buffer,size_t length)
{
	encoder->mode = bert_mode_buffer;
	encoder->buffer.ptr = buffer;
	encoder->buffer.length = length;
}

void bert_encoder_callback(bert_encoder_t *encoder,bert_write_func callback,void *data)
{
	encoder->mode = bert_mode_callback;
	encoder->callback.ptr = callback;
	encoder->callback.data = data;
}

void bert_encoder_destroy(bert_encoder_t *encoder)
{
	free(encoder);
}
