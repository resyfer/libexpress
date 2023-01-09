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
	res->status = 404;
	hmap_push(res->headers, "Content-Type", "text/plain");
	res_send(res, "404");
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
	route(app, "/*", "*", controller_404);

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
	print_warning("\nShutting down Server\n");
	close(server_socket);
	print_ok("Server Closed\n");
	exit(0);
}

/**
 * @brief Connection Handler arguments
 *
 * Due to connection handler being run in a
 * thread, this data structure holds the file
 * descriptor of the client, as well as the
 * server instance to pass it down to the
 * `connection handler`
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
	char buf[MAX_SIZE] = {0};
	int k = read(*client, buf, sizeof(buf));
	buf[k] = '\0';

	req_t *req = parse_req(buf, k);

	route_t *route = find_route(server, req);

	// Initialize response
	res_t *res = malloc(sizeof(res));
	res->status = 200; // Default value
	res->headers = hmap_new_cap(5);
	res->body = NULL;
	res->sent = false;
	res->next = false;
	res->client = *client;

	// Execute all the middlewares and controller of the route.
	while(1) {
		controller_t *controller = queue_pop(req->c_queue);

		if(!controller) {
			if(res->next) {
				print_error("\nCan't use next on last controller\n");
				exit(1);
			} else if(!res->sent) {
				print_error("\nResponse is not sent by end of final controller\n");
				exit(1);
			}

			break;
		}

		controller(req, res);

		// If request is already sent, no need to loop again.
		if(res->sent) {
			break;
		}

		if(res->next) {
			res->next = false;
		} else {
			print_error("\nNeither `next` was reached, nor any responses were sent\n");
			exit(1);
		}
	}

	clock_t end_time = clock(); // End time for serving response

	// Logs
	if(res->sent) {
		printf("%s %s %d - %f ms\n", req->method, req->path, res->status, ((double) (end_time - start_time) / CLOCKS_PER_SEC) * 1000);
	}

	// Cleanup of client
	close(*client);
	free(client);
}

void
server_listen(server_t *server, port_t port)
{
	async_init();

	signal(SIGINT, graceful_exit); // Signal handler for SIGINT for graceful shutdown

	// Setting up socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == -1) {
		print_error("Error establishing server\n");
		exit(1);
	}

	if (setsockopt(server_socket,
			SOL_SOCKET, SO_REUSEADDR, &(int){1},
			sizeof(int)) < 0) {
		print_error("Error acquring port\n");
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
			print_error("Error binding to port");
			exit(1);
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