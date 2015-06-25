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

int startTCPClient()
{	int result = 0;
	//Open a client socket
	int socket_fd;
	socklen_t client_addr_len;
	int port = 8080;
	int buff_size = 255;
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
			result = 0;
		}
		else
		{
			char * userinput = prompt_and_read("Enter your username: ");
			char str[255];
			strncpy(str, userinput, 255);
			struct tcpquery query = pack_tcp_data(str);
			struct tcpquery * buf = serialization_tcp(query);
			//Connection established. Start sending data to TCP server
			send(socket_fd, buf, buff_size, 0);
			//Receving response from server
			struct tcpquery * buff_recv = (struct tcpquery *) malloc(sizeof(struct tcpquery));
			int n = recv(socket_fd, buff_recv, sizeof(struct tcpquery), 0);
			if (n < 0)
			{
				printf("Connection to server failed");
				result = 0;
			} else
			{
				struct tcpquery incoming = deserialization_tcp(buff_recv);
				int r = verify_tcp_packet(&incoming);
				if (r == 1)
				{	//Check servr response if username exists
					int userexist = strcmp(incoming.command, "0");
					if (userexist == 0) {
						printf("Username does not exist.");

					}
					else
					{
						//This is a random number received from TCP server_address
						//We concanate it with user password P2 to generate H1 SH256 hash value
						char R[255];
						strncpy(R, incoming.command, sizeof(R));
						printf("Received response from server. ");
						//To-DO: Prompt for user name Password.
						char * pass = prompt_and_read("Please enter your password: ");
						char p2[255];
						strncpy(p2, pass, 255);
						char H2[255];
						if (sizecheck(p2, R) == 1)
						{
							strcpy(H2, p2);
							strcat(H2, R);
							query = pack_tcp_data(H2);
							buf = serialization_tcp(query);
							send(socket_fd, buf, buff_size, 0);
							free(buf);
						}
						else
						{
							printf("Your input exceeds transmission limit");
						}
						//Then concanate password P2 with random number R from server
						//SHA256 hash calculation(H1 value).incoming
						//Send H1 to server for verification
					}
				}
			}
		}
	}
	else
	{
		printf("Connection to server failed");
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
