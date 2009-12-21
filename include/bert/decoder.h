#ifndef _BERT_DECODER_H_
#define _BERT_DECODER_H_

#include <bert/data.h>
#include <bert/types.h>

#include <sys/types.h>

typedef ssize_t (*bert_read_func)(unsigned char *dest,size_t length,void *data);

#define BERT_SHORT_BUFFER	512

struct bert_decoder
{
	bert_mode mode;
	union
	{
		int stream;

		struct
		{
			bert_read_func ptr;
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

/*
 * Allocates a new bert_decoder_t and sets the mode to bert_mode_none.
 */
bert_decoder_t * bert_decoder_create();

/*
 * Sets the mode of the given decoder to bert_mode_stream, and uses the given
 * file descriptor as the stream to read BERT encoded data from.
 */
void bert_decoder_stream(bert_decoder_t *decoder,int fd);

/*
 * Sets the mode of the given decoder to bert_mode_callback, and uses the
 * given bert_read_func as the callback to read BERT encoded data from.
 */
void bert_decoder_callback(bert_decoder_t *decoder,bert_read_func callback,void *data);

/*
 * Sets the mode of the given decoder to bert_mode_buffer, and uses the
 * given buffer as the BERT encoded data to decode.
 */
void bert_decoder_buffer(bert_decoder_t *decoder,const unsigned char *buffer,size_t length);

/*
 * Reads BERT encoded data from the decoder and attempts to decode it.
 * Points the given data_ptr to the newly decoded bert_data_t.
 * Returns 1 when a bert_data_t has been decoded.
 * Returns 0 when there is no more data to be decoded.
 * Returns BERT_ERRNO_INVALID when invalid BERT data is encountered.
 * Returns BERT_ERRNO_SHORT_READ when there is not enough data left.
 * Returns BERT_ERRNO_MALLOC when a call to malloc fails.
 */
extern int bert_decoder_pull(bert_decoder_t *decoder,bert_data_t **data_ptr);

/*
 * Destroys a previously allocated bert_decoder_t.
 */
extern void bert_decoder_destroy(bert_decoder_t *decoder);

#endif
