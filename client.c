/*
This file is referenced from Lab 9 of the Computer Systems COMP30023 unit, Melbourne University, 2021.
This file contains functions to connect with an upstream server as a client.
*/

#include "client.h"

Packet_t *
ask_server(char *IP_addr, char *port, Packet_t *query)
{

	// println("!#@$!@#$!#$!@$!#@$!@#$!@# ASKING SERVER"); fflush(stdout);
	struct addrinfo hints, *servinfo, *rp;

	// Create address
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;      // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	// Get addrinfo of server. 
	int s = getaddrinfo(IP_addr, port, &hints, &servinfo);
	if (s != 0) {
		fprintf(stderr, "Error in ask_server(): getaddrinfo() - %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}


    int serverfd;
	// Connect to first valid result
	for (rp = servinfo; rp != NULL; rp = rp->ai_next) {
		serverfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (serverfd == -1)
			continue;

		if (connect(serverfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break; // success

		close(serverfd);
	}
	if (rp == NULL) exit_with_error("Error in ask_server(): client failed to connect.");
	freeaddrinfo(servinfo);

    // Send message to server
    int n = write(serverfd, query->raw_message, query->length);
    if (n < 0) exit_with_error("Error in ask_server(): write() failed");


    // Read message from server
    Packet_t *response = NULL;
	while (!response) response = receive_new_tcp_message(serverfd);

    // Close the socket
	close(serverfd);

	// println("!#@$!@#$!#$!@$!#@$!@#$!@# FINISHED ASKING SERVER"); fflush(stdout);

	return response;
}
