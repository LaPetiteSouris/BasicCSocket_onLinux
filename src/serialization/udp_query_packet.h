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
struct udpquery pack_udp_data();
/*** Function to serialize data before sending
Remember to free allocated pointer after usage
***/
struct udpquery * serialization_tcp(struct udpquery data);
/*** Function to de-serialize received data
Allocated memory freed automatically
***/
struct udpquery deserialization_tcp(struct udpquery * ptr);
int verify_udp_packet(struct udpquery * ptr);
#endif