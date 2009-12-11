#ifndef _BERT_DECODER_H_
#define _BERT_DECODER_H_

#include <bert/data.h>
#include <bert/buffer.h>
#include <bert/types.h>

#include <sys/types.h>

typedef ssize_t (*bert_decoder_func)(unsigned char *dest,size_t length,void *data);

#define BERT_SHORT_BUFFER	(BERT_CHUNK_SIZE * 2)

struct bert_decoder
{
	bert_mode mode;
	union
	{
		int stream;

		struct
		{
			bert_decoder_func ptr;
			void *data;
		} callback;
		
		struct
		{
			const unsigned char *ptr;
			size_t length;

			unsigned int index;
		} buffer;
	};

	size_t short_length;
	unsigned int short_index;

	unsigned char short_buffer[BERT_SHORT_BUFFER];
};
typedef struct bert_decoder bert_decoder_t;

bert_decoder_t * bert_decoder_create();

void bert_decoder_stream(bert_decoder_t *decoder,int fd);
void bert_decoder_callback(bert_decoder_t *decoder,bert_decoder_func callback,void *data);
void bert_decoder_buffer(bert_decoder_t *decoder,const unsigned char *buffer,size_t length);

extern int bert_decoder_empty(const bert_decoder_t *decoder);
extern int bert_decoder_push(bert_decoder_t *decoder,const unsigned char *data,size_t length);
extern int bert_decoder_next(bert_decoder_t *decoder,bert_data_t **data_ptr);

extern void bert_decoder_destroy(bert_decoder_t *decoder);

#endif
