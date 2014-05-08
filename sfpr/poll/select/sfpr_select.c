#include "sfpr_poll.h"

/**
 *\ingroup poll_struct
 * @brief poll结构体
 */
struct sfpr_poll_s{
    sfpr_int_t max_size; /**<允许检测的最大文件描述符个数*/
    sfpr_int_t max_fd; /**<当前检测的文件描述符中的最大值*/
    sfpr_int_t cur_num; /**<当前待检测文件描述符个数*/
    sfpr_pollfd_t *query_set;/**<待检测文件描述符对应的参数*/
    sfpr_pollfd_t *result_set;/**<检测到的文件描述符对应的参数*/
    fd_set readset;/**<检测可读的文件描述符集合*/
    fd_set writeset;/**<检测可写的文件描述符集合*/
    fd_set exceptset;/**<检测异常的文件描述符集合*/
};

sfpr_int_t sfpr_poll_create(sfpr_poll_t **pollset, sfpr_int_t size)
{
    sfpr_int_t ret = SFPR_SUCCESS;

    sfpr_poll_t *_poll = malloc(sizeof(sfpr_poll_t));
    if(_poll == NULL){
	    return SFPR_NOMEM;
    }
    memset(_poll, 0x0, sizeof(sfpr_poll_t));
    _poll->max_size = size;
    _poll->max_fd = 0;
    _poll->cur_num = 0;

    _poll->query_set = malloc(size * sizeof(sfpr_pollfd_t));
    memset(_poll->query_set, 0x0, size * sizeof(sfpr_pollfd_t));
    _poll->result_set = malloc(size * sizeof(sfpr_pollfd_t));
    memset(_poll->result_set, 0x0, size * sizeof(sfpr_pollfd_t));

    FD_ZERO(&(_poll->readset));
    FD_ZERO(&(_poll->writeset));
    FD_ZERO(&(_poll->exceptset));

    *pollset = _poll;

    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_poll_destroy(sfpr_poll_t **pollset)
{
    sfpr_poll_t *_poll = *pollset;
    if(_poll == NULL)
	    return SFPR_SUCCESS;
    free(_poll->query_set);
    free(_poll->result_set);

    free(_poll);

    *pollset = NULL;
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_poll_add(sfpr_poll_t *pollset, const sfpr_pollfd_t *descriptor)
{
    sfpr_int_t fd;

    if (pollset->cur_num >= SFPR_MAX_REQ_FD){
        return SFPR_ERROR;
    }

    pollset->query_set[pollset->cur_num] = *descriptor;

    fd = descriptor->sock->sock;

    if (descriptor->event & SFPR_POLLIN) {
        FD_SET(fd, &(pollset->readset));
    }
    if (descriptor->event & SFPR_POLLOUT){
        FD_SET(fd, &(pollset->writeset));
    }
    if (descriptor->event &
        (SFPR_POLLPRI | SFPR_POLLERR | SFPR_POLLHUP | SFPR_POLLNVAL)) {
        FD_SET(fd, &(pollset->exceptset));
    }
    if (fd > pollset->max_fd){
        pollset->max_fd = fd;
    }
    pollset->cur_num++;
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_poll_remove(sfpr_poll_t * pollset, const sfpr_pollfd_t * descriptor)
{
    sfpr_int_t i;
    sfpr_int_t fd;

    fd = descriptor->sock->sock;

    for (i = 0; i < pollset->cur_num; i++){
	if (descriptor->sock == pollset->query_set[i].sock){
	    sfpr_int_t dst = i;
	    sfpr_int_t old_nelts = pollset->cur_num;
	    pollset->cur_num--;
	    for (i++; i < old_nelts; i++){
		if (descriptor->sock == pollset->query_set[i].sock){
		    pollset->cur_num--;
		}
		else{
		    pollset->query_set[dst] = pollset->query_set[i];
		    dst++;
		}
	    }
	    FD_CLR(fd, &(pollset->readset));
	    FD_CLR(fd, &(pollset->writeset));
	    FD_CLR(fd, &(pollset->exceptset));
	    if ((fd == pollset->max_fd) && (pollset->max_fd > 0)){
		pollset->max_fd--;
	    }
	    return SFPR_SUCCESS;
	}
    }
    return SFPR_ERROR;
}

sfpr_int_t sfpr_poll_wait(sfpr_poll_t *pollset, sfpr_int_t timeout, sfpr_int_t *num, sfpr_pollfd_t **descriptors)
{
    sfpr_int_t rs;
    sfpr_int_t i, j=0;
    struct timeval tv, *tvptr;
    fd_set readset, writeset, exceptset;
    sfpr_int_t rv = SFPR_ERROR;

    if (timeout < 0) {
        tvptr = NULL;
    }
    else{
        tv.tv_sec = timeout/1000;
        tv.tv_usec = timeout%1000*1000;
        tvptr = &tv;
    }

    #ifdef WIN32
    if(0 == pollset->cur_num){
	sfpr_time_msleep(timeout);
	return SFPR_TIMEOUT;
    }
    #endif

    memcpy(&readset, &(pollset->readset), sizeof(fd_set));
    memcpy(&writeset, &(pollset->writeset), sizeof(fd_set));
    memcpy(&exceptset, &(pollset->exceptset), sizeof(fd_set));

    rs = select(pollset->max_fd + 1, &readset, &writeset, &exceptset, tvptr);

    if (rs < 0) {
	#ifdef WIN32
	return -WSAGetLastError();
	#else
        return -errno;
	#endif
    }
    else if (rs == 0) 
        return SFPR_TIMEOUT;
    memset(pollset->result_set,0,pollset->max_size * sizeof(sfpr_pollfd_t));
    for (i = 0; i < pollset->cur_num; i++) {
        sfpr_int_t fd;
        fd = pollset->query_set[i].sock->sock;
	    if (FD_ISSET(fd, &readset) || FD_ISSET(fd, &writeset) ||
	        FD_ISSET(fd, &exceptset)) {
	        pollset->result_set[j] = pollset->query_set[i];
	        pollset->result_set[j].event = 0;
			
	        if (FD_ISSET(fd, &readset)) {
	            pollset->result_set[j].event |= SFPR_POLLIN;
	        }
	        if (FD_ISSET(fd, &writeset)) {
	            pollset->result_set[j].event |= SFPR_POLLOUT;
	        }
	        if (FD_ISSET(fd, &exceptset)) {
	            pollset->result_set[j].event |= SFPR_POLLERR;
	        }
	        j++;
	    }
    }

    if (((*num) = j) != 0)
        rv = SFPR_SUCCESS;

    if (descriptors)
        *descriptors = pollset->result_set;
    return rv;
}

sfpr_int_t sfpr_poll_get_count(sfpr_poll_t * pollset)
{
    return pollset->cur_num;
}

sfpr_int_t sfpr_poll_check(sfpr_poll_t *pollset)
{
    int i;

    for (i = 0; i < pollset->cur_num; i++){
	printf("sfpr_poll_check() %d sock:%d\n",i,pollset->query_set[i].sock->sock);
    }
    return 0;
}
 

