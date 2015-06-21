#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "serialization/udp_query_packet.h"
#include <time.h>

int32_t random1_6() {
	return ((rand() % 6) + 1);
}
struct udpquery packdata()
{
	//Declare query packet frame here.
	char s[5] = "Ping";
	srand(time(NULL));
	int32_t sid = random1_6();
	int32_t tid = random1_6();
	struct udpquery data = {'D', 'S', 4, sid, tid, 4, "any"};
	strncpy(data.msg, s, sizeof(data.msg));
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
			int buff_size = 5;
			memset((char *)&server_address, 0, sizeof(server_address));
			//Assign value to server address
			server_address.sin_family = AF_INET;
			server_address.sin_port = htons(port);
			if (inet_pton(AF_INET, IP, &server_address.sin_addr.s_addr))
			{
				//Serialization
				struct udpquery * buf = (struct udpquery *)malloc(sizeof(struct udpquery));
				struct udpquery query;
				query = packdata();
				memcpy(buf, &query, sizeof(struct udpquery));
				//Send ping message to server
				socklen_t server_addr_len = sizeof(server_address);
				sendto(socket_fd, buf, sizeof(*buf), 0, (struct sockaddr *)&server_address, server_addr_len);
				free(buf);

			}

		}
		else
		{
			result = 0;
		}
		return result;
	}
}




main()
{
	startUDPClient();

}