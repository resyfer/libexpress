#ifndef __LIBEXPRESS_REQ_H
#define __LIBEXPRESS_REQ_H 1

#include <libhmap/hmap.h>
#include <libqueue/queue.h>

typedef struct {
	char *method;
	char *path;
	hmap_t *queries;
	hmap_t *headers;
	hmap_t *params;
	char *body;
	queue_t *c_queue; //controller queue, includes middlewares as well
} req_t;

req_t* parse_req(char *req_str, int size);
char* get_param(req_t* req, char* param);

#endif