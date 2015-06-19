#include "tcpserver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int start_TCP_socket()
{
	int result = 0;
	int port = 8080;
	int socket_fd;
	struct sockaddr_in server_address;
	socklen_t client_addr_len;
	int buff_size = 255;
	//Open socket here
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
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
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		// socket binding
		if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) >= 0)
		{
			//Create a struct to store peer address:
			struct sockaddr peer_addr;
			memset((char *)&peer_addr, 0, sizeof(peer_addr));
			socklen_t peer_addr_len = sizeof(peer_addr);
			listen(socket_fd, 5);
			char buff[255];
			char buff_response[] = "Received data. Process completed. \0";
			int len = sizeof(buff_response);
			for (;;)
			{
				//Accept connection
				int new_sock = accept(socket_fd, (struct sockaddr *) &peer_addr, &peer_addr_len );
				if (new_sock < 0)
				{

					result = 0;
				} else
				{
					//Receiving connection request from client
					int n = recv(new_sock, buff, buff_size,0 );
					if (n < 0)
					{
						printf("Error reading socket");
						result = 0;
					} else
					{
						printf("Receiving data from client. Content is: \n");
						printf("%s \n", buff);
						//Response
						n = send(new_sock, buff_response, len,0);
						if (n < 0)
						{
							printf("Error writing socket");
							result = 0;
						}
					}
				}
			}

		}
		else
		{
			result = 0;
		}
	}
	return result;

}

