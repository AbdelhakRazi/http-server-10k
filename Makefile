CC=g++
CPPFLAGS=--std=c++14
OBJECTS = main.o tcp_server.o http_parser.o thread_pool.o add_client.o read_data.o send_request.o
# we should include header files, because a rule detects when a file changes, in our case it's detecting only .cc since
# we're pasing only cc files, we should put headers used in prequesities fields, to be detected if changed.
# automatic variables
# $@: target of the rule
# $^: all prequisites: like parser.cc source.cc...
# $<: first prerequisite
# $*: target file name without extension
# % : is used for pattern matching, for example %.o matches all .o files, %.c matches all .c files
server.out: main.o
	$(CC) $(CPPFLAGS) -o $*.out $(OBJECTS)
tcp_server.o: server/tcp_server.cc http_parser.o thread_pool.o add_client.o read_data.o send_request.o
	$(CC) $(CPPFLAGS) -c $< -o $@
main.o: main.cc tcp_server.o
	$(CC) $(CPPFLAGS) -c $< -o $@
http_parser.o: parser/http_parser.cc
	$(CC) $(CPPFLAGS) -c $^ -o $@
thread_pool.o: thread_pool/thread_pool.cc
	$(CC) $(CPPFLAGS) -c $^ -o $@
add_client.o: task/add_client.cc
	$(CC) $(CPPFLAGS) -c $^ -o $@
read_data.o: task/read_data.cc
	$(CC) $(CPPFLAGS) -c $^ -o $@
send_request.o: task/send_request.cc
	$(CC) $(CPPFLAGS) -c $^ -o $@

.PHONY: clean # phony is used when directory contains file that might be named "clean", so to avoid conflict.
clean:
	rm $(OBJECTS)