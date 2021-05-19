/*
This file is referenced from Lab 9 of the Computer Systems COMP30023 unit, Melbourne University, 2021.
This file contains functions to act as a server and provide connections to clients.
*/

#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"


#define LISTENING_PORT 8053

/*  creates a port that listens on the default LISTENING_PORT */
int create_listening_socket();

/*  accepts a new connection on the given socketfd */
int accept_new_connection(int socketfd);

/*  writes a message to the client at socketfd */
void write_to_client(int socketfd, byte_t *message, size_t message_len);

#endif
