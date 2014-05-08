#include "sfpr_thread_mutex.h"
#include "sfpr_thread_cond.h"

#include <limits.h>

sfpr_int_t sfpr_cond_create(sfpr_cond_t *cond)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	if(!cond)
		return -1;
	//自动方式，实现单播条件触发
    cond->semaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
    if (cond->semaphore == NULL) 
        return -1;

    InitializeCriticalSection(&cond->csection);
    return ret;
}

static int _cond_timedwait(sfpr_cond_t *cond, sfpr_mutex_t *mutex, DWORD timeout_ms)
{
    int res = 0, rv=0;
    unsigned int wake = 0;
    unsigned long generation;

    EnterCriticalSection(&cond->csection);
    cond->num_waiting++;
    generation = cond->generation;
    LeaveCriticalSection(&cond->csection);

    sfpr_mutex_unlock(mutex);

    do {
        res = WaitForSingleObject(cond->semaphore, timeout_ms);
        EnterCriticalSection(&cond->csection);
        if (cond->num_wake) {
            if (cond->generation != generation) {
                cond->num_wake--;
                cond->num_waiting--;
                rv = SFPR_SUCCESS;
                break;
            } else 
                wake = 1;
        }
        else if (res != WAIT_OBJECT_0) {
            cond->num_waiting--;
            rv = SFPR_TIMEOUT;
            break;
        }

        LeaveCriticalSection(&cond->csection);

        if (wake) {
            wake = 0;
            ReleaseSemaphore(cond->semaphore, 1, NULL);
        }
    } while (1);

    LeaveCriticalSection(&cond->csection);
    sfpr_mutex_lock(mutex);

    return rv;
}

sfpr_int_t sfpr_cond_wait(sfpr_cond_t *cond, sfpr_mutex_t *mutex)
{
    return _cond_timedwait(cond, mutex, INFINITE);
}

sfpr_int_t sfpr_cond_timedwait(sfpr_cond_t *cond, sfpr_mutex_t *mutex, sfpr_int_t timeout)
{
    DWORD timeout_ms = (DWORD)timeout;
    return _cond_timedwait(cond, mutex, timeout_ms);
}

sfpr_int_t sfpr_cond_signal(sfpr_cond_t *cond)
{
    unsigned int wake = 0;
    EnterCriticalSection(&cond->csection);
    if (cond->num_waiting > cond->num_wake) {
        wake = 1;
        cond->num_wake++;
        cond->generation++;
    }
    LeaveCriticalSection(&cond->csection);

    if (wake)
        ReleaseSemaphore(cond->semaphore, 1, NULL);
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_cond_signal_all(sfpr_cond_t *cond)
{
    unsigned long num_wake = 0;
    EnterCriticalSection(&cond->csection);
    if (cond->num_waiting > cond->num_wake) {
        num_wake = cond->num_waiting - cond->num_wake;
        cond->num_wake = cond->num_waiting;
        cond->generation++;
    }
    LeaveCriticalSection(&cond->csection);
    if (num_wake)
        ReleaseSemaphore(cond->semaphore, num_wake, NULL);
    return SFPR_SUCCESS;
}
sfpr_int_t sfpr_cond_destroy(sfpr_cond_t *cond)
{
	if(!cond)
		return -1;
    CloseHandle(cond->semaphore);
    DeleteCriticalSection(&cond->csection);
    return SFPR_SUCCESS;
}

