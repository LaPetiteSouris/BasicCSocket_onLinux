#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
int start_UDP_socket()
{	int result = 0;
	char *IP = "127.0.0.1";
	int port = 8080;
	int socket_fd;
	struct sockaddr_in server_address;
	socklen_t client_addr_len;
	int buff_size = 255;
	//Open socket here
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd < 0)
	{
		printf("Error in creating socket");
		result = 0;
	} else
	{
		//Declare socket address
		//First, clear structure
		memset((char *)&server_address, 0, sizeof(server_address));
		//Assign value to server address
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(port);
		//Convert IP address binary network bytes order
		if (inet_pton(AF_INET, IP, &server_address.sin_addr.s_addr))
		{
			//Conversion completed.Continue socket binding
			if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) >= 0)
			{
				struct sockaddr client_address;
				client_addr_len = sizeof(client_address);
				//Add one character as terminator
				char  buff[256];
				for (;;) {
					int message_size = recvfrom(socket_fd, buff, 255 , 0, &client_address, &client_addr_len );
					//Echo back
					sendto(socket_fd, buff, buff_size,0, &client_address, client_addr_len);
					//buff[255] = '\0';
					printf("Server receiving request \n");
					printf("%s\n", buff);
				}

			} else
			{
				result = 0;
			}


		} else
		{
			result = 0;
		}
	}
	return result;

}