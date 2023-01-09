#ifndef __LIBEXPRESS_RES_H
#define __LIBEXPRESS_RES_H 1

#include <stdbool.h>
#include <libhmap/hmap.h>

#ifndef __BASE__
#define __BASE__ ""
#endif

typedef struct {
	int status;
	hmap_t *headers;
	char *body;
	bool sent;
	bool next; // To transfer control to the next middleware
	int client;
} res_t;

void status_codes_init(void);
void mime_init(void);

void res_send(res_t *res, char* body);
void res_send_file(res_t *res, char *path);

/* Utility */
void set_res_body(res_t* res, char* body);
void set_res_header(res_t* res, const char* header, char* value);
char* get_res_header(res_t* res, const char* header);

#endif