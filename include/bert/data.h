#ifndef _BERT_DATA_H_
#define _BERT_DATA_H_

#include <sys/types.h>

typedef enum
{
	bert_data_none = 0,
	bert_data_int,
	bert_data_float,
	bert_data_atom,
	bert_data_string,
	bert_data_tupel,
	bert_data_list,
	bert_data_bin,
	bert_data_null
} bert_data_type;

struct bert_list_node
{
	struct bert_data *data;

	struct bert_list_node *next;
};
typedef struct bert_list_node bert_list_node_t;

struct bert_data
{
	bert_data_type type;

	union
	{
		int integer;
		float floating_point;

		struct
		{
			size_t length;
			char *name;
		} atom;

		struct
		{
			size_t length;
			char *text;
		} string;

		struct
		{
			unsigned int length;
			struct bert_data **elements;
		} tupel;

		struct
		{
			struct bert_list_node *head;
			struct bert_list_node *tail;
		} list;

		struct
		{
			size_t length;
			unsigned char *data;
		} bin;
	};
};
typedef struct bert_data bert_data_t;

extern bert_list_node_t * bert_list_node_create();
extern void bert_list_node_destroy(bert_list_node_t *node);

extern bert_data_t * bert_data_create();
extern bert_data_t * bert_data_create_null();
extern bert_data_t * bert_data_create_int(int i);
extern bert_data_t * bert_data_create_float(float f);
extern bert_data_t * bert_data_create_atom(const char *name,size_t length);
extern bert_data_t * bert_data_create_string(const char *name,size_t length);
extern bert_data_t * bert_data_create_tupel(unsigned int length);
extern bert_data_t * bert_data_create_list();
extern bert_data_t * bert_data_create_bin(unsigned char *data,size_t length);

extern void bert_data_destroy(bert_data_t *data);

#endif
