#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <openssl/sha.h>

int start_TCP_socket();
int getpassword(char username[255]);
int generateH2(int socket_fd, char R[]);
int verifyH1(int socket_fd, char H1[]);
int sizecheck(char array1[], char array2[]);
char * generateSHA(SHA256_CTX context, char * input, size_t len);
#endif