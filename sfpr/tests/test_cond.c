#include <stdio.h>
#include "sfpr_thread.h"
#include "sfpr_thread_mutex.h"
#include "sfpr_thread_cond.h"

sfpr_mutex_t g_mutex;
sfpr_cond_t g_cond;

RET_THREAD thread_func(void *arg)
{
	int *tmp = (int*)arg;
	while(1)
	{
	    sfpr_mutex_lock(&g_mutex);
	    sfpr_cond_wait(&g_cond,&g_mutex);
	    sfpr_mutex_unlock(&g_mutex);    

        printf("this is thread:%d\n", *tmp);
	}
}

int main()
{
	int i;
	char buffer[16];
	sfpr_mutex_create(&g_mutex, 0);
	sfpr_cond_create(&g_cond);
	
	for(i=0; i<3; i++){
		int *a = (int*)malloc(sizeof(int));
		*a = i;
		sfpr_thread_id thread = sfpr_thread_create(thread_func, a);
	}

	while(1){
		printf("Please input:");
		fgets(buffer,sizeof(buffer),stdin);
		sfpr_cond_signal_all(&g_cond);
	}
}

