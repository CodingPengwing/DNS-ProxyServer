#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "util.h"
#include "packet.h"
#include "server.h"

#define CACHE_LEN 5

void sigint_handler(int sig);
void handle_query(int socketfd, FILE *log_file, Packet_t *cache[], unsigned int cache_len, pthread_mutex_t *cache_log);

int main(int argc, char* argv[]) 
{
    signal(SIGINT, sigint_handler);
    FILE *log_file = fopen("dns_svr.log", "w");
    Packet_t *cache[CACHE_LEN];
    pthread_mutex_t cache_lock;

    int socketfd = create_listening_socket(); 
    while (true)
    {
        // int newsocketfd = accept_new_connection(socketfd);
        handle_query(STDIN_FILENO, log_file, cache, CACHE_LEN, &cache_lock);
        // Create new tid in linked_list
        // pthread_create(handle_query);
    }

    // close(socketfd);

    // close thread and also free linked list node

    fclose(log_file);
    return EXIT_SUCCESS;
}

void
sigint_handler(int sig)
{
    close(LISTENING_PORT);
    println("\n----- Hey there! Thanks for marking my assignment. Hope you have a nice day and stay hydrated! -----");
    exit(EXIT_SUCCESS);
}

void
handle_query(int socketfd, FILE *log_file, Packet_t *cache[], unsigned int cache_len, pthread_mutex_t *cache_lock)
{
    /* RECEIVE NEW QUERY */
    Packet_t *packet = receive_new_message(socketfd);

    if (packet)
    {
        print_packet(packet);
        log_request(log_file, REQUEST, packet->question->QNAME, NULL);
        fflush(log_file);

        /* CHECK VALIDITY OF PACKET, IF INVALID, SEND BACK ERROR CODE 4 STRAIGHT AWAY --> FINISH THREAD */
        
        // pthread_mutex_lock(cache_lock)
        /*  Check Cache for expired records */
        /*  Check Cache for existing valid record, should return record if there's a match, else NULL */
        // pthread_mutex_unlock(cache_lock)
        /* IF THERE IS A VALID MATCHING EXISTING RECORD, SEND BACK STRAIGHT AWAY --> FINISH THREAD */
        
        /* CREATE SOCKET CONNECTION WITH SERVER */
        /* SEND QUERY TO SERVER, GET RESPONSE */
        /* LOG THE RESPONSE */

        // pthread_mutex_lock(cache_lock)
        /* UPDATE CACHE */
        // pthread_mutex_unlock(cache_lock)

        /* RELAY THE SERVER'S RESPONSE TO THE ORIGINAL QUERIER */

        // finished using packet, free it
        free_packet(packet);
        
        /* --> FINISH THREAD */
    }

}



// READING FROM CLIENT
// 	char buffer[256];
// 	// Read characters from the connection, then process
// 	int n = read(newsocketfd, buffer, 255); // n is number of characters read
// 	if (n < 0) exit_with_error("Error in accept_new_connection(): read() failed.");

// 	// Null-terminate string
// 	buffer[n] = '\0';

// 	// Write message back
// 	printf("Here is the message: %s\n", buffer);
// 	n = write(newsocketfd, "I got your message", 18);
// 	if (n < 0) {
// 		perror("write");
// 		exit(EXIT_FAILURE);
// 	}

// 	close(socketfd);
// 	close(newsocketfd);
// 	return 0;
// }
