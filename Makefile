CC=g++
CXCFLAGS=--std=c++14
OBJECTS = main.o tcp_server.o http_parser.o thread_pool.o add_client.o read_data.o send_request.o
# we should include header files, because a rule detects when a file changes, in our case it's detecting only .cc since
# we're pasing only cc files, we should put headers used in prequesities fields, to be detected if changed.
server.out: main.o
	$(CC) $(CXCFLAGS) -o server.out $(OBJECTS)
http_parser.o: parser/http_parser.cc
	$(CC) $(CXCFLAGS) -c parser/http_parser.cc -o http_parser.o
thread_pool.o: thread_pool/thread_pool.cc
	$(CC) $(CXCFLAGS) -c thread_pool/thread_pool.cc -o thread_pool.o
add_client.o: task/add_client.cc
	$(CC) $(CXCFLAGS) -c task/add_client.cc -o add_client.o
read_data.o: task/add_client.cc
	$(CC) $(CXCFLAGS) -c task/read_data.cc -o read_data.o
send_request.o: task/send_request.cc
	$(CC) $(CXCFLAGS) -c task/send_request.cc -o send_request.o
tcp_server.o: server/tcp_server.cc http_parser.o thread_pool.o add_client.o read_data.o send_request.o
	$(CC) $(CXCFLAGS) -c server/tcp_server.cc -o tcp_server.o
main.o: tcp_server.o
	$(CC) $(CXCFLAGS) -c main.cc -o main.o

.PHONY: clean # phony is used when directory contains file that might be named "clean", so to avoid conflict.
clean:
	rm $(OBJECTS)