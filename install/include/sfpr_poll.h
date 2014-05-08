#ifndef SFPR_POLL_H
#define SFPR_POLL_H
/**
 * @file sfpr_poll.h
 * @brief SFPR库的poll模块
 * @author nahaitao
 * @version 0.1.0
 * @date 2012-02-7
 */

#include <string.h>

#include "sfpr_global.h"
#include "sfpr_network.h"

#if defined WIN32

#elif defined VXWORKS
#include "selectLib.h"
#else
#include <sys/select.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**************************************************************************************************/
/**
 *\defgroup poll 多路复用
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup poll_struct 基本数据结构
 *\ingroup poll
 **************************************************************************************************/

/**
 * Poll 选项
 */
#define SFPR_POLLIN    0x001     /**<检测有数据可读*/
#define SFPR_POLLPRI   0x002     /**<Priority data available*/
#define SFPR_POLLOUT   0x004     /**<检测可写*/
#define SFPR_POLLERR   0x010     /**<Pending error*/
#define SFPR_POLLHUP   0x020     /**<Hangup occurred*/
#define SFPR_POLLNVAL  0x040     /**<Descriptor invalid*/

#define SFPR_MAX_REQ_FD 100

/**
 *\ingroup poll_struct 
 * @brief Poll中要检测的socket连接的信息
 */
typedef struct sfpr_pollfd_s {
    sfpr_int_t 		    event;      /**< 事件类型，通常为SFPR_POLLIN或SFPR_POLLOUT*/
    sfpr_socket_t 	*sock;            /**<待检测的socket连接信息 @see  sfpr_socket_t*/
    void       		    *user_data;          /**<用户数据*/
}sfpr_pollfd_t;

/**
 *\ingroup poll_struct
 * @brief poll结构体；当前只实现了select方式
 */
typedef struct sfpr_poll_s sfpr_poll_t;

/**************************************************************************************************/
/**
 *\defgroup poll_base 基本函数
 *\ingroup poll
 **************************************************************************************************/

/**
 *\ingroup poll_base
 * @brief 创建一个pollset对象，主要是分配内存，初始化各字段
 * @param[in] *pollset   sfpr_pollset_t结构体
 * @param[in] size 允许检测的最大文件描述符个数
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_NOMEM 内存分配失败
 */
sfpr_int_t sfpr_poll_create(sfpr_poll_t **pollset, sfpr_int_t size);

/**
 *\ingroup poll_base
 * @brief 销毁一个pollset对象
 * @param[in] *pollset sfpr_pollset_t结构体
 * @retval SFPR_SUCCESS 成功
 */
sfpr_int_t sfpr_poll_destroy(sfpr_poll_t **pollset);

/**************************************************************************************************/
/**
 *\defgroup poll_ability 功能函数
 *\ingroup poll
 **************************************************************************************************/

/**
 *\ingroup poll_ability
 * @brief 向pollset对象中添加一个待检测的文件描述符信息
 * @param[in] *pollset 用于检测的poll对象
 * @param[in] *descriptor 需要检测的文件描述符信息
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_poll_add(sfpr_poll_t *pollset, const sfpr_pollfd_t *descriptor);

/**
 *\ingroup poll_ability
 * @brief 从pollset对象中移除一个的文件描述符信息
 * @param[in] pollset 用于检测的poll对象
 * @param[in] descriptor 需要移除的文件描述符信息
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_poll_remove(sfpr_poll_t *pollset, const sfpr_pollfd_t *descriptor);

/**************************************************************************************************/
/**
 *\defgroup poll_status 状态类函数
 *\ingroup poll
 **************************************************************************************************/

/**
 *\ingroup poll_status
 * @brief 检测在指定的时间内，是否有事件发生；并且将触发事件的文件描述符信息通过参数descriptors返回
 * @param[in] *pollset 用于检测的poll对象
 * @param[in] timeout 超时时间；ms；0:不阻塞，立即返回；-1:阻塞，直到有事件发生；大于0:超时的毫秒数
 * @param[out] *num 检测到的事件个数
 * @param[out] **descriptors 检测到的文件描述符信息
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_TIMEOUT 超时
 * @retval 其他非0值 错误号
 */
sfpr_int_t sfpr_poll_wait(sfpr_poll_t *pollset, sfpr_int_t timeout, sfpr_int_t *num, sfpr_pollfd_t **descriptors);


#ifdef __cplusplus
}
#endif

#endif
