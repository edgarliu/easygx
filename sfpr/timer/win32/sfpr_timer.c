#include "sfpr_timer.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

sfpr_timer_t* sfpr_timer_create()
{
	sfpr_timer_id_t timer_id;
	sfpr_timer_t *timer;
	int ret;

	timer = (sfpr_timer_t*)malloc(sizeof(sfpr_timer_t));
	if(!timer)
	{
		return NULL;
	}
	memset(timer, 0x0, sizeof(sfpr_timer_t));
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
	free(_timer);
	return ret;
}

int  sfpr_timer_start(sfpr_timer_t *timer, int msec, int nloop)
{
	int ret ;
	if(!timer)
		return -1;

	return ret;
}

int  sfpr_timer_cancel(sfpr_timer_t *timer)
{
	int ret =SFPR_ERROR;
	if(!timer)
		return -1;
	return ret;
}

#ifdef __cplusplus
}
#endif

