#ifndef __LIBEXPRESS_H
#define __LIBEXPRESS_H

#include "router.h"
#include "util.h"

static int server_socket;

server_t* server_new();
void server_listen(server_t *server, port_t port, bool force_port);

#define MAX_CON 50

#endif