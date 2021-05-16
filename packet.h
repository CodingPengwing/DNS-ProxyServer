
#ifndef PACKET_H
#define PACKET_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "util.h"


#define QUERY 0
#define RESPONSE 1

#define FULL_STOP '.'
#define NULL_BYTE '\0'

// A DNS header is always of length 12
#define HEADER_LENGTH 12


typedef struct packet Packet_t;
typedef struct header Header_t;
typedef struct question Question_t;
typedef struct resourceRecord ResourceRecord_t;


struct packet 
{
    byte_t *raw_message;
    unsigned int length;
    unsigned int type;
    time_t time_received;
    time_t TTL;
    time_t time_expire;
    Header_t *header;
    Question_t *question;
    ResourceRecord_t *answer;
    // Authority
    // Additional
};


Packet_t * receive_new_message();

Packet_t *new_packet(byte_t *raw_message, unsigned int data_length);

Packet_t *parse_raw_message(Packet_t* packet);

void print_packet(Packet_t *packet);

void free_packet(Packet_t* packet);



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

/*  
    This function creates a new header, it assumes that the raw data given has length of 12 bytes
    as specified by RFC2535. If this condition is not met, there will be an error.
*/
Header_t *new_header(byte_t *header_raw_message);

void print_header(Header_t *header);

void free_header(Header_t *header);



struct question 
{
    unsigned int length;
    char *QNAME;
    unsigned int QNAME_length;
    double_byte_t QTYPE;
    double_byte_t QCLASS;
};

Question_t *new_question(byte_t *question_raw_message);

void print_question(Question_t *question);

void free_question(Question_t *question);



struct resourceRecord
{
    unsigned int length;
    double_byte_t NAME;
    double_byte_t TYPE;
    double_byte_t CLASS;
    quad_byte_t TTL;
    double_byte_t RDLENGTH;
    byte_t *RDATA;
    char IP_address[INET6_ADDRSTRLEN];
};


ResourceRecord_t *new_resourceRecord(byte_t *resourceRecord_raw_message);

void print_resourceRecord(ResourceRecord_t *resourceRecord);

void free_resourceRecord(ResourceRecord_t *resourceRecord);


#endif
