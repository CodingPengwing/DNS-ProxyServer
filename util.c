
#include "util.h"

/*  Prints a message to stderr then exits with failure code 1. */
void 
exit_with_error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(FAILURE);
}

/*  Prints a message to stdout followed by a newline character. */
void
println(char *message)
{
    printf("%s\n", message);
}

double_byte_t
append_2_bytes(byte_t byte_1, byte_t byte_2)
{
    int by_8_bits = 8;
    double_byte_t result = byte_1;
    result = result << by_8_bits;
    result = result | byte_2;
    return result;
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
