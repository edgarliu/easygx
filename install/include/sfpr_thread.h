
#ifndef SFPR_THREAD_H
#define SFPR_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\file sfpr_thread.h
 *\brief SFPR����̳߳�ģ��
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
 *\defgroup thread �߳�
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup thread_struct �������ݽṹ
 *\ingroup thread
 **************************************************************************************************/

typedef unsigned long sfpr_thread_id;

/**
 *\ingroup thread_struct
 *\brief �߳�����
 */
typedef struct sfpr_thread_attr_s{
	char thread_name[64];
	int priority;
	int  options;
	unsigned long stack_size;
}sfpr_thread_attr_t;


/**
 *\ingroup thread_struct
 *\brief �̹߳������߳̽ڵ������
 */
typedef struct sfpr_thread_s{
	sfpr_thread_id thread_id;   /**< �߳�id */
	int is_busy;    /**< �߳��Ƿ�æ */
	sfpr_cond_t  cond;    /**<  �߳��ź���*/
	sfpr_mutex_t mutex;   /**<  �߳���*/
	unsigned long flags;   /**<  �̱߳�ʶ*/
	
	void *user_data;  /**< �߳���������û����� */
	int data_size;	  /**<  �û����ݵĴ�С*/
	
}sfpr_thread_t;

/**************************************************************************************************/
/**
 *\defgroup thread_base ��������
 *\ingroup thread
 **************************************************************************************************/

/**
 *\ingroup thread_base
 *\brief �����߳�
 *\param[in] func �̴߳�����
 *\param[in] param  �̲߳���
 *\return �������߳�id
 */
sfpr_thread_id sfpr_thread_create(sfpr_thread func,void *param);

/**
 *\ingroup thread_base
 *\brief �����߳�
 *\param[in] func �̴߳�����
 *\param[in] param  �̲߳���
 *\param[in] name  �߳�����
 *\param[in] stack_size  �߳�ջ��С
 *\param[in] priority  �߳����ȼ�
 *\return �������߳�id
 */
sfpr_thread_id sfpr_thread_create_ex(sfpr_thread func,void *param,char *name,int stack_size,int priority);


sfpr_thread_id sfpr_thread_create_with_attr(sfpr_thread func,void *ptr,sfpr_thread_attr_t *attr_info);

sfpr_thread_id sfpr_thread_get_id(void);

int sfpr_thread_destory(sfpr_thread_id thread);


#ifdef __cplusplus
}
#endif

#endif
