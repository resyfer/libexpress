#ifndef __LIBEXPRESS_UTIL_H
#define __LIBEXPRESS_UTIL_H 1

#include <libcol/col.h>

void print_info(const char *msg, color_t color, bool exit_process, bool print_line, const char* file, int line);

#define success(msg) print_info(msg, GREEN, false, false, NULL, 0);
#define warning(msg) print_info(msg, YELLOW, false, true, __FILE__, __LINE__);
#define error(msg) print_info(msg, RED, true, true, __FILE__, __LINE__);

char* tokenize(char* s, char c);

#endif