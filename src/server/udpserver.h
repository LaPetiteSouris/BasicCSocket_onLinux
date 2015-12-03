#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <sys/socket.h>
#include <netinet/in.h>
int start_UDP(char key[255], int port);
void response_to_client(int result, char key[], int socket_fd, struct sockaddr_in client_address);
#endif