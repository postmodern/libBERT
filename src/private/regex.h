#ifndef _BERT_REGEX_PRIVATE_H_
#define _BERT_REGEX_PRIVATE_H_

#include <bert/regex.h>

struct bert_regex_option
{
	const char *name;
	unsigned int mask;
};

unsigned int bert_regex_optmask(const char *name);
const char * bert_regex_optname(unsigned int mask);

#endif
