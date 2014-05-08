#include "sfpr_thread.h"


sfpr_thread_id sfpr_thread_create(sfpr_thread func,void *param)
{
    sfpr_thread_id threadHandle;   //声明线程
    pthread_attr_t attr;      //声明线程属性
    pthread_attr_init(&attr); //初始化线程属性
    pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM); //设置绑定属性，为绑定
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//设置分离属性为分离
    if(pthread_create(&threadHandle,&attr,func,param)==0)
    {
       return threadHandle;
    }
    return (sfpr_thread_id) -1;
}

sfpr_thread_id sfpr_thread_create_ex(sfpr_thread func,void *param,char *name,int stack_size,int priority)
{
    sfpr_thread_id threadHandle;   //声明线程
    pthread_attr_t attr;      //声明线程属性
    pthread_attr_init(&attr); //初始化线程属性
    pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM); //设置绑定属性，为绑定
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//设置分离属性为分离
    if(pthread_create(&threadHandle,&attr,func,param)==0)
    {
       return threadHandle;
    }
    return (sfpr_thread_id) -1;
}


sfpr_thread_id sfpr_thread_create_with_attr(sfpr_thread func,void *ptr,sfpr_thread_attr_t *attr_info)
{
	int taskID;
	sfpr_thread_id threadHandle;   //声明线程
	pthread_attr_t attr;      //声明线程属性

	if (NULL !=attr_info)
	{
		pthread_attr_init(&attr); //初始化线程属性
		pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM); //设置绑定属性，为绑定
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//设置分离属性为分离
		
		pthread_attr_setstacksize(&attr ,(size_t)attr_info->stack_size);
		
		if(pthread_create(&threadHandle,&attr,func,ptr)==0)
		{
			return threadHandle;
		}
	}
	else
	{
		pthread_attr_init(&attr); //初始化线程属性
		pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM); //设置绑定属性，为绑定
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//设置分离属性为分离
		if(pthread_create(&threadHandle,&attr,func,ptr)==0)
		{
			return threadHandle;
		}
	}

	return (sfpr_thread_id) -1;

}


/*获取thread ID*/
sfpr_thread_id sfpr_thread_get_id(void)
{
    sfpr_thread_id threadID ;

    threadID = pthread_self();

    return threadID;
}

int  sfpr_thread_destory(sfpr_thread_id thread)
{
	int ret ;
	
	ret = pthread_cancel(thread); 
	return ret;
}


