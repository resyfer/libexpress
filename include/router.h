/**
 * @file router.h
 * @brief This contains the router functions.
 *
 * It contains and imports all the functions of
 * router that are made available.
 *
 * @author Saurav Pal (resyfer)
 * @bug No known bugs
 */

#ifndef __ROUTER_LIBEXPRESS_H
#define __ROUTER_LIBEXPRESS_H 1

/* Personal Libraries */
#include <libvector/vector.h>

/* Header Includes */
#include "controller.h"
#include "util.h"

/**
 * @brief Port number
 */
typedef u_int16_t port_t;

/**
 * @brief Route
 *
 * It is the route for a particular method.
 * It just contains the list of all the controllers
 * falling in the route.
 */
typedef struct {
	vector_t *controllers;
} route_t;

/**
 * @brief A macro that defines the end of the controllers in a router.
 *
 * It defines the end of the controllers in a route and is to be used as a
 * parameter after the final controller.
 */
#define ROUTE_END NULL

/**
 * @brief Router
 *
 * It is the router for a particular path.
 * It also contains the routers of children according
 * to the path.
 *
 * eg. If the router has a path /hello, then the new router
 * with path /hello/world will be a child of the original
 * with path /world.
 */
typedef struct {
	char *path;
	hmap_t *routes; /* Of type route_t */
	hmap_t *child_routers; /* Of type router_t */
} router_t;
typedef router_t server_t;

// TODO:
// router_t* router_new(server_t *server, char *path);
// void router_use(router_t *router, char *path, middleware_t middleware);

/**
 * @brief Finds the router most appropriate for serving request
 *
 * @param router Router
 * @param req Request Instance
 * @return route_t* Route
 */
route_t* find_route(router_t *router, req_t *req);

/**
 * @brief Assign Route for a specific method for a specific path
 *
 * @param router Router instance, can be server instance as well
 * @param path Path for router, only part of it which should be it's path if the
 * 			   tree of router hierarchy is maintained
 * @param method The method of the HTTP requests
 * @param ... Any number of controllers or middlewares of the type `controller_t*`
 */
void route(router_t *router, const char *path, char* method, ...);

/**
 * @brief Assign a route for GET request for a router.
 *
 * @param router Router instance, can be server instance as well
 * @param path Path for router, only part of it which should be it's path if the
 * 			   tree of router hierarchy is maintained
 * @param ... Any number of controllers or middlewares of the type `controller_t*`
 */
void route_get(router_t *router, const char *path, ...);


/**
 * @brief Assign a route for POST request for a router.
 *
 * @param router Router instance, can be server instance as well
 * @param path Path for router, only part of it which should be it's path if the
 * 			   tree of router hierarchy is maintained
 * @param ... Any number of controllers or middlewares of the type `controller_t*`
 */
void route_post(router_t *router, const char *path, ...);

/**
 * @brief Assign a route for PUT request for a router.
 *
 * @param router Router instance, can be server instance as well
 * @param path Path for router, only part of it which should be it's path if the
 * 			   tree of router hierarchy is maintained
 * @param ... Any number of controllers or middlewares of the type `controller_t*`
 */
void route_put(router_t *router, const char *path, ...);

/**
 * @brief Assign a route for DELETE request for a router.
 *
 * @param router Router instance, can be server instance as well
 * @param path Path for router, only part of it which should be it's path if the
 * 			   tree of router hierarchy is maintained
 * @param ... Any number of controllers or middlewares of the type `controller_t*`
 */
void route_delete(router_t *router, const char *path, ...);

#endif