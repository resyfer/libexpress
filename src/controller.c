#include <include/controller.h>
#include <include/util.h>

void
next(res_t *res)
{
	if(res->sent) {
		error("Can not use `next` if response is already sent");
	}

	res->next = true;
}