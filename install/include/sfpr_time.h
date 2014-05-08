
/**
 *\file sfpr_time.h
 *\brief SFPR库的时间模块
 *\author 刘晓强
 *\version 0.1.0
 *\date 2012-02-15
 */
 
#ifndef SFPR_TIME_H
#define SFPR_TIME_H


#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

#if defined VXWORKS
#include <timers.h>

#elif defined WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif 

#include "sfpr_global.h"

/**************************************************************************************************/
/**
 *\defgroup time 时间模块
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup time_base 基本结构和函数
 *\ingroup time
 **************************************************************************************************/

/**
 *\ingroup time_base
 *\brief 时间描述
 */
typedef struct sfpr_time_s{
	time_t sec;/**< 秒*/
	sfpr_long_t usec;/**< 微秒*/
}sfpr_time_t;

/**
 *\ingroup time_base
 *\brief 格式化时间描述
 */
typedef struct sfpr_tm_s{
	sfpr_int_t year;/**< 年*/
	sfpr_int_t month;/**< 月*/
	sfpr_int_t mday;/**< 日*/
	sfpr_int_t wday;/**< 一周的第几天*/
	sfpr_int_t hour;/**< 小时*/
	sfpr_int_t min;/**< 分*/
	sfpr_int_t sec;/**< 秒*/
	sfpr_int_t msec;/**< 毫秒*/
}sfpr_tm_t;

/**
 *\ingroup time_base
 *\brief 获取当前时间
 *\param[out] time 时间指针。
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
int sfpr_time_now(sfpr_time_t *time);

/**
 *\ingroup time_base
 *\brief 把当前时间格式化,转为本地时间
 *\param[in] time 时间指针。
 *\param[out] tm 格式化时间指针。
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
int sfpr_time_localtime(sfpr_time_t *time,sfpr_tm_t *tm);

/**
 *\ingroup time_base
 *\brief 把当前时间格式化,转为UTC时间
 *\param[in] time 时间指针。
 *\param[out] tm 格式化时间指针。
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
int sfpr_time_get_utctime(sfpr_time_t *time,sfpr_tm_t *tm);

/**
 *\ingroup time_base
 *\brief 把格式化的时间转化为秒数
 *\param[in] tm 格式化时间指针。
 *\param[out] time 时间指针。
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
int sfpr_time_mktime(sfpr_tm_t *tm,sfpr_time_t *time);


/**
 *\ingroup time_base
 *\brief 设置系统时间
 *\param[in] tm 格式化时间指针。
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
int sfpr_time_set(sfpr_tm_t *tm);

/**************************************************************************************************/
/**
 *\defgroup time_bench 压力测试时间
 *\ingroup time
 **************************************************************************************************/


/**
 *\ingroup time_bench
 *\brief 压力测试时间描述
 */
typedef struct sfpr_bench_elapse_s{
	sfpr_time_t start;/**< 开始时间*/
	sfpr_time_t stop;/**< 结束时间*/
	union{
		sfpr_int_t _int;
		sfpr_float_t _float;		
	}etime;/**< 流逝时间(毫秒)*/
	union{
		sfpr_uint32_t _uint32;
		sfpr_float_t _float;		
	}bench;/**< 压力大小*/
	sfpr_float_t op_per_ms;/**< 每毫秒操作数*/
	sfpr_float_t ms_per_op;/**< 每个操作花费的时间(毫秒)*/
}sfpr_bench_elapse_t;


/**
 *\ingroup time_bench
 *\brief 开始压力测试
 *\param[in] bench 压力测试时间结构指针。
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
sfpr_int_t sfpr_bench_start(sfpr_bench_elapse_t *bench);

/**
 *\ingroup time_bench
 *\brief 结束压力测试
 *\param[in] bench 压力测试时间结构指针。
 *\param[in] bench_count 当前压力大小。
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
sfpr_int_t sfpr_bench_stop(sfpr_bench_elapse_t *bench,sfpr_uint32_t bench_count);

/**
 *\ingroup time_bench
 *\brief 打印压力测试结果
 *\param[in] bench 压力测试时间结构指针。
 *\param[in] title 标题。 
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
sfpr_int_t sfpr_bench_print(sfpr_bench_elapse_t *bench,char *title);

/**
 *\ingroup time_base
 *\brief 睡眠一段时间
 *\param 睡眠的时间(毫秒)。
 *\return 是否成功
 *\retval 0 成功
 *\retval <0 失败 
 */
sfpr_int_t sfpr_time_msleep(int microseconds);

#ifdef __cplusplus
}
#endif

#endif

