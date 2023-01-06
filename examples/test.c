#include <libexpress/express.h>
#include <stdio.h>

void* hi(req_t *req, res_t *res) {
	res_send(res, "Bye");
}

int main() {
	server_t *app = server_new();

	route_get(app, "/hello/worldy", NULL, hi);
	route_get(app, "/hello/bye", NULL, hi);
	route_get(app, "/foo", NULL, hi);

	server_listen(app, 3000, true);
	return 0;
}