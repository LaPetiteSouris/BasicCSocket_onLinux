#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
int start_UDP_socket()
{	int result=0;
	char *IP = "127.0.0.1";
	int port =8080;
	int socket_fd;
	struct sockaddr_in server_address;
	//Open socket here
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd < 0)
	{
		printf("Error in creating socket");
		result=0;
	} else
	{
		//Declare socket address
		//First, clear structure
		memset((char *)&server_address, 0, sizeof(server_address));
		//Assign value to server address
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(port);
		//Convert IP address binary network bytes order
		if(inet_pton(AF_INET, IP, &server_address.sin_addr.s_addr))
		{
			//Conversion completed.Continue socket binding
			if(bind(socket_fd, server_address, sizeof(server_address)));


		} else 
		{
			result=0;
		}

	}




}