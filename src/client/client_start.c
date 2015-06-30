#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tcpclient.h"
#include "udpclient.h"
main()
{
	int auth = startTCPClient();
	if (auth == 1)
	{
		char * hash_ptr = getH1key();
		char hash[255];
		strncpy(hash, hash_ptr, sizeof(hash));
		free(hash_ptr);
		startUDPClient(hash);
	}

}