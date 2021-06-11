/*
File created by Hoang Dang (1080344) Melbourne University 2021.
This file contains functions and structures for DNS packets.
*/
#include "packet.h"

/*  Checks stdin for an incoming message. When a message has been fully received,
    creates a Packet object and returns it. 
*/
Packet_t * 
receive_new_tcp_message(int input_file_descriptor)
{   
    // try to read initial 2 bytes
    byte_t *raw_data = (byte_t*)malloc(LENGTH_HEADER_SIZE*sizeof(byte_t));
    size_t bytes_read = 0;
    while (bytes_read < LENGTH_HEADER_SIZE)
        bytes_read += read(input_file_descriptor, &raw_data[bytes_read], sizeof(byte_t));

    //  If failed to read, return
    if (bytes_read <= 0) return NULL;

    // Otherwise read the entire message
    double_byte_t message_length = append_2_bytes(raw_data[0], raw_data[1]);
    message_length += LENGTH_HEADER_SIZE;
    raw_data = realloc(raw_data, message_length*sizeof(byte_t));
    // Read until all the bytes for the packet have been received
    while (bytes_read < message_length) 
        bytes_read += read(input_file_descriptor, &raw_data[bytes_read], sizeof(byte_t));

    // Here all the bytes have been received, we can parse the packet
    Packet_t *packet = new_packet(raw_data, message_length);
    return packet;
    
    return NULL;
}

/*  Creates a new Packet object that holds a raw_message and the length of that raw message. */
Packet_t *
new_packet(byte_t *raw_message, size_t length)
{
    Packet_t *packet = (Packet_t*) malloc(sizeof(*packet));
    if (!packet) exit_with_error("Error in new_packet(): failed malloc.");
    packet->raw_message = raw_message;
    packet->length = length;
    packet->time_received = get_current_time_raw();
    packet->TTL = 0;
    parse_raw_message(packet);
    return packet;
}

/*  Parses a raw message inside a packet to extract the contents and fill the attributes of the packet. 
    This function assumes that a valid raw_message was provided when the Packet object was created.
*/
Packet_t * 
parse_raw_message(Packet_t* packet)
{
    if (!packet) exit_with_error("Error in parse_raw_message(): null pointer.");
    if (!packet->raw_message) exit_with_error("Error in parse_raw_message(): no raw_message in packet.");
    // Skip the first 2 bytes because of length header bytes
    byte_t *raw_message = packet->raw_message + LENGTH_HEADER_SIZE;

    Header_t *header = new_header(raw_message);
    packet->header = header;
    // Determine whether this packet is a query or response.
    packet->type = packet->header->QR;

    raw_message += HEADER_SIZE;
    // Functionality can be added to handle queries with no questions or malformed queries
    if (!packet->header->QDCOUNT) exit_with_error("Error in parse_raw_message(): no questions in query.");
    
    packet->question = new_question(raw_message);

    // Parse the first answer if this is a response
    if (packet->header->ANCOUNT)
    {
        // Skip the question bytes, assuming there's only 1 question
        raw_message += packet->question->length;
        packet->answer = new_resourceRecord(raw_message);
        packet->TTL = packet->answer->TTL;
    }
    else packet->answer = NULL;

    packet->time_expire = packet->time_received + packet->TTL;
    
    return packet;
}


/*  Prints all the contents of a Packet */
void
print_packet(Packet_t *packet)
{
    // println("PRINTPOINT 1");fflush(stdout);
    if (!packet) exit_with_error("Error in print_packet(): null pointer.");
    static int TIME_BUFFER_LEN = 80;
    char time_buffer[TIME_BUFFER_LEN];

    println("----- Packet -----");

    // Print the time parameters
    convert_raw_time(time_buffer, TIME_BUFFER_LEN, packet->time_received);
    printf("Time received: %s\n", time_buffer);
    if (packet->answer) 
    {
        convert_raw_time(time_buffer, TIME_BUFFER_LEN, packet->time_expire);
        printf("Time expire: %s", time_buffer); println("");
    }

    // Print the message content
    double_byte_t message_length = packet->length;
    printf("Message length: %d\n", message_length);
    printf("Message content: ");

    for (uint16_t i = 0; i < message_length; i++) 
    {
        print_byte_as_hexes(packet->raw_message[i]);
        if (i < message_length-1) printf(", ");
        else println("");
    } println("");
    
    // Print the header
    print_header(packet->header); println("");

    // Print the question
    print_question(packet->question); println("");

    // Print the answer
    if (packet->answer) { print_resourceRecord(packet->answer); println(""); }

    println("------------------");
    fflush(stdout);
}

