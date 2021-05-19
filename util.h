
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

typedef uint8_t byte_t;
typedef uint16_t double_byte_t;
typedef uint32_t quad_byte_t;
typedef enum {REQUEST, UNIMPLEMENTED_REQUEST, CACHE_EXPIRY, CACHE_EVICTION, RESPONSE} log_t;


/*  Prints a message to stderr then exits with failure code 1. */
void exit_with_error(char *message);

/*  Prints a message to stdout followed by a newline character. */
void println(char *message);

time_t get_current_time_raw();

void get_current_timestamp(char *time_buffer, size_t buffer_length);

void convert_raw_time(char *time_buffer, size_t buffer_length, time_t raw_time);

void log_request(FILE *fp, log_t log_type, char *req_domain_name, char *IP_address);

void log_cache(FILE *fp, log_t log_type, char *req_domain_name, char *evict_domain_name, char *cache_expiry_timestamp);


double_byte_t append_2_bytes(byte_t byte_1, byte_t byte_2);

quad_byte_t append_4_bytes(byte_t byte_1, byte_t byte_2, byte_t byte_3, byte_t byte_4);

byte_t get_1st_nibble_from_byte(byte_t byte);

byte_t get_2nd_nibble_from_byte(byte_t byte);

byte_t get_1st_byte_from_double_byte(double_byte_t double_byte);

byte_t get_2nd_byte_from_double_byte(double_byte_t double_byte);

double_byte_t get_1st_double_byte_from_quad_byte(quad_byte_t quad_byte);

double_byte_t get_2nd_double_byte_from_quad_byte(quad_byte_t quad_byte);


void print_byte_as_hexes(byte_t byte);

void print_double_byte_as_hexes(double_byte_t double_byte);

void print_quad_byte_as_hexes(quad_byte_t quad_byte);

#endif
