// Run with `make run` from root of `libexpress`. Make sure all dependencies
// are install first.

#include <libexpress/express.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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
	set_res_body(res, get_req_param(req, "id"));
	res_send(res);
}

void hi(req_t *req, res_t *res) {
	set_res_body(res, "Hi\n");
	res_send(res);
}

void middle(req_t *req, res_t *res) {
	printf("Middle\n");
}

int main() {
	server_t *app = server_new();

	route_get(app, "/hello", middle, MID_END, hi);
	route_get(app, "/hello/bye", MID_END, hi);
	route_get(app, "/:file", MID_END, adios);
	route_get(app, "/", MID_END, NULL);

	route(app, "/foo", "*", MID_END, frontend);

	route_post(app, "/hello/:id/world", MID_END, blah);

	server_listen(app, 3000);
	return 0;
}