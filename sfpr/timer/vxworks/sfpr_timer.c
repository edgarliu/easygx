#include "sfpr_timer.h"
#include <errno.h>
#ifdef __cplusplus
extern "C" {
#endif

static void timer_handler(timer_t timerid, int arg) {
	sfpr_timer_t* timer = (sfpr_timer_t*)arg;
	if(timer->handle)
		timer->handle(timer->userid, timer->param);
	if(timer->nloop != -1) {
		if(timer->nloop == 0 || timer->nloop == 1)
			timer_cancel(timer->timerid);
		else
			timer->nloop--;
	}
}


sfpr_timer_t* sfpr_timer_create()
{
	int ret;
	sfpr_timer_t *timer;
	sfpr_timer_id_t timer_id;
	//timer_param_t* param = (timer_param_t*)malloc(sizeof(timer_param_t));

	ret = timer_create(CLOCK_REALTIME,NULL,&timer_id);
	if(ret){
		fprintf(stdout, "sfpr_timer_create failed:%d !\n", errno);
		return NULL;
	}

	timer = (sfpr_timer_t*)malloc(sizeof(sfpr_timer_t));
	if(!timer)
	{
		timer_delete(timer_id);
		return NULL;
	}

	memset(timer, 0x0, sizeof(sfpr_timer_t));
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
	
	ret = timer_connect(timer->timerid, timer_handler, (int)timer);
	if(ret)
		fprintf(stdout, "timer_connect failed!\n");	
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

int  sfpr_timer_start(sfpr_timer_t *timer ,int msec, int nloop)
{
	int ret;
	struct itimerspec ts;
	if(!timer)
		return -1;
	ts.it_interval.tv_sec = msec/1000;
	ts.it_interval.tv_nsec = msec%1000*1000000;
	ts.it_value.tv_sec = msec/1000;
	ts.it_value.tv_nsec = msec%1000*1000000;
	timer->nloop = nloop;
	ret = timer_settime(timer->timerid, 0, &ts, NULL);
	return ret;
}

int  sfpr_timer_cancel(sfpr_timer_t *timer)
{
	int ret;
	if(!timer)
		return -1;
	ret = timer_cancel(timer->timerid);
	if(ret)
		printf("timer_cancel failed !!\n");
	return ret;
}


#ifdef __cplusplus
}
#endif

