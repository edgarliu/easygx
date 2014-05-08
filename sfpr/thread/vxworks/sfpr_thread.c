#include "sfpr_thread.h"

#define PRIORITY 100

sfpr_thread_id sfpr_thread_create(sfpr_thread func,void *param)
{
	int taskID;
	taskID = taskSpawn("tSfprTask",   /* name of new task (stored at pStackBase) */
		     PRIORITY,   /* priority of new task */
		     0,        /* task option word */
		     10240, /* size (bytes) of stack needed plus name */
		     (FUNCPTR)func,     /* entry point of new task */
		     param, 0, 0, 0, 0, 0, 0, 0, 0, 0);    /* 1st of 10 req'd task args to pass to func */
	if (SFPR_ERROR == taskID)  
	{
		printf("pr_thread_create taskSpawn  failed\n"); 
	}
	return taskID;
}

sfpr_thread_id sfpr_thread_create_ex(sfpr_thread func,void *param,char *name,int stack_size,int priority)
{
	int taskID;

	taskID = taskSpawn(name?name:"tSfprTask",   /* name of new task (stored at pStackBase) */
                     priority,   /* priority of new task */
                     0,        /* task option word */
                     stack_size, /* size (bytes) of stack needed plus name */
                     (FUNCPTR)func,     /* entry point of new task */
                     param, 0, 0, 0, 0, 0, 0, 0, 0, 0);    /* 1st of 10 req'd task args to pass to func */
	if (SFPR_ERROR == taskID)  
	{
		printf("pr_thread_create taskSpawn  failed\n"); 
	}
  return taskID;
}

sfpr_thread_id sfpr_thread_create_with_attr(sfpr_thread func,void *ptr,sfpr_thread_attr_t *attr_info)
{
	int taskID;
	if (NULL !=attr_info)
	{
		taskID = taskSpawn(attr_info->thread_name,       /* name of new task (stored at pStackBase) */
	                     attr_info->priority,   /* priority of new task */
	                     attr_info->options,        /* task option word */
	                     attr_info->stack_size,       /* size (bytes) of stack needed plus name */
	                     (FUNCPTR)func,     /* entry point of new task */
	                     ptr, 0, 0, 0, 0, 0, 0, 0, 0, 0);    /* 1st of 10 req'd task args to pass to func */
	}
	else
	{
		taskID = taskSpawn(NULL ,       /* name of new task (stored at pStackBase) */
                     PRIORITY,   /* priority of new task */
                     0,        /* task option word */
                     10000,       /* size (bytes) of stack needed plus name */
                     (FUNCPTR)func,     /* entry point of new task */
                     ptr, 0, 0, 0, 0, 0, 0, 0, 0, 0);    /* 1st of 10 req'd task args to pass to func */
	}

	if (SFPR_ERROR == taskID)  
	{
		printf("pr_thread_create taskSpawn  failed\n"); 
	}

  return taskID;
}

/*ªÒ»°task ID*/
sfpr_thread_id sfpr_thread_get_id(void)
{
	int threadID ;
	threadID = taskIdSelf();
	return threadID;
}

int sfpr_thread_destory(sfpr_thread_id thread)
{
	int ret ;
	ret = taskDelete(thread); 
	return ret ;
}
