#include "sfpr_thread.h"


sfpr_thread_id sfpr_thread_create(sfpr_thread func,void *param)
{
    sfpr_thread_id threadHandle;   //�����߳�
    pthread_attr_t attr;      //�����߳�����
    pthread_attr_init(&attr); //��ʼ���߳�����
    pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM); //���ð����ԣ�Ϊ��
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//���÷�������Ϊ����
    if(pthread_create(&threadHandle,&attr,func,param)==0)
    {
       return threadHandle;
    }
    return (sfpr_thread_id) -1;
}

sfpr_thread_id sfpr_thread_create_ex(sfpr_thread func,void *param,char *name,int stack_size,int priority)
{
    sfpr_thread_id threadHandle;   //�����߳�
    pthread_attr_t attr;      //�����߳�����
    pthread_attr_init(&attr); //��ʼ���߳�����
    pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM); //���ð����ԣ�Ϊ��
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//���÷�������Ϊ����
    if(pthread_create(&threadHandle,&attr,func,param)==0)
    {
       return threadHandle;
    }
    return (sfpr_thread_id) -1;
}


sfpr_thread_id sfpr_thread_create_with_attr(sfpr_thread func,void *ptr,sfpr_thread_attr_t *attr_info)
{
	int taskID;
	sfpr_thread_id threadHandle;   //�����߳�
	pthread_attr_t attr;      //�����߳�����

	if (NULL !=attr_info)
	{
		pthread_attr_init(&attr); //��ʼ���߳�����
		pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM); //���ð����ԣ�Ϊ��
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//���÷�������Ϊ����
		
		pthread_attr_setstacksize(&attr ,(size_t)attr_info->stack_size);
		
		if(pthread_create(&threadHandle,&attr,func,ptr)==0)
		{
			return threadHandle;
		}
	}
	else
	{
		pthread_attr_init(&attr); //��ʼ���߳�����
		pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM); //���ð����ԣ�Ϊ��
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//���÷�������Ϊ����
		if(pthread_create(&threadHandle,&attr,func,ptr)==0)
		{
			return threadHandle;
		}
	}

	return (sfpr_thread_id) -1;

}


/*��ȡthread ID*/
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


