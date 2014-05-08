/**
 *\file sfpr_timer.h
 *\brief SFPR库的定时器模块
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
 *\defgroup timer 定时器模块
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup timer_base 基本结构和函数
 *\ingroup timer
 **************************************************************************************************/
typedef void (*sfpr_timer_cb_t)(int userid, void*param);

/**
 *\ingroup timer_base
 *\brief 定时器数据结构
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
 *\brief 创建定时器，提供统一封装。
 *\return 创建的定时器
 */
sfpr_timer_t* sfpr_timer_create();

/**
 *\ingroup timer_base
 *\brief 初始化定时器
 *\return 是否成功
 */
int sfpr_timer_init(sfpr_timer_t* timer, int userid, sfpr_timer_cb_t cb, void *param);

/**
 *\ingroup timer_base
 *\brief 销毁定时器
 *\param[in] Timerid 定时器id
 *\return 是否成功
 */
int  sfpr_timer_destory(sfpr_timer_t **timer);

/**
 *\ingroup timer_base
 *\brief 启动定时器
 *\param[in] Timerid 定时器id
 *\param[in] sec 定时器启动的时间间隔;ms
 *\return 是否成功
 */
int  sfpr_timer_start(sfpr_timer_t *timer, int msec, int nloop);

/**
 *\ingroup timer_base
 *\brief 取消定时器执行
 *\param[in] Timerid 定时器id
 *\return 是否成功
 */
int  sfpr_timer_cancel(sfpr_timer_t *timer);



#ifdef __cplusplus
}
#endif

#endif

