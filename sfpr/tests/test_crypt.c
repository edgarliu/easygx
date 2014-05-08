#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sfpr_crypt.h"
#include "sfpr_string.h"

#if defined VXWORKS
int crypt_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{ 
	char test[1024];
	char *tmp; 
	while(1)
	{
		printf("input plaint:\n");
		memset(test, 0x0, sizeof(test));
		gets(test);
	 	tmp = sfpr_crypt((const char*)test, "$3$", sfpr_encrypt);
		printf("encrypt:%s\n", tmp);
		//sfpr_str_hex_print("encrypt", tmp, 128);
	 	tmp = sfpr_crypt((const char*)tmp, "$3$", sfpr_decypt);
		printf("decrypt:%s\n", tmp);
	}
    return 0;
}

