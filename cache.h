
#ifndef H_CACHE
#define H_CACHE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "packet.h"

void update_cache(Packet_t **cache, size_t cache_len);

void put_in_cache(Packet_t **cache, size_t cache_len, Packet_t *packet);

Packet_t *find_in_cache(Packet_t **cache, size_t cache_len, char *QNAME);

size_t num_empty_slots(Packet_t **cache, size_t cache_len);

void reformat_cache(Packet_t **cache, size_t cache_len);

#endif
