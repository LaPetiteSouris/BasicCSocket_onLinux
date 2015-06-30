#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tcpserver.h"
#include "udpserver.h"


main()
{
	int auth = start_TCP_socket();
	if (auth == 1) {
		char * hash_ptr = getHkey();
		char hash[255];
		strncpy(hash, hash_ptr, sizeof(hash));
		hash_ptr = NULL;
		free(hash_ptr);
		start_UDP(hash);
	}
}



