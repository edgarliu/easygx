
#include "sfpr_global.h"
#include "sfpr_thread_mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

sfpr_int_t sfpr_mutex_create(sfpr_mutex_t *pMutex, sfpr_int_t para)
{
	InitializeCriticalSection(pMutex);
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_mutex_lock(sfpr_mutex_t *pMutex)
{
    EnterCriticalSection(pMutex);
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_mutex_trylock(sfpr_mutex_t *pMutex)
{
    if (!TryEnterCriticalSection(pMutex))
	{
        return SFPR_BUSY;
    }
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_mutex_unlock(sfpr_mutex_t *pMutex)
{
    LeaveCriticalSection(pMutex);
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_mutex_destroy(sfpr_mutex_t *pMutex)
{
    DeleteCriticalSection(pMutex);
    return SFPR_SUCCESS;
}

#ifdef __cplusplus
}
#endif

