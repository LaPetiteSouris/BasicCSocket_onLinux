#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "tcpclient.h"
#include <stdio.h>
#include "../serialization/tcp_query_packet.h"
#include "tcpclient.h"
#include "udpclient.h"
//H1 verificatino value
char H1[255];
char * prompt_and_read(char * prompt) {
	char * response;
	char * bufsize;
	printf("%s", prompt);
	asprintf(&bufsize, "%%%us", BUFSIZ);

	if ((response = malloc(BUFSIZ + 1)) == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	scanf(bufsize, response);
	free(bufsize);
	return response;
}

int  startTCPClient()
{	int result = 0;
	//Open a client socket
	int socket_fd;
	socklen_t client_addr_len;
	int port = 8080;
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
			exit(1);
		}
		else
		{
			for (;;)
			{
				char * userinput = prompt_and_read("Enter your username: ");
				char str[255];
				strncpy(str, userinput, 255);
				struct tcpquery query = pack_tcp_data(str);
				struct tcpquery * buf = serialization_tcp(query);
				//Connection established. Start sending data to TCP server
				printf("Sending data\n");
				send(socket_fd, buf, sizeof(*buf), 0);
				//Receving response from server
				struct tcpquery * buff_recv = (struct tcpquery *) malloc(sizeof(struct tcpquery));
				int n = recv(socket_fd, buff_recv, sizeof(struct tcpquery), 0);
				if (n < 0)
				{
					printf("Connection to server failed");
					exit(1);
				} else
				{

					struct tcpquery incoming = deserialization_tcp(buff_recv);
					int r = verify_tcp_packet(&incoming);
					if (r == 1)
					{	//Check servr response if username exists
						int userexist = strcmp(incoming.command, "0");
						if (userexist == 0) {
							printf("Username does not exist.\n");
							break;
						}
						else
						{
							//This is a random number received from TCP server_address
							//We concanate it with user password P2 to generate H1 SH256 hash value
							char R[255];
							strncpy(R, incoming.command, sizeof(R));
							printf("Received response from server. \n");
							//To-DO: Prompt for user name Password.
							char * pass = prompt_and_read("Please enter your password: ");
							if (authenciation(socket_fd, pass, R) == 1)
							{
								//Receive authentication result from server
								if (recv(socket_fd, buff_recv, sizeof(struct tcpquery), 0) > 0)
								{
									if (check_auth_result(buff_recv))
									{
										printf("Authentication completed successfully ! Welcome !...\n");
										result = 1;
										startUDPClient(H1);
										break;
									} else
									{
										printf("Authentication failed ! Connection terminated for security reason.\n");
										break;
									}

								} else
								{
									printf("Connection to server failed\n");
									exit(1);
								}
							}
						}
					}
				}
			}
			close(socket_fd);
		}
	}
	else
	{
		printf("Connection to server failed");
	}

	return result;
}

int authenciation(int socket, char * pass, char R[])
{
	int result = 0;
	char p2[255];
	strncpy(p2, pass, 255);
	if (sizecheck(p2, R) == 1)
	{
		strcpy(H1, p2);
		strcat(H1, R);
		struct tcpquery hash = signed_withSHA(H1);
		strncpy(H1, hash.command, sizeof(H1));
		struct tcpquery * buf = serialization_tcp(hash);
		send(socket, buf, sizeof(*buf), 0);
		result = 1;
	}
	else
	{
		printf("Your input exceeds transmission limit.\n");
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

int check_auth_result(struct tcpquery * buff)
{	int r = 0;
	struct tcpquery incoming = deserialization_tcp(buff);
	if (verify_tcp_packet(&incoming) == 1)
	{
		int cmp = strcmp(incoming.command, "Auth_Suc");
		if (cmp == 0)
		{	r = 1;
		}
	}
	else
	{
		printf("Packet received did not follow defined protocol. Rejected. \n");

	}
	return r;
}

char * getH1Key()
{
	char * res = malloc(255);
	int i;
	for (i = 0; i < 255; ++i)
	{
		res[i] = H1[i];
	}
	return res;
}

main()
{
	startTCPClient();
}