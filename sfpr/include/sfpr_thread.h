
#ifndef SFPR_THREAD_H
#define SFPR_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\file sfpr_thread.h
 *\brief SFPR库的线程池模块
 *\author bijing
 *\version 0.1.0
 *\date 2012-02-14
 */


#include "sfpr_thread_cond.h"


#if defined WIN32
#include <windows.h>
#include <process.h>
#define RET_THREAD void
#define NULL_THREAD
typedef void(__cdecl *sfpr_thread)(void *);

#elif defined VXWORKS
#define RET_THREAD void*
#define NULL_THREAD NULL
typedef void*(*sfpr_thread)(void *);

#else
#include <unistd.h>
#include <pthread.h>
#define RET_THREAD void*
#define NULL_THREAD NULL
typedef void*(*sfpr_thread)(void *);

#endif

/**************************************************************************************************/
/**
 *\defgroup thread 线程
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup thread_struct 基本数据结构
 *\ingroup thread
 **************************************************************************************************/

typedef unsigned long sfpr_thread_id;

/**
 *\ingroup thread_struct
 *\brief 线程属性
 */
typedef struct sfpr_thread_attr_s{
	char thread_name[64];
	int priority;
	int  options;
	unsigned long stack_size;
}sfpr_thread_attr_t;


/**
 *\ingroup thread_struct
 *\brief 线程管理中线程节点的描述
 */
typedef struct sfpr_thread_s{
	sfpr_thread_id thread_id;   /**< 线程id */
	int is_busy;    /**< 线程是否忙 */
	sfpr_cond_t  cond;    /**<  线程信号量*/
	sfpr_mutex_t mutex;   /**<  线程锁*/
	unsigned long flags;   /**<  线程标识*/
	
	void *user_data;  /**< 线程所处理的用户数据 */
	int data_size;	  /**<  用户数据的大小*/
	
}sfpr_thread_t;

/**************************************************************************************************/
/**
 *\defgroup thread_base 基本函数
 *\ingroup thread
 **************************************************************************************************/

/**
 *\ingroup thread_base
 *\brief 创建线程
 *\param[in] func 线程处理函数
 *\param[in] param  线程参数
 *\return 创建的线程id
 */
sfpr_thread_id sfpr_thread_create(sfpr_thread func,void *param);

/**
 *\ingroup thread_base
 *\brief 创建线程
 *\param[in] func 线程处理函数
 *\param[in] param  线程参数
 *\param[in] name  线程名称
 *\param[in] stack_size  线程栈大小
 *\param[in] priority  线程优先级
 *\return 创建的线程id
 */
sfpr_thread_id sfpr_thread_create_ex(sfpr_thread func,void *param,char *name,int stack_size,int priority);


sfpr_thread_id sfpr_thread_create_with_attr(sfpr_thread func,void *ptr,sfpr_thread_attr_t *attr_info);

sfpr_thread_id sfpr_thread_get_id(void);

int sfpr_thread_destory(sfpr_thread_id thread);


#ifdef __cplusplus
}
#endif

#endif
