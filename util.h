
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE 1

typedef uint8_t byte_t;
typedef uint16_t double_byte_t;

/*  Prints a message to stderr then exits with failure code 1. */
void exit_with_error(char *message);

/*  Prints a message to stdout followed by a newline character. */
void println(char *message);

double_byte_t append_2_bytes(byte_t byte_1, byte_t byte_2);

byte_t get_1st_nibble_from_byte(byte_t byte);

byte_t get_2nd_nibble_from_byte(byte_t byte);

byte_t get_1st_byte_from_double_byte(double_byte_t double_byte);

byte_t get_2nd_byte_from_double_byte(double_byte_t double_byte);

void print_byte_as_hexes(byte_t byte);

void print_double_byte_as_hexes(double_byte_t double_byte);

#endif
