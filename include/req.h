/**
 * @file req.h
 * @brief This contains all the request related material.
 *
 * @author Saurav Pal (resyfer)
 * @bug No known bugs
 */
#ifndef __LIBEXPRESS_REQ_H
#define __LIBEXPRESS_REQ_H 1

/* Personal Libraries */
#include <libhmap/hmap.h>
#include <libqueue/queue.h>

/**
 * @brief Request made by client
 *
 * This structure contains all the information received in the request
 * from the client.
 */
typedef struct {
	char *method;
	char *path;
	hmap_t *queries; // queries like /hello?query1=value1&query2=value2
	hmap_t *headers;
	hmap_t *params; // params like /person/1234abcde
	char *body;
	queue_t *c_queue; //controller queue, includes middlewares as well
} req_t;

/**
 * @brief Parses the HTTP request string sent by client.
 *
 * It parses the HTTP request string sent by the client and gives
 * a `req_t` Request that contains all the information from the
 * request in a meaningful manner.
 *
 * @param req_str HTTP Request String of the client
 * @param size No. of bytes (string length) of the request string
 * @return req_t* Request
 */
req_t* parse_req(char *req_str, int size);

/* Utility */

/**
 * @brief Get the body of request
 *
 * @param req Request
 * @return char* Body
 */
char* get_req_body(req_t* req);

/**
 * @brief Get a particular header from the request.
 *
 * @param req Request
 * @param header Header key
 * @return char* Header value
 */
char* get_req_header(req_t* req, const char* header);

/**
 * @brief Get the method of the request
 *
 * @param req Request
 * @return char* Method
 */
char* get_req_method(req_t* req);

/**
 * @brief Get the value of a particular request path parameter
 *
 * The key (`param`) is set according to the route you're calling this
 * function in. If the route is for path /hello/:id/world, then something
 * like /hello/abcd/world will populate the param "id" with the value "abcd".
 *
 * @param req Request
 * @param param Name of the param, as used by the route.
 * @return char*
 */
char* get_req_param(req_t* req, const char* param);

/**
 * @brief Get the path of the request
 *
 * It gives the path requested by the client, minus any queries.
 *
 * If the request by client is for `/hello?search=world&foo=bar` then
 * the path is /hello
 *
 * @param req Request
 * @return char* Path
 */
char* get_req_path(req_t* req);

/**
 * @brief Get a particular query from the request.
 *
 * If the request by client is for `/hello?search=world&foo=bar` then
 * the queries are as follows:
 *
 * search = world
 * foo = bar
 *
 * in the form of `key = value`
 *
 * @param req
 * @param query
 * @return char*
 */
char* get_req_query(req_t* req, const char* query);

// TODO: Add instructions in docs for iterating through maps

#endif