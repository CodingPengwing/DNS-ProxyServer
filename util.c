
#include "util.h"

/*  Prints a message to stderr then exits with failure code 1. */
void 
exit_with_error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

/*  Prints a message to stdout followed by a newline character. */
void
println(char *message)
{
    printf("%s\n", message);
}

/*  Get the current time in raw time_t format. */
time_t
get_current_time_raw()
{
    time_t raw_time;
    // Get the timer value since time 0 in UTC and put into raw_time
    time(&raw_time);
    return raw_time;
}

/*  Get the current time in readable time format. */
void
get_current_timestamp(char *time_buffer, unsigned int buffer_length)
{
    time_t raw_time = get_current_time_raw();
    convert_raw_time(time_buffer, buffer_length, raw_time);
}

/*  Converts a raw time into the specified time format and fills the string time_buffer. */
void
convert_raw_time(char *time_buffer, unsigned int buffer_length, time_t raw_time)
{
    // Compute the localtime by using the raw_time value
    struct tm *info = localtime(&raw_time);
    // Fill the time_buffer with time info according to the format
    strftime(time_buffer, buffer_length, "%FT%T%z", info);
}

/*  Logs an event that is standard operation. */
void
log_request(FILE *fp, log_t log_type, char *req_domain_name, char *IP_address)
{
    // Get the current timestamp
    static int TIME_BUFFER_LEN = 80;
    char current_timestamp[TIME_BUFFER_LEN];
    time_t current_time_raw = get_current_time_raw();
    convert_raw_time(current_timestamp, TIME_BUFFER_LEN, current_time_raw);
    
    // Log the item
    switch (log_type)
    {
        case REQUEST:
            if (!req_domain_name) exit_with_error("Error in log_request(): case REQUEST, not enough arguments provided.");
            fprintf(fp, "%s requested %s\n", current_timestamp, req_domain_name);
            break;
        case UNIMPLEMENTED_REQUEST:
            fprintf(fp, "%s unimplemented request\n", current_timestamp);
            break;
        case RESPONSE:
            if (!req_domain_name || !IP_address) exit_with_error("Error in log_request(): case RESPONSE, not enough arguments provided.");
            fprintf(fp, "%s %s is at %s\n", current_timestamp, req_domain_name, IP_address);
            break;
        default:
            exit_with_error("Error in log_request(): invalid log type.");
            break;
    }
}


/*  Logs an event that is related to cache. */
void
log_cache(FILE *fp, log_t log_type, char *req_domain_name, char *evict_domain_name, char *cache_expiry_timestamp)
{
    // Get the current timestamp
    static int TIME_BUFFER_LEN = 80;
    char current_timestamp[TIME_BUFFER_LEN];
    time_t current_time_raw = get_current_time_raw();
    convert_raw_time(current_timestamp, TIME_BUFFER_LEN, current_time_raw);
    
    // Log the item
    switch (log_type)
    {
        case CACHE_EXPIRY:
            if (!req_domain_name || !cache_expiry_timestamp)  exit_with_error("Error in log_cache(): case CACHE_EXPIRY, not enough arguments provided.");
            fprintf(fp, "%s %s expires at %s\n", current_timestamp, req_domain_name, cache_expiry_timestamp);
            break;
        case CACHE_EVICTION:
            if (!req_domain_name || !evict_domain_name) exit_with_error("Error in log_cache(): case REQUEST, not enough arguments provided.");
            fprintf(fp, "%s replacing %s by %s\n", current_timestamp, evict_domain_name, req_domain_name);
            break;
        default:
            exit_with_error("Error in log_cache(): invalid log type.");
            break;
    }
}



/***    Byte and bit manipulations  ***/

double_byte_t
append_2_bytes(byte_t byte_1, byte_t byte_2)
{
    int by_8_bits = 8;
    double_byte_t result = byte_1;
    result = result << by_8_bits;
    result = result | byte_2;
    return result;
}

quad_byte_t
append_4_bytes(byte_t byte_1, byte_t byte_2, byte_t byte_3, byte_t byte_4)
{
    int by_8_bits = 8, by_16_bits = 16, by_24_bits = 24;
    quad_byte_t byte__1 = (quad_byte_t) byte_1;
    byte__1 = byte__1 << by_24_bits;
    quad_byte_t byte__2 = (quad_byte_t) byte_2;
    byte__2 = byte__2 << by_16_bits;
    quad_byte_t byte__3 = (quad_byte_t) byte_3;
    byte__3 = byte__3 << by_8_bits;
    quad_byte_t byte__4 = (quad_byte_t) byte_4;
    return byte__1 | byte__2 | byte__3 | byte__4;
}

byte_t
get_1st_nibble_from_byte(byte_t byte)
{
    int by_4_bits = 4;
    byte_t result = byte >> by_4_bits;
    return result;
}

byte_t
get_2nd_nibble_from_byte(byte_t byte)
{
    int by_4_bits = 4;
    byte_t result = byte << by_4_bits;
    result = result >> by_4_bits;
    return result;
}

byte_t
get_1st_byte_from_double_byte(double_byte_t double_byte)
{
    int by_8_bits = 8;
    byte_t result = double_byte >> by_8_bits;
    return result;
}

byte_t
get_2nd_byte_from_double_byte(double_byte_t double_byte)
{
    int by_8_bits = 8;
    double_byte = double_byte << by_8_bits;
    byte_t result = double_byte >> by_8_bits;
    return result;
}



void 
print_byte_as_hexes(byte_t byte) 
{
    byte_t nibble_1 = get_1st_nibble_from_byte(byte);
    byte_t nibble_2 = get_2nd_nibble_from_byte(byte);
    printf("%X%X", nibble_1, nibble_2);
}

void print_double_byte_as_hexes(double_byte_t double_byte)
{
    byte_t byte_1 = get_1st_byte_from_double_byte(double_byte);
    byte_t byte_2 = get_2nd_byte_from_double_byte(double_byte);
    print_byte_as_hexes(byte_1);
    print_byte_as_hexes(byte_2);
}

void print_quad_byte_as_hexes(quad_byte_t quad_byte)
{
    int by_16_bits = 16;
    quad_byte_t double_byte_1 = quad_byte;
    double_byte_1 = double_byte_1 >> by_16_bits;
    quad_byte_t double_byte_2 = quad_byte;
    double_byte_2 = double_byte_2 << by_16_bits;
    double_byte_2 = double_byte_2 >> by_16_bits;
    print_double_byte_as_hexes(double_byte_1);
    print_double_byte_as_hexes(double_byte_2);
}
