/**
 * @file res.h
 * @brief This contains all the response related material.
 *
 * @author Saurav Pal (resyfer)
 * @bug No known bugs
 */
#ifndef __LIBEXPRESS_RES_H
#define __LIBEXPRESS_RES_H 1

/* Personal libraries */
#include <libhmap/hmap.h>

/* libc */
#include <stdbool.h>

/**
 * @brief Base from where files are looked up
 */
#ifndef __BASE__
#define __BASE__ ""
#endif

/**
 * @brief Response being sent to the client
 */
typedef struct {
	u_int16_t status;
	hmap_t *headers;
	const char *body;
	bool sent; // To check if response has already been sent
	int client; // To identify who to send the response to
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

/**
 * @brief Send response to client.
 *
 * This sends the response to the client (gets the client ID through res->client)
 * and sets the res->sent to true. It prevents sending a response if res->sent is
 * already true.
 *
 * @param res Response
 */
void res_send(res_t *res);

/**
 * @brief Send a file to the client.
 *
 * It sends the file at the specified path to the client. The usage can be like
 * the following
 *
 * #undef __BASE__
 * #define __BASE__ "/home/user/server"
 *
 * res_send_file(res, __BASE__ "/index.html");
 *
 * @param res Response
 * @param path Absolute path of the file.
 */
void res_send_file(res_t *res, const char *path);

/* Utility */

/**
 * @brief Set the body of the response.
 *
 * @param res Response
 * @param body Body to send to client.
 */
void set_res_body(res_t* res, char* body);

/**
 * @brief Set the status code of the response.
 *
 * @param res Response
 * @param status Status Code for HTTP Response
 */
void set_res_status(res_t* res, u_int16_t status);

/**
 * @brief Set a particular header of the response.
 *
 * It sets a `header` to the given `value` in the response sent by the client
 * and any case of setting it multiple times will lead to the latest one being
 * considered.
 *
 * @param res Response
 * @param header Header name
 * @param value Value
 */
void set_res_header(res_t* res, const char* header, char* value);

/**
 * @brief Get the value of a particular header of the response.
 *
 * @param res Response
 * @param header Header name
 * @return char* Value of header in the response.
 */
char* get_res_header(res_t* res, const char* header);

#endif