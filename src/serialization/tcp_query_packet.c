#include "tcp_query_packet.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct tcpquery pack_tcp_data(char msg[255])
{
	struct tcpquery data = {"DISTRIB2015", 255, 'D', "any"};
	strncpy(data.command, msg, sizeof(data.command));
	return data;
}

struct tcpquery * serialization_tcp(struct tcpquery data)
{
	printf("Serialization data...\n");
	struct tcpquery * buf = (struct tcpquery *)malloc(sizeof(struct tcpquery));
	return memcpy(buf, &data, sizeof(struct tcpquery));
}
struct tcpquery deserialization_tcp(struct tcpquery * ptr)
{
	printf("Deserialization data...\n");
	struct tcpquery * buff = (struct tcpquery *)malloc(sizeof(struct tcpquery));
	struct tcpquery incoming;
	memcpy(&incoming, buff, sizeof(struct tcpquery));
	free(buff);
	return incoming;
}
int verify_tcp_packet(struct tcpquery * ptr)
{
	printf("%s\n", ptr->str);
	int result = 1;
	if (sizeof(ptr->len) > 4 || sizeof(ptr->mode) != 1
	        || sizeof(ptr->command) != (ptr->len)
	        || sizeof(ptr -> str) != 11 )
	{
		result = 0;
		printf("Failed\n");
	}
	return result;

}