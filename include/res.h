#ifndef __LIBEXPRESS_RES_H
#define __LIBEXPRESS_RES_H 1

#include <stdbool.h>
#include <libhmap/hmap.h>

#ifndef __BASE__
#define __BASE__ ""
#endif

typedef struct {
	u_int16_t status;
	hmap_t *headers;
	char *body;
	bool sent;
	bool next; // To transfer control to the next middleware
	int client;
} res_t;

/**
 * @brief Initialize the `status_codes` hmap.
 *
 * The `status_codes` is initialized to contain all of the status
 * codes as well as their message.
 */
void status_codes_init(void);

/**
 * @brief Initialize the `mime_types` hmap.
 *
 * The `mime_types` is initialized to contain all of the popular
 * extensions and their mime-types.
 */
void mime_init(void);

void res_send(res_t *res, char* body);
void res_send_file(res_t *res, char *path);

/* Utility */
void set_res_body(res_t* res, char* body);
void set_res_status(res_t* res, u_int16_t status);
void set_res_header(res_t* res, const char* header, char* value);
char* get_res_header(res_t* res, const char* header);

#endif