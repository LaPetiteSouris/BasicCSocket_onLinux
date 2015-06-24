#include <stdio.h>
#include "udpserver.h"
#include <string.h>
#include <stdlib.h>

main()
{
	//int result=start_UDP();
	char  input[4] = "Ping";
	SHA256_CTX cxt;
	char * hash = generateSHA(cxt, input, strlen(input));
	int i;
	for (i = 0; i < 32; i++) {
		printf("%02x ", hash[i]);
	}

}