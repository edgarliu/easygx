
/**
 *\file sfpr_time.h
 *\brief SFPR���ʱ��ģ��
 *\author ����ǿ
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
 *\defgroup time ʱ��ģ��
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup time_base �����ṹ�ͺ���
 *\ingroup time
 **************************************************************************************************/

/**
 *\ingroup time_base
 *\brief ʱ������
 */
typedef struct sfpr_time_s{
	time_t sec;/**< ��*/
	sfpr_long_t usec;/**< ΢��*/
}sfpr_time_t;

/**
 *\ingroup time_base
 *\brief ��ʽ��ʱ������
 */
typedef struct sfpr_tm_s{
	sfpr_int_t year;/**< ��*/
	sfpr_int_t month;/**< ��*/
	sfpr_int_t mday;/**< ��*/
	sfpr_int_t wday;/**< һ�ܵĵڼ���*/
	sfpr_int_t hour;/**< Сʱ*/
	sfpr_int_t min;/**< ��*/
	sfpr_int_t sec;/**< ��*/
	sfpr_int_t msec;/**< ����*/
}sfpr_tm_t;

/**
 *\ingroup time_base
 *\brief ��ȡ��ǰʱ��
 *\param[out] time ʱ��ָ�롣
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
int sfpr_time_now(sfpr_time_t *time);

/**
 *\ingroup time_base
 *\brief �ѵ�ǰʱ���ʽ��,תΪ����ʱ��
 *\param[in] time ʱ��ָ�롣
 *\param[out] tm ��ʽ��ʱ��ָ�롣
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
int sfpr_time_localtime(sfpr_time_t *time,sfpr_tm_t *tm);

/**
 *\ingroup time_base
 *\brief �ѵ�ǰʱ���ʽ��,תΪUTCʱ��
 *\param[in] time ʱ��ָ�롣
 *\param[out] tm ��ʽ��ʱ��ָ�롣
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
int sfpr_time_get_utctime(sfpr_time_t *time,sfpr_tm_t *tm);

/**
 *\ingroup time_base
 *\brief �Ѹ�ʽ����ʱ��ת��Ϊ����
 *\param[in] tm ��ʽ��ʱ��ָ�롣
 *\param[out] time ʱ��ָ�롣
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
int sfpr_time_mktime(sfpr_tm_t *tm,sfpr_time_t *time);


/**
 *\ingroup time_base
 *\brief ����ϵͳʱ��
 *\param[in] tm ��ʽ��ʱ��ָ�롣
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
int sfpr_time_set(sfpr_tm_t *tm);

/**************************************************************************************************/
/**
 *\defgroup time_bench ѹ������ʱ��
 *\ingroup time
 **************************************************************************************************/


/**
 *\ingroup time_bench
 *\brief ѹ������ʱ������
 */
typedef struct sfpr_bench_elapse_s{
	sfpr_time_t start;/**< ��ʼʱ��*/
	sfpr_time_t stop;/**< ����ʱ��*/
	union{
		sfpr_int_t _int;
		sfpr_float_t _float;		
	}etime;/**< ����ʱ��(����)*/
	union{
		sfpr_uint32_t _uint32;
		sfpr_float_t _float;		
	}bench;/**< ѹ����С*/
	sfpr_float_t op_per_ms;/**< ÿ���������*/
	sfpr_float_t ms_per_op;/**< ÿ���������ѵ�ʱ��(����)*/
}sfpr_bench_elapse_t;


/**
 *\ingroup time_bench
 *\brief ��ʼѹ������
 *\param[in] bench ѹ������ʱ��ṹָ�롣
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
sfpr_int_t sfpr_bench_start(sfpr_bench_elapse_t *bench);

/**
 *\ingroup time_bench
 *\brief ����ѹ������
 *\param[in] bench ѹ������ʱ��ṹָ�롣
 *\param[in] bench_count ��ǰѹ����С��
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
sfpr_int_t sfpr_bench_stop(sfpr_bench_elapse_t *bench,sfpr_uint32_t bench_count);

/**
 *\ingroup time_bench
 *\brief ��ӡѹ�����Խ��
 *\param[in] bench ѹ������ʱ��ṹָ�롣
 *\param[in] title ���⡣ 
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
sfpr_int_t sfpr_bench_print(sfpr_bench_elapse_t *bench,char *title);

/**
 *\ingroup time_base
 *\brief ˯��һ��ʱ��
 *\param ˯�ߵ�ʱ��(����)��
 *\return �Ƿ�ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ�� 
 */
sfpr_int_t sfpr_time_msleep(int microseconds);

#ifdef __cplusplus
}
#endif

#endif

