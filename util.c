
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

time_t
get_current_time_raw()
{
    time_t raw_time;
    // Get the timer value since time 0 in UTC and put into raw_time
    time(&raw_time);
    return raw_time;
}

void
get_current_timestamp(char *time_buffer, unsigned int buffer_length)
{
    time_t raw_time = get_current_time_raw();
    convert_raw_time(time_buffer, buffer_length, raw_time);
}

void
convert_raw_time(char *time_buffer, unsigned int buffer_length, time_t raw_time)
{
    // Compute the localtime by using the raw_time value
    struct tm *info = localtime(&raw_time);
    // Fill the time_buffer with time info according to the format
    strftime(time_buffer, buffer_length, "%FT%T%z", info);
}

void
print_log(FILE *fp, log_t log_type, char *domain_name, char *TTL_timestamp)
{

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
