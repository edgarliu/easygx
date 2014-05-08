
#ifndef SFPR_THREAD_POOL_H
#define SFPR_THREAD_POOL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\file sfpr_thread_pool.h
 *\brief SFPR����̳߳�ģ��
 *\author bijing
 *\version 0.1.0
 *\date 2012-02-14
 */

#include "sfpr_global.h"
#include "sfpr_thread.h"
#include "sfpr_network.h"

/**************************************************************************************************/
/**
 *\defgroup thread_pool �̳߳�
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup thread_pool_struct �������ݽṹ
 *\ingroup thread_pool
 **************************************************************************************************/

#define SFPR_THREAD_POOL_MANAGER_INTERVAL 5	/**<�����߳�sleep���ʱ������ */

#define SFPR_THREAD_POOL_DEALY_TIME		6

#define SFPR_THREAD_POOL_MAX_TASK_NUM   128    /**< ���нڵ�洢������ */

#define SFPR_THREAD_POOL_WORKER_TASK_NUM		16

#define SFPR_THREAD_POOL_TASK_MAX_ARGC		8  /**< ������������������ */

#define SFPR_THREAD_POOL_FLAG_DELETE		0x10000001

#define SFPR_THREAD_POOL_NAME_DEF		"SfprTP"

#define SFPR_THREAD_POOL_KEY_MAGIC	0xaa99

enum{
	sfpr_threadpool_mode_none = 0,
	sfpr_threadpool_mode_socket
};

typedef union sfpr_threadpool_key_s{
	struct{
		sfpr_uint32_t addr;
		sfpr_int_t port;
		sfpr_int_t fd;
	}sock;
	struct{
		sfpr_int_t fd;
	}other;
}sfpr_threadpool_key_t;

/**
 *\ingroup thread_pool_struct
 *\brief ����ڵ�����������û��ύ����ʱ���ýṹ
 */
typedef struct sfpr_threadpool_task_s
{
	void (*task_process)(struct sfpr_threadpool_task_s *this);/**< �������� */
	sfpr_uint32_t flags;
	int argc;/**< �������Ĳ������� */
	void *argv[SFPR_THREAD_POOL_TASK_MAX_ARGC];/**< �������ĸ�������ֵ��ֵ */
	int busy;
	sfpr_threadpool_key_t key;
	sfpr_uint32_t magic;
}sfpr_threadpool_task_t;

typedef struct sfpr_threadpool_s sfpr_threadpool_t;


/**************************************************************************************************/
/**
 *\defgroup thread_pool_base ��������
 *\ingroup thread_pool
 **************************************************************************************************/

/**
 *\ingroup thread_pool_base
 *\brief �����̳߳�
 *\param[in] name  �̳߳�����
 *\param[in] min_num  ��С�߳���
 *\param[in] max_num  ����߳���
 *\param[in] stack_size  �߳�ջ��С
 *\param[in] priority  �߳����ȼ�
 *\return �������̳߳ؽṹָ��
 */
sfpr_threadpool_t *sfpr_threadpool_create(char *name,int min_num,int max_num,int stack_size,int priority);

int sfpr_threadpool_set_mode(sfpr_threadpool_t *pthreadpool,int mode);

/**
 *\ingroup thread_pool_base
 *\brief ��ʼ���̳߳أ������̺߳ͳ�ʼ���������
 *\param[in] *pTpool ָ���̳߳ص�ָ��			
 *\return SFPR_SUCCESS �ɹ� SFPR_ERROR ʧ��
 */
int sfpr_threadpool_start(sfpr_threadpool_t *pthreadpool);

/**
 *\ingroup thread_pool_base
 *\brief �̳߳عرգ������̺߳���Ϣ���С��ͷ�����Ŀռ�
 *\param[in] ppthreadpool ָ���̳߳صĶ���ָ��			
 *\return SFPR_SUCCESS �ɹ� SFPR_ERROR ʧ��
 */
int sfpr_threadpool_close(sfpr_threadpool_t **ppthreadpool);

/**************************************************************************************************/
/**
 *\defgroup thread_pool_ability ���ܺ���
 *\ingroup thread_pool
 **************************************************************************************************/

/**
 *\ingroup thread_pool_ability
 *\brief �ύ�����������������̳߳ؽ��д���
 *\param[in] *pTpool ָ���̳߳ص�ָ��	
  *\param[in] *taskmgr ָ���������ṹ��ָ��			
 *\return SFPR_SUCCESS �ɹ� SFPR_ERROR ʧ��
 */
int sfpr_threadpool_submit(sfpr_threadpool_t *pthreadpool,sfpr_threadpool_task_t *task);


char* sfpr_threadpool_get_name(sfpr_threadpool_t *pthreadpool);


#ifdef __cplusplus
}
#endif

#endif
