#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>
#include "util.h"
#include "packet.h"
#include "server.h"
#include "client.h"

#define CACHE_LEN 5

void sigint_handler(int sig);
void handle_query(int socketfd, FILE *log_file, char *server_IP, char *server_port, Packet_t *cache[], size_t cache_len, pthread_mutex_t *cache_lock);

int main(int argc, char* argv[]) 
{
    signal(SIGINT, sigint_handler);
    FILE *log_file = fopen("dns_svr.log", "w");
    Packet_t *cache[CACHE_LEN];
    pthread_mutex_t cache_lock;
    char *server_IP = argv[1], *server_port = argv[2];
    // char server_IP[] = "192.168.1.1", server_port[] = "53";

    int socketfd = create_listening_socket(); 
    while (true)
    {
        int newsocketfd = accept_new_connection(socketfd);
        // pthread_t thread_id;
        // pthread_create(handle_query);
        handle_query(newsocketfd, log_file, server_IP, server_port, cache, CACHE_LEN, &cache_lock);
        // Create new tid in linked_list
    }

    close(socketfd);

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
handle_query(int clientfd, FILE *log_file, char *server_IP, char *server_port, Packet_t *cache[], size_t cache_len, pthread_mutex_t *cache_lock)
{
    static int RCODE_ERROR = 4;
    static int AAAA_TYPE = 0x1C;
    /* RECEIVE NEW QUERY */
    Packet_t *query = receive_new_tcp_message(clientfd);

    if (query)
    {
        print_packet(query);
        log_request(log_file, REQUEST, query->question->QNAME, NULL);
        fflush(log_file);

        /* CHECK VALIDITY OF QUERY, IF INVALID, RESPOND WITH ERROR CODE 4 --> FINISH THREAD */
        if (query->question->QTYPE != AAAA_TYPE) 
        {
            update_RCODE(query, RCODE_ERROR);
            write_to_client(clientfd, query->raw_message, query->length);
            log_request(log_file, UNIMPLEMENTED_REQUEST, NULL, NULL);
            fflush(log_file);
            close(clientfd);
            return;
        }

        // pthread_mutex_lock(cache_lock)
        /*  Check Cache for expired records */
        /*  Check Cache for existing valid record, should return record if there's a match, else NULL */
        // pthread_mutex_unlock(cache_lock)
        /* IF THERE IS A VALID MATCHING EXISTING RECORD, SEND BACK STRAIGHT AWAY --> FINISH THREAD */
        
        /* SEND QUERY TO SERVER, GET RESPONSE */
        Packet_t *response = ask_server(server_IP, server_port, query);
        /* LOG THE RESPONSE */
        if (response->answer) 
        {
            if (response->answer->TYPE == AAAA_TYPE) log_request(log_file, RESPONSE, response->question->QNAME, response->answer->IP_address);
        }
        

        // pthread_mutex_lock(cache_lock)
        /* UPDATE CACHE */
        // pthread_mutex_unlock(cache_lock)

        /* RELAY THE SERVER'S RESPONSE TO THE ORIGINAL QUERIER */
        write_to_client(clientfd, response->raw_message, response->length);

        // finished using packets, free
        free_packet(query);
        free_packet(response);

        close(clientfd);
        
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
