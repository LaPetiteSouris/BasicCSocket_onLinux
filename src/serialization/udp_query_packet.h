#ifndef UDPQuerry_H
#define UDPQuerry_H
#include <sys/types.h>
struct udpquery
{
	char version;
	char direction;	
	int len;
	int32_t sid;
	int32_t tid;
	int msg_len;
	//Messag is 255 byte max
	char msg[5];
	//int hmac_len;
	//char * hmac;

};
#endif