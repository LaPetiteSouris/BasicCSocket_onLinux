#ifndef UDP_Q_PACKET_H
#define UDP_Q_PACKET_H
struct udpquery
{
	char version;
	char direction;	
	int len;
	int32_t sid;
	int32_t tid;
	int msg_len;
	//Messag is 255 byte max
	char buff[255];
	//int hmac_len;
	//char * hmac;

}
#endif