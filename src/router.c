#include <include/router.h>
#include <stdio.h>
#include <string.h>

/* Find */
void
vpush_to_call_queue(route_t *route, req_t *req)
{
	if(!route || !route->controllers) {
		return;
	}

	int n = vec_size(route->controllers);
	for(u_int32_t i = 0; i<n; i++) {
		controller_t *controller = vec_get(route->controllers, i);
		if(controller == ROUTE_END) {
			break;
		}
		queue_push(req->c_queue, (void*) controller);
	}
}

route_t*
find_route_r(router_t *router, req_t *req, vector_t *tokens, u_int32_t index)
{
	route_t *ret = NULL;

	if(index >= vec_size(tokens) || !strcmp(router->path, "*")) {

		// Exact method
		ret = hmap_get(router->routes, req->method);

		// All methods
		if(!ret) {
			ret = hmap_get(router->routes, "*");
		}

		if(!ret) {
			return NULL;
		}

		// Controllers
		route_t *controller_route = hmap_get(router->routes, req->method);
		if(!controller_route) {
			controller_route = hmap_get(router->routes, "*");
		}
		vpush_to_call_queue(controller_route, req);

		return ret;
	}

	char *token = vec_get(tokens, index);

	router_t *next_router = NULL;

	// Exact match
	next_router = hmap_get(router->child_routers, token);
	if(next_router) {
		ret = find_route_r(next_router, req, tokens, index + 1);
	}

	// Params
	if(!ret) {
		hmap_itr_t *itr = hmap_itr_new(router->child_routers);
		hmap_node_t *node;

		while((node = hmap_itr_adv(itr)) != NULL) {
			router_t *temp_router = node->value;

			if(temp_router->path[0] == ':') {
				ret = find_route_r(temp_router, req, tokens, index + 1);
				if(ret) {
					next_router = temp_router;
					if(!req->params) {
						req->params = hmap_new_cap(2);
					}

					hmap_push(req->params, temp_router->path + 1, vec_get(tokens, index));

					break;
				}
			}
		}
	}

	// Match All
	if(!ret) {
		next_router = hmap_get(router->child_routers, "*");

		if(next_router) {
			ret = find_route_r(next_router, req, tokens, index + 1);
		}
	}

	if(ret) {
		// Middlewares
		route_t *middleware_route = hmap_get(next_router->routes, "*");
		if(middleware_route) {
			vpush_to_call_queue(middleware_route, req);
		}

		// // Params
		// if(next_router->path[0] == ':') {
		// 	char *key = strdup((next_router->path) + 1);
		// 	char *value = token;


		// 	if(!req->params) {
		// 		req->params = hmap_new_cap(2);
		// 	}

		// 	hmap_push(req->params, key, value);
		// }
	}


	return ret;
}

route_t*
find_route(router_t *router, req_t *req)
{
	char *p = strdup(req->path);

	router_t *temp = router;
	char *k = p, *l;

	vector_t *tokens = vec_new();

	while(1) {
		l = tokenize(k, '/');

		char *s = strdup(k);

		int n = strlen(s);

		if(n == 0 && *l == 0) {
			free(s);
			break;
		}

		if(n == 0) {
			free(s);
		} else {
			vec_push_back(tokens, s);
		}

		char *temp = k;
		k = l;
		l = temp;
	}

	route_t *dest_route = find_route_r(router, req, tokens, 0);

	if(!dest_route && !strcmp(req->path, "*")) {
		strcpy(req->path, "/*");
		dest_route =  find_route(router, req);
	}

	if(!dest_route && !strcmp(req->method, "*")) {
		printf("Hi\n");
		strcpy(req->method, "*");
		dest_route =  find_route(router, req);
	}

	if(!dest_route) {
		print_error("Internal Server Error\n");
		exit(1);
	}

	return dest_route;
}

/* Add */
void
vroute(router_t *router, const char *path, const char *method, vector_t *controllers)
{

	char *p = strdup(path);

	router_t *temp = router;

	char *k, *l = p; // Reverse assignment for first iteration
	while(*l) {

		// Exchange l and k for token
		char *t = l;
		l = k;
		k = t;

		l = tokenize(k, '/');

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
	route->controllers = controllers;

	if(!temp->routes) {
		temp->routes = hmap_new_cap(3);
	}

	hmap_push(temp->routes, method, (void*) route);

	free(p);
}

void route(router_t *router, const char *path, char* method, ...)
{
	va_list args;
	va_start(args, method);

	vector_t* controllers = vec_new();

	while(1) {
		controller_t *controller = va_arg(args, controller_t*);
		if(controller == ROUTE_END) {
			break;
		}
		vec_push_back(controllers, (void*) controller);
	}
	vec_shrink(controllers);

	vroute(router, path, method, controllers);
	va_end(args);
}

void route_get(router_t *router, const char *path, ...)
{
	va_list args;
	va_start(args, path);

	vector_t* controllers = vec_new();

	while(1) {
		controller_t *controller = va_arg(args, controller_t*);
		if(controller == ROUTE_END) {
			break;
		}
		vec_push_back(controllers, (void*) controller);
	}
	vec_shrink(controllers);

	vroute(router, path, "GET", controllers);
	va_end(args);
}

void route_post(router_t *router, const char *path, ...)
{
	va_list args;
	va_start(args, path);

	vector_t* controllers = vec_new();

	while(1) {
		controller_t *controller = va_arg(args, controller_t*);
		if(controller == ROUTE_END) {
			break;
		}
		vec_push_back(controllers, (void*) controller);
	}
	vec_shrink(controllers);

	vroute(router, path, "POST", controllers);
	va_end(args);
}

void route_put(router_t *router, const char *path, ...)
{
	va_list args;
	va_start(args, path);

	vector_t* controllers = vec_new();

	while(1) {
		controller_t *controller = va_arg(args, controller_t*);
		if(controller == ROUTE_END) {
			break;
		}
		vec_push_back(controllers, (void*) controller);
	}
	vec_shrink(controllers);

	vroute(router, path, "PUT", controllers);
	va_end(args);
}

void route_delete(router_t *router, const char *path, ...)
{
	va_list args;
	va_start(args, path);

	vector_t* controllers = vec_new();

	while(1) {
		controller_t *controller = va_arg(args, controller_t*);
		if(controller == ROUTE_END) {
			break;
		}
		vec_push_back(controllers, (void*) controller);
	}
	vec_shrink(controllers);

	vroute(router, path, "DELETE", controllers);
	va_end(args);
}