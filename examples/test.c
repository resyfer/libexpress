#include <libexpress/express.h>
#include <stdio.h>
#include <string.h>

#undef __BASE__
#define __BASE__ "/home/resyfer/self/projects/libexpress/examples" // Give your own directories here

void frontend(req_t *req, res_t *res) {
	res_send_file(res, __BASE__ "/index.html");
}

void adios(req_t *req, res_t *res) {
	char *file = get_req_param(req, "file");
	char path[PATH_MAX] = {0};
	sprintf(path, "%s/%s", __BASE__, file);
	res_send_file(res, path);
}

void blah(req_t *req, res_t *res) {
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

	route_post(app, "/hello/:id/world", blah, ROUTE_END);
	route_get(app, "/hello/*", middle, hi, ROUTE_END);
	route_get(app, "/hello/bye", hi, ROUTE_END);
	route_get(app, "/:file", adios, ROUTE_END);
	route_get(app, "/", frontend, ROUTE_END);

	server_listen(app, 3000);
	return 0;
}