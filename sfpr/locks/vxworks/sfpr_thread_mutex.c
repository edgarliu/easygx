#include "sfpr_thread_mutex.h"

sfpr_int_t sfpr_mutex_create(sfpr_mutex_t *pMutex, sfpr_int_t para)
{
    sfpr_int_t ret = SFPR_SUCCESS;
    *pMutex = semMCreate(SEM_Q_PRIORITY);
/*
	if(-1 == *pMutex)
		ret = SFPR_ERROR;
*/
    return ret;
}

sfpr_int_t sfpr_mutex_lock(sfpr_mutex_t *pMutex)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = semTake(*pMutex, WAIT_FOREVER);
    
    return ret;
}

sfpr_int_t sfpr_mutex_trylock(sfpr_mutex_t *pMutex)
{
    sfpr_int_t ret = SFPR_SUCCESS;
    ret = semTake(*pMutex, NO_WAIT);
	if((-1 == ret) && (errno == S_objLib_OBJ_UNAVAILABLE))
	{
		ret = SFPR_BUSY;
	}
	return ret;
}

sfpr_int_t sfpr_mutex_unlock(sfpr_mutex_t *pMutex)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    ret = semGive(*pMutex);

    return ret;
}

sfpr_int_t sfpr_mutex_destroy(sfpr_mutex_t *pMutex)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	ret = semDelete(*pMutex);
	return ret;
}

