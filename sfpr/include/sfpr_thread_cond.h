#ifndef SFPR_THREAD_CONDITION_H
#define SFPR_THREAD_CONDITION_H

/**
 * @file sfpr_thread_cond.h
 * @brief SFPR库的locks模块
 * @author nahaitao
 * @version 0.1.0
 * @date 2012-02-09
 */

#if defined WIN32
#include "sfpr_thread_mutex.h"
typedef struct
{
    HANDLE semaphore;
    CRITICAL_SECTION csection;
    unsigned long num_waiting;
    unsigned long num_wake;
    unsigned long generation;
}sfpr_cond_t;

#elif defined VXWORKS

#include "sfpr_thread_mutex.h"
#define TICK_PER_SECOND 60
typedef SEM_ID sfpr_cond_t;

#else
#include "sfpr_thread_mutex.h"

typedef pthread_cond_t sfpr_cond_t;

#endif

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup cond 条件变量
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup cond_base 基本函数
 *\ingroup cond
 **************************************************************************************************/

/**
 *\ingroup cond_base
 * @brief 创建并初始化条件变量
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_cond_create(sfpr_cond_t *cond);


/**************************************************************************************************/
/**
 *\defgroup cond_status 状态函数
 *\ingroup cond
 **************************************************************************************************/

/**
 *\ingroup cond_status
 * @brief 等待信号触发，否则一直阻塞
 * @param[in] *cond 条件变量
 * @param[in] *mutex 互斥锁
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_cond_wait(sfpr_cond_t *cond, sfpr_mutex_t *mutex);

/**
 *\ingroup cond_status
 * @brief 等待信号; 如果超时设定时间依然没有信号触发，则返回SFPR_TIMEOUT
 * @param[in] *cond 条件变量
 * @param[in] *mutex 互斥锁
 * @param[in] timeout 超时时间，ms
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_cond_timedwait(sfpr_cond_t *cond, sfpr_mutex_t *mutex, sfpr_int_t timeout);

/**
 *\ingroup cond_status
 * @brief 发送信号；等待信号的所有任务中将会有一个抢到该信号并执行
 * @param[in] *cond 条件变量
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_cond_signal(sfpr_cond_t *cond);

/**
 * @brief 发送信号;通知所有等待任务执行
 * @param[in] cond *条件变量
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 对于Win32系统下，没有实现该接口
 */
sfpr_int_t sfpr_cond_signal_all(sfpr_cond_t *cond);

/**
 *\ingroup cond_base
 * @brief 销毁条件变量
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_cond_destroy(sfpr_cond_t *cond);

#ifdef __cplusplus
}
#endif

#endif


