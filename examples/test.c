#include <libexpress/express.h>
#include <stdio.h>
#include <string.h>

void adios(req_t *req, res_t *res) {
	res_send(res, hmap_get(req->params, "id"));
}

void hi(req_t *req, res_t *res) {
	res_send(res, "Hi\n");
}

void middle(req_t *req, res_t *res) {
	printf("Middle\n");
	return next(res);
}

int main() {
	server_t *app = server_new();

	route_get(app, "/hello/:id/world", adios, ROUTE_END);
	route_get(app, "/hello/*", middle, hi, ROUTE_END);
	route_get(app, "/hello/bye", hi, ROUTE_END);

	route_get(app, "/foo", middle, ROUTE_END);
	// deliberate bug of just giving a middleware
	// not no ending controllers

	route_get(app, "/", hi, ROUTE_END);

	server_listen(app, 3000);
	return 0;
}