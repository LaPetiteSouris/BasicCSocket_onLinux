OBJS = server_run.o client_run.o
CC = gcc
DEBUG = -g
LFLAGS = -lssl -lcrypto 

socket :	$(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o socket

server_run.o : server/server_start.c server/tcpserver.h server/tcpserver.c serialization/tcp_query_packet.h  serialization/tcp_query_packet.c server/udpserver.h server/udpserver.c serialization/udp_query_packet.h serialization/udp_query_packet.c
	$(CC) $(DEBUG) server/server_start.c -o server_run $(LFLAGS)

client_run.o : client/client_start.cclient/tcpclient.h /client/tcpclient.c serialization/tcp_query_packet.h serialization/tcp_query_packet.c client/udpclient.h client/udpclient.c serialization/udp_query_packet.h serialization/udp_query_packet.c
	$(CC) client/client_start.c -o client_run $(LFLAGS)

