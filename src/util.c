#include <include/util.h>
#include <stdio.h>

void
print_info(const char *msg, color_t color, bool exit_process, bool print_line, const char* file, int line)
{
	char *text_col_bold = col_str_style(color, BOLD);

	// Can't see how to use a ternary operator on an integer (__LINE__)
	if(print_line) {
		printf("\n%s%s Line %d: %s%s\n",
			text_col_bold,
			file,
			line,
			msg,
			RESET);
	} else {
		printf("%s%s%s", text_col_bold, msg, RESET);
	}

	free(text_col_bold);

	if(exit_process) {
		exit(1);
	}
}

// This attempts to modify the string, so please do not use string literals
char*
tokenize(char* s, char c)
{
	char *k = s;
	for(k; *k != c && *k != 0; k++);

	if(*k) {
		*k = 0;
		k++;
	}
	return k;
}