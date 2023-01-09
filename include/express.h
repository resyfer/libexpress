#ifndef __LIBEXPRESS_H
#define __LIBEXPRESS_H 1

#include "router.h"
#include "util.h"
#include <limits.h>

// TODO: Fix memory leaks
// TODO: Tidy

static int server_socket;

server_t* server_new(void);
void server_listen(server_t *server, port_t port);

#define MAX_CON 50

#endif