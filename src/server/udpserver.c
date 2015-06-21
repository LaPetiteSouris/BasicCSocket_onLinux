#include "udpserver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "../serialization/udp_query_packet.h"
int start_UDP()
{	int result = 0;
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
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		//Continue socket binding
		if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) >= 0)
		{
			struct sockaddr client_address;
			client_addr_len = sizeof(client_address);
			//Allocate space before deserialization
			struct udpquery * buff = (struct udpquery *)malloc(sizeof(struct udpquery));
			struct udpquery incoming;

			for (;;) {
				int message_size = recvfrom(socket_fd, buff, sizeof(*buff) , 0, &client_address, &client_addr_len );
				//De-serialization
				memcpy(&incoming, buff, sizeof(struct udpquery));
				//Echo back
				//sendto(socket_fd, buff, buff_size, 0, &client_address, client_addr_len);
				//buff[255] = '\0';
				printf("Server receiving request. Request content is:  \n");
				printf("%s\n", incoming.msg);
				free(buff);
			}

		} else
		{
			result = 0;
		}


	}
	return result;

}