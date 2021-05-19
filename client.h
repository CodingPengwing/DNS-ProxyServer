
#ifndef CLIENT_H
#define CLIENT_H

// #define _POSIX_C_SOURCE 200112L
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "packet.h"

Packet_t *ask_server(char *IP_addr, char *port, Packet_t *query);

#endif