/*  Frees a Packet object */
void
free_packet(Packet_t *packet)
{
    if (!packet) exit_with_error("Error in free_packet(): null pointer.");
    free(packet->raw_message);
    free_header(packet->header);
    free_question(packet->question);
    if (packet->answer) free_resourceRecord(packet->answer);
}


/*  This function creates a new header, it assumes that the raw data given has length of 12 bytes
    as specified by RFC2535. If this condition is not met, there will be an error.
*/
Header_t *
new_header(byte_t *header_raw_message)
{
    Header_t *header = (Header_t*) malloc(sizeof(*header));
    if (!header) exit_with_error("Error in new_header(): failed malloc.");

    // *** Parse the raw header data into individual fields ***
    // The ID field is the first 2 bytes of the header
    header->ID = append_2_bytes(header_raw_message[0], header_raw_message[1]);
    // The query parameters are bytes 3 and 4 of the header
    double_byte_t query_parameters = append_2_bytes(header_raw_message[2], header_raw_message[3]);
    // The QDCOUNT is bytes 5 and 6 of the header
    header->QDCOUNT = append_2_bytes(header_raw_message[4], header_raw_message[5]);
    // The ANCOUNT is bytes 6 and 6 of the header
    header->ANCOUNT = append_2_bytes(header_raw_message[6], header_raw_message[7]);
    // The NSCOUNT is bytes 8 and 9 of the header
    header->NSCOUNT = append_2_bytes(header_raw_message[8], header_raw_message[9]);
    // The ARCOUNT is bytes 10 and 11 of the header
    header->ARCOUNT = append_2_bytes(header_raw_message[10], header_raw_message[11]);

    // Bit masks
    static double_byte_t QR_MASK = 0x8000;
    static double_byte_t OPCODE_MASK = 0x7800;
    static double_byte_t AA_MASK = 0x400;
    static double_byte_t TC_MASK = 0x200;
    static double_byte_t RD_MASK = 0x100;
    static double_byte_t RA_MASK = 0x80;
    static double_byte_t Z_MASK = 0x40;
    static double_byte_t AD_MASK = 0x20;
    static double_byte_t CD_MASK = 0x10;
    static double_byte_t RCODE_MASK = 0xf;

    // Bit shifts
    static int QR_BIT_SHIFT = 15;
    static int OPCODE_BIT_SHIFT = 11;
    static int AA_BIT_SHIFT = 10;
    static int TC_BIT_SHIFT = 9;
    static int RD_BIT_SHIFT = 8;
    static int RA_BIT_SHIFT = 7;
    static int Z_BIT_SHIFT = 6;
    static int AD_BIT_SHIFT = 5;
    static int CD_BIT_SHIFT = 4;
    static int RCODE_BIT_SHIFT = 0;

    // Extract the exact bits of the query parameters
    double_byte_t QR = query_parameters & QR_MASK;
    double_byte_t Opcode = query_parameters & OPCODE_MASK;
    double_byte_t AA = query_parameters & AA_MASK;
    double_byte_t TC = query_parameters & TC_MASK;
    double_byte_t RD = query_parameters & RD_MASK;
    double_byte_t RA = query_parameters & RA_MASK;
    double_byte_t Z = query_parameters & Z_MASK;
    double_byte_t AD = query_parameters & AD_MASK;
    double_byte_t CD = query_parameters & CD_MASK;
    double_byte_t RCODE = query_parameters & RCODE_MASK;

    // Shift the bits of the parameters
    header->QR = QR >> QR_BIT_SHIFT;
    header->Opcode = Opcode >> OPCODE_BIT_SHIFT;
    header->AA = AA >> AA_BIT_SHIFT;
    header->TC = TC >> TC_BIT_SHIFT;
    header->RD = RD >> RD_BIT_SHIFT;
    header->RA = RA >> RA_BIT_SHIFT;
    header->Z = Z >> Z_BIT_SHIFT;
    header->AD = AD >> AD_BIT_SHIFT;
    header->CD = CD >> CD_BIT_SHIFT;
    header->RCODE = RCODE >> RCODE_BIT_SHIFT;
    return header;
}

