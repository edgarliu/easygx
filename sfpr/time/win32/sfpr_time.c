
#include <stdio.h>
#include <dos.h>
#include "sfpr_time.h"

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

int sfpr_time_now(sfpr_time_t *time)
{
	time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;

    GetLocalTime(&wtm);
    tm.tm_year    = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst   = -1;
    clock = mktime(&tm);
	time->sec = clock;
	time->usec = wtm.wMilliseconds * 1000;
	return 0;
}

int sfpr_time_localtime(sfpr_time_t *time,sfpr_tm_t *tm)
{
	struct tm *mytm;
	if(!time || !tm)
		return -1;

	mytm=localtime(&(time->sec));

	tm->year = mytm->tm_year+1900;
	tm->month = mytm->tm_mon+1;
	tm->mday = mytm->tm_mday;
	tm->wday = mytm->tm_wday;
	tm->hour = mytm->tm_hour;
	tm->min = mytm->tm_min;
	tm->sec = mytm->tm_sec;
	tm->msec = time->usec/1000;
	return 0;
}

int sfpr_time_get_utctime(sfpr_time_t *time,sfpr_tm_t *tm)
{
	struct tm *mytm;
	if(!time || !tm)
		return -1;

	mytm=gmtime(&(time->sec));

	tm->year = mytm->tm_year+1900;
	tm->month = mytm->tm_mon+1;
	tm->mday = mytm->tm_mday;
	tm->wday = mytm->tm_wday;
	tm->hour = mytm->tm_hour;
	tm->min = mytm->tm_min;
	tm->sec = mytm->tm_sec;
	tm->msec = time->usec/1000;
	return 0;
}

int sfpr_time_mktime(sfpr_tm_t *tm,sfpr_time_t *time)
{
	struct tm mytm;
	mytm.tm_year = tm->year - 1900;
	mytm.tm_mon = tm->month - 1;
	mytm.tm_mday = tm->mday;
	mytm.tm_wday = tm->wday;
	mytm.tm_hour = tm->hour;
	mytm.tm_min = tm->min;
	mytm.tm_sec = tm->sec;
	
	time->sec = mktime(&mytm);
	time->usec = tm->msec*1000;
	return 0;
}

int sfpr_time_set(sfpr_tm_t *tm)
{
	SYSTEMTIME time;
	time.wYear = tm->year;
	time.wMonth = tm->month;
	time.wDay = tm->mday;
	time.wHour = tm->hour;
	time.wMinute = tm->min;
	time.wSecond = tm->sec;
	time.wMilliseconds = tm->msec;
	SetLocalTime(&time);
	return 0;		
}


sfpr_int_t sfpr_bench_start(sfpr_bench_elapse_t *bench)
{
	sfpr_tm_t mytm;

	if(!bench)
		return -1;

	sfpr_time_now(&(bench->start));
	sfpr_time_localtime(&(bench->start),&mytm);
	printf("Start Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n",
				mytm.year,mytm.month,
				mytm.mday,mytm.hour,
				mytm.min,mytm.sec,
				mytm.msec);
	
	return 0;
}


sfpr_int_t sfpr_bench_stop(sfpr_bench_elapse_t *bench,sfpr_uint32_t bench_count)
{
	sfpr_tm_t mytm;

	if(!bench)
		return -1;

	sfpr_time_now(&(bench->stop));
	sfpr_time_localtime(&(bench->stop),&mytm);
	printf("Stop  Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n",
				mytm.year,mytm.month,
				mytm.mday,mytm.hour,
				mytm.min,mytm.sec,
				mytm.msec);

	bench->bench._uint32 = bench_count;
	bench->etime._int = (bench->stop.sec - bench->start.sec)*1000 + 
					(bench->stop.usec - bench->start.usec)/1000;
	
	bench->op_per_ms = bench->bench._float/bench->etime._float;
	bench->ms_per_op = bench->etime._float/bench->bench._float;

	return 0;	
}

sfpr_int_t sfpr_bench_print(sfpr_bench_elapse_t *bench,char *title)
{
	if(!bench)
		return -1;

	printf("========== Time Stat ============\n");
	printf("Title               : %s\n",title);	
	printf("Bench               : %d\n",bench->bench._uint32);
	printf("Esp Time(msec)      : %d\n",bench->etime._int);
	printf("Operate/msec        : %f\n",bench->op_per_ms);
	printf("msec/Operate        : %f\n",bench->ms_per_op);
	printf("========== Time Stat ============\n\n");

	return 0;
}

sfpr_int_t sfpr_time_msleep(int microseconds)
{
	Sleep(microseconds);
	return 0;
}

#ifdef __cplusplus
}
#endif
