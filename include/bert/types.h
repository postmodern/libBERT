#ifndef _BERT_TYPES_H_
#define _BERT_TYPES_H_

#include <stdint.h>

typedef uint8_t bert_magic_t;

typedef uint32_t bert_string_size_t;
typedef uint16_t bert_atom_size_t;

typedef uint32_t bert_bin_size_t;
typedef bert_bin_size_t bert_regex_size_t;

typedef uint32_t bert_tuple_size_t;
typedef uint32_t bert_list_size_t;

typedef enum
{
	bert_mode_none = 0,
	bert_mode_streaming,
	bert_mode_callback,
	bert_mode_buffer
} bert_mode;

#endif
