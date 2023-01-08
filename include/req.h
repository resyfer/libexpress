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

/* Utility */
char* get_req_body(req_t* req);
char* get_req_header(req_t* req, const char* header);
char* get_req_method(req_t* req);
char* get_req_param(req_t* req, const char* param);
char* get_req_path(req_t* req);
char* get_req_query(req_t* req, const char* query);

// TODO: Add instructions in docs for iterating through maps

#endif