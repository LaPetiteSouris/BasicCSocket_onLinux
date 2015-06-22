#ifndef UDPSERVER_H
#define UDPSERVER_H
#include "../serialization/udp_query_packet.h"
int start_UDP();
int verify_packet(struct udpquery * ptr);

#endif