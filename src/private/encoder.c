#include "encoder.h"
#include <bert/errno.h>

#include <unistd.h>
#include <string.h>

int bert_encoder_write(bert_encoder_t *encoder,const unsigned char *data,size_t length)
{
	switch (encoder->mode)
	{
		case bert_mode_stream:
			if (write(encoder->stream,data,length) < 0)
			{
				return BERT_ERRNO_WRITE;
			}
			break;
		case bert_mode_buffer:
			if ((encoder->buffer.index + length) > encoder->buffer.length)
			{
				return BERT_ERRNO_SHORT;
			}

			memcpy(encoder->buffer.ptr+encoder->buffer.index,data,sizeof(unsigned char)*length);
			encoder->buffer.index += length;
			break;
		case bert_mode_callback:
			if (encoder->callback.ptr(data,length,encoder->callback.data) < 0)
			{
				return BERT_ERRNO_INVALID;
			}
			break;
		default:
			return BERT_ERRNO_INVALID;
	}

	return BERT_SUCCESS;
}
