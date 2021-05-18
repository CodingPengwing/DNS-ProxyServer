
#include "cache.h"

void
update_cache(Packet_t **cache, unsigned int cache_len)
{
    time_t current_time = get_current_time_raw();
    for (unsigned int i = 0; i < cache_len; i++)
    {
        if (!cache[i]) continue;
        if (current_time > cache[i]->time_expire)
        {
            free_packet(cache[i]);
            cache[i] = NULL;
        }
        else cache[i]->TTL = cache[i]->time_expire - current_time;
    }
    reformat_cache(cache, cache_len);
}

void
put_in_cache(Packet_t **cache, unsigned int cache_len, Packet_t *packet)
{
    update_cache(cache, cache_len);
    // free last accessed resource in cache
    if (!num_empty_slots(cache, cache_len))
    {
        free_packet(cache[cache_len-1]);
        cache[cache_len-1] = NULL;
    }
    reformat_cache(cache, cache_len);
    for (unsigned int i = cache_len-1; i >= 0; i--)
    {
        if (!cache[i])
        {
            cache[i] = packet;
            break;
        }
    }
}

Packet_t *
find_in_cache(Packet_t **cache, unsigned int cache_len, char *QNAME)
{
    update_cache(cache, cache_len);
    for (unsigned int i = 0; i < cache_len; i++)
    {
        if (!cache[i]) continue;
        if (strcmp(cache[i]->question->QNAME, QNAME) == 0) return cache[i];
    }
    return NULL;
}

void
replace_in_cache(Packet_t **cache, unsigned int cache_len, char *QNAME_evict, Packet_t *packet_insert)
{
    Packet_t *evict = find_in_cache(cache, cache_len, QNAME_evict);
    if (!evict) exit_with_error("Error in replace_in_cache(): QNAME_evict does not exist.");
    for (unsigned int i = 0; i < cache_len; i++)
    {
        if (cache[i] == evict)
        {
            free_packet(cache[i]);
            cache[i] = NULL;
            update_cache(cache, cache_len);
            put_in_cache(cache, cache_len, packet_insert);
            return;
        }
    }

    // for (unsigned int i = 0; i < cache_len; i++)
    // {
    //     if (!cache[i]) continue;
    //     if (strcmp(cache[i]->question->QNAME, QNAME_evict) == 0) 
    //     {
    //         free_packet(cache[i]);
    //         cache[i] = NULL;
    //         update_cache(cache, cache_len);
    //         put_in_cache(cache, cache_len, packet_insert);
    //         return;
    //     }
    // }
}

unsigned int
num_empty_slots(Packet_t **cache, unsigned int cache_len)
{
    unsigned int count = 0;
    for (unsigned int i = 0; i < cache_len; i++)
    {
        if (!cache[i]) ++count;
    }
    return count;
}

void
reformat_cache(Packet_t **cache, unsigned int cache_len)
{
    for (unsigned int i = cache_len-1; i > 0; i--) 
    {
        if (!cache[i])
        {
            int j = i-1;
            while(j >= 0)
            {
                if (cache[j])
                {
                    cache[i] = cache[j];
                    cache[j] = NULL;
                }
                --j;
            }
        }
    }
}
