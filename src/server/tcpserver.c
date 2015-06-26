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
#include "../serialization/tcp_query_packet.h"

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
			//Allocate space for the buffer server is about to receive
			struct tcpquery * buffer = (struct tcpquery *) malloc(sizeof(struct tcpquery));
			for (;;)
			{
				//Accept connection
				int new_sock = accept(socket_fd, (struct sockaddr *) &peer_addr, &peer_addr_len );
				if (new_sock < 0)
				{
					result = 0;
				} else
				{
					//Receiving connection request from client.This is an username send for verification
					int n = recv(new_sock, buffer, sizeof(*buffer), 0 );
					if (n < 0)
					{
						printf("Error reading socket");
						result = 0;
					} else
					{
						printf("Receiving data from client. \n");
						//Deserialize data
						struct tcpquery incoming = deserialization_tcp(buffer);
						int verification = verify_tcp_packet(&incoming);
						printf("Verification completed");
						printf("%d\n", verification);
						if (verification == 1)
						{
							int password = getpassword(incoming.command);
							if (password != 0)
							{
								//Username exists.
								//Create a random number
								int R = random1_6();
								char r[255];
								char pass[3];
								char H1[255];
								sprintf(r, "%d", R);
								sprintf(pass, "%d", password);
								struct tcpquery query = pack_tcp_data(r);
								//Serialize response
								buffer = serialization_tcp(query);
								//TO-Do Calculate P1+R1=>H2 value
								if (sizecheck(pass, r) == 1)
								{
									//Size does not reach limit. Proceed to concat
									strcpy(H1, pass);
									strcat(H1, r);
									//Response
									n = send(new_sock, buffer, sizeof(*buffer), 0);
									if (n < 0)
									{
										printf("Error writing socket");
										result = 0;
									} else
									{
										//Receive H1 from client
										int size = recv(new_sock, buffer, sizeof(buffer), 0);
										if (size > 0)
										{
											incoming = deserialization_tcp(buffer);
											verification = verify_tcp_packet(&incoming);
											if (verification == 1)
											{
												char H2[255];
												strcpy(H2, incoming.command);
												//TO-DOCompare H1 with H2
												int cmp = strcmp(H1, H2);
												if (cmp == 0)
												{
													result = 1;
													printf("Authorization completed");
												} else {
													result = 0;
													printf("Authorization failed");
												}
											} else
											{
												printf("Packet received did not follow defined protocol. Rejected.");
												result = 0;
											}

										} else
										{
											result = 0;

										}
									}
								}
								else
								{	result = 0;
									printf("Password and random string key size exceed transmission limit 255 character.");
								}
							} else {
								int R = 0;
								char r[255];
								sprintf(r, "%d", R);
								struct tcpquery query = pack_tcp_data(r);
								//Serialize response
								buffer = serialization_tcp(query);
								//Response
								n = send(new_sock, buffer, sizeof(*buffer), 0);
							}
						} else 
						{
							result = 0;
							printf("Packet received did not follow defined protocol. Rejected.");
						}

					}
				}
				close(socket_fd);
			}
		}
		else
		{
			result = 0;
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