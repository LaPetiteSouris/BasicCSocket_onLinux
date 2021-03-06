#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include "../serialization/tcp_query_packet.h"

int startTCPClient(char * hostname, int port);
int authenciation(int socket, char * pass, char r[]);
char * prompt_and_read(char * prompt);
int sizecheck(char array1[], char array2[]);
int check_auth_result(struct tcpquery * ptr);
#endif