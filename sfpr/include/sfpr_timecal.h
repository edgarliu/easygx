/**
 *\file sfpr_time.h
 *\brief SFPR库的时间模块
 *\author 刘晓强
 *\version 0.1.0
 *\date 2012-02-15
 */
 
#ifndef SFPR_TIME_CAL_H
#define SFPR_TIME_CAL_H

#ifdef __cplusplus
extern "C" {
#endif


int sfpr_time_get_days_of_year(int year);

int sfpr_time_get_days_of_february(int year);

int sfpr_time_get_elapse_seconds_in_day(int hour,int minite,int second);

int sfpr_time_get_elapse_days_in_year(int year,int month,int day);

int sfpr_time_days_calculate(int year1,int month1,int day1,
								  int year2,int month2,int day2);

long int sfpr_time_time2second(int year,int month,int day,int hour,int minite,int second,int start_year);

int sfpr_time_second2time(int seconds,int* year,int* month,
		int* day,int* hour,int* minite,int* second,int start_year);

int sfpr_time_get_sum(sfpr_tm_t *time1,sfpr_tm_t* time2);

int sfpr_time_count_time(sfpr_tm_t *time1,int offset,int mode);

#ifdef __cplusplus
}
#endif

#endif
