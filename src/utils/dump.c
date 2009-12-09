#include <bert/decoder.h>
#include <bert/errno.h>

#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>

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

	puts("<<");

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

	puts(">>");
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

			puts(", ");
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
			puts(", ");
		}
	}

	putchar(']');
	return 0;
}

int bert_print_dict(const bert_data_t *data)
{
	bert_dict_node_t *dict_node = data->dict->head;

	puts("{bert, dict, [");

	while (dict_node)
	{
		putchar('{');

		if (bert_print(dict_node->key) == -1)
		{
			return -1;
		}

		puts(", ");

		if (bert_print(dict_node->value) == -1)
		{
			return -1;
		}

		putchar('}');

		if ((dict_node = dict_node->next))
		{
			puts(", ");
		}
	}

	puts("]}");
	return 0;
}

void bert_print_time(const bert_data_t *data)
{
	printf("{bert, time, %u, %u, %u}",((unsigned int)data->time / 1000000),(unsigned int)data->time,0);
}

void bert_print_regex(const bert_data_t *data)
{
	printf("{bert, regex, <<%s>>, []}",data->regex.source);
}

int bert_print(const bert_data_t *data)
{
	switch (data->type)
	{
		case bert_data_boolean:
			if (data->boolean)
			{
				puts("true");
			}
			else
			{
				puts("false");
			}
		case bert_data_int:
			printf("%d",data->integer);
			break;
		case bert_data_float:
			printf("%f",data->floating_point);
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
		fprintf(stderr,"bert_dump: %s\n",bert_strerror(BERT_ERRNO_MALLOC));
		return -1;
	}

	size_t count;
	unsigned char buffer[512];

	while ((count = read(fd,buffer,sizeof(buffer))) > 0)
	{
		if (bert_decoder_push(decoder,buffer,count) == BERT_ERRNO_MALLOC)
		{
			fprintf(stderr,"bert_dump: %s\n",bert_strerror(BERT_ERRNO_MALLOC));
			bert_decoder_destroy(decoder);
			return -1;
		}
	}

	bert_data_t *next_data;

	while (1)
	{
		result = bert_decoder_next(decoder,&next_data);

		switch (result)
		{
			case 1:
				bert_print(next_data);
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
		int i;
		int fd;

		for (i=2;i<argc;i++)
		{
			if ((fd=open(argv[i],O_RDONLY)))
			{
			}

			if (bert_dump(fd) == -1)
			{
				return -1;
			}

			close(fd);
		}
	}

	return bert_dump(STDIN_FILENO);
}
