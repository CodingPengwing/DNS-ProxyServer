/*
Program created by Hoang Dang (1080344), Melbourne University, 2021, as a solution to
project 2 of Computer Systems Comp30023. 
This file contains the main function that calls upon functions from other modules to
create a DNS server that receives and responds to DNS queries. 
*/


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
#include "cache.h"

#define CACHE
#define NONBLOCKING
#define CACHE_SIZE 5
#define LOG_FILE "dns_svr.log"

// https://stackoverflow.com/questions/1352749/multiple-arguments-to-function-called-by-pthread-create
/*  This struct is used to pass arguments into the handle_query() function, in order to make use of multithreading. */
struct arg_struct {
    int clientfd;
    FILE *log_file;
    char *server_IP;
    char *server_port;
    Packet_t **cache;
    size_t cache_len;
    pthread_mutex_t *cache_lock;
};

void sigint_handler(int sig);
// void handle_query(int socketfd, FILE *log_file, char *server_IP, char *server_port, Packet_t **cache, size_t cache_len, pthread_mutex_t *cache_lock);
void *handle_query(void *args);

int main(int argc, char* argv[]) 
{
    signal(SIGINT, sigint_handler);
    FILE *log_file = fopen(LOG_FILE, "w");
    Packet_t **cache = create_cache();
    pthread_mutex_t cache_lock;
    char *server_IP = argv[1], *server_port = argv[2];

    int socketfd = create_listening_socket(); 
    while (true)
    {
        int newsocketfd = accept_new_connection(socketfd);

        struct arg_struct args;
        args.clientfd = newsocketfd;
        args.log_file = log_file;
        args.server_IP = server_IP;
        args.server_port = server_port;
        args.cache = cache;
        args.cache_len = CACHE_SIZE;
        args.cache_lock = &cache_lock;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_query, (void *)&args);
        // handle_query(newsocketfd, log_file, server_IP, server_port, cache, CACHE_SIZE, &cache_lock);
        // Create new tid in linked_list
    }
    close(socketfd);
    fclose(log_file);
    pthread_mutex_lock(&cache_lock);
    free_cache(cache, CACHE_SIZE);
    pthread_mutex_unlock(&cache_lock);
    return EXIT_SUCCESS;
}

void
sigint_handler(int file)
{
    close(LISTENING_PORT);
    FILE *log_file = fopen(LOG_FILE, "a");
    fclose(log_file);
    println("\n----- Hey there! Thanks for marking my assignment. Hope you have a nice day and stay hydrated! -----");
    exit(EXIT_SUCCESS);
}

// void
// handle_query(int clientfd, FILE *log_file, char *server_IP, char *server_port, Packet_t **cache, size_t cache_len, pthread_mutex_t *cache_lock)
void *
handle_query(void *args)
{
    println("STARTING QUERY"); fflush(stdout);

    struct arg_struct *args_ = (struct arg_struct *) args;
    // Extract function arguments
    int clientfd = args_->clientfd;
    FILE *log_file = args_->log_file;
    char *server_IP = args_->server_IP;
    char *server_port = args_->server_port;
    Packet_t **cache = args_->cache;
    size_t cache_len = args_->cache_len;
    pthread_mutex_t *cache_lock = args_->cache_lock;

    /* RECEIVE NEW QUERY */
    Packet_t *query = receive_new_tcp_message(clientfd);

    if (query)
    {
        print_packet(query);
        log_request(log_file, REQUEST, query->question->QNAME, NULL);

        /* CHECK VALIDITY OF QUERY, IF INVALID, RESPOND WITH ERROR CODE 4 --> FINISH THREAD */
        if (query->question->QTYPE != AAAA_TYPE) 
        {
            int message_len = LENGTH_HEADER_SIZE + HEADER_SIZE;
            query->raw_message[0] = 0x0;
            query->raw_message[1] = message_len - LENGTH_HEADER_SIZE;
            reset_header(query);
            update_QUERYCODE(query, QR_RESPONSE);
            update_RCODE(query, RCODE_ERROR);
            write_to_client(clientfd, query->raw_message, message_len);
            log_request(log_file, UNIMPLEMENTED_REQUEST, NULL, NULL);
            close(clientfd);
            free_packet(query);
            return NULL;
        }

        println("BREAKPOINT 1"); fflush(stdout);

        // pthread_mutex_lock(cache_lock)


        pthread_mutex_lock(cache_lock);
        /*  Update records in cache */
        update_cache(cache, CACHE_SIZE, server_IP, server_port, log_file);

        /*  Check Cache for existing valid record, should return record if there's a match, else NULL */
        /* IF THERE IS A VALID MATCHING EXISTING RECORD, SEND BACK STRAIGHT AWAY --> FINISH THREAD */
        Packet_t *cached_packet = find_in_cache(cache, cache_len, query->question->QNAME);
        if (cached_packet) 
        {
            byte_t byte_1 = get_1st_byte_from_double_byte(query->header->ID);
            byte_t byte_2 = get_2nd_byte_from_double_byte(query->header->ID);
            update_ID(cached_packet, byte_1, byte_2);
            update_TTL(cached_packet);
            move_to_front(cache, cache_len, cached_packet);
            log_cache(log_file, CACHE_EXPIRY, cached_packet->question->QNAME, NULL, cached_packet->time_expire);

            int message_len = cached_packet->length;
            write_to_client(clientfd, cached_packet->raw_message, message_len);
            if (cached_packet->header->ANCOUNT) 
            {
                if (cached_packet->answer->TYPE == AAAA_TYPE) log_request(log_file, RESPONSE, cached_packet->question->QNAME, cached_packet->answer->IP_address);
            }
            free_packet(query);
            close(clientfd);
            return NULL;
        }
        pthread_mutex_unlock(cache_lock);


        println("BREAKPOINT 2");fflush(stdout);


        // pthread_mutex_unlock(cache_lock)
        
        /* SEND QUERY TO SERVER, GET RESPONSE */
        Packet_t *response = ask_server(server_IP, server_port, query);

        /* LOG AND CACHE THE RESPONSE */
        if (response->header->ANCOUNT) 
        {
            if (response->answer->TYPE == AAAA_TYPE) log_request(log_file, RESPONSE, response->question->QNAME, response->answer->IP_address);

            pthread_mutex_lock(cache_lock);
            Packet_t *evict = put_in_cache(cache, cache_len, response);
            if (evict)
            {
                log_cache(log_file, CACHE_EVICTION, response->question->QNAME, evict->question->QNAME, 0);
                free_packet(evict);
            }
            pthread_mutex_unlock(cache_lock);
        }
        
        println("BREAKPOINT 3");fflush(stdout);

        /* RELAY THE SERVER'S RESPONSE TO THE ORIGINAL QUERIER */
        write_to_client(clientfd, response->raw_message, response->length);

        // finished using packets, free
        free_packet(query);

        // println("BREAKPOINT 4");fflush(stdout);

        close(clientfd);
        
        /* --> FINISH THREAD */
        
    }
    return NULL;
}


