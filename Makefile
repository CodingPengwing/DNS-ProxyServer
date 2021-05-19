# This file is referenced from sample files provided to students as part of project 2 Comp30023 Melbourne University 2021.
# This file contains all the logic required to compile and run the program, as well as clean afterwards.

# CC - compiler
CC=clang
# OBJ - compiled source files that should be linked
OBJ=util.o packet.o server.o client.o cache.o
# COPT - compiler flags
COPT=-Wall -Wpedantic -g 
# BIN - binary
BIN_MAIN=dns_svr

# Running "make" with no argument will make the first target in the file
all: $(BIN_MAIN)

# Rules of the form
#     target_to_be_made : dependencies_to_be_up-to-date_first
#     <tab>commands_to_make_target
# (Note that spaces will not work.)

$(BIN_MAIN): main.c $(OBJ)
	$(CC) -o $(BIN_MAIN) main.c $(OBJ) $(COPT) -lpthread


# Wildcard rule to make any .o file,
# given a .c and .h file with the same leading filename component
%.o: %.c %.h
	$(CC) -c $< $(COPT) -g

format:
	clang-format -i *.c *.h

.PHONY: clean
clean:
	rm -rf dns_svr *.o *.log

run:
	./dns_svr 128.250.66.5 53
