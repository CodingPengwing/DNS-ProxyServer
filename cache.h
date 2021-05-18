
#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "packet.h"

void update_cache(Packet_t **cache, unsigned int cache_len);

void put_in_cache(Packet_t **cache, unsigned int cache_len, Packet_t *packet);

Packet_t *find_in_cache(Packet_t **cache, unsigned int cache_len, char *QNAME);

unsigned int num_empty_slots(Packet_t **cache, unsigned int cache_len);

void reformat_cache(Packet_t **cache, unsigned int cache_len);

#endif
