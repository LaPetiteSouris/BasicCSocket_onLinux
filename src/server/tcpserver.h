#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "../serialization/tcp_query_packet.h"
int start_TCP_socket();
int getpassword(char username[255]);
void rejectconnection(int socket_fd);
int randomtokenhandling(int socket_fd);
int H_value_compare(struct tcpquery * buffer);
int generateH2value(int socket_fd, int password, int r);
int verifyH1(int socket_fd, char H1[]);
int sizecheck(char array1[], char array2[]);

#endif