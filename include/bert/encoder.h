#ifndef _BERT_ENCODER_H_
#define _BERT_ENCODER_H_

#include <bert/types.h>
#include <bert/data.h>

#include <sys/types.h>

typedef ssize_t (*bert_write_func)(const unsigned char *data,size_t length,void *user_data);

struct bert_encoder
{
	bert_mode mode;
	unsigned int wrote_magic;

	union
	{
		int stream;

		struct
		{
			unsigned char *ptr;
			size_t length;

			unsigned int index;
		} buffer;

		struct
		{
			bert_write_func ptr;
			void *data;
		} callback;
	};
};
typedef struct bert_encoder bert_encoder_t;

/*
 * Allocates a new bert_encoder_t.
 */
bert_encoder_t * bert_encoder_create();

/*
 * Sets the mode of the given encoder to bert_mode_stream, and uses the
 * given file descriptor as the stream to write BERT encoded data to.
 */
void bert_encoder_stream(bert_encoder_t *encoder,int stream);

/*
 * Sets the mode of the given encoder to bert_mode_buffer, and uses the
 * given buffer to write BERT encoded data to.
 */
void bert_encoder_buffer(bert_encoder_t *encoder,unsigned char *buffer,size_t length);

/*
 * Sets the mode of the given encoder to bert_mode_callback, and uses the
 * given callback to write BERT encoded data to.
 */
void bert_encoder_callback(bert_encoder_t *encoder,bert_write_func callback,void *data);

/*
 * Encodes the given bert_data_t and writes it to the encoder.
 * Returns BERT_SUCCESS on success.
 */
int bert_encoder_push(bert_encoder_t *encoder,const bert_data_t *data);

/*
 * Destroys a previously allocated bert_encoder_t.
 */
void bert_encoder_destroy(bert_encoder_t *encoder);

#endif
