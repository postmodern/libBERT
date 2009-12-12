#ifndef _BERT_ENCODER_H_
#define _BERT_ENCODER_H_

#include <bert/types.h>
#include <bert/data.h>

#include <sys/types.h>

typedef ssize_t (*bert_write_func)(const unsigned char *data,size_t length,void *user_data);

struct bert_encoder
{
	bert_mode mode;

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

bert_encoder_t * bert_encoder_create();

void bert_encoder_stream(bert_encoder_t *encoder,int stream);
void bert_encoder_buffer(bert_encoder_t *encoder,unsigned char *buffer,size_t length);
void bert_encoder_callback(bert_encoder_t *encoder,bert_write_func callback,void *data);

size_t bert_encoder_sizeof(const bert_data_t *data);
void bert_encoder_destroy(bert_encoder_t *encoder);

#endif
