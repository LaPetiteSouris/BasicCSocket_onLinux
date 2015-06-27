#include "tcpserver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
//This is the H2 value, generate by server for client verification
char H2[255];
//This is random token generate by server.
int R;
int32_t random1_6() {
	return ((rand() % 6) + 1);
}

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
			//Allocate space for the buffer server is about to receive
			struct tcpquery * buffer = (struct tcpquery *) malloc(sizeof(struct tcpquery));
			struct tcpquery * buffer2 = (struct tcpquery *) malloc(sizeof(struct tcpquery));
			for (;;)
			{
				//Accept connection
				int new_sock = accept(socket_fd, (struct sockaddr *) &peer_addr, &peer_addr_len );
				if (new_sock == -1)
				{
					exit(1);
				}
				printf("Receiving connection from client. \n");
				for (;;) {

					//Receiving connection request from client.This is an username send for verification
					int n = recv(new_sock, buffer, sizeof(*buffer), 0 );
					if (n < 0)
					{
						printf("Data receiving from socket failed. Exit.\n");
						exit(1);
					} else if (n == 0)
					{
						printf("Connection error or closed\n");
						break;
					}
					struct tcpquery incoming = deserialization_tcp(buffer);
					if (verify_tcp_packet(&incoming) == 1)
					{
						int password = getpassword(incoming.command);
						if (password != 0)
						{
							if (randomtokenhandling(new_sock) < 0)
							{
								printf("Socket writing error.\n");
								exit(1);
							}
							int n = recv(new_sock, buffer2, sizeof(*buffer2), 0);
							if (n < 0)
							{
								printf("Data receiving from socket failed. Exit.\n");
								exit(1);
							} else if (n == 0)
							{
								printf("Connection closed\n");
								break;
							}
							if (generateH2value(new_sock, password, R))
							{
								if (H_value_compare(buffer2))
								{
									printf("Authorization completed. Success...\n");
								}
								else
								{
									printf("Authorization Failed\n");
								}
							} else {
								printf("Cannot generate authetication token\n");
							}

						} else
						{
							//User not found. Reject connection.
							printf("User not found. Rejected...\n");
							rejectconnection(new_sock);
							break;
						}

					} else
					{
						printf("Packet received did not follow defined protocol. Rejected. \n");
						break;
					}
				}
				close(new_sock);
			}

		}

	}
	return result;
}

int getpassword(char username[255])
{
	int result = 123;
	char user[255] = "debug\0";
	int compare_result = strcmp(username, user);
	if (compare_result != 0)
	{
		result = 0;
	}
	return result;
}


void rejectconnection(int new_sock)
{
	int R = 0;
	char r[255];
	sprintf(r, "%d", R);
	struct tcpquery query = pack_tcp_data(r);
	//Serialize response
	struct tcpquery *buffer = serialization_tcp(query);
	//Response
	send(new_sock, buffer, sizeof(*buffer), 0);
}

int randomtokenhandling(int new_sock)
{
	R = random1_6();
	char r[255];
	char pass[3];
	char H1[255];
	sprintf(r, "%d", R);
	struct tcpquery query = pack_tcp_data(r);
	struct tcpquery *buffer = serialization_tcp(query);
	int n = send(new_sock, buffer, sizeof(*buffer), 0);
	return n;

}

int H_value_compare(struct tcpquery * buffer)
{
	int result = 0;
	struct tcpquery incoming2 = deserialization_tcp(buffer);
	int verification2 = verify_tcp_packet(&incoming2);
	if (verification2 == 1)
	{
		//break;
		char H1[255];
		strcpy(H1, incoming2.command);
		//TO-DOCompare H1 with H2
		int cmp = strcmp(H1, H2);
		if (cmp == 0)
		{	result = 1;
		}
	} else
	{
		printf("Packet received did not follow defined protocol. Rejected. \n");
	}
	return result;
}

int  generateH2value(int new_sock, int password, int R)
{
	int result = 0;
	char r[255];
	char pass[3];
	sprintf(r, "%d", R);
	sprintf(pass, "%d", password);
	if (sizecheck(pass, r) == 1)
	{
		//Size does not reach limit. Proceed to concat
		strcpy(H2, pass);
		strcat(H2, r);
		result = 1;
	}
	return result;
}

int sizecheck(char array1[], char array2[])
{
	int result = 1;
	if ((strlen(array1) + strlen(array2) + 1) >= 255)
	{
		result = 0;
	}
	return result;
}


char * generateSHA(SHA256_CTX c, char * input, size_t len)
{
	char * hash = (char *) malloc(32);
	if (!SHA256_Init(&c))
	{
		hash = NULL;
	}
	if (!SHA256_Update(&c, input, len))
	{
		hash = NULL;
	}
	if (!SHA256_Final(hash, &c)) {
		hash = NULL;
	}
	return hash;
}
