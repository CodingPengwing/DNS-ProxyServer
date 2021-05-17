#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h"
#include <ctype.h>
#include "packet.h"


int main(int argc, char* argv[]) 
{
    FILE *fp = fopen("dns_svr.log", "w");
    pthread_mutex_t cache_lock;
    
    while (true)
    {
        /* RECEIVE NEW QUERY */
        Packet_t *packet = receive_new_message();

        if (packet) 
        {

            print_packet(packet);
            log_request(fp, REQUEST, packet->question->QNAME, NULL);
            fflush(fp);

            /* CHECK VALIDITY OF PACKET, IF INVALID, SEND BACK ERROR CODE 4 STRAIGHT AWAY --> FINISH THREAD */
            
            // pthread_mutex_lock(&cache_lock)
            /*  Check Cache for expired records */
            /*  Check Cache for existing valid record, should return record if there's a match, else NULL */
            // pthread_mutex_unlock(&cache_lock)
            /* IF THERE IS A VALID MATCHING EXISTING RECORD, SEND BACK STRAIGHT AWAY --> FINISH THREAD */
            
            /* CREATE SOCKET CONNECTION WITH SERVER */
            /* SEND QUERY TO SERVER, GET RESPONSE */
            /* LOG THE RESPONSE */

            // pthread_mutex_lock(&cache_lock)
            /* UPDATE CACHE */
            // pthread_mutex_unlock(&cache_lock)

            /* RELAY THE SERVER'S RESPONSE TO THE ORIGINAL QUERIER */

            // finished using packet, free it
            free_packet(packet);
            
            /* --> FINISH THREAD */
        }
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

