#include <include/util.h>
#include <stdio.h>

void
print_info(const char *msg, color_t color)
{
	char *text_col_bold = col_str_style(color, BOLD);
	printf("%s%s%s", text_col_bold, msg, RESET);
	free(text_col_bold);
}

void
print_ok(const char *msg)
{
	print_info(msg, GREEN);
}

void
print_warning(const char *warning)
{
	print_info(warning, YELLOW);
}

void
print_error(const char *err)
{
	print_info(err, RED);
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