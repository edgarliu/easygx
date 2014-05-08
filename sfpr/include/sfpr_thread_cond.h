#ifndef SFPR_THREAD_CONDITION_H
#define SFPR_THREAD_CONDITION_H

/**
 * @file sfpr_thread_cond.h
 * @brief SFPR���locksģ��
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
 *\defgroup cond ��������
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup cond_base ��������
 *\ingroup cond
 **************************************************************************************************/

/**
 *\ingroup cond_base
 * @brief ��������ʼ����������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_cond_create(sfpr_cond_t *cond);


/**************************************************************************************************/
/**
 *\defgroup cond_status ״̬����
 *\ingroup cond
 **************************************************************************************************/

/**
 *\ingroup cond_status
 * @brief �ȴ��źŴ���������һֱ����
 * @param[in] *cond ��������
 * @param[in] *mutex ������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_cond_wait(sfpr_cond_t *cond, sfpr_mutex_t *mutex);

/**
 *\ingroup cond_status
 * @brief �ȴ��ź�; �����ʱ�趨ʱ����Ȼû���źŴ������򷵻�SFPR_TIMEOUT
 * @param[in] *cond ��������
 * @param[in] *mutex ������
 * @param[in] timeout ��ʱʱ�䣬ms
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_cond_timedwait(sfpr_cond_t *cond, sfpr_mutex_t *mutex, sfpr_int_t timeout);

/**
 *\ingroup cond_status
 * @brief �����źţ��ȴ��źŵ����������н�����һ���������źŲ�ִ��
 * @param[in] *cond ��������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_cond_signal(sfpr_cond_t *cond);

/**
 * @brief �����ź�;֪ͨ���еȴ�����ִ��
 * @param[in] cond *��������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark ����Win32ϵͳ�£�û��ʵ�ָýӿ�
 */
sfpr_int_t sfpr_cond_signal_all(sfpr_cond_t *cond);

/**
 *\ingroup cond_base
 * @brief ������������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_cond_destroy(sfpr_cond_t *cond);

#ifdef __cplusplus
}
#endif

#endif


