/**
 *\file sfpr_timer.h
 *\brief SFPR��Ķ�ʱ��ģ��
 *\author bijing
 *\version 0.1.0
 *\date 2012-02-16
 */
 
#ifndef SFPR_TIMER_H
#define SFPR_TIMER_H


#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <time.h>

#include "sfpr_global.h"

#if defined VXWORKS
#include "vxWorks.h"
#include <timers.h>
typedef timer_t sfpr_timer_id_t;

#elif defined WIN32
typedef sfpr_uint32_t sfpr_timer_id_t;

#else
#include <sys/time.h>
#include<signal.h>
typedef timer_t sfpr_timer_id_t;

#endif 

#include "sfpr_global.h"
#include "sfpr_time.h"

/**************************************************************************************************/
/**
 *\defgroup timer ��ʱ��ģ��
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup timer_base �����ṹ�ͺ���
 *\ingroup timer
 **************************************************************************************************/
typedef void (*sfpr_timer_cb_t)(int userid, void*param);

/**
 *\ingroup timer_base
 *\brief ��ʱ�����ݽṹ
 */
typedef struct
{
	sfpr_timer_id_t timerid;
	sfpr_timer_cb_t handle; 
	void  *param;
	int userid;
	int nloop;
	long offset;
}sfpr_timer_t;


/**
 *\ingroup timer_base
 *\brief ������ʱ�����ṩͳһ��װ��
 *\return �����Ķ�ʱ��
 */
sfpr_timer_t* sfpr_timer_create();

/**
 *\ingroup timer_base
 *\brief ��ʼ����ʱ��
 *\return �Ƿ�ɹ�
 */
int sfpr_timer_init(sfpr_timer_t* timer, int userid, sfpr_timer_cb_t cb, void *param);

/**
 *\ingroup timer_base
 *\brief ���ٶ�ʱ��
 *\param[in] Timerid ��ʱ��id
 *\return �Ƿ�ɹ�
 */
int  sfpr_timer_destory(sfpr_timer_t **timer);

/**
 *\ingroup timer_base
 *\brief ������ʱ��
 *\param[in] Timerid ��ʱ��id
 *\param[in] sec ��ʱ��������ʱ����;ms
 *\return �Ƿ�ɹ�
 */
int  sfpr_timer_start(sfpr_timer_t *timer, int msec, int nloop);

/**
 *\ingroup timer_base
 *\brief ȡ����ʱ��ִ��
 *\param[in] Timerid ��ʱ��id
 *\return �Ƿ�ɹ�
 */
int  sfpr_timer_cancel(sfpr_timer_t *timer);



#ifdef __cplusplus
}
#endif

#endif

