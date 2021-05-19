/*
This file is referenced from Lab 9 of the Computer Systems COMP30023 unit, Melbourne University, 2021.
This file contains functions to connect with an upstream server as a client.
*/


#ifndef CLIENT_H
#define CLIENT_H

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "packet.h"

/*  Contacts the upstream server to pass on the query and get back a response. */
Packet_t *ask_server(char *IP_addr, char *port, Packet_t *query);

#endif

