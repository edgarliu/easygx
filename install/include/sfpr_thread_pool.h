
#ifndef SFPR_THREAD_POOL_H
#define SFPR_THREAD_POOL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\file sfpr_thread_pool.h
 *\brief SFPR库的线程池模块
 *\author bijing
 *\version 0.1.0
 *\date 2012-02-14
 */

#include "sfpr_global.h"
#include "sfpr_thread.h"
#include "sfpr_network.h"

/**************************************************************************************************/
/**
 *\defgroup thread_pool 线程池
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup thread_pool_struct 基本数据结构
 *\ingroup thread_pool
 **************************************************************************************************/

#define SFPR_THREAD_POOL_MANAGER_INTERVAL 5	/**<管理线程sleep间隔时间设置 */

#define SFPR_THREAD_POOL_DEALY_TIME		6

#define SFPR_THREAD_POOL_MAX_TASK_NUM   128    /**< 队列节点存储的数据 */

#define SFPR_THREAD_POOL_WORKER_TASK_NUM		16

#define SFPR_THREAD_POOL_TASK_MAX_ARGC		8  /**< 任务处理函数最多参数个数 */

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
 *\brief 任务节点的描述，由用户提交任务时填充该结构
 */
typedef struct sfpr_threadpool_task_s
{
	void (*task_process)(struct sfpr_threadpool_task_s *this);/**< 任务处理函数 */
	sfpr_uint32_t flags;
	int argc;/**< 处理程序的参数个数 */
	void *argv[SFPR_THREAD_POOL_TASK_MAX_ARGC];/**< 处理程序的各个参数值数值 */
	int busy;
	sfpr_threadpool_key_t key;
	sfpr_uint32_t magic;
}sfpr_threadpool_task_t;

typedef struct sfpr_threadpool_s sfpr_threadpool_t;


/**************************************************************************************************/
/**
 *\defgroup thread_pool_base 基本函数
 *\ingroup thread_pool
 **************************************************************************************************/

/**
 *\ingroup thread_pool_base
 *\brief 创建线程池
 *\param[in] name  线程池名称
 *\param[in] min_num  最小线程数
 *\param[in] max_num  最大线程数
 *\param[in] stack_size  线程栈大小
 *\param[in] priority  线程优先级
 *\return 创建的线程池结构指针
 */
sfpr_threadpool_t *sfpr_threadpool_create(char *name,int min_num,int max_num,int stack_size,int priority);

int sfpr_threadpool_set_mode(sfpr_threadpool_t *pthreadpool,int mode);

/**
 *\ingroup thread_pool_base
 *\brief 初始化线程池，创建线程和初始化任务队列
 *\param[in] *pTpool 指向线程池的指针			
 *\return SFPR_SUCCESS 成功 SFPR_ERROR 失败
 */
int sfpr_threadpool_start(sfpr_threadpool_t *pthreadpool);

/**
 *\ingroup thread_pool_base
 *\brief 线程池关闭，销毁线程和消息队列、释放申请的空间
 *\param[in] ppthreadpool 指向线程池的二级指针			
 *\return SFPR_SUCCESS 成功 SFPR_ERROR 失败
 */
int sfpr_threadpool_close(sfpr_threadpool_t **ppthreadpool);

/**************************************************************************************************/
/**
 *\defgroup thread_pool_ability 功能函数
 *\ingroup thread_pool
 **************************************************************************************************/

/**
 *\ingroup thread_pool_ability
 *\brief 提交任务处理函数及参数给线程池进行处理
 *\param[in] *pTpool 指向线程池的指针	
  *\param[in] *taskmgr 指向管理任务结构的指针			
 *\return SFPR_SUCCESS 成功 SFPR_ERROR 失败
 */
int sfpr_threadpool_submit(sfpr_threadpool_t *pthreadpool,sfpr_threadpool_task_t *task);


char* sfpr_threadpool_get_name(sfpr_threadpool_t *pthreadpool);


#ifdef __cplusplus
}
#endif

#endif
