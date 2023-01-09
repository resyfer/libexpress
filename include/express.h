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

#include "router.h"
#include "util.h"
#include <limits.h>

// TODO: Fix memory leaks

/**
 * @brief The socket file descriptor.
 *
 * It is the file descriptor of the socket used
 * by the libexpress for accepting and responding
 * to requests by sending appropriate responses
 * through the socket file descriptor.
 */
static int server_socket;

/**
 * @brief Creates a new server.
 */
server_t* server_new(void);

void server_listen(server_t *server, port_t port);

#define MAX_CON 50

#endif