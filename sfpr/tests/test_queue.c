#include <stdio.h>
#include <string.h>

#include "sfpr_queue.h"
#include "sfpr_thread.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LENGTH 3

static char queuetmp[5][9]={"1111111", "2222222", "3333333", "4444444", "55555555"};	


RET_THREAD sfpr_queue_Loopget(void *param)
{
	sfpr_queue_t *queue = (sfpr_queue_t*)param;
	int count =0;
	void *ret = NULL;

	while(1)
	{
		ret = sfpr_queue_get(queue);
		
		count++;
		printf("\r\nsfpr_queue_Loopget() index:<<%d>>----- %s\r\n",count,ret);

	}
	return;
}

RET_THREAD sfpr_queue_Loopput(void *param)
{
	sfpr_queue_t *queue = (sfpr_queue_t*)param;
	char *datatmp;
	int i =0;
		
	for(i = 0;i <LENGTH;i++)
    	{    		
    		datatmp = queuetmp[i];
		sfpr_queue_put(queue,datatmp);
		printf("\r\nsfpr_queue_Loopput index:<<%d>>----- %s\r\n", i+1,  datatmp);
    	}
	sfpr_time_msleep(5000);
	for(i = 0;i <LENGTH;i++)
    	{    		
    		datatmp = queuetmp[i];
		sfpr_queue_put(queue,datatmp);
		printf("\r\nsfpr_queue_Loopput index:<<%d>>----- %s\r\n", i+1,  datatmp);
    	}

	return ;
	
}

#if defined VXWORKS
int queue_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	int  threadQUE;
	sfpr_queue_t  *test_queue;
	int i =0,testnum =0;
	
	fprintf(stderr,"main() | debug | BEGIN\n");
	test_queue = sfpr_queue_create();
	sfpr_queue_init(test_queue);
	
	printf("input a case num(1:put  2-4:get/put  5:Clear queue   6:destroy queue  ):  ");
	scanf("%d", &testnum);

	switch(testnum)
	{
	case 1:
		sfpr_queue_Loopput(test_queue);
		break;
	case 2:
	        
		sfpr_queue_Loopput(test_queue);
		sfpr_queue_Loopget(test_queue);
		
		break;
	case 3:
		
		sfpr_thread_create_ex(sfpr_queue_Loopput,(void*)test_queue,"test",10000,170);
		
		sfpr_queue_Loopget(test_queue);
		
		break;	
	case 4:
		
		sfpr_thread_create_ex(sfpr_queue_Loopget,(void*)test_queue,"test",10000,170);
		
		sfpr_thread_create_ex(sfpr_queue_Loopput,(void*)test_queue,"test",10000,170);
		
		sfpr_thread_create_ex(sfpr_queue_Loopget,(void*)test_queue,"test",10000,170);
		break;	
	case 5:
		sfpr_queue_clear(test_queue);
		break;	
	case 6:
		sfpr_queue_destroy(test_queue);
		break;	
		
	 default: 
	   fprintf(stderr,"main() | debug | default !!\n");
	}

     sfpr_time_msleep(200000);
	//sfpr_queue_destroy(test_queue);
	fprintf(stderr,"main() | debug | end !!\n");
      return 0;
}



#ifdef __cplusplus
}
#endif

