#ifndef SFPR_MUTEX_H
#define SFPR_MUTEX_H
/**
 * @file sfpr_thread_mutex.h
 * @brief SFPR库的locks模块
 * @author nahaitao
 * @version 0.1.0
 * @date 2012-02-09
 */
#include "sfpr_global.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#if defined __MINGW32__
#include <windows.h>
typedef CRITICAL_SECTION sfpr_mutex_t;

#elif defined WIN32
#include <windows.h>
typedef CRITICAL_SECTION sfpr_mutex_t;

#elif defined VXWORKS
#include "vxWorks.h"
#include "semLib.h"
typedef SEM_ID sfpr_mutex_t;

#else
#include <pthread.h>
typedef pthread_mutex_t sfpr_mutex_t;

#endif

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup mutex 互斥锁
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup mutex_base 基本函数
 *\ingroup mutex
 **************************************************************************************************/

/**
 *\ingroup mutex_base
 * @brief 创建并初始化互斥锁.
 * @param *pMutex 互斥锁
 * @param para 附件参数，一般设置为0即可
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 该接口中并没有给pMutex参数分配内存
 */
sfpr_int_t sfpr_mutex_create(sfpr_mutex_t *pMutex, sfpr_int_t para);

/**************************************************************************************************/
/**
 *\defgroup mutex_ability 功能函数
 *\ingroup mutex
 **************************************************************************************************/

/**
 *\ingroup mutex_ability
 * @brief 加锁,如果已经被上锁，则该操作会导致阻塞
 * @param *pMutex 互斥锁
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_mutex_lock(sfpr_mutex_t *pMutex);

/**
 *\ingroup mutex_ability
 * @brief 尝试加锁，如果已经被上锁，则返回SFPR_BUSY
 * @param *pMutex 互斥锁
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_BUSY 已经上锁
 * @retval SFPR_ERROR 异常
 */
sfpr_int_t sfpr_mutex_trylock(sfpr_mutex_t *pMutex);

/**
 *\ingroup mutex_ability
 * @brief 解锁
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_mutex_unlock(sfpr_mutex_t *pMutex);

/**
 *\ingroup mutex_base
 * @brief 销毁锁
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_mutex_destroy(sfpr_mutex_t *pMutex);

#ifdef __cplusplus
}
#endif

#endif

