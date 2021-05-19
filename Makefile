# Sample Makefile
# CC - compiler
# HELPERS - compiled source files that should be linked
# COPT - compiler flags
# BIN - binary
CC=clang
# CC=gcc
HELPERS=util.o packet.o server.o client.o cache.o
CI_HELPERS=util.o packet.o client.o cache.o server.o
COPT=-Wall -Wpedantic -g # -lpthread
# BIN_PHASE1=phase1
BIN_PHASE2=dns_svr

# Running "make" with no argument will make the first target in the file
all: $(BIN_PHASE1) $(BIN_PHASE2)

# Rules of the form
#     target_to_be_made : dependencies_to_be_up-to-date_first
#     <tab>commands_to_make_target
# (Note that spaces will not work.)

$(BIN_PHASE2): main.c $(HELPERS)
	$(CC) -o $(BIN_PHASE2) main.c $(HELPERS) $(COPT) -lpthread

# $(BIN_PHASE1): phase1.c $(HELPERS)
# 	$(CC) -o $(BIN_PHASE1) phase1.c $(HELPERS) $(COPT)

# Wildcard rule to make any .o file,
# given a .c and .h file with the same leading filename component
%.o: %.c %.h
	$(CC) -c $< $(COPT) -g

format:
	clang-format -i *.c *.h

.PHONY: clean
clean:
	rm -rf dns_svr *.o *.log

clean_local:
	rm dns_svr *.o *.log

run:
	./dns_svr 128.250.66.5 53

run1:
	./dns_svr < packets/1.comp30023.a.req.raw
run2:
	./dns_svr < packets/1.comp30023.req.raw
run3:
	./dns_svr < packets/1.comp30023.res.raw
run4:
	./dns_svr < packets/2.comp30023.req.raw
run5:
	./dns_svr < packets/2.comp30023.res.raw
run6:
	./dns_svr < packets/5s.comp30023.0x.quest.req.raw
run7:
	./dns_svr < packets/5s.comp30023.0x.quest.res.raw
run8:
	./dns_svr < packets/30s.comp30023.0x.quest.req.raw
run9:
	./dns_svr < packets/30s.comp30023.0x.quest.res.raw
run10:
	./dns_svr < packets/60s.comp30023.0x.quest.req.raw
run11:
	./dns_svr < packets/60s.comp30023.0x.quest.res.raw
run12:
	./dns_svr < packets/cloudflare.com.req.raw
run13:
	./dns_svr < packets/cloudflare.com.res.raw
run14:
	./dns_svr < packets/none.comp30023.req.raw
run15:
	./dns_svr < packets/none.comp30023.res.raw
