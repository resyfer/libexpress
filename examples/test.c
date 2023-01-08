#include <libexpress/express.h>
#include <stdio.h>
#include <string.h>

void blah(req_t *req, res_t *res) {
	res_send(res, get_req_param(req, "id"));
}

void adios(req_t *req, res_t *res) {
	res_send(res, get_req_param(req, "id"));
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
	route_post(app, "/hello/:id/world", blah, ROUTE_END);
	route_get(app, "/hello/*", middle, hi, ROUTE_END);
	route_get(app, "/hello/bye", hi, ROUTE_END);
	route_get(app, "/", hi, ROUTE_END);

	server_listen(app, 3000);
	return 0;
}