/*  Prints all the content of a Header section */
void
print_header(Header_t *header)
{
    if (!header) exit_with_error("Error in print_header(): null pointer.");
    printf("- Header - ID: "); print_double_byte_as_hexes(header->ID); printf(", ");
    printf("QR: %X, ", header->QR);
    printf("Opcode: %X, ", header->Opcode);
    printf("AA: %X, ", header->AA);
    printf("TC: %X, ", header->TC);
    printf("RD: %X, ", header->RD);
    printf("RA: %X, ", header->RA);
    printf("Z: %X, ", header->Z);
    printf("AD: %X, ", header->AD);
    printf("CD: %X, ", header->CD);
    printf("RCODE: %X, ", header->RCODE);
    printf("QDCOUNT: %X, ", header->QDCOUNT);
    printf("ANCOUNT: %X, ", header->ANCOUNT);
    printf("NSCOUNT: %X, ", header->NSCOUNT);
    printf("ARCOUNT: %X", header->ARCOUNT);
    fflush(stdout);
}

/*  Frees a Header object */
void
free_header(Header_t *header)
{
    if (!header) exit_with_error("Error in free_header(): null pointer.");
    free(header);
}


/*  Creates a new question section for a packet, parsing all the data in this section into the Question struct.
    The question_raw_message pointer should point to the start of the question section and not the start of
    the entire raw_message. 
*/
Question_t *
new_question(byte_t *question_raw_message)
{
    Question_t *question = (Question_t*) malloc(sizeof(*question));
    if (!question) exit_with_error("Error in new_question(): failed malloc.");

    // Read the QNAME
    size_t i = 0;
    // Get first byte (length octet) to know what length to read in next
    size_t bytes_to_read = (size_t) question_raw_message[i];
    ++i;

    // malloc enough space for next domain name section
    char *QNAME = (char*) malloc((bytes_to_read+i)*sizeof(char));
    // start reading the bytes and parsing to char array QNAME
    while (question_raw_message[i])
    {
        // if we are in the middle of a domain name section
        if (bytes_to_read > 0)
        {   
            // parse data to array
            QNAME[i-1] = (char) question_raw_message[i];
            --bytes_to_read;
        }
        // if we encountered a length octet
        else 
        {   
            QNAME[i-1] = FULL_STOP;
            // determine the length of the next section
            bytes_to_read = (size_t) question_raw_message[i];
            QNAME = (char*) realloc(QNAME, i + bytes_to_read + 1);
        }
        i++;
    }
    QNAME[i-1] = NULL_BYTE;
    question->QNAME = QNAME;
    // Length of QNAME including null byte
    question->QNAME_length = i;
    // Skip the null byte in the raw_message
    ++i;
    // Extract the QTYPE and QCLASS
    byte_t QTYPE_byte_1 = question_raw_message[i++];
    byte_t QTYPE_byte_2 = question_raw_message[i++];
    byte_t QCLASS_byte_1 = question_raw_message[i++];
    byte_t QCLASS_byte_2 = question_raw_message[i++];
    question->QTYPE = append_2_bytes(QTYPE_byte_1, QTYPE_byte_2);
    question->QCLASS = append_2_bytes(QCLASS_byte_1, QCLASS_byte_2);
    
    // Total length of the question section
    question->length = i;
    return question;
}

/*  Prints all the content of a Question object */
void
print_question(Question_t *question)
{
    if (!question) exit_with_error("Error in print_question(): null pointer.");
    printf("- Question - QNAME: "); 
    printf("%s, ", question->QNAME);
    printf("QTYPE: %X, ", question->QTYPE);
    printf("QCLASS: %X, ", question->QCLASS);
    printf("Length: %d", (int)question->length);
    fflush(stdout);
}

