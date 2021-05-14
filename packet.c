
#include "packet.h"

Packet_t *
new_packet(byte_t *raw_data, int length)
{
    Packet_t *packet = (Packet_t*)malloc(sizeof(*packet));
    if (!packet) exit_with_error("Error in new_packet(): failed malloc.");
    packet->raw_data = raw_data;
    packet->length = length;

    // find header section

    // check if is_question

    // find question section, create question struct

    // find resourceRecord section, create resourceRecord struct

    return packet;
}

Packet_t * 
parse_packet_data(Packet_t* packet)
{
    byte_t *raw_data = packet->raw_data;
    int data_length = packet->length;
    printf("The content of the file is:\n");
    for (int i = 0; i < data_length; i++) 
    {
        print_byte_as_hexes(raw_data[i]);
        if (i < data_length-1) printf(", ");
        else println("");
    }

    Header_t *header = new_header(raw_data);
    packet->header = header;
    print_header(packet->header);

    if (packet->header->QDCOUNT) 
    {

    }
    else exit_with_error("Error in parse_packet_data(): ");
    
    return packet;
}

void
free_packet(Packet_t* packet)
{
    if (!packet) exit_with_error("Error in free_packet(): null pointer.");
    if (packet->raw_data) free(packet->raw_data);
    if (packet->header) free_header(packet->header);
    if (packet->question) free_question(packet->question);
    if (packet->answer) free_resourceRecord(packet->answer);
}

/*  
    This function creates a new header, it assumes that the raw data given has length of 12 bytes
    as specified by RFC2535. If this condition is not met, there will be an error.
*/
Header_t *
new_header(byte_t *header_raw_data)
{
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

    Header_t *header = (Header_t*)malloc(sizeof(*header));
    if (!header) exit_with_error("Error in new_header(): failed malloc.");

    // *** Parse the raw header data into individual fields ***

    // The ID field is the first 2 bytes of the header
    header->ID = append_2_bytes(header_raw_data[0], header_raw_data[1]);
    // The query parameters are bytes 3 and 4 of the header
    double_byte_t query_parameters = append_2_bytes(header_raw_data[2], header_raw_data[3]);
    // The QDCOUNT is bytes 5 and 6 of the header
    header->QDCOUNT = append_2_bytes(header_raw_data[4], header_raw_data[5]);
    // The ANCOUNT is bytes 6 and 6 of the header
    header->ANCOUNT = append_2_bytes(header_raw_data[6], header_raw_data[7]);
    // The NSCOUNT is bytes 8 and 9 of the header
    header->NSCOUNT = append_2_bytes(header_raw_data[8], header_raw_data[9]);
    // The ARCOUNT is bytes 10 and 11 of the header
    header->ARCOUNT = append_2_bytes(header_raw_data[10], header_raw_data[11]);

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

void
print_header(Header_t *header)
{
    if (!header) exit_with_error("Error in print_header(): null pointer.");
    printf("Header ID: "); print_double_byte_as_hexes(header->ID);
    println("");
    printf("QR: %X\n", header->QR);
    printf("Opcode: %X\n", header->Opcode);
    printf("AA: %X\n", header->AA);
    printf("TC: %X\n", header->TC);
    printf("RD: %X\n", header->RD);
    printf("RA: %X\n", header->RA);
    printf("Z: %X\n", header->Z);
    printf("AD: %X\n", header->AD);
    printf("CD: %X\n", header->CD);
    printf("RCODE: %X\n", header->RCODE);
    printf("QDCOUNT: %X\n", header->QDCOUNT);
    printf("ANCOUNT: %X\n", header->ANCOUNT);
    printf("NSCOUNT: %X\n", header->NSCOUNT);
    printf("ARCOUNT: %X\n", header->ARCOUNT);
}

void
free_header(Header_t *header)
{
    if (!header) exit_with_error("Error in free_header(): null pointer.");
    free(header);
}





Question_t *
new_question(byte_t *question_raw_data, int length)
{
    Question_t *question = (Question_t*)malloc(sizeof(*question));
    if (!question) exit_with_error("Error in new_question(): failed malloc.");
    // find domain name, malloc
    // find type
    // find class
    return question;
}

void
free_question(Question_t *question)
{
    if (!question) exit_with_error("Error in free_question(): null pointer.");
    free(question->domain_name);
    free(question);
}





ResourceRecord_t *
new_resourceRecord(byte_t *resourceRecord_raw_data, int length)
{
    ResourceRecord_t *resourceRecord = (ResourceRecord_t*)malloc(sizeof(*resourceRecord));
    if (!resourceRecord) exit_with_error("Error in new_resourceRecord(): failed malloc.");
    return resourceRecord;
}

void
free_resourceRecord(ResourceRecord_t *resourceRecord)
{
    if (!resourceRecord) exit_with_error("Error in free_resourceRecord(): null pointer.");
    free(resourceRecord);
}


