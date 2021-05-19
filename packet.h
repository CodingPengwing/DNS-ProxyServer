/*
File created by Hoang Dang (1080344) Melbourne University 2021.
This file contains functions and structures for DNS packets.
*/

#ifndef PACKET_H
#define PACKET_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "util.h"

#define QR_QUERY 0
#define QR_RESPONSE 1
#define FULL_STOP '.'
#define NULL_BYTE '\0'
#define RCODE_ERROR 4
#define AAAA_TYPE 0x1C

// A TCP message has 2-byte header indicating the length of the message that follows
#define LENGTH_HEADER_SIZE 2
// A DNS header is always of length 12
#define HEADER_SIZE 12


typedef struct packet Packet_t;
typedef struct header Header_t;
typedef struct question Question_t;
typedef struct resourceRecord ResourceRecord_t;


/*  This structure is used to parse and hold the different elements of a DNS message. */
struct packet 
{
    byte_t *raw_message;
    size_t length;
    byte_t type;
    time_t time_received;
    time_t TTL;
    time_t time_expire;
    Header_t *header;
    Question_t *question;
    ResourceRecord_t *answer;
    // Authority
    // Additional
};

/*  Reads a new DNS message from the given file descriptor and creates a new Packet struct. */
Packet_t * receive_new_tcp_message(int input_file_descriptor);

/*  Creates a new Packet struct from the given raw_message (in bytes) */
Packet_t *new_packet(byte_t *raw_message, size_t data_length);

/*  Parses all the fields of a DNS message into Packet struct fields for ease of use. */
Packet_t *parse_raw_message(Packet_t* packet);

/*  Prints all the contents of a Packet. */
void print_packet(Packet_t *packet);

/*  Frees all the contents of a Packet. */
void free_packet(Packet_t* packet);


/*  A Header structure to represent a DNS header section. */
struct header 
{
    double_byte_t ID;
    byte_t QR;
    byte_t Opcode;
    byte_t AA;
    byte_t TC;
    byte_t RD;
    byte_t RA;
    byte_t Z;
    byte_t AD;
    byte_t CD;
    byte_t RCODE;
    double_byte_t QDCOUNT;
    double_byte_t ANCOUNT;
    double_byte_t NSCOUNT;
    double_byte_t ARCOUNT;
};

/*  Creates a new Header struct. This function assumes that the raw data given has length of 
    12 bytes as specified by RFC2535. If this condition is not met, there will be an error.
*/
Header_t *new_header(byte_t *header_raw_message);

/*  Prints all the contents of a Header struct. */ 
void print_header(Header_t *header);

/*  Frees a Header struct */
void free_header(Header_t *header);


/*  A Question structure to represent a DNS question section. */
struct question 
{
    size_t length;
    char *QNAME;
    size_t QNAME_length;
    double_byte_t QTYPE;
    double_byte_t QCLASS;
};

Question_t *new_question(byte_t *question_raw_message);

void print_question(Question_t *question);

void free_question(Question_t *question);


/*  A ResourceRecord structure to represent a DNS resource record section. 
    (answer/additional record/authoritative record) 
*/
struct resourceRecord
{
    size_t length;
    double_byte_t NAME;
    double_byte_t TYPE;
    double_byte_t CLASS;
    quad_byte_t TTL;
    double_byte_t RDLENGTH;
    byte_t *RDATA;
    char IP_address[INET6_ADDRSTRLEN];
};

/*  Creates a new ResourceRecord structure. */
ResourceRecord_t *new_resourceRecord(byte_t *resourceRecord_raw_message);

/*  Prints all the contents of a Resource Record. */
void print_resourceRecord(ResourceRecord_t *resourceRecord);

/*  Frees all the contents of a Resource Record. */
void free_resourceRecord(ResourceRecord_t *resourceRecord);




/*  Updates the QR field in the query parameters of the Header section of the packet. */
void update_QUERYCODE(Packet_t *packet, uint8_t QUERYCODE);

/*  Updates the RCODE field in the query parameters of the Header section of the packet. */
void update_RCODE(Packet_t *packet, uint8_t RCODE);

/*  Updates the RD field in the query parameters of the Header section of the packet. */
void update_RD(Packet_t *packet, uint8_t RD);

/*  Updates the ID field of the Header section of the packet. */
void update_ID(Packet_t *packet, byte_t ID_byte_1, byte_t ID_byte_2);

/*  Updates the TTL field in the answer section of the packet. */
void update_TTL(Packet_t *packet);

/*  Resets the header without changing the ID (sets everything else to 0). */
void reset_header(Packet_t *packet);

/*  Resets the query parameters of a Header section (sets everything to 0). */
void reset_query_parameters(Packet_t *packet);

#endif
