#ifndef UDPSERVER_H
#define UDPSERVER_H
#include "../serialization/udp_query_packet.h"
#include <openssl/sha.h>
int start_UDP();
int verify_packet(struct udpquery * ptr);
char * generateSHA(SHA256_CTX context, char * input, size_t len);
#endif