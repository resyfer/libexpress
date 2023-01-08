#ifndef __ROUTER_LIBEXPRESS_H
#define __ROUTER_LIBEXPRESS_H

#include <sys/types.h>
#include <stdarg.h>
#include <libvector/vector.h>

#include "controller.h"
#include "util.h"

#define ROOT "/"

typedef u_int16_t port_t;

typedef struct {
	vector_t *controllers;
} route_t;
#define ROUTE_END NULL

typedef struct {
	char *path;
	hmap_t *routes; /* Of type route_t */
	hmap_t *child_routers; /* Of type router_t */
} router_t;

typedef router_t server_t;

// TODO:
// router_t* router_new(server_t *server, char *path);
// void router_use(router_t *router, char *path, middleware_t middleware);

route_t* find_route(router_t *router, req_t *req);

void route(router_t *router, const char *path, char* method, ...);
void route_get(router_t *router, const char *path, ...);
void route_post(router_t *router, const char *path, ...);
void route_put(router_t *router, const char *path, ...);
void route_delete(router_t *router, const char *path, ...);

#endif