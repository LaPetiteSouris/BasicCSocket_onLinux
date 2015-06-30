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
	char msg[255];
	int hmac_len;
	char  hmac[255];

};
struct udpquery pack_udp_data(char key[255]);
/*** Function to serialize data before sending
Remember to free allocated pointer after usage
***/
struct udpquery * serialization_udp(struct udpquery data);
/*** Function to de-serialize received data
Allocated memory freed automatically
***/
struct udpquery deserialization_udp(struct udpquery * ptr);
int verify_udp_packet(struct udpquery * ptr);
char * hmac_calculation(char key[255], char msg[255]);
int verify_hmac(struct udpquery data, char key[255]);
#endif