#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h"
#include <ctype.h>
#include "packet.h"

#include <time.h>

#define REQ_LOG 0
#define UNIMPLEMENTED_LOG 1
#define RES_LOG 2


int main(int argc, char* argv[]) 
{
    static int TIME_BUFFER_LEN = 80;
    time_t rawtime;
    struct tm *info;
    char time_buffer[TIME_BUFFER_LEN];
    time(&rawtime);

    FILE *fp;
    fp = fopen("dns_svr.log", "w");
     //Don't forget to close the file when finished

    while (true)
    {
        /* RECEIVE NEW QUERY */
        Packet_t *packet = receive_new_message();

        info = localtime(&rawtime);
        strftime(time_buffer, TIME_BUFFER_LEN, "%FT%T%z", info);
        // printf("%s\n", time_buffer );

        if (packet) 
        {
            print_packet(packet);
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
