#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "tcpclient.h"
#include <time.h>

int32_t random1_6() {
	return ((rand() % 6) + 1);
}

int startTCPClient()
{	int result = 0;
	//Open a client socket
	int socket_fd;
	socklen_t client_addr_len;
	int port = 8080;
	char buff_recv[255];
	int buff_size = 255;
	int buff_recv_size = sizeof(buff_recv);
	char *IP = "127.0.0.1";
	//Open socket here
	struct sockaddr_in client_address;
	memset((char *)&client_address, 0, sizeof(client_address));
	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = htonl(INADDR_ANY);
	client_address.sin_port = htons(0);
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	//Connect to TCP server
	struct sockaddr_in server_address;
	memset((char *)&server_address, 0, sizeof(server_address));
	//Assign value to server address
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	if (inet_pton(AF_INET, IP, &server_address.sin_addr.s_addr))
	{
		socklen_t server_addr_len = sizeof(server_address);
		int n = connect(socket_fd, (struct sockaddr *) &server_address, server_addr_len);
		if (n < 0)
		{
			printf("Connection to server failed");
			result = 0;
		}
		else
		{
			char str[255];
			printf("Enter your username: ");
			scanf("%s", str);
			struct tcpquery * buf = (struct tcpquery *)malloc(sizeof(struct tcpquery));
			struct tcpquery query;
			query = packdata(str);
			memcpy(buf, &query, sizeof(struct tcpquery));
			//Connection established. Start sending data to TCP server
			send(socket_fd, buf, buff_size, 0);
			//Receving response from server.


			int n = recv(socket_fd, buff_recv, buff_recv_size, 0);
			if (n < 0)
			{
				printf("Connection to server failed");
				result = 0;
			} else
			{
				struct tcpquery * buff = (struct tcpquery *)malloc(sizeof(struct tcpquery));
				struct tcpquery incoming;
				printf("Received response from server: ");
				memcpy(&incoming, buff, sizeof(struct tcpquery));
				printf("%s \n", incoming.command);
			}
		}
	}
	else
	{
		printf("Connection to server failed");
		result = 0;

	}

	return result;

}


struct tcpquery packdata( char msg[255])
{
	struct tcpquery data = {"DISTRIB2015", 255, 'D', "any"};
	strncpy(data.command, msg, sizeof(data.command));
	return data;
}
