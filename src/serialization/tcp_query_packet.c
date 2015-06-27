#include "tcp_query_packet.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/sha.h>
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
	        || sizeof(ptr->command) != (ptr->len)
	        || sizeof(ptr -> str) != 11 )
	{
		result = -1;
		printf("Protocol verification failed\n");
	}
	return result;

}

struct tcpquery signed_withSHA(char input[])
{
	SHA256_CTX c;
	char * hash = (char *) malloc(255);
	char hash_arr[255];
	if (!SHA256_Init(&c))
	{
		hash = NULL;
	}
	if (!SHA256_Update(&c, input, strlen(input)))
	{
		hash = NULL;
	}
	if (!SHA256_Final(hash, &c)) {
		hash = NULL;
	}
	strncpy(hash_arr, hash, sizeof(hash_arr));
	struct tcpquery query = pack_tcp_data(hash_arr);
	return query;
}