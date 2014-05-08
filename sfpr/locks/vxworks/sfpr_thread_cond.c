#include "sfpr_thread_mutex.h"
#include "sfpr_thread_cond.h"

sfpr_int_t sfpr_cond_create(sfpr_cond_t *cond)
{
    sfpr_int_t ret = SFPR_SUCCESS;
    *cond = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
/*
	if(-1 == *cond)
		ret = SFPR_ERROR;
*/
    return ret;
}

sfpr_int_t sfpr_cond_wait(sfpr_cond_t *cond, sfpr_mutex_t *mutex)
{
    sfpr_int_t ret = SFPR_SUCCESS;
    sfpr_mutex_unlock(mutex);
    ret = semTake(*cond, WAIT_FOREVER);
    sfpr_mutex_lock(mutex);
    return ret;
}

sfpr_int_t sfpr_cond_timedwait(sfpr_cond_t *cond, sfpr_mutex_t *mutex, sfpr_int_t timeout)
{
    sfpr_int_t ret = SFPR_SUCCESS;
	sfpr_int_t tick;
	tick = timeout*60/1000;
    sfpr_mutex_unlock(mutex);
    ret = semTake(*cond, tick);
    sfpr_mutex_lock(mutex);
	if((-1 == ret) && (errno == S_objLib_OBJ_TIMEOUT))
	{
		ret = SFPR_TIMEOUT;
	}
    return ret;
}


sfpr_int_t sfpr_cond_signal(sfpr_cond_t *cond)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = semGive(*cond);
    return ret;
}

sfpr_int_t sfpr_cond_signal_all(sfpr_cond_t *cond)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = semFlush(*cond);
    return ret;
}

sfpr_int_t sfpr_cond_destroy(sfpr_cond_t *cond)
{
	sfpr_int_t ret = SFPR_SUCCESS;

    ret = semDelete(*cond);
	return ret;
}

