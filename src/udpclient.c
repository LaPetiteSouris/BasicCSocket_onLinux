#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "serialization/udp_query_packet.h"



main()
{
	startUDPClient();

}

struct udpquery packdata()
{
	//Declare query packet frame here.
	char s[5] = "Ping";
	srand(time(NULL));
	int32_t sid = rand();
	int32_t tid = rand();
	struct udpquery data = {'D', 'S', 4, sid, tid, 4, "any"};
	strcpy(data.buff, s);
	return data;

}

int startUDPClient()
{
	int result = 0;

//Open a client socket
	int socket_fd;
	socklen_t client_addr_len;
	//Open socket here
	struct sockaddr_in client_address;
	memset((char *)&client_address, 0, sizeof(client_address));
	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = htonl(INADDR_ANY);
	client_address.sin_port = htons(0);
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd < 0)
	{
		printf("Error in creating socket");
		result = 0;
	} else
	{
		//Bind client socket
		if (bind(socket_fd, (struct sockaddr *)&client_address, sizeof(client_address)) >= 0)
		{
			//Declare server connection:
			char *IP = "127.0.0.1";
			int port = 8080;
			struct sockaddr_in server_address;
			int buff_size = 255;
			memset((char *)&server_address, 0, sizeof(server_address));
			//Assign value to server address
			server_address.sin_family = AF_INET;
			server_address.sin_port = htons(port);
			if (inet_pton(AF_INET, IP, &server_address.sin_addr.s_addr))
			{
				//Send ping message to server
				void * buf;
				struct udpquery data = packdata();
				memcpy(buf, &data, sizeof(struct udpquery));
				socklen_t server_addr_len = sizeof(server_address);
				sendto(socket_fd, buf, sizeof(buf), 0, (struct sockaddr *)&server_address, server_addr_len);
			}

		}
		else
		{
			result = 0;
		}
		return result;
	}
}




