#include <include/router.h>
#include <string.h>
#include <stdio.h>

/* Find */
void
add_middlewares_to_stack(route_t *route, req_t *req)
{
	if(!route->middlewares) {
		return;
	}

	for(int i = 0; route->middlewares[i] != 0; i++) {
		stack_push(req->middleware_stack, (void*) route->middlewares[i]);
	}
}

route_t*
find_route_r(router_t *router, req_t *req, char* k, char *l)
{

	l = token(k, '/');

	int n = strlen(k);


	// If extra / is used in the path
	if(n == 0 && *l != 0) {
		return find_route_r(router, req, l, k);
	}

	router_t *router_next = NULL;

	// End of path
	if(n == 0 && *l == 0) {
		route_t *route;

		if(!router->routes) {
			printf("404\n");
			return NULL;
		}

		/*
		 * Priority for method:
		 * 1) Exact match
		 * 2) *
		 * 3) None (404 router)
		 */

		// Get an exact match
		route = hmap_get(router->routes, req->method);

		// Get for all methods
		if(!route) {
			route = hmap_get(router->routes, "*");
		}

		// Get 404
		// Already taken care of by return

		if(route) {
			add_middlewares_to_stack(route, req);
		}

		return route;
	}

	/*
	 * Priority of matching:
	 * 1) Exact match
	 * 2) Param
	 * 3) *
	 */

	// If an exact match exists
	if(!router_next) {
		router_next = hmap_get(router->child_routers, k);
	}

	// For match with params
	if(!router_next) {

		hmap_itr_t *itr = hmap_itr_new(router->child_routers);
		hmap_node_t *node;

		while((node = hmap_itr_adv(itr)) != NULL) {
			router_t *temp_router = node->value;
			if(temp_router->path[0] == ':') {
				router_next = temp_router;
				break;
			}
		}

		free(itr);
	}

	// For wildcard match
	if(!router_next) {
		router_next = hmap_get(router->child_routers, "*");
	}

	if(!router_next) {
		return NULL;
	}

	route_t *ret = find_route_r(router_next, req, l, k);

	if(ret) {
		route_t *middleware_route = hmap_get(router_next->routes, "*");

		if(middleware_route) {
			add_middlewares_to_stack(middleware_route, req);
		}
	}

	return ret;

	//TODO: Remove distinction between middlewares and controllers by using
	//TODO: variadic functions
}

route_t*
find_route(router_t *router, req_t *req)
{
	int n = strlen(req->path);
	char *p = malloc(sizeof(char) * (n+1));
	strcpy(p, req->path);


	router_t *temp = router;
	char *k = p, *l;

	return find_route_r(router, req, k, l);
}

/* Add */
void route(router_t *router,
		const char *path,
		const char  *method,
		middleware_t *middlewares,
		controller_t controller)
{

	char *p = malloc(sizeof(char) * (strlen(path) + 1));
	strcpy(p, path);

	router_t *temp = router;

	char *k, *l = p; // Reverse assignment for first iteration
	while(*l) {

		// Exchange l and k for token
		char *t = l;
		l = k;
		k = t;

		l = token(k, '/');

		int n = strlen(k);
		if(n == 0) {
			continue;
		}

		router_t *temp2;

		if(!temp->child_routers) {
			temp->child_routers = hmap_new_cap(5);

			temp2 = malloc(sizeof(router_t));
			temp2->path = malloc(sizeof(char) * (n+1));
			strcpy(temp2->path, k);
			temp2->routes = NULL;

			hmap_push(temp->child_routers, temp2->path, (void*) temp2);
			temp = temp2;
			continue;
		}

		temp2 = hmap_get(temp->child_routers, k);

		if(!temp2) {
			temp2 = malloc(sizeof(router_t));
			temp2->child_routers = NULL;
			temp2->path = malloc(sizeof(char) * (n+1));
			strcpy(temp2->path, k);
			temp2->routes = NULL;

			hmap_push(temp->child_routers, temp2->path, (void*) temp2);
		}

		temp = temp2;

	}

	route_t *route = malloc(sizeof(route_t));
	route->middlewares = middlewares;
	route->controller = controller;

	if(!temp->routes) {
		temp->routes = hmap_new_cap(3);
	}

	hmap_push(temp->routes, method, (void*) route);

	free(p);
}

void route_get(router_t *router,
		const char *path,
		middleware_t *middlewares,
		controller_t controller)
{
	route(router, path, "GET", middlewares, controller);
}

void route_post(router_t *router,
		const char *path,
		middleware_t *middlewares,
		controller_t controller)
{
	route(router, path, "POST", middlewares, controller);
}

void route_put(router_t *router,
		const char *path,
		middleware_t *middlewares,
		controller_t controller)
{
	route(router, path, "PUT", middlewares, controller);
}

void route_delete(router_t *router,
		const char *path,
		middleware_t *middlewares,
		controller_t controller)
{
	route(router, path, "DELETE", middlewares, controller);
}