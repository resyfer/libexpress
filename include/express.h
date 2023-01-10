/**
 * @file express.h
 * @brief This contains the entry point to the library.
 *
 * It contains and imports all the functions that are made
 * public to the user.
 *
 * @author Saurav Pal (resyfer)
 * @bug No known bugs
 */

#ifndef __LIBEXPRESS_H
#define __LIBEXPRESS_H 1

/* Header Includes */
#include "router.h"

// TODO: Fix memory leaks

/**
 * @brief Maximum simultaneous connections accepted
 *        by server instance.
 */
#define MAX_CON 100

/**
 * @brief The socket file descriptor.
 *
 * It is the file descriptor of the socket used
 * by the libexpress for accepting and responding
 * to requests by sending appropriate responses
 * through the socket file descriptor.
 *
 * It is set once, and never again. It is used to
 * close the socket connection of the server
 * instance once SIGINT is sent to the program
 * to shut the server down.
 */
static int server_socket;

/**
 * @brief Creates a new server instance.
 *
 * @return A server instance.
 * @warning Please keep only one instance per application.
 */
server_t* server_new(void);

/**
 * @brief Makes the server instance listen for HTTP requests
 *        at specified port.
 *
 * The server instance listens for HTTP requests at the
 * specified port and accepts requests from a maximum of
 * `MAX_CON` clients.
 *
 * Since `server_t` and `router_t` are the same types, any
 * router can be made to listen for connections, not just
 * the server-instance. However, all routes requested will
 * be resolved according to the router listening to the
 * connection.
 *
 * @param server The server instance
 * @param port The port on which the server instance will listen for requests
 */
void server_listen(server_t *server, port_t port);

#endif