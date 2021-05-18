
#include "server.h"

// A simple server in the internet domain using TCP
// The port number is passed as an argument
// To compile: gcc server.c -o server
// Reference: Beej's networking guide, man pages



int
create_listening_socket()
{
    struct addrinfo hints, *res;

    // Create address we're going to listen on (with given port number)
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6;       // IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE;     // for bind, listen, accept

    char service_port[10];
    sprintf(service_port, "%d", LISTENING_PORT);

	// node (NULL means any interface), service (port), hints, res
	int s = getaddrinfo(NULL, service_port, &hints, &res);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	// Create socket
	int socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketfd < 0) exit_with_error("Error in create_listening_socket(): socket() failed.");

	// Reuse port if possible
	int enable = 1;
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		exit_with_error("Error in create_listening_socket(): setsockopt() failed.");

	// Bind address to the socket
	if (bind(socketfd, res->ai_addr, res->ai_addrlen) < 0) 
        exit_with_error("Error in create_listening_socket(): bind() failed.");

	freeaddrinfo(res);

    // // Set the socket to NON-BLOCKING
    // if (fcntl(socketfd, F_SETFD, O_NONBLOCK) < 0) exit_with_error("Error in create_listening_socket(): fcntl() failed.");

	// Listen on socket - means we're ready to accept connections,
	// incoming connection requests will be queued, man 3 listen
	if (listen(socketfd, 15) < 0) exit_with_error("Error in create_listening_socket(): listen() failed.");

;

    return socketfd;    
}



int accept_new_connection(int socketfd)
{
	struct sockaddr_storage client_addr;
	socklen_t client_addr_size = sizeof client_addr;
	// Accept a connection - blocks until a connection is ready to be accepted
	// Get back a new file descriptor to communicate on
	int newsocketfd = accept(socketfd, (struct sockaddr*)&client_addr, &client_addr_size);
	if (newsocketfd < 0) exit_with_error("Error in accept_new_connection(): accept() failed.");

    return newsocketfd;
}


