#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h"
#include <ctype.h>
#include "packet.h"


int main(int argc, char* argv[]) 
{
    FILE *fp = fopen("dns_svr.log", "w");
    // static int TIME_BUFFER_LEN = 80;
    // char time_buffer[TIME_BUFFER_LEN];

    while (true)
    {
        /* RECEIVE NEW QUERY */
        Packet_t *packet = receive_new_message();

        if (packet) 
        {
            print_packet(packet);
            log_request(fp, REQUEST, packet->question->QNAME, NULL);
            fflush(fp);
            // --> START A THREAD HERE

            /* CHECK VALIDITY OF PACKET, IF INVALID, SEND BACK ERROR CODE 4 STRAIGHT AWAY --> FINISH THREAD */
            
            /* CHECK CACHE FOR EXISTING RECORDS, CHECK EXPIRY OF RECORDS */

            /* IF THERE IS A VALID MATCHING EXISTING RECORD, SEND BACK STRAIGHT AWAY --> FINISH THREAD */

            /* IF THERE IS NO VALID MATCHING EXISTING RECORD, SEND QUERY TO SERVER */

            /* RELAY THE SERVER'S RESPONSE TO THE ORIGINAL QUERIER */

            /* UPDATE CACHE */

            // finished using packet, free it
            free_packet(packet);
            
            /* --> FINISH THREAD */
        }
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

