#include "regex.h"

#include <string.h>

const struct bert_regex_option bert_regex_options[] = {
	{"unicode", BERT_REGEX_UNICODE},
	{"anchored", BERT_REGEX_ANCHORED},
	{"caseless", BERT_REGEX_CASELESS},
	{"dollar_endonly", BERT_REGEX_DOLLAR_ENDONLY},
	{"dotall", BERT_REGEX_DOTALL},
	{"extended", BERT_REGEX_EXTENDED},
	{"firstline", BERT_REGEX_FIRSTLINE},
	{"multiline", BERT_REGEX_MULTILINE},
	{"no_auto_capture", BERT_REGEX_NO_AUTO_CAPTURE},
	{"dupnames", BERT_REGEX_NO_AUTO_CAPTURE},
	{"ungreedy", BERT_REGEX_UNGREEDY},
	{"bsr_any_crlf", BERT_REGEX_BSR_ANYCRLF},
	{"bsr_unicode", BERT_REGEX_BSR_UNICODE},
	{NULL, 0}
};

unsigned int bert_regex_optmask(const char *name)
{
	const char *opt_name;
	unsigned int i = 0;

	while ((opt_name = bert_regex_options[i].name))
	{
		if (!strcmp(name,opt_name))
		{
			return bert_regex_options[i].mask;
		}

		++i;
	}

	return 0;
}

const char * bert_regex_optname(unsigned int mask)
{
	unsigned int opt_mask;
	unsigned int i = 0;

	while ((opt_mask = bert_regex_options[i].mask))
	{
		if (mask == opt_mask)
		{
			return bert_regex_options[i].name;
		}

		++i;
	}

	return NULL;
}