/*  Frees a Question object */
void
free_question(Question_t *question)
{
    if (!question) exit_with_error("Error in free_question(): null pointer.");
    free(question->QNAME);
    free(question);
}


/* Creates a new resource record object, this function assumes that the NAME field of the resource record is in compressed form */
ResourceRecord_t *
new_resourceRecord(byte_t *resourceRecord_raw_message)
{
    ResourceRecord_t *resourceRecord = (ResourceRecord_t*) malloc(sizeof(*resourceRecord));
    if (!resourceRecord) exit_with_error("Error in new_resourceRecord(): failed malloc.");

    // Read the NAME from the 1st and 2nd bytes of the raw message 
    resourceRecord->NAME = append_2_bytes(resourceRecord_raw_message[0], resourceRecord_raw_message[1]);
    // Read the TYPE from the 3rd and 4th bytes of the raw message
    resourceRecord->TYPE = append_2_bytes(resourceRecord_raw_message[2], resourceRecord_raw_message[3]);
    // Read the CLASS from the 5th and 6th bytes of the raw message
    resourceRecord->CLASS = append_2_bytes(resourceRecord_raw_message[4], resourceRecord_raw_message[5]);
    // Read the TTL from the 7th and 8th bytes of the raw message
    resourceRecord->TTL = append_4_bytes(resourceRecord_raw_message[6], resourceRecord_raw_message[7], resourceRecord_raw_message[8], resourceRecord_raw_message[9]);
    // Read the RDLENGTH from the 9th and 10th bytes of the raw message
    resourceRecord->RDLENGTH = append_2_bytes(resourceRecord_raw_message[10], resourceRecord_raw_message[11]);

    // Skip the first 12 bytes we just read in
    resourceRecord_raw_message += 12;
    if (resourceRecord->RDLENGTH != 0)
    {
        resourceRecord->RDATA = (byte_t*) malloc(resourceRecord->RDLENGTH * sizeof(byte_t));
        for (int i = 0; i < resourceRecord->RDLENGTH; i++) resourceRecord->RDATA[i] = resourceRecord_raw_message[i];
        
        if (inet_ntop(AF_INET6, resourceRecord->RDATA, resourceRecord->IP_address, INET6_ADDRSTRLEN) == NULL)
            exit_with_error("Error in new_resourceRecord(): failed to convert RDATA to IP address.");
    }
    return resourceRecord;
}

void 
print_resourceRecord(ResourceRecord_t *resourceRecord)
{
    if (!resourceRecord) exit_with_error("Error in print_resourceRecord(): null pointer.");
    printf("- Resource Record - NAME: "); print_double_byte_as_hexes(resourceRecord->NAME);
    printf(", TYPE: "); print_double_byte_as_hexes(resourceRecord->TYPE);
    printf(", CLASS: "); print_double_byte_as_hexes(resourceRecord->CLASS);
    printf(", TTL: "); print_quad_byte_as_hexes(resourceRecord->TTL);
    printf(", RDLENGTH: "); print_double_byte_as_hexes(resourceRecord->RDLENGTH);
    printf(", RDATA: "); 
    for (int i = 0; i < resourceRecord->RDLENGTH; i++) print_byte_as_hexes(resourceRecord->RDATA[i]);
    printf(", IP address: %s", resourceRecord->IP_address);
    fflush(stdout);
}

void
free_resourceRecord(ResourceRecord_t *resourceRecord)
{
    if (!resourceRecord) exit_with_error("Error in free_resourceRecord(): null pointer.");
    if (resourceRecord->RDLENGTH) free(resourceRecord->RDATA);
    free(resourceRecord);
}

void
update_QUERYCODE(Packet_t *packet, uint8_t QUERYCODE)
{
    size_t QUERYCODE_POS = 4;
    if (QUERYCODE) 
    {
        packet->raw_message[QUERYCODE_POS] = packet->raw_message[QUERYCODE_POS] | 0x80;
        packet->header->QR = true;
    }
    else 
    {
        packet->raw_message[QUERYCODE_POS] = packet->raw_message[QUERYCODE_POS] & 0x7F;
        packet->header->QR = false;
    }
}

