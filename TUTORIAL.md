# Tutorial

## Terminologies
For us all to be on the same page.
- `server`: Refers to the server instance.

- `client`: The user requesting information from server.

- `HTTP request/response`: Request or Response that is in the form as specified by
  the HTTP specification.

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

- `HTTP method`: Some methods that are defined in the HTTP specification.

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

- `port`: A unique number used by a process accessing the network for identification
  and routing the request to the specific process by the operating system. If we
  say that port 3000 is free, it means 3000 is a port that is not being currently
  used by a process. If we say server is listening on port 3000, it means that
  the port number 3000 will be used by the operating system to identify the server
  process and thus route any requests to port 3000 to the server.

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

Now `app` is your server (specifically, your server instance).

You can make the server listen for HTTP requests using:

```c
server_listen(app, 3000);
```

Where `3000` is an example for a port number. Any port that is free will do of course.

## Configure Routes

Now, just initializing the server and listening for port won't do anything useful.
It doesn't know what to do when a client makes a HTTP request.

So **before** you listen to the server (and **after** initializing the server)
you need to configure your routes.

Four of the most common HTTP methods have been simplified for you to configure, as
they represent the basic Create-Read-Update-Delete (CRUD) operations.

```c
route_get(app, "/rest/api/route", middleware1, middleware2, ..., MID_END, controller); // GET method
route_post(app, "/rest/api/route", middleware1, middleware2, ..., MID_END, controller); // POST method
route_put(app, "/rest/api/route", middleware1, middleware2, ..., MID_END, controller); // PUT method
route_delete(app, "/rest/api/route", middleware1, middleware2, ..., MID_END, controller); // DELETE method
```

First, it takes the router. Now, it can be the topmost router (which is the
server), or it can take a child router (will be explained below).

Second, it takes in the path. Now, the route is **relative** to the path of the router
that you gave. If the router is the server (ie. path is `/`) then giving a path
as `/rest/api/route` ultimately gives you a route for `/rest/api/route`. If your
router has a path, say, `/rest`, then giving the path `/api/route` will utlimately
give you the route for `/rest/api/route`.

Third, it takes in **any** number of middlewares. Middlewares have the following
syntax:

```c
void name(req_t *req, res_t *res) {

}
```

Where the pointer `req` is of type `req_t` and `res` of type `res_t`, and they
are the request and response objects respectively (more on that later).

After you return from the function, the control is shifted to the next middleware
or controller in route.

Fourth, the second last parameter is a macro named `MID_END` that signifies the
end of middlewares, and after that the controller is used.

Fifth, the last parameter a route **may** take is a controller. Now, **controllers
are no different from middlewares** except in the name and essence. A controller
is that handler which finally sends a response to the client (shown later). Do note
that a middleware may send a response as well if it wants, but controllers
**have** to send a response before it returns (or client will not get any response).

Remember, it is not necessary for a route to have a controller (you can give a NULL).
However it is necessary for a route that the user is requesting to end with a controller.
Sounds confusing? Let me elaborate:

Suppose you have 2 routes `/hello` and `/hello/world`. Accessing `/hello/world`
will lead to middlewares of **both** the routes being executed in sequential
order, so middlewares of `/hello` and `/hello/world` both will be executed
sequentially. This means that, assuming the client **only** requests `/hello/world`,
then `/hello` need not have a controller as long as `/hello/world` has one.

That's all there is to configuring routes.

There's another way to configure a route, one that lets you use any HTTP method
you like, or even custom ones of your own (like the suprisingly popular "PATCH"
method):

```c
route(app, "/rest/api/route", "method", middleware1, middleware2, ..., MID_END, controller);
```

A special method `*` can also be used, which is a method that matches any method
given that the method in the request is not matched by any other configured
method, as well as assuming that a route with `*` method was configured.

## Path

A path can be of various forms:

```txt
/hello/*
/hello/:id/world
/hello/world
```

The second way includes a route param (more on that later).

Unlike Express, here the order of configuring routes don't matter when it comes
to selecting the route based on the HTTP request path. It will always be the
same, given the same routes are configured.

Assuming the above routes are configured for GET request and the client is
making requests using GET method, the following will be routes matched for
requests according to priority:

| request path | route |
| ------------ | ----- |
| /hello/world | /hello/world |
| /hello//world | /hello/world |
| /hello/abcd/world | /hello/:id/world |
| /hello/abcde | /hello/* |
| /hello/abcd/efgh | /hello/* |
| /hell/abcd | 404 |

### Route params

Route params are variable content in the array that usually provide information about
the resource to make requests for, but are not part of the URL queries.

eg. `/user/resyfer/profile` has a variable part `resyfer` which refers to the username
of the user who's profile I want to view.

In libexpress, writing such route params as `/hello/:username/profile` can help you
extract the `username` value that the client gives, *inside a middleware or controller*
like:

```c
char *username = get_req_param(req, "username");
```

### Query params

Query params point to the same path, but add additional info on the resource they
are fetching eg. `www.youtube.com/watch?v=dQw4w9WgXcQ` has path `/watch` but
specifies the ID of the video `dQw4w9WgXcQ` as query param. The query param
`v` has the value `dQw4w9WgXcQ`.

Multiple query params are added like `/path?hello=world&foo=bar` for which `foo`
has the value `bar` and `hello` has the value `world`.

You can fetch the value of a query param using:
```c
char* value = get_req_query(req, "query");
```

**Coming Soon**: The ability to iterate over params.

### Request

**Beside** the above, you can extract more information from the HTTP request:

```c
char *body = get_req_body(req);
char *value = get_req_header(req, "header");
char *method = get_req_method(req);
char *path = get_req_path(req);
```

### Response

You need to prepare your response **before it is sent**. The following way will
ensure it.

```c
set_res_body(res, body); // body is char*
set_res_header(res, header, value); // header, value are char*
char* header = get_res_header(res, header);
```

After that you need to send a response before the controller reaches the end:

```c
res_send(res);

// try to
// return res_send(res);
// so that execution will not happen below it
```