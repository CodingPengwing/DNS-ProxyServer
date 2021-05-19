/*
File created by Hoang Dang (1080344) Melbourne University 2021.
This file contains functions for a "cache" that holds a DNS packets.
*/

#include "cache.h"

Packet_t **
create_cache()
{
    Packet_t **cache = (Packet_t**)malloc(CACHE_SIZE * sizeof(*cache));
    return cache;
}

void
print_cache(Packet_t **cache, size_t cache_len)
{
    println("Cache:");
    for (int i = 0; i < cache_len; i++)
    {
        if (cache[i]) println(cache[i]->question->QNAME);
    }
}

void
free_cache(Packet_t **cache, size_t cache_len)
{
    for (int i = 0; i < cache_len; i++)
    {
        if (cache[i]) free_packet(cache[i]);
    }
    free(cache);
}

void
update_cache(Packet_t **cache, size_t cache_len, char *server_IP, char *server_port, FILE *log_file)
{

    if (num_empty_slots(cache, cache_len) == cache_len) return;
    time_t current_time = get_current_time_raw();
    for (size_t i = 0; i < cache_len; i++)
    {
        if (!cache[i]) continue;
        if (current_time > cache[i]->time_expire)
        {
            Packet_t *tmp = cache[i];
            update_QUERYCODE(cache[i], QR_QUERY);
            update_RD(cache[i], true);
            cache[i] = ask_server(server_IP, server_port, cache[i]);
            log_cache(log_file, CACHE_EVICTION, cache[i]->question->QNAME, tmp->question->QNAME, 0);
            free_packet(tmp);
        }
        else update_TTL(cache[i]);
    }
    reformat_cache(cache, cache_len);
}



Packet_t *
put_in_cache(Packet_t **cache, size_t cache_len, Packet_t *packet)
{
    update_QUERYCODE(packet, QR_RESPONSE);
    // update_cache(cache, cache_len);
    // free last accessed resource in cache
    Packet_t *evict = NULL;
    if (!num_empty_slots(cache, cache_len))
    {
        evict = cache[cache_len-1];
        cache[cache_len-1] = NULL;
    }
    reformat_cache(cache, cache_len);

    for (size_t i = cache_len-1; i >= 0; i--)
    {
        if (!cache[i])
        {
            cache[i] = packet;
            break;
        }
    }
    return evict;
}

Packet_t *
find_in_cache(Packet_t **cache, size_t cache_len, char *QNAME)
{
    if (num_empty_slots(cache, cache_len) == cache_len) return NULL;
    // update_cache(cache, cache_len);
    for (size_t i = 0; i < cache_len; i++)
    {
        if (!cache[i]) continue;
        if (strcmp(cache[i]->question->QNAME, QNAME) == 0) return cache[i];
    }
    return NULL;
}

size_t
num_empty_slots(Packet_t **cache, size_t cache_len)
{
    size_t count = 0;
    for (size_t i = 0; i < cache_len; i++)
    {
        if (!cache[i]) {
            count++;
        }
    }
    return count;
}

void
reformat_cache(Packet_t **cache, size_t cache_len)
{
    if (num_empty_slots(cache, cache_len) == cache_len) return; 
    for (size_t i = cache_len-1; i > 0; i--) 
    {
        if (cache[i]) continue;
        int j = i-1;
        while(j >= 0)
        {
            if (cache[j])
            {
                cache[i] = cache[j];
                cache[j] = NULL;
                break;
            }
            --j;
        }
    }
}

void
move_to_front(Packet_t **cache, size_t cache_len, Packet_t *packet)
{
    if (!find_in_cache(cache, cache_len, packet->question->QNAME)) exit_with_error("Error in move_to_front(): packet not in cache.");
    if (num_empty_slots(cache, cache_len) >= 4) 
    {
        reformat_cache(cache, cache_len);
        return;
    }
    int swap_1 = -1, swap_2 = -1;
    for (size_t i = 0; i < cache_len; i++)
    {
        if (cache[i]) 
        {
            swap_1 = i;
            break;
        }
    } 
    for (size_t i = 0; i < cache_len; i++)
    {
        if (cache[i] == packet)
        {
            swap_2 = i;
            break;
        }
    }
    if (swap_1 < 0 || swap_2 < 0) exit_with_error("Error in move_to_front(): swaps failed to establish.");
    Packet_t *tmp = cache[swap_1];
    cache[swap_1] = cache[swap_2];
    cache[swap_2] = tmp;
}
