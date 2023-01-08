#include <include/controller.h>
#include <include/util.h>

void
next(res_t *res)
{
	if(res->sent) {
		print_error("Can not use `next` if response is already sent");
		exit(1);
	}

	res->next = true;
}