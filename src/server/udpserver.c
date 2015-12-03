#include "udpserver.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "../serialization/udp_query_packet.h"
#include "log.h"
char log_c[2560] ;
//Client IP address
char ip[256];
int start_UDP(char key[255], int port)
{	int result = 0;
	int socket_fd;
	struct sockaddr_in server_address;
	socklen_t client_addr_len;
	int buff_size = 255;
	//Open socket here
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd < 0)
	{
		perror("Error in creating socket");
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
			struct sockaddr_in client_address;
			client_addr_len = sizeof(client_address);
			//Allocate space to receive buffer data
			struct udpquery * buff = (struct udpquery *)malloc(sizeof(struct udpquery));
			struct udpquery incoming;
			for (;;) {
				int message_size = recvfrom(socket_fd, buff, sizeof(*buff) , 0,  (struct sockaddr *)&client_address, &client_addr_len );
				sprintf(ip, "Client IP %s", inet_ntoa(client_address.sin_addr));
				printf("%s\n", "UDP Server received a query." );
				sprintf(log_c, "%s :UDP Server received a query.\n", ip);
				log_server(log_c);
				if (message_size > 0 && sizeof(*buff) == sizeof(incoming))
				{
					//De-serialization
					incoming = deserialization_udp(buff);
					//Free allocated space storing received buffer data
					free(buff)	;
					int packet_verification = verify_udp_packet(&incoming);
					if (packet_verification == 1)
					{
						printf("UDP Query follows defined protocol. Accepted. \n");
						sprintf(log_c, "%s :UDP Query follows defined protocol. Accepted. \n",ip);
						log_server(log_c);
						int integrity_check = verify_hmac(incoming, key);
						response_to_client(integrity_check, key, socket_fd, client_address);
						break;
					}
					else if (packet_verification == 0)
					{
						printf("UDP: Client query does not follow protocol. Rejected. \n\n");
						sprintf(log_c, "%s :UDP Query follows defined protocol. Accepted. \n",ip);
						log_server(log_c);
						break;
					}
				}
			}
		} else
		{
			result = 0;
		}
	}
	return result;
}

void response_to_client(int result, char key[], int socket_fd, struct sockaddr_in client_address)
{
	if (result == 0)
	{
		printf("%s\n", "Sent ping to client and terminated connection\n");
		sprintf(log_c, "%s :Sent ping to client and terminated connection\n",ip);
		log_server(log_c);
		struct udpquery query = pack_udp_data(key);
		//Serialization
		struct udpquery * buf = serialization_udp(query);
		//Send ping message to server
		sendto(socket_fd, buf, sizeof(*buf), 0, (struct sockaddr *)&client_address, sizeof(client_address));
		free(buf);
	}
}



