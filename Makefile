CC=g++
CPPFLAGS=--std=c++14
SRC = $(wildcard *.c) $(wildcard subdir/*.c)
OBJ = $(SRC:.c=.o) # pattern to replace .c by .o files, we need OBJ for last compilation to generate executable.
# we should include header files, because a rule detects when a file changes, in our case it's detecting only .cc since
# we're pasing only cc files, we should put headers used in prequesities fields, to be detected if changed.
# automatic variables
# $@: target of the rule
# $^: all prequisites: like parser.cc source.cc...
# $<: first prerequisite
# $*: target file name without extension
# % : is used for pattern matching, for example %.o matches all .o files, %.c matches all .c files
# better approach now: % takes for each c file, and generate .o file.
server.out: $(OBJ)
	$(CC) $(CPPFLAGS) $*.out -o $@
tcp_server.o: server/tcp_server.cc http_parser.o thread_pool.o add_client.o read_data.o send_request.o
	$(CC) $(CPPFLAGS) -c $< -o $@
main.o: main.cc tcp_server.o
	$(CC) $(CPPFLAGS) -c $< -o $@
%.o: %.cc #useful for compiling files that are not depdendent
	$(CC) $(CPPFLAGS) -c $<  -o $@

.PHONY: clean # phony is used when directory contains file that might be named "clean", so to avoid conflict.
clean:
	rm $(OBJECTS)