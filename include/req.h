#ifndef __LIBEXPRESS_REQ_H
#define __LIBEXPRESS_REQ_H 1

#include <libhmap/hmap.h>
#include <libstack/stack.h>

typedef struct {
	char *method;
	char *path;
	hmap_t *queries;
	hmap_t *headers;
	hmap_t *params;
	char *body;
	stack_t *middleware_stack;
} req_t;

req_t* parse_req(char *req_str, int size);

#endif