OBJS1 = tcpserver.o udpserver.o tcp_query_packet.o udp_query_packet.o log.o
OBJS2 = tcpclient.o udpclient.o tcp_query_packet.o udp_query_packet.o
CC = gcc
DEBUG = -g
LFLAGS = -lssl -lcrypto 
CFLAGS = -c 

#Main server object

sock_server :	$(OBJS1)
	$(CC)   $(OBJS1) -o  sock_server $(LFLAGS)

#Server side dependencies

tcpserver.o :  server/tcpserver.c  server/tcpserver.h server/udpserver.h serialization/tcp_query_packet.h server/log.h
	$(CC) $(DEBUG) $(CFLAGS) server/tcpserver.c 

udpserver.o : server/udpserver.c server/udpserver.h serialization/udp_query_packet.h
	$(CC) $(DEBUG) $(CFLAGS) server/udpserver.c 

#Main client side object

sock_client : $(OBJS2)
	$(CC) $(OBJS2) -o sock_client $(LFLAGS)

#Client side dependencies

tcpclient.o : client/tcpclient.c client/tcpclient.h client/udpclient.h
	$(CC) $(DEBUG) $(CFLAGS)  client/tcpclient.c 

udpclient.o : client/udpclient.c client/udpclient.h 
	$(CC) $(DEBUG)  $(CFLAGS) client/udpclient.c 


#Data serialization dependencies

tcp_query_packet.o : serialization/tcp_query_packet.c serialization/tcp_query_packet.h
	$(CC) $(CFLAGS) $(DEBUG) serialization/tcp_query_packet.c  

udp_query_packet.o : serialization/udp_query_packet.c serialization/udp_query_packet.h
	$(CC) $(CFLAGS) $(DEBUG) serialization/udp_query_packet.c 

log.o : server/log.c server/log.h
	$(CC) $(CFLAGS) $(DEBUG) server/log.c


clean:
	\rm *.o *~