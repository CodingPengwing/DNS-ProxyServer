
#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "packet.h"
#include "client.h"

#define CACHE_SIZE 5

Packet_t **create_cache();

void print_cache(Packet_t **cache, size_t cache_len);

void free_cache(Packet_t **cache, size_t cache_len);


void update_cache(Packet_t **cache, size_t cache_len, char *server_IP, char *server_port, FILE *log_file);

Packet_t *put_in_cache(Packet_t **cache, size_t cache_len, Packet_t *packet);

Packet_t *find_in_cache(Packet_t **cache, size_t cache_len, char *QNAME);

size_t num_empty_slots(Packet_t **cache, size_t cache_len);

void reformat_cache(Packet_t **cache, size_t cache_len);

void move_to_front(Packet_t **cache, size_t cache_len, Packet_t *packet);

#endif
