#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h"
#include <ctype.h>
#include "packet.h"

Packet_t * 
receive_new_query();

Packet_t * 
parse_packet(byte_t *raw_data, int data_length);

int main(int argc, char* argv[]) 
{
    // while (true)
    {
        // --> START A THREAD HERE

        /* RECEIVE NEW QUERY */
        Packet_t *packet = receive_new_query();

        /* CHECK VALIDITY OF PACKET, IF INVALID, SEND BACK ERROR CODE 4 STRAIGHT AWAY --> FINISH THREAD */
        
        /* CHECK CACHE FOR EXISTING RECORDS, CHECK EXPIRY OF RECORDS */

        /* IF THERE IS A VALID MATCHING EXISTING RECORD, SEND BACK STRAIGHT AWAY --> FINISH THREAD */

        /* IF THERE IS NO VALID MATCHING EXISTING RECORD, SEND QUERY TO SERVER */

        /* RELAY THE SERVER'S RESPONSE TO THE ORIGINAL QUERIER */

        /* UPDATE CACHE */
        
        /* --> FINISH THREAD */
    }
    return SUCCESS;
}

Packet_t * 
receive_new_query()
{   
    double_byte_t data_length;
    bool query_received = read(STDIN_FILENO, &data_length, 2*sizeof(byte_t));

    if (query_received)
    {   
        data_length = ntohs(data_length);
        printf("Length of the file is %d \n", data_length);

        byte_t *raw_data = (byte_t*)malloc(data_length*sizeof(byte_t));
        // Read until all the bytes for the packet are received
        int bytes_read = 0;
        while (bytes_read < data_length) 
            bytes_read += read(STDIN_FILENO, &raw_data[bytes_read], 1*sizeof(byte_t));

        // Here all the bytes have been received, we can parse the packet
        Packet_t * packet = parse_packet(raw_data, data_length);
    
        // // Reset the data_length for the next read (if there is one)
        // data_length = 0;
        return packet;
    }
    return NULL;
}


Packet_t * 
parse_packet(byte_t *raw_data, int data_length)
{
    printf("The content of the file is:\n");
    for (int i = 0; i < data_length; i++) 
    {
        print_byte_as_hexes(raw_data[i]);
        printf(", ");
    }

    Header_t *header = new_header(raw_data);

    Packet_t *packet = new_packet(raw_data, data_length);
    return packet;
}
