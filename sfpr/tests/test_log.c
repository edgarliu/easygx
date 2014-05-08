
#include <stdio.h>

#include "sfpr_log.h"

static sfpr_log_t *log;

#if defined VXWORKS
int hash_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	char buffer[1024];
	
	log = sfpr_log_new();
	sfpr_log_init(log,"sfpr.log",SFPR_LOG_DEBUG,SFPR_LOG_DAILY);
	printf("sfpr_log_init() success\n");
	sfpr_log_open(log);

	while(1){
		printf("Please input key:");
		fgets(buffer,sizeof(buffer),stdin);
		if(!strncmp(buffer,"quit",4)){
			break;
		}
		buffer[strlen(buffer)-1] = 0;

		sfpr_log(log,SFPR_LOG_INFO,"One Log %s",buffer);
		sfpr_log_info(log,SFPR_LOG_INFO,buffer);
		sfpr_log_hex(log,SFPR_LOG_INFO,"Hex The Log",buffer,strlen(buffer),1);
		
		memset(buffer,0,sizeof(buffer));		
	}	
	
	sfpr_log_close(log);
	sfpr_log_quit(&log);
	printf("sfpr_log_close() success\n");	
	return 0;
}
