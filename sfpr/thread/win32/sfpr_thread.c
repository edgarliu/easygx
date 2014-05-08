#include "sfpr_thread.h"


sfpr_thread_id sfpr_thread_create(sfpr_thread func,void *param)
{
    sfpr_thread_id threadHandle;   //声明线程
    threadHandle = _beginthread(func ,0, param);
    return (sfpr_thread_id)threadHandle;
}


sfpr_thread_id sfpr_thread_create_ex(sfpr_thread func,void *param,char *name,int stack_size,int priority)
{
    sfpr_thread_id threadHandle;   //声明线程
//    unsigned temp;
//    threadHandle = _beginthreadex(NULL,stack_size,func,param,0,&temp);
    threadHandle = _beginthread(func,stack_size,param);
    return (sfpr_thread_id)threadHandle;
}

sfpr_thread_id sfpr_thread_create_with_attr(sfpr_thread func,void *param,sfpr_thread_attr_t *attr_info)
{
    sfpr_thread_id threadHandle;   //声明线程
//    unsigned temp;
//    threadHandle = _beginthreadex(NULL,attr_info->stack_size,func,param,0,&temp);
    threadHandle = _beginthread(func,attr_info->stack_size,param);
    return (sfpr_thread_id)threadHandle;
}


/*获取thread ID*/
sfpr_thread_id sfpr_thread_get_id(void)
{
    sfpr_thread_id threadID ;

    threadID = GetCurrentThreadId();

    return threadID;
}

int  sfpr_thread_destory(sfpr_thread_id thread)
{
    _endthreadex(0); 
    return 0;
}


