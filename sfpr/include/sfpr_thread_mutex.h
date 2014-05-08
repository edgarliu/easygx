#ifndef SFPR_MUTEX_H
#define SFPR_MUTEX_H
/**
 * @file sfpr_thread_mutex.h
 * @brief SFPR���locksģ��
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
 *\defgroup mutex ������
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup mutex_base ��������
 *\ingroup mutex
 **************************************************************************************************/

/**
 *\ingroup mutex_base
 * @brief ��������ʼ��������.
 * @param *pMutex ������
 * @param para ����������һ������Ϊ0����
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �ýӿ��в�û�и�pMutex���������ڴ�
 */
sfpr_int_t sfpr_mutex_create(sfpr_mutex_t *pMutex, sfpr_int_t para);

/**************************************************************************************************/
/**
 *\defgroup mutex_ability ���ܺ���
 *\ingroup mutex
 **************************************************************************************************/

/**
 *\ingroup mutex_ability
 * @brief ����,����Ѿ�����������ò����ᵼ������
 * @param *pMutex ������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_mutex_lock(sfpr_mutex_t *pMutex);

/**
 *\ingroup mutex_ability
 * @brief ���Լ���������Ѿ����������򷵻�SFPR_BUSY
 * @param *pMutex ������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_BUSY �Ѿ�����
 * @retval SFPR_ERROR �쳣
 */
sfpr_int_t sfpr_mutex_trylock(sfpr_mutex_t *pMutex);

/**
 *\ingroup mutex_ability
 * @brief ����
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_mutex_unlock(sfpr_mutex_t *pMutex);

/**
 *\ingroup mutex_base
 * @brief ������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_mutex_destroy(sfpr_mutex_t *pMutex);

#ifdef __cplusplus
}
#endif

#endif

