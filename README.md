# libexpress

Express in C. Add some things that I wanted in express, and you get libexpress.

## Documentation

### TL;DR
```c
#include <libexpress/express.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#undef __BASE__
#define __BASE__ "/home/resyfer/libexpress/examples" // Give your own directories here

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
```

### Initialize

Import the header:
```c
#include <libexpress/express.h>
```

Compile using:
```sh
gcc a.c -o a.o -lexpress -lcol -lasync -ltpool -lqueue -lhmap -lvector
```

Initialize the server instance:
```c
server_t *app = server_new();
```

### Add routes

```c
// Get request
route_get(app, "/hello/*", middle, hi, ROUTE_END);
route_get(app, "/hello/bye", hi, ROUTE_END);
route_get(app, "/:file", adios, ROUTE_END);
route_get(app, "/", frontend, ROUTE_END);

// Post request
route_post(app, "/hello/:id/world", blah, ROUTE_END);
```

Routes are of the form:
```c
route_get(app, "path", controller1, controller2, ..., ROUTE_END);
```

Some points to be noted:
- Last parameter should be `ROUTE_END`.
- Controllers should be of the form:
```c
void controller(req_t *req, res_t *res) {
	
}
```
- Routes can be added using:
```c
route(app, "path", "method", controller1, controller2, ..., ROUTE_END);
route_get(app, "path", controller1, controller2, ..., ROUTE_END);
route_post(app, "path", controller1, controller2, ..., ROUTE_END);
route_put(app, "path", controller1, controller2, ..., ROUTE_END);
route_delete(app, "path", controller1, controller2, ..., ROUTE_END);
```
- If you're using custom method using the `route(app, "path", "method", ...)` way, you can add any method. A special method is `*`, which will match any method for a requested route, given that there is not a route specified for that specific method, and also that there is a route specified for `*` method.
- `app` can be any router, not just the topmost server instance.
- The route matched would be based on some priorities, rather than sequentially like how Express does it. Assume routes are defined like this:
```c
// ... stands for controllers, removed for simplicity
router_get(app, "/hello/:id/world", ...);
router_get(app, "/hello/*", ...);
router_get(app, "/foo", ...);
router_get(app, "/hello/abcd/world", ...);
```
The following routes requested will match the routes mentioned in the comment beside it:
```c
"/hello/abcd/world" // "/hello/abcd/world"
"/hello/abcde/world" // "/hello/:id/world"
"/hello/abcde/worldy" // "/hello/*"
"/hello/fooo" // 404
```
**Further documentation on the way**

## Installation

Following dependencies need to be installed (their individual installation instructions are provided in their READMEs). They are all written by me, so installation instructions will be similar and installing in same order will be best:

- [libvector](https://github.com/resyfer/libvector)
- [libhmap](https://github.com/resyfer/libhmap)
- [libqueue](https://github.com/resyfer/libqueue)
- [libtpool](https://github.com/resyfer/libtpool)
- [libasync](https://github.com/resyfer/libasync)
- [libcol](https://github.com/resyfer/libcol)

After this, installation instructions would be:
```sh
git clone https://github.com/resyfer/libexpress.git
cd libexpress
make install
cd ..
rm -rf libexpress
```
