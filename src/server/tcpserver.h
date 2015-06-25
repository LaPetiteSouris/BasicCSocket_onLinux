#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <openssl/sha.h>
int start_TCP_socket();
int getpassword(char username[255]);
char * generateSHA(SHA256_CTX context, char * input, size_t len);
#endif