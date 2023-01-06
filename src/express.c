#include <include/express.h>
#include <libasync/async.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

void* controller_404(req_t *req, res_t *res) {
	res->status = 404;
	hmap_push(res->headers, "Content-Type", "text/plain");
	res_send(res, "404");
}

server_t*
server_new()
{
	status_codes_init();

	server_t *app = malloc(sizeof(server_t));
	app->path = "";
	app->routes = NULL;
	app->child_routers = hmap_new_cap(5);

	/* 404 CatchAll */
	router_t *catch_all_router = malloc(sizeof(router_t));
	catch_all_router->path = malloc(2 * sizeof(char));
	strcpy(catch_all_router->path, "*");
	catch_all_router->routes = hmap_new_cap(3);
	catch_all_router->child_routers = NULL;

	route_t *route_404 = malloc(sizeof(route_t));
	route_404->middlewares = NULL;
	route_404->controller = controller_404;

	hmap_push(catch_all_router->routes, catch_all_router->path /* both are "*" */, (void*) route_404);
	hmap_push(app->child_routers, catch_all_router->path, (void*) catch_all_router);

	return app;
}

void
graceful_exit(int sig)
{
	print_warning("\nShutting down Server\n");
	close(server_socket);
	print_ok("Server Closed\n");
	exit(0);
}

typedef struct {
	int* client;
	server_t* server;
} con_args_t;

void*
connection_handler(void* args)
{
	con_args_t *con_args = args;
	int *client = con_args->client;
	server_t *server = con_args->server;


	char buf[MAX_SIZE] = {0};
	int k = read(*client, buf, sizeof(buf));
	buf[k] = '\0';

	req_t *req = parse_req(buf, k);

	route_t *route = find_route(con_args->server, req);

	if(!route) {
		router_t *catch_all = hmap_get(server->child_routers, "*");
		route = hmap_get(catch_all->routes, "*");
	}

	res_t *res = malloc(sizeof(res));
	res->status = 200; // Default value
	res->headers = hmap_new_cap(5);
	res->body = NULL;
	res->sent = false;
	res->next = false;
	res->client = *client;

	bool complete = false;
	while(1) {

		middleware_t mid = stack_pop(req->middleware_stack);

		if(!mid) {
			complete = true;
			break;
		}

		mid(req, res);

		if(res->sent) {
			break;
		}

		if(res->next) {
			res->next = false;
		} else {
			break;
		}

	}

	if(complete) {
		route->controller(req, res);
	}

	printf("%s %d\n", req->path, res->status);

	close(*client);
	free(client);
}

void
server_listen(server_t *server, port_t port, bool force_port)
{
	async_init();

	signal(SIGINT, graceful_exit);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == -1) {
		print_error("Error establishing server\n");
		exit(1);
	}

	if (force_port &&
		setsockopt(server_socket,
			SOL_SOCKET, SO_REUSEADDR, &(int){1},
			sizeof(int)) < 0) {
		print_error("Error acquring port\n");
	}

	struct sockaddr_in serv_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr.s_addr = INADDR_ANY,
	};

	if(bind(server_socket,
		(struct sockaddr*) &serv_addr,
		sizeof(serv_addr)) == -1) {
			print_error("Error binding to port");
			exit(1);
		}

	listen(server_socket, MAX_CON);

	int serv_addr_len = sizeof(serv_addr);
	while(1) {
		int *client = malloc(sizeof(int));
		*client = accept(server_socket, (struct sockaddr*) &serv_addr,
						&serv_addr_len);

		con_args_t *args = malloc(sizeof(con_args_t));
		args->client = client;
		args->server = server;
		async(connection_handler, (void*) args);
	}
}