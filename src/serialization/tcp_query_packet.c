#include "tcp_query_packet.h"
#include <string.h>
#include <stdlib.h>
struct tcpquery pack_tcp_data(char msg[255])
{
	struct tcpquery data = {"DISTRIB2015", 255, 'D', "any"};
	strncpy(data.command, msg, sizeof(data.command));
	return data;
}

struct tcpquery * serialization_tcp(struct tcpquery data)
{
	struct tcpquery * buf = (struct tcpquery *)malloc(sizeof(struct tcpquery));
	memcpy(buf, &data, sizeof(struct tcpquery));
	return buf;
}
struct tcpquery deserialization_tcp(struct tcpquery * ptr)
{
	struct tcpquery * buff = (struct tcpquery *)malloc(sizeof(struct tcpquery));
	struct tcpquery incoming;
	memcpy(&incoming, buff, sizeof(struct tcpquery));
	free(buff);
	return incoming;
}
int verify_tcp_package(struct tcpquery * ptr)
{
	int result = 1;
	if (sizeof(ptr->len) > 4 || sizeof(ptr->mode) != 1
	        || sizeof(ptr->command) != ptr->len
	        || sizeof(ptr -> str) != 11 )
	{
		result = 0;
	}
	return result;
}