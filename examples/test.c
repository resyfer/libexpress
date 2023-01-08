#include <libexpress/express.h>
#include <stdio.h>

void adios(req_t *req, res_t *res) {
	printf("-> %s\n", hmap_get(req->params, "id"));
	//TODO:
	res_send(res, "Adios\n");
}

void hi(req_t *req, res_t *res) {
	printf("Hi\n");
	res_send(res, "Bye\n");
}

void middle(req_t *req, res_t *res) {
	printf("Hello ");
	return next(res);
}

int main() {
	server_t *app = server_new();

	route_get(app, "/hello/:id/world", middle, adios, ROUTE_END);
	route_get(app, "/hello/world", middle, hi, ROUTE_END);
	route_get(app, "/hello/bye", hi, ROUTE_END);
	route_get(app, "/foo", middle, ROUTE_END);
	route_get(app, "/", hi, ROUTE_END);

	server_listen(app, 3000);
	return 0;
}