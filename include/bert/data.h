#ifndef _BERT_DATA_H_
#define _BERT_DATA_H_

#include <bert/types.h>
#include <bert/list.h>
#include <bert/dict.h>
#include <bert/regex.h>

#include <stdint.h>
#include <time.h>

/*
 * BERT data types
 */
typedef enum
{
	bert_data_none = 0,
	bert_data_boolean,
	bert_data_int,
	bert_data_float,
	bert_data_atom,
	bert_data_string,
	bert_data_tuple,
	bert_data_list,
	bert_data_dict,
	bert_data_bin,
	bert_data_time,
	bert_data_regex,
	bert_data_nil
} bert_data_type;

struct bert_data;
typedef struct bert_data bert_data_t;

/*
 * BERT data structure
 */
struct bert_data
{
	bert_data_type type;

	union
	{
		unsigned int boolean;
		int64_t integer;
		double floating_point;

		struct
		{
			bert_atom_size_t length;
			char *name;
		} atom;

		struct
		{
			bert_string_size_t length;
			char *text;
		} string;

		struct
		{
			bert_bin_size_t length;
			unsigned char *data;
		} bin;

		struct
		{
			bert_tuple_size_t length;
			bert_data_t **elements;
		} tuple;

		struct bert_list *list;
		struct bert_dict *dict;

		time_t time;
		
		struct
		{
			bert_regex_size_t length;
			char *source;

			int options;
		} regex;
	};
};

/*
 * Allocates a new bert_data_t with the type of bert_data_none.
 */
extern bert_data_t * bert_data_create();

/*
 * Allocates a new bert_data_t with the type of bert_data_nil.
 */
extern bert_data_t * bert_data_create_nil();

/*
 * Allocates a new bert_data_t wtih the type of bert_data_boolean
 * and boolean value of 1.
 */
extern bert_data_t * bert_data_create_true();

/*
 * Allocates a new bert_data_t with the type of bert_data_boolean
 * and boolean value of 0.
 */
extern bert_data_t * bert_data_create_false();

/*
 * Allocates a new bert_data_t with the type of bert_data_int and the
 * given integer value.
 */
extern bert_data_t * bert_data_create_int(int64_t i);

/*
 * Allocates a new bert_data_t with the type of bert_data_float
 * and the given floating point value.
 */
extern bert_data_t * bert_data_create_float(double f);

/*
 * Allocates a new bert_data_t with the type of bert_data_atom,
 * and allocates an empty character array of the given length.
 */
extern bert_data_t * bert_data_create_empty_atom(bert_atom_size_t length);

/*
 * Allocates a new bert_data_t with the type of bert_data_atom,
 * and uses a copy of the given name as the value of the BERT atom.
 */
extern bert_data_t * bert_data_create_atom(const char *name);

/*
 * Allocates a new bert_data_t with the type of bert_data_string,
 * and allocates an empty character array of the given length.
 */
extern bert_data_t * bert_data_create_empty_string(bert_string_size_t length);

/*
 * Allocates a new bert_data_t with the type of bert_data_string
 * and uses a copy of the given text as the value of the BERT atom.
 */
extern bert_data_t * bert_data_create_string(const char *text);

/*
 * Allocates a new bert_data_t with the type of bert_data_tuple,
 * and allocates an empty array of bert_data_t pointers of the given
 * length.
 */
extern bert_data_t * bert_data_create_tuple(bert_tuple_size_t length);

/*
 * Allocates a new bert_data_t with the type of bert_data_list and
 * initializes the list field of the bert_data_t.
 */
extern bert_data_t * bert_data_create_list();

/*
 * Allocates a new bert_data_t with the type of bert_data_dict and
 * initializes the dict field of the bert_data_t.
 */
extern bert_data_t * bert_data_create_dict();

/*
 * Allocates a new bert_data_t with the type of bert_data_time and with
 * the given timestamp.
 */
extern bert_data_t * bert_data_create_time(time_t timestamp);

/*
 * Allocates a new bert_data_t with the type of bert_data_regex,
 * the given Regular Expression source and PCRE options.
 */
extern bert_data_t * bert_data_create_regex(const char *source,bert_regex_size_t length,int options);

/*
 * Allocates a new bert_data_t with the type of bert_data_bin,
 * and allocates an empty unsigned character array of the given length.
 */
extern bert_data_t * bert_data_create_empty_bin(bert_bin_size_t length);

/*
 * Allocates a new bert_data_t with the type of bert_data_bin,
 * and uses a copy of the binary data as the binary value of the BERT
 * binary data.
 */
extern bert_data_t * bert_data_create_bin(const unsigned char *data,bert_bin_size_t length);

/*
 * Returns the required space in bytes needed to encode the given
 * bert_data_t.
 */
extern size_t bert_data_sizeof(const bert_data_t *data);

/*
 * Compares the string, atom or binary data with the given string.
 * Returns -1, 0 or 1 if the bert_data is less than, equal to or
 * greater than the given string, respectively.
 */
extern int bert_data_strequal(const bert_data_t *data,const char *str);

/*
 * Destroys a previously allocated bert_data_t.
 */
extern void bert_data_destroy(bert_data_t *data);

#endif
