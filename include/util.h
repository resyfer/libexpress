#ifndef __LIBEXPRESS_UTIL_H
#define __LIBEXPRESS_UTIL_H

#include <libcol/col.h>

void print_ok(const char *msg);
void print_warning(const char *warning);
void print_error(const char *err);

char* tokenize(char* s, char c);

#endif