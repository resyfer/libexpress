# Documentation

## Terminologies

- `server`: Refers to the server instance.
- `request`: Refers to the HTTP request sent to the server by the client.
- `response`: Refers to the HTTP response sent by the server to the client in
  response to the request.
- `path`: A unique URL in the domain of the server. eg. If server is running
  on 'helloworld.com' then in the URL 'helloworld.com/abc/def', '/abc/def' is
  the path
- `method`: Refers to one of the HTTP Methods defined by the HTTP Spec. A single
  route can have multiple methods, eg. GET '/hello' and POST '/hello'. Both the
  methods are *supposed* to perform different tasks, and which method is used to
  serve the request based on the method specified in the request.
- `route`: Refers to one of the configured paths where the server will listen
  for HTTP Requests for a specific method.
- `router`: A group of a bunch of routes based on their paths. eg. Both the
  routes for GET '/hello/world' and GET '/hello/bye' will have the common
  router '/hello' but the router `/hello/world or '/hello/bye' is unique to
  both the routes respectively.
- `query`: It is some additional information provided in the url for a specific
  route without sending it in the body. eg. In the URL
  'helloworld.com/abc/def?hi=bye&foo=bar', the path is still '/abc/def', and the
  queries are 'hi' and 'foo', with values 'bye' and 'bar' respectively.
- `param`: It refers to the variable part of the URL that maybe used to point
  to a specific resource eg. In the URL 'github.com/resyfer', the variable part
  is 'resyfer', which basically refers to the username of the person who's
  profile we want to view. So it would be written as `github.com/:username` and
  thus the param would be 'username' with value 'resyfer' in this case.
- `base`: Refers to the directory relative to which you want to give paths for
  files to serve. eg. If I have files '/home/resyfer/hello/a.html' and
  '/home/resyfer/hello/b.css', then I can set the base (shown later) as
  '/home/resyfer/hello' and provide the paths to the file as '/a.html' and
  '/b.css'.
- `controller`: A user-defined function that performs a task given the request
  and sends the response.
- `middleware`: A user-defined function that performs a task given the request
  and then transfers the control to another middleware or a controller.

## Import the library

```c
#include <libexpress/express.h>
```

## Compile your code

Assuming you're writing your code in a file, say `a.c`, then:
```c
$ gcc a.c -o a.o -lexpress -lcol -lasync -ltpool -lqueue -lhmap -lvector
```

## Initialize the server

```c
server_t *app = server_new();
```

Now `app` is your server (more specifically, your server instance).

### Configuring routes

```c
route(app, "/rest/api/route", "method", middleware1, middleware2, ..., controller, ROUTE_END);

route_get(app, "/rest/api/route", middleware1, middleware2, ..., controller, ROUTE_END); // GET method
route_post(app, "/rest/api/route", middleware1, middleware2, ..., controller, ROUTE_END); // POST method
route_put(app, "/rest/api/route", middleware1, middleware2, ..., controller, ROUTE_END); // PUT method
route_delete(app, "/rest/api/route", middleware1, middleware2, ..., controller, ROUTE_END); // DELETE method
```

You can add as many middlewares as you like. However there should be a controller
at the end as well a `ROUTE_END`. The `middleware1`, `middleware2`, `controller`, etc. that you see are
the name of the functions that will act as a middleware or a controller (format shown below).

### Middlewares and Controllers

Middlewares and Controllers both have the same structure:

```c
void name(req_t *req, res_t* res) {

}
```

Where `name` can then be used inside one of the `route` functions as shown above.

The difference between a middleware and a controller is shown later.