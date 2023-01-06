#ifndef __ROUTER_LIBEXPRESS_H
#define __ROUTER_LIBEXPRESS_H

#include <sys/types.h>
#include <libstack/stack.h>

#include "controller.h"
#include "middleware.h"
#include "util.h"

#define ROOT "/"

typedef u_int16_t port_t;

// static char* methods[] = {
// 	"GET",
// 	"HEAD",
// 	"POST",
// 	"PUT",
// 	"DELETE",
// 	"CONNECT",
// 	"OPTIONS",
// 	"TRACE",
// 	0
// };

typedef struct {
	middleware_t *middlewares; // Null terminated array
	controller_t controller;
} route_t;

typedef struct {
	char *path;
	hmap_t *routes; /* Of type route_t */
	hmap_t *child_routers; /* Of type router_t */
	/* Initially thought of using linked lists for these two, but hmap is better */
} router_t;

typedef router_t server_t;

// router_t* router_new(server_t *server, char *path);

// void router_use(router_t *router, char *path, middleware_t middleware);

route_t* find_route(router_t *router, req_t *req);

void route(router_t *router,
		const char *path,
		const char *method,
		middleware_t *middlewares,
		controller_t controller);

void route_get(router_t *router,
		const char *path,
		middleware_t *middlewares,
		controller_t controller);
void route_post(router_t *router,
		const char *path,
		middleware_t *middlewares,
		controller_t controller);
void route_put(router_t *router,
		const char *path,
		middleware_t *middlewares,
		controller_t controller);
void route_delete(router_t *router,
		const char *path,
		middleware_t *middlewares,
		controller_t controller);

#endif