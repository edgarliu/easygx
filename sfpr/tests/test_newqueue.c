#include <stdio.h>
#include <string.h>

#include "sfpr_queue.h"
#include "sfpr_thread.h"
#include "sfpr_time.h"

#ifdef __cplusplus
extern "C" {
#endif

static int current_time_get(char *currtime)
{
	sfpr_time_t now;
	sfpr_tm_t tm;
	
	sfpr_time_now(&now);
	sfpr_time_localtime(&now,&tm);
		
	sprintf(currtime,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
		tm.year,
		tm.month,
		tm.mday,
		tm.hour,
		tm.min,
		tm.sec,
		tm.msec);
}


RET_THREAD sfpr_queue_Loopget(void *param)
{
	char buffer[256];
	sfpr_queue_t *queue = (sfpr_queue_t*)param;
	int count =0;
	void *ret = NULL;

	while(1)
	{
		ret = sfpr_queue_get(queue);
		count++;
		memset(buffer,0,sizeof(buffer));
		current_time_get(buffer);
		printf("sfpr_queue_get() %s index:<<%d>>----- %s\r\n",buffer,count,ret);
	}
	return NULL_THREAD;
}

#if defined VXWORKS
int queue_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	char buffer[512],*data = NULL;
	sfpr_queue_t  *test_queue;
	int level = 0;
	
	test_queue = sfpr_queue_create();
	sfpr_queue_init(test_queue);

	sfpr_thread_create_ex(sfpr_queue_Loopget,(void*)test_queue,"test",10000,170);
	
	while(1){
		level = 0;
		printf("Please input data:");
		fgets(buffer,sizeof(buffer),stdin);
		if(!strncmp(buffer,"quit",4)){
			break;
		}
		if(buffer[0] == '\r' || buffer[0] == '\n'){
			continue;
		}
		buffer[strlen(buffer)-1] = 0;
		level = buffer[0] - '0';
		data = (char*)malloc(strlen(buffer)+16);
		memset(data,0,strlen(buffer)+16);
		strcpy(data,buffer);

		memset(buffer,0,sizeof(buffer));
		current_time_get(buffer);		
		printf("sfpr_queue_put() %s\n",buffer);
		sfpr_queue_put_with_level(test_queue,data,level);
//		sfpr_queue_enum(test_queue);
		
		memset(buffer,0,sizeof(buffer));
	}	

	sfpr_queue_destroy(test_queue);
	return 0;
}



#ifdef __cplusplus
}
#endif

