#include "sfpr_poll.h"

#include <sys/epoll.h>

struct sfpr_poll_s{
    sfpr_int_t max_size; /**<允许检测的最大文件描述符个数*/
    sfpr_int_t max_fd; /**<当前检测的文件描述符中的最大值*/
    sfpr_int_t cur_num; /**<当前待检测文件描述符个数*/
    sfpr_pollfd_t *query_set;/**<待检测文件描述符对应的参数*/
    sfpr_pollfd_t *result_set;/**<检测到的文件描述符对应的参数*/
    int epfd;
    struct epoll_event *events;
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
    _poll->query_set = malloc(10240 * sizeof(sfpr_pollfd_t));
    memset(_poll->query_set, 0x0, 10240 * sizeof(sfpr_pollfd_t));
    _poll->result_set = malloc(size * sizeof(sfpr_pollfd_t));
    memset(_poll->result_set, 0x0, size * sizeof(sfpr_pollfd_t));

    _poll->events = malloc(size * sizeof(struct epoll_event));
    memset(_poll->events, 0x0, size * sizeof(struct epoll_event));
    _poll->epfd = epoll_create(size);
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
    free(_poll->events);
	close(_poll->epfd);
    free(_poll);
    *pollset = NULL;
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_poll_add(sfpr_poll_t *pollset, const sfpr_pollfd_t *descriptor)
{
    sfpr_int_t fd;
    struct epoll_event ee;
 
    fd = descriptor->sock->sock;
 
    memcpy(&(pollset->query_set[fd]),descriptor,sizeof(sfpr_pollfd_t));

    memset(&ee,0,sizeof(struct epoll_event));
    ee.data.u64 = 0; /* avoid valgrind warning */
    ee.data.fd = fd;
    if (descriptor->event & SFPR_POLLIN) {
       ee.events |= EPOLLIN;
    }
    if (descriptor->event & SFPR_POLLOUT){
       ee.events |= EPOLLOUT;
    }

    if (epoll_ctl(pollset->epfd,EPOLL_CTL_ADD,fd,&ee) == -1){
		if (errno == EEXIST){
			if (epoll_ctl(pollset->epfd,EPOLL_CTL_MOD,fd,&ee) == -1){
				return SFPR_ERROR;
			}
		}else{
			return SFPR_ERROR;
		}
	}
    if (fd > pollset->max_fd){
        pollset->max_fd = fd;
    }
    pollset->cur_num++;
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_poll_remove(sfpr_poll_t * pollset, const sfpr_pollfd_t * descriptor)
{
    sfpr_int_t fd;
    struct epoll_event ee;
 
    fd = descriptor->sock->sock;
 
    memset(&ee,0,sizeof(struct epoll_event));
    ee.data.u64 = 0; /* avoid valgrind warning */
    ee.data.fd = fd;
    if (descriptor->event & SFPR_POLLIN) {
       ee.events |= EPOLLIN;
    }
    if (descriptor->event & SFPR_POLLOUT){
       ee.events |= EPOLLOUT;
    }

    if (epoll_ctl(pollset->epfd,EPOLL_CTL_DEL,fd,&ee) == -1){
	return SFPR_ERROR;
    }
    memset(&(pollset->query_set[fd]),0,sizeof(sfpr_pollfd_t));	
    pollset->cur_num--;
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_poll_get_count(sfpr_poll_t * pollset)
{
    return pollset->cur_num;
}

sfpr_int_t sfpr_poll_wait(sfpr_poll_t *pollset, sfpr_int_t timeout, sfpr_int_t *num, sfpr_pollfd_t **descriptors)
{
    struct timeval tv;
    int retval = 0;

    retval = epoll_wait(pollset->epfd,pollset->events,pollset->max_size,timeout);
    if(retval > 0){
	int j;
    for (j = 0; j < retval; j++) {
	struct epoll_event *e = pollset->events+j;
	int fd = 0,mask = 0;
	if (e->events & EPOLLIN) mask |= SFPR_POLLIN;
	if (e->events & EPOLLOUT) mask |= SFPR_POLLOUT;
	if (e->events & EPOLLERR) mask |= SFPR_POLLOUT;
	if (e->events & EPOLLHUP) mask |= SFPR_POLLOUT;
	    fd = e->data.fd;
	    memcpy(&(pollset->result_set[j]),&(pollset->query_set[fd]),sizeof(sfpr_pollfd_t));
	    pollset->result_set[j].event = mask;
	}
    }
    if(num){
	*num = retval;		
    }
    if(retval == 0){
	return SFPR_TIMEOUT;
    }
    if (descriptors)
        *descriptors = pollset->result_set;	
	return SFPR_SUCCESS;
}

sfpr_int_t sfpr_poll_check(sfpr_poll_t *pollset)
{
    return 0;
}
 
