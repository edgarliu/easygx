
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "sfpr_thread_pool.h"
#include "sfpr_time.h"

static void quitSignalHandler(int sig)
{
	fprintf(stderr,"quitSignalHandler()\n");
//	pthread_exit(0);
}

void task_process_test(sfpr_threadpool_task_t *this)
{
	char *temp;
	fprintf(stderr,"task_process_test() argc=%d argv[0]=%s\n",this->argc,this->argv[0]);
	free(this->argv[0]);
	memset(temp,0,1024);
//	sleep(70);
}

#if defined VXWORKS
int thread_pool_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	char buffer[512],*data = NULL;
	int id;
	sfpr_time_t time;
	sfpr_tm_t tm;
	
	sfpr_threadpool_task_t task;
	sfpr_threadpool_t *thread_pool = NULL;
	
	thread_pool = sfpr_threadpool_create("testthpool",6,12,10000,170);
	if(!thread_pool){
		fprintf(stderr,"sfpr_threadpool_create() failed\n");
		return 1;
	}
	printf("sfpr_threadpool_create() success\n");

	signal(SIGSEGV,quitSignalHandler);

	sfpr_threadpool_start(thread_pool);

	task.task_process = task_process_test;
	task.argc = 1;
	task.flags = 0;

	while(1){
		sfpr_time_msleep(1000);
		printf("Please input id:");
		fgets(buffer,sizeof(buffer),stdin);
		if(!strncmp(buffer,"quit",4)){
			break;
		}
		buffer[strlen(buffer)-1] = 0;
		id = atoi(buffer);
		if(id == 0)
		{
			continue;
		}
		data = (char*)malloc(256);
		memset(data,0,256);
		sfpr_time_now(&time);
		sfpr_time_localtime(&time,&tm);
		sprintf(data," Test Thread Pool Data, Id=%d  Time=%04d-%02d-%02d %02d:%02d:%02d.%03d",
						id,tm.year,tm.month,tm.mday,tm.hour,tm.min,tm.sec,tm.msec);
		task.argv[0] = data;

		if(sfpr_threadpool_submit(thread_pool,&task)<0)
		{
			fprintf(stderr,"main() | Put Failed\n");
		}
		memset(buffer,0,sizeof(buffer));
	}


	sfpr_threadpool_close(&thread_pool);
	printf("sfpr_threadpool_close() success\n");	
//	getchar();
	return 0;
}
