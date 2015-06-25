#ifndef TCPQuerry_H
#define TCPQuerry_H

struct tcpquery
{
	char str[11];
	int len;
	char mode;
	char command[256];
};
struct tcpquery pack_tcp_data(char msg[255]);
/*** Function to serialize data before sending
Remember to free allocated pointer after usage
***/
struct tcpquery * serialization_tcp(struct tcpquery data);
/*** Function to de-serialize received data
Allocated memory freed automatically
***/
struct tcpquery deserialization_tcp(struct tcpquery * ptr);
int verify_tcp_packet(struct tcpquery * ptr);
#endif