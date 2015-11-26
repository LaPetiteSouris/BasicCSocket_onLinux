#include "tcp_query_packet.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/sha.h>
struct tcpquery pack_tcp_data(char msg[])
{
	struct tcpquery data = {"LINUX2015UPMC", sizeof(struct tcpquery), 'D', ""};
	strcpy(data.command, msg);
	return data;
}
/***
Serialize data before sending to socket. Allocated buffer must be released
manually after usage
***/
struct tcpquery * serialization_tcp(struct tcpquery data)
{
	struct tcpquery * buf = malloc(sizeof(struct tcpquery));
	memcpy(buf, &data, sizeof(struct tcpquery));
	return buf;
}
/***
De-serialize data before sending to socket.
***/
struct tcpquery deserialization_tcp(struct tcpquery * ptr)
{

	struct tcpquery incoming;
	if (sizeof(incoming) == sizeof(*ptr)) {
		memcpy(&incoming, ptr, sizeof(struct tcpquery));
	} else
	{
		printf("Undefined Behavior");
	}
	//free(ptr);
	return incoming;
}
int verify_tcp_packet(struct tcpquery * ptr)
{
	int result = 1;
	if (sizeof(ptr->len) > 4 || sizeof(ptr->mode) != 1
	        || sizeof(*ptr) != (ptr->len)
	        || sizeof(ptr -> str) != 11 )
	{
		result = -1;
		printf("Protocol verification failed\n");
	}
	return result;

}
//SHA256-signed value must be converted to HEX before trasmission. Never
//transmit raw binary

struct tcpquery signed_withSHA(char input[])
{
	char hash[32];
	SHA256(input, strlen(input), hash);
	int i;
	char out[97];
	for (i = 0; i < 32; i++) {
		snprintf(out + i * 3, 4, "%02x ", hash[i]);
	}
	struct tcpquery query = pack_tcp_data(out);
	return query;
}