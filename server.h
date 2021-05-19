
#ifndef SERVER_H
#define SERVER_H

// #define _POSIX_C_SOURCE 200112L
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"


#define LISTENING_PORT 8053

int create_listening_socket();

int accept_new_connection(int socketfd);

void write_to_client(int socketfd, byte_t *message, size_t message_len);

#endif
