#include "sfpr_thread_mutex.h"
#include "sfpr_thread_cond.h"

#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

sfpr_int_t sfpr_cond_create(sfpr_cond_t *cond)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = pthread_cond_init(cond, NULL);
    return ret;
}

sfpr_int_t sfpr_cond_wait(sfpr_cond_t *cond, sfpr_mutex_t *mutex)
{
    sfpr_int_t ret;

    ret = pthread_cond_wait(cond, mutex);
    return ret;
}

sfpr_int_t sfpr_cond_timedwait(sfpr_cond_t *cond, sfpr_mutex_t *mutex, sfpr_int_t timeout)
{
    //struct timeval abstime;
    sfpr_int_t ret = SFPR_SUCCESS;
	struct timespec abstime;

    abstime.tv_sec = time(0) + timeout/1000;
//    abstime.tv_nsec = timeout%1000 * 1000 * 1000;
    abstime.tv_nsec = timeout%1000* 1000 * 1000;

    ret = pthread_cond_timedwait(cond, mutex, &abstime);
    if (ETIMEDOUT == ret) 
	{
        return SFPR_TIMEOUT;
    }
    return ret;
}


sfpr_int_t sfpr_cond_signal(sfpr_cond_t *cond)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = pthread_cond_signal(cond);
    return ret;
}

sfpr_int_t sfpr_cond_signal_all(sfpr_cond_t *cond)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = pthread_cond_broadcast(cond);
    return ret;
}

sfpr_int_t sfpr_cond_destroy(sfpr_cond_t *cond)
{
    sfpr_int_t ret = SFPR_SUCCESS;

	ret = pthread_cond_destroy(cond);
	return ret;
}

