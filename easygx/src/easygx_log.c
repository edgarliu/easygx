
#include <stdlib.h>
#include <string.h>

#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

sfpr_log_t *easygx_log = NULL;

int easygx_log_set(sfpr_log_t *log)
{
	easygx_log = log;
}

int easygx_log_init(char *path,int level)
{
	char buffer[512];
	
	int ret = 0;
	if(NULL == path){
		return -1;
	}
	easygx_log = sfpr_log_new();
	if(NULL == easygx_log){
		return -1;
	}
	if(0 == strcmp(path,"stdout") || 0 == strcmp(path,"stderr")){
		ret = sfpr_log_init(easygx_log,path,level,0);	
	}else{
		sprintf(buffer,"%s//easygx-.log",path);
		ret = sfpr_log_init(easygx_log,buffer,level,SFPR_LOG_DAILY);	
	}
	if(ret < 0)
		return -1;
	sfpr_log_open(easygx_log); 
	return 0;
}

#ifdef __cplusplus
}
#endif
