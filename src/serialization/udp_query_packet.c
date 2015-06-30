#include "udp_query_packet.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
int32_t random_number_1_6() {
	return ((rand() % 6) + 1);
}

struct udpquery pack_udp_data(char key[255])
{
	//Declare query packet frame here.
	char s[255] = "Ping";
	srand(time(NULL));
	int32_t sid = random_number_1_6();
	int32_t tid = random_number_1_6();
	//Generate HMAC signed
	char * hmac_ptr = hmac_calculation(key, s);
	struct udpquery data = {'D', 'S', sizeof(struct udpquery), sid, tid, strlen(s), "any", strlen(hmac_ptr), "any"};
	strncpy(data.msg, s, sizeof(data.msg));
	strncpy(data.hmac, hmac_ptr, sizeof(data.hmac));
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
	int hmac_len = ptr->hmac_len;
	int32_t sid = ptr->sid;
	int32_t tid = ptr->tid;
	if (sizeof(version) != 1 || sizeof(direction) != 1
	        || sizeof(msg_len) > 4 || sizeof(len) > 4
	        || strlen(ptr -> msg) != msg_len || sizeof(tid) > 4 || sizeof(sid) > 4 || strlen(ptr->hmac) != hmac_len)
	{
		result = 0;
	}
	return result;
}

char * hmac_calculation(char key[255], char msg[255])
{
	char * hmac_res = HMAC(EVP_sha256(), key, 255, msg, 255, NULL, NULL);
	return hmac_res;
}

//This function return 0 if verification success. 
int verify_hmac(struct udpquery data, char key[255])
{
	char hmac_recv[128];
	strcpy(hmac_recv, data.hmac);
	char * hmac_calculate =hmac_calculation(key, data.msg);
	char hmac_arr[128];
	strncpy(hmac_arr, hmac_calculate, sizeof(hmac_arr));
	return strcmp(hmac_recv, hmac_arr);
}