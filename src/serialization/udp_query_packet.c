#include "udp_query_packet.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

int32_t random1_6() {
	return ((rand() % 6) + 1);
}

struct udpquery pack_udp_data()
{
	//Declare query packet frame here.
	char s[5] = "Ping";
	srand(time(NULL));
	int32_t sid = random1_6();
	int32_t tid = random1_6();
	struct udpquery data = {'D', 'S', 4, sid, tid, 5, "any"};
	strncpy(data.msg, s, sizeof(data.msg));
	return data;

}

struct udpquery * serialization_udp(struct udpquery data)
{
	struct udpquery * buf = (struct udpquery *)malloc(sizeof(struct udpquery));
	memcpy(buf, &data, sizeof(struct udpquery));
	return buf;
}
struct udpquery deserialization_udp(struct udpquery * ptr)
{
	struct udpquery incoming;
	if (sizeof(incoming) == sizeof(*ptr)) {
		memcpy(&incoming, ptr, sizeof(struct udpquery));
	} else
	{
		printf("Undefined Behavior");
	}
	free(ptr);
	return incoming;
}
int verify_udp_packet(struct udpquery * ptr)
{
	int result = 1;
	char version = ptr->version;
	char direction = ptr ->direction;
	int msg_len = ptr->msg_len;
	int len = ptr -> len;
	int32_t sid = ptr->sid;
	int32_t tid = ptr->tid;
	if (sizeof(version) != 1 || sizeof(direction) != 1
	        || sizeof(msg_len) > 4 || sizeof(len) > 4
	        || sizeof(ptr -> msg) != msg_len || sizeof(tid) > 4 || sizeof(sid) > 4)
	{
		result = 0;
	}
	return result;
}