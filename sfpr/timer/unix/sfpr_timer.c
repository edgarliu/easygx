#include "sfpr_timer.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

static void timer_handler(/*unsigned long*/union sigval arg) {
	sfpr_timer_t* timer = (sfpr_timer_t*)arg.sival_ptr;
	if(timer->handle)
		timer->handle(timer->userid, timer->param);
	if(timer->nloop != -1) {
		if(timer->nloop == 0 || timer->nloop == 1)
			sfpr_timer_cancel(timer);
		else
			timer->nloop--;
	}
}

sfpr_timer_t* sfpr_timer_create()
{
	sfpr_timer_id_t timer_id;
	sfpr_timer_t *timer;
	struct sigevent evp;
	int ret;

	timer = (sfpr_timer_t*)malloc(sizeof(sfpr_timer_t));
	if(!timer)
	{
		return NULL;
	}
	memset(timer, 0x0, sizeof(sfpr_timer_t));

	memset(&evp, 0, sizeof(evp));
	evp.sigev_value.sival_ptr = &timer_id;
	evp.sigev_notify = SIGEV_THREAD;//SIGEV_SIGNAL;//SIGEV_THREAD
	evp.sigev_notify_function = timer_handler;
	evp.sigev_value.sival_ptr = timer;   //作为函数的参数

	timer_create(CLOCK_REALTIME,&evp,&timer_id);
	if( ret)
	{
		free(timer);
		fprintf(stdout, "sfpr_timer_create | timer_create | failed !:%d\n", errno);
		return NULL;
	}
	timer->timerid = timer_id;
	return timer;
}

int sfpr_timer_init(sfpr_timer_t* timer, int userid, sfpr_timer_cb_t cb, void *param)
{
	int ret=0;
	if(!timer)
		return -1;
	timer->handle = cb;
	timer->param = param;
	timer->userid = userid;
	return ret;
}

int  sfpr_timer_destory(sfpr_timer_t **timer)
{
	int ret=0;
	sfpr_timer_t *_timer = *timer;
	if(!_timer)
		return -1;
	timer_delete(_timer->timerid);
	free(_timer);
	return ret;
}

int  sfpr_timer_start(sfpr_timer_t *timer, int msec, int nloop)
{
	int ret   ;
	struct itimerspec ts;
	if(!timer)
		return -1;
	ts.it_interval.tv_sec = msec/1000;
	ts.it_interval.tv_nsec = (msec%1000)*1000000;
	ts.it_value.tv_sec = msec/1000;
	ts.it_value.tv_nsec = (msec%1000)*1000000;
	timer->nloop = nloop;
	ret = timer_settime (timer->timerid,CLOCK_REALTIME,&ts,NULL);

	return ret;
}

int  sfpr_timer_cancel(sfpr_timer_t *timer)
{
	int ret =SFPR_ERROR;
	struct itimerspec ts;
	if(!timer)
		return -1;
	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec =0;
	ts.it_value.tv_nsec = 0;
	ret = timer_settime (timer->timerid, TIMER_ABSTIME, &ts, NULL);
	return ret;
}

#ifdef __cplusplus
}
#endif

