/* Header Files */
#include <include/express.h>
#include <include/util.h>

/* Personal Libraries */
#include <libasync/async.h>

/* libc */
#include <limits.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>

/**
 * @brief 404 Controller
 *
 * The default controller for requests that
 * reach the 404 route.
 *
 * @param req Server Request
 * @param res Server Response
 */
void controller_404(req_t *req, res_t *res) {
	set_res_status(res, 404);
	set_res_header(res, "Content-Type", "text/plain");
	set_res_body(res, "404");
	res_send(res);
}

server_t*
server_new(void)
{
	/* Intializers for the server */
	status_codes_init();
	mime_init();

	server_t *app = malloc(sizeof(server_t));
	app->path = "";
	app->routes = NULL;
	app->child_routers = hmap_new_cap(5);

	/* Adding the catch-all route handler for 404 requests */
	route(app, "*", "*", MID_END, controller_404);

	return app;
}

/**
 * @brief Signal Handler for server shutdown
 *
 * Shuts down the server on SIGINT and clears up any left
 * over memory as well as releases the hold on port.
 *
 * @param sig Signal
 */
void
graceful_exit(int sig)
{
	warning("Shutting down Server");
	close(server_socket);
	success("Server Closed");
	exit(0);
}

/**
 * @brief Connection Handler arguments
 *
 * Due to connection handler being run in a
 * thread, this structure holds the file
 * descriptor of the client, as well as the
 * server instance to pass it down to the
 * `connection handler` function.
 *
 * Just a temporary struct used to pass arguments
 * into the `connection_handler` function.
 */
typedef struct {
	int* client;
	server_t* server;
} con_args_t;

/**
 * @brief Handles connections, and serves appropriate responses
 *
 * Understands the request and accordinly serves the most
 * appropriate response back to the client.
 *
 * @param args A void pointer to the `con_args_t` struct having
 * 			   the respective values of `server` and `client`
 */
void*
connection_handler(void* args)
{
	// Gets the required values from args.
	con_args_t *con_args = args;
	int *client = con_args->client;
	server_t *server = con_args->server;

	clock_t start_time = clock(); // Start timestamp for response time logging

	// Read the Request from Client
	char req_buf[MAX_SIZE] = {0};
	int k = read(*client, req_buf, sizeof(req_buf));
	req_buf[k] = '\0';

	req_t *req = parse_req(req_buf, k);

	route_t *route = find_route(server, req);

	// Initialize response
	res_t *res = malloc(sizeof(res_t));
	res->status = 200; // Default value
	res->headers = hmap_new_cap(5);
	res->body = NULL;
	res->sent = false;
	res->client = *client;

	// Execute all the middlewares and controller of the route.
	while(1) {
		middleware_t *middleware = queue_pop(req->c_queue);

		if(!middleware) {
			break;
		}

		middleware(req, res);

		// If request is already sent, no need to loop again.
		if(res->sent) {
			break;
		}
	}
	if(route->controller) {
		route->controller(req, res);
	} else {
		error("No controller available");
	}


	if(!res->sent) {
		error("Controller does not send any response by the time it finishes.");
	}

	clock_t end_time = clock(); // End time for serving response

	// Logs
	if(res->sent) {
		printf("%s %s %d - %f ms\n",
			req->method,
			req->path,
			res->status,
			((double) (end_time - start_time) / CLOCKS_PER_SEC) * 1000);
	}

	// Free
	free(req->method);
	free(req->path);
	hmap_free(req->queries);
	hmap_free(req->headers);
	hmap_free(req->params);
	free(req->body);
	queue_free(req->c_queue);
	free(req);

	hmap_free(res->headers);
	free(res);

	close(*client);
	free(client);
}

void
server_listen(server_t *server, port_t port)
{
	async_init();

	signal(SIGINT, graceful_exit);
	// Signal handler for SIGINT for graceful shutdown

	// Setting up socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == -1) {
		error("Error establishing server");
	}

	if (setsockopt(server_socket,
			SOL_SOCKET, SO_REUSEADDR, &(int){1},
			sizeof(int)) < 0) {
		error("Error acquring port");
	}

	// Binding socket to port
	struct sockaddr_in serv_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr.s_addr = INADDR_ANY,
	};

	if(bind(server_socket,
		(struct sockaddr*) &serv_addr,
		sizeof(serv_addr)) == -1) {
			error("Error binding to port");
		}

	// Listening for connections, at max connections being MAX_CON
	listen(server_socket, MAX_CON);

	// Serve requests using thread-pool as an when they arrive
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