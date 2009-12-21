#include <bert/decoder.h>
#include <bert/errno.h>
#include <bert.h>
#include "../private/regex.h"

#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int bert_print(const bert_data_t *data);

void bert_print_string(const bert_data_t *data)
{
	size_t length = data->string.length;
	unsigned int i;
	char c;

	putchar('\"');

	for (i=0;i<length;i++)
	{
		c = data->string.text[i];

		if (isprint(c))
		{
			putchar(c);
		}
		else
		{
			printf("\\x%.2x",c);
		}
	}

	putchar('\"');
}

void bert_print_binary(const bert_data_t *data)
{
	size_t length = data->bin.length;
	unsigned int i;
	unsigned char b;

	printf("<<");

	for (i=0;i<length;i++)
	{
		b = data->bin.data[i];

		if (isprint(b))
		{
			putchar(b);
		}
		else
		{
			printf("\\x%.2x",b);
		}
	}

	printf(">>");
}

int bert_print_tuple(const bert_data_t *data)
{
	size_t length = data->tuple.length;

	putchar('{');

	if (length > 1)
	{
		unsigned int i;

		for (i=0;i<length-1;i++)
		{
			if (bert_print(data->tuple.elements[i]) == -1)
			{
				return -1;
			}

			printf(", ");
		}
	}

	if (length)
	{
		if (bert_print(data->tuple.elements[length - 1]) == -1)
		{
			return -1;
		}
	}

	putchar('}');
	return 0;
}

int bert_print_list(const bert_data_t *data)
{
	bert_list_node_t *list_node = data->list->head;

	putchar('[');

	while (list_node)
	{
		if (bert_print(list_node->data) == -1)
		{
			return -1;
		}

		if ((list_node = list_node->next))
		{
			printf(", ");
		}
	}

	putchar(']');
	return 0;
}

int bert_print_dict(const bert_data_t *data)
{
	bert_dict_node_t *dict_node = data->dict->head;

	printf("{bert, dict, [");

	while (dict_node)
	{
		putchar('{');

		if (bert_print(dict_node->key) == -1)
		{
			return -1;
		}

		printf(", ");

		if (bert_print(dict_node->value) == -1)
		{
			return -1;
		}

		putchar('}');

		if ((dict_node = dict_node->next))
		{
			printf(", ");
		}
	}

	printf("]}");
	return 0;
}

void bert_print_time(const bert_data_t *data)
{
	printf("{bert, time, %u, %u, %u}",((unsigned int)data->time / 1000000),(unsigned int)data->time,0);
}

void bert_print_regex(const bert_data_t *data)
{
	printf("{bert, regex, <<%s>>, [",data->regex.source);

	unsigned int options = data->regex.options;
	unsigned int bit_index = 0;

	unsigned int mask;
	const char *name;

	while (options)
	{
		mask = ((options & 0x01) << bit_index);

		options >>= 1;
		++bit_index;

		if (mask && (name = bert_regex_optname(mask)))
		{
			if (options)
			{
				printf("%s, ",name);
			}
			else
			{
				printf("%s",name);
			}
		}
	}

	printf("]}");
}

int bert_print(const bert_data_t *data)
{
	switch (data->type)
	{
		case bert_data_nil:
			printf("nil");
			break;
		case bert_data_boolean:
			if (data->boolean)
			{
				printf("true");
			}
			else
			{
				printf("false");
			}
			break;
		case bert_data_int:
#if (__WORDISZE == 64 || defined(__GLIBC_HAVE_LONG_LONG))
			printf("%ld",data->integer);
#else
			printf("%lld",data->integer);
#endif
			break;
		case bert_data_float:
			printf("%15.15lf",data->floating_point);
			break;
		case bert_data_atom:
			printf("%s",data->atom.name);
			break;
		case bert_data_string:
			bert_print_string(data);
			break;
		case bert_data_tuple:
			return bert_print_tuple(data);
		case bert_data_list:
			return bert_print_list(data);
		case bert_data_bin:
			bert_print_binary(data);
			break;
		case bert_data_dict:
			return bert_print_dict(data);
		case bert_data_time:
			bert_print_time(data);
			break;
		case bert_data_regex:
			bert_print_regex(data);
			break;
		default:
			fprintf(stderr,"bert_dump: unknown bert data type %u\n",data->type);
			return -1;
	}

	return 0;
}

int bert_dump(int fd)
{
	int result;
	bert_decoder_t *decoder;

	if (!(decoder = bert_decoder_create()))
	{
		fprintf(stderr,"bert_dump: malloc failed\n");
		return -1;
	}

	bert_decoder_stream(decoder,fd);

	bert_data_t *next_data;

	while (1)
	{
		result = bert_decoder_pull(decoder,&next_data);

		switch (result)
		{
			case 1:
				bert_print(next_data);
				putchar('\n');
				break;
			case 0:
				return 0;
			default:
				fprintf(stderr,"bert_dump: %s\n",bert_strerror(result));
				return -1;
		}

		bert_data_destroy(next_data);
	}

	return 0;
}

int main(int argc,const char **argv)
{
	if (argc >= 2)
	{
		if (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help"))
		{
			printf("usage: bert_dump [FILES ...]\n");
			return 0;
		}

		if (!strcmp(argv[1],"-V") || !strcmp(argv[1],"--version"))
		{
			printf("bert_dump %s\n",bert_version());
			return 0;
		}

		int i;
		int fd;

		for (i=1;i<argc;i++)
		{
			if ((fd = open(argv[i],O_RDONLY)) == -1)
			{
				fprintf(stderr,"bert_dump: %s\n",strerror(errno));
				return -1;
			}

			if (bert_dump(fd) == -1)
			{
				close(fd);
				return -1;
			}

			close(fd);
		}

		return 0;
	}

	return bert_dump(STDIN_FILENO);
}
