/**
 * @file util.h
 * @brief This contains all the utilities used through the library.
 *
 * @author Saurav Pal (resyfer)
 * @bug No known bugs
 */
#ifndef __LIBEXPRESS_UTIL_H
#define __LIBEXPRESS_UTIL_H 1

#include <libcol/col.h>

/**
 * @brief An internal function used by error, warning and success to print
 * 		  the messages, along with necessary elements like line number or file name
 * 		  and even the option to exit out of the process with an exit(1)
 *
 * Please don't use this outside of util.c
 */
void print_info(const char *msg, color_t color, bool exit_process, bool print_line, const char* file, int line);

/**
 * @brief Macros to print success, warning and error in colors and bold as
 *		  well exit the process if required.
 */

#define success(msg) print_info(msg, GREEN, false, false, NULL, 0);
#define warning(msg) print_info(msg, YELLOW, false, true, __FILE__, __LINE__);
#define error(msg) print_info(msg, RED, true, true, __FILE__, __LINE__);

/**
 * @brief This splits the string s at the first occurence of character c in s.
 *
 * Then s becomes the original string till that character, while the returned
 * value points to the string that was split from it. It can be used like this:
 *
 * char s[] = "hello world foo bye";
 *
 * char *k = tokenize(s, ' ');
 *
 * now s is "hello" while k is "world foo bye"
 *
 * @param s
 * @param c
 * @return char*
 */
char* tokenize(char* s, char c);

#endif