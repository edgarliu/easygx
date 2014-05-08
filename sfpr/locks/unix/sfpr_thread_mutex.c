#include "sfpr_thread_mutex.h"


sfpr_int_t sfpr_mutex_create(sfpr_mutex_t *pMutex, sfpr_int_t para)
{
    sfpr_int_t ret = SFPR_SUCCESS;
    ret = pthread_mutex_init(pMutex, NULL);

    return ret;
}

sfpr_int_t sfpr_mutex_lock(sfpr_mutex_t *pMutex)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = pthread_mutex_lock(pMutex);
    
    return ret;
}

sfpr_int_t sfpr_mutex_trylock(sfpr_mutex_t *pMutex)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = pthread_mutex_trylock(pMutex);
	
    if (EBUSY == ret) 
	{
        return SFPR_BUSY;
    }
	return SFPR_SUCCESS;
}

sfpr_int_t sfpr_mutex_unlock(sfpr_mutex_t *pMutex)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = pthread_mutex_unlock(pMutex);

    return ret;
}

sfpr_int_t sfpr_mutex_destroy(sfpr_mutex_t *pMutex)
{
    sfpr_int_t ret = SFPR_SUCCESS;
    sfpr_mutex_lock(pMutex);
    sfpr_mutex_unlock(pMutex);
	ret = pthread_mutex_destroy(pMutex);
	return ret;
}