void
update_RCODE(Packet_t *packet, uint8_t RCODE)
{
    size_t RCODE_POS = 5;
    packet->header->RCODE = RCODE;
    packet->raw_message[RCODE_POS] = packet->raw_message[RCODE_POS] & 0xfff0;
    packet->raw_message[RCODE_POS] = packet->raw_message[RCODE_POS] | RCODE;
}

void
update_RD(Packet_t *packet, uint8_t RD)
{
    size_t RD_POS = 4;
    if (RD) 
    {
        packet->raw_message[RD_POS] = packet->raw_message[RD_POS] | 0x1;
        packet->header->RD = true;
    }
    else 
    {
        packet->raw_message[RD_POS] = packet->raw_message[RD_POS] & 0xFE;
        packet->header->RD = false;
    }
}

void
update_ID(Packet_t *packet, byte_t ID_byte_1, byte_t ID_byte_2)
{
    // Bytes at index 2 and 3 of the raw_message are ID field bytes
    size_t ID_POS = 2;
    packet->raw_message[ID_POS] = ID_byte_1;
    packet->raw_message[ID_POS+1] = ID_byte_2;
    packet->header->ID = append_2_bytes(ID_byte_1, ID_byte_2);
}

void
update_TTL(Packet_t *packet)
{
    if (!packet->header->ANCOUNT) return;
    time_t current_time = get_current_time_raw();
    packet->TTL = packet->time_expire - current_time;
    int TTL_POS = LENGTH_HEADER_SIZE + HEADER_SIZE + packet->question->length;
    // TTL starts at the 7th byte of the answer section and takes up 4 bytes
    TTL_POS += 6;
    if (packet->TTL > 0)
    {
        double_byte_t TTL_1 = get_1st_double_byte_from_quad_byte(packet->TTL);
        double_byte_t TTL_2 = get_2nd_double_byte_from_quad_byte(packet->TTL);
        byte_t byte_1 = get_1st_byte_from_double_byte(TTL_1);
        byte_t byte_2 = get_2nd_byte_from_double_byte(TTL_1);
        byte_t byte_3 = get_1st_byte_from_double_byte(TTL_2);
        byte_t byte_4 = get_2nd_byte_from_double_byte(TTL_2);
        packet->raw_message[TTL_POS] = byte_1;
        packet->raw_message[TTL_POS+1] = byte_2;
        packet->raw_message[TTL_POS+2] = byte_3;
        packet->raw_message[TTL_POS+3] = byte_4;
    }
    else
    {
        packet->raw_message[TTL_POS] = 0;
        packet->raw_message[TTL_POS+1] = 0;
        packet->raw_message[TTL_POS+2] = 0;
        packet->raw_message[TTL_POS+3] = 0;
    }
}

/*  Resets the header without changing the ID or the QDCOUNT. */
void
reset_header(Packet_t *packet)
{
    reset_query_parameters(packet);
    // Here we are accessing all the bytes in the header except ID bytes
    // and setting them all to 0.
    packet->header->QDCOUNT = 0;
    packet->raw_message[6] = packet->raw_message[6] & 0x0;
    packet->raw_message[7] = packet->raw_message[7] & 0x0;

    packet->header->ANCOUNT = 0;
    packet->raw_message[8] = packet->raw_message[8] & 0x0;
    packet->raw_message[9] = packet->raw_message[9] & 0x0;

    packet->header->NSCOUNT = 0;
    packet->raw_message[10] = packet->raw_message[10] & 0x0;
    packet->raw_message[11] = packet->raw_message[11] & 0x0;

    packet->header->ARCOUNT = 0;
    packet->raw_message[12] = packet->raw_message[12] & 0x0;
    packet->raw_message[13] = packet->raw_message[13] & 0x0;
}

void
reset_query_parameters(Packet_t *packet)
{
    static int QUERY_PARAMS_POS = 4;
    packet->raw_message[QUERY_PARAMS_POS] = packet->raw_message[QUERY_PARAMS_POS] & 0x0;
    packet->raw_message[QUERY_PARAMS_POS+1] = packet->raw_message[QUERY_PARAMS_POS+1] & 0x0;
}
