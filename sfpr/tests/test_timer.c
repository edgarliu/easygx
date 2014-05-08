#include <stdio.h>
#include"sfpr_timer.h"

void  time_printf()
{
	 sfpr_timer_t t;
	 char p[32];
	sfpr_time_t timenow;
	sfpr_tm_t mytm;
	
	sfpr_time_now(&timenow);
	sfpr_time_localtime(&timenow,&mytm);
	printf("Now Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n",
				mytm.year,mytm.month,
				mytm.mday,mytm.hour,
				mytm.min,mytm.sec,
				mytm.msec);

	//printf("thread %lu, \n",  sfpr_thread_get_id());
 
	 return;
}

void  handle(void *param)
{
	 sfpr_timer_t t;
	 char p[32];
	sfpr_time_t timenow;
	sfpr_tm_t mytm;
	
	sfpr_time_now(&timenow);
	sfpr_time_localtime(&timenow,&mytm);
	printf("Now Time: %04d-%02d-%02d %02d:%02d:%02d.%03d",
				mytm.year,mytm.month,
				mytm.mday,mytm.hour,
				mytm.min,mytm.sec,
				mytm.msec);

	printf("param:%s\n",  (char*)param);
 
	// time(&t);
	 //strftime(p, sizeof(p), "%T", localtime(&t));
	// printf("%s thread %lu, val = %d, signal captured.\n", p, pthread_self(), v.sival_int);
	 return;
}
static int start_flag=0;
#if defined VXWORKS
int timer_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	int testnum;
	int time;
	sfpr_timer_t *timer;
	int count = 0;

	char *buff;
	
	while(1)
	{
		printf("\ninput a case num(1:create  2:destory	3:start   4:cancel	):	");
		scanf("%d", &testnum);
		switch(testnum)
		{
		case 1:
			if(start_flag)
				break;
			else
				start_flag = 1;
			
			timer = sfpr_timer_create();
			printf("sfpr_timer_create--Timerid=%p--",timer);
			break;	
		case 2:
		printf("\ninput timer id:  ");
			sfpr_timer_destory(&timer);
			break;	
		case 3:
			printf("\ninput timer time value:  ");
			scanf("%d", &time);	
			time_printf();
			buff = malloc(128);
			memset(buff, 0x0, sizeof(128));
			sprintf(buff, "this is testing:%d\n", count);
			sfpr_timer_init(timer, handle, buff);
			sfpr_timer_start(timer,time);
			count++;
			break;	

		case 4:
			sfpr_timer_cancel(timer);
			break;	
				
			 default: 
				fprintf(stderr,"main() | debug | default !!\n");

		}

	}

}



