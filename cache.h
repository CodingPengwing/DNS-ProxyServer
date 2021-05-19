/*
File created by Hoang Dang (1080344) Melbourne University 2021.
This file contains functions for a "cache" that holds a DNS packets.
*/

#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "packet.h"
#include "client.h"

#define CACHE_SIZE 5

/*  Creates a cache object. */
Packet_t **create_cache();

/*  Prints all the QNAMEs that are currently in the cache. */
void print_cache(Packet_t **cache, size_t cache_len);

/*  Frees the cache and all the Packets inside it. */
void free_cache(Packet_t **cache, size_t cache_len);

/*  Checks for any expired Packets and contacts the server to renew information. */
void update_cache(Packet_t **cache, size_t cache_len, char *server_IP, char *server_port, FILE *log_file);

/*  Puts a new Packet into the cache, evicting another Packet if need be. The packet evicted will be
    at the bottom of the cache queue (meaning it was accessed the longest time ago).
*/
Packet_t *put_in_cache(Packet_t **cache, size_t cache_len, Packet_t *packet);

/*  Finds a Packet in the cache matching QNAME. */
Packet_t *find_in_cache(Packet_t **cache, size_t cache_len, char *QNAME);

/*  Computes the number of empty slots in the cache. */
size_t num_empty_slots(Packet_t **cache, size_t cache_len);

/*  Pushes all the Packets currently in the cache to the right (the bottom of the queue),
    so that incoming Packets will be at the front of the queue. 
*/
void reformat_cache(Packet_t **cache, size_t cache_len);

/*  Moves a Packet to the front of the queue. */
void move_to_front(Packet_t **cache, size_t cache_len, Packet_t *packet);

#endif
