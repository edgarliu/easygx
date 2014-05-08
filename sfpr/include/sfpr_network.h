#ifndef SFPR_NETWORK_H
#define SFPR_NETWORK_H
/**
 * @file sfpr_network.h
 * @brief SFPR库的network模块
 * @author nahaitao
 * @version 0.1.0
 * @date 2012-02-7
 */

#include "sfpr_thread_mutex.h"
#include "sfpr_global.h"

#if defined WIN32

#elif defined VXWORKS
#include "socket.h"
#include "types.h"
#include "ioLib.h"
#include "inetLib.h"
#include <netinet/tcp.h> 

typedef int socklen_t; 
#else
#include <sys/types.h>		
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**************************************************************************************************/
/**
 *\defgroup network 网络
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup network_struct 基本数据结构
 *\ingroup network
 **************************************************************************************************/
/**
 *\ingroup network_struct
 * @brief socket shutdown方式
 */
typedef enum
{
	sfpr_sd_receive = 0,
	sfpr_sd_send,
	sfpr_sd_both
}sfpr_sd_type_t;

/**
 *\ingroup network_struct
 * @brief socket协议枚举
 */
typedef enum
{
	sfpr_socket_protocol_tcp, /**<TCP方式*/
	sfpr_socket_protocol_udp  /**<UDP方式*/
}sfpr_socket_protocol_t;

/**
 *\ingroup network_struct
 * @brief 字节序
 */
typedef enum
{
	sfpr_little_end = 1, /**<小端*/
	sfpr_big_end  /**<大端*/
}sfpr_byte_order_t;


/**
 *\ingroup network_struct
 * @brief socket模式枚举
 */
typedef enum
{
	sfpr_socket_mode_client = 0, /**<服务端*/
	sfpr_socket_mode_server  /**<客户端*/
}sfpr_socket_mode_t;

/**
 *\ingroup network_struct
 * @brief socket 阻塞方式；
 */
typedef enum
{
	sfpr_socket_block = 0, /**<阻塞方式*/
	sfpr_socket_noblock /**<非阻塞方式*/
}sfpr_socket_block_t;

/**
 *\ingroup network_struct
 * @brief 互斥标识
 */
#define SFPR_SOCKET_FLAG_MUTEX	0x80000000

/**
 *\ingroup network_struct
 * @brief 描述一个socket结构
 */
typedef struct sfpr_socket_s
{
    sfpr_int_t       					sock; /**<socket文件描述符*/
	sfpr_socket_protocol_t	protocol;/**<socket协议方式；[default]tcp @see sfpr_socket_protocol_t*/
	sfpr_socket_mode_t		mode;/**<socket模式；[default]客户端 @see sfpr_socket_mode_t*/
	sfpr_int_t							timeout;/**<超时时间；ms*/
	sfpr_int_t       					error; /**<socket操作中最后发生错误的错误号*/
	sfpr_uint32_t       				flag;  /**<附加参数，如长连接/短连接*/
	sfpr_socket_block_t       no_block; /**<socket阻塞方式；[default]阻塞方式 @see sfpr_socket_block_t*/
	struct sockaddr_in 			sockaddr;
	sfpr_int_t							sockaddr_len;	
	union{
		struct socket_server_s{
			sfpr_mutex_t 			mutex;/**<互斥锁 @remark 用于服务端发送数据时同步 @see sfpr_mutex_t*/
			sfpr_int_t       			port; /**<服务端监听端口*/
		}server;
		struct socket_client_s{
		  sfpr_char_t      			*server_host;/**<服务端地址*/
			sfpr_int_t       			server_port;/**<服务端监听端口*/
		}client;
	}end;
	sfpr_uint32_t 					user_flag;/**<用户标识*/
}sfpr_socket_t;

/**************************************************************************************************/
/**
 *\defgroup network_base 基本函数
 *\ingroup network
 **************************************************************************************************/

/**
 *\ingroup network_base
 * @brief 创建socket结构，分配内存
 * @param[out] **sock sfpr_socket_t结构体
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_NOMEM 失败
 */
sfpr_int_t sfpr_socket_create(sfpr_socket_t **sock);

/**
 *\ingroup network_base
 * @brief 初始化sfpr_socket_t实例；调用api sock，并且初始化参数协议、模式、地址、端口
 * @param[in] *sock sfpr_socket_t结构体
 * @param[in] host socket服务端地址; 对于服务端模式，可以设置为NULL
 * @param[in] port socket服务端监听端口
 * @param[in] mode socket模式；服务端/客户端
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 */
sfpr_int_t sfpr_socket_init(sfpr_socket_t *sock, sfpr_socket_protocol_t protocol, sfpr_char_t *host, sfpr_int_t port, sfpr_socket_mode_t mode);
/**
 *\ingroup network_base
 * @brief 判断socket是否有效
 * @retval SFPR_SUCCESS 有效
 * @retval SFPR_ERROR 无效
 */
sfpr_int_t sfpr_socket_isvalid(sfpr_socket_t *sock);

/**
 *\ingroup network_base
 * @brief 设置一个标志值，用来表示各种状态，如长连接/短连接
 * @param[in] *sock sfpr_socket_t结构体
 * @param[in] flag 用户附加参数，如长连接/短连接
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_socket_set_flag(sfpr_socket_t *sock, sfpr_int_t flag);

/**
 *\ingroup network_base
 * @brief 获取用户设置的标志字段
 * @param[in] *sock sfpr_socket_t结构体
 * @return 用户设置的标记参数
 */
sfpr_int_t sfpr_socket_get_flag(sfpr_socket_t *sock);

/**
 *\ingroup network_base
 * @brief 创建socket句柄
 * @param[in] *sock sfpr_socket_t结构体
 * @param[in] protocol socket采用的协议方式
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_NOMEM 失败
 */
sfpr_int_t sfpr_socket_socket(sfpr_socket_t *sock, sfpr_socket_protocol_t protocol);

/**
 *\ingroup network_base
 * @brief 销毁socket结构，释放内存
 * @param **sock sfpr_socket_t结构体；销毁后该参数为NULL
 * @retval SFPR_SUCCESS 成功
 */
sfpr_int_t sfpr_socket_destroy(sfpr_socket_t **sock);

/**
 *\ingroup network_base
 * @brief 关闭socket连接
 * @param[in] *sock sfpr_socket_t结构体
 * @param[in] how 关闭方式
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 */
sfpr_int_t sfpr_socket_shutdown(sfpr_socket_t *sock, sfpr_sd_type_t how);

/**************************************************************************************************/
/**
 *\defgroup network_ability 功能函数
 *\ingroup network
 **************************************************************************************************/

/**
 *\ingroup network_ability
 * @brief 关闭socket连接；同时置socket句柄为-1
 * @param[in] *sock sfpr_socket_t结构体
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 */
sfpr_int_t sfpr_socket_close(sfpr_socket_t *sock);

/**
 *\ingroup network_ability
 * @brief 绑定socket连接；用于socket服务端
 * @param[in] *sock sfpr_socket_t结构体
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 */
sfpr_int_t sfpr_socket_bind(sfpr_socket_t *sock);

/**
 *\ingroup network_ability
 * @brief 建立socket监听；用于socket服务端
 * @param[in] *sock sfpr_socket_t结构体
 * @param[in] max_num 同时监听的最大客户端连接数
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 */
sfpr_int_t sfpr_socket_listen(sfpr_socket_t *sock, sfpr_int_t max_num);

/**
 *\ingroup network_ability
 * @brief 受理客户端的连接请求
 * @param[in] *sock 服务端socket
 * @param[in] **sock_client 受理到的客户端连接对应的socket信息
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 如果调用成功，通过参数sock_client返回客户端连接信息，该结构在函数中动态分配内存，用户在使用结束后需要手动销毁
 */
sfpr_int_t sfpr_socket_accept(sfpr_socket_t *sock, sfpr_socket_t **sock_client);

/**
 *\ingroup network_ability
 * @brief 向服务端发起连接请求
 * @param[in] *sock 服务端socket
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 对于阻塞方式，该调用将阻塞，直到连接成功或者超时[该超时为系统默认]
 * @remark 对于非阻塞方式，connect调用返回后需要通过sfpr_socket_wait检测在指定时间内是否可写
 * @remark 如果可写，通过发送0字节数据来检测发起的socket连接是否有效
 */
sfpr_int_t sfpr_socket_connect(sfpr_socket_t *sock);

/**
 *\ingroup network_ability
 * @brief 通过指定socket连接发送用户数据；用于TCP协议的socket连接
 * @param[in] *sock 需要发送数据的socket连接
 * @param[in] *data 需要发送的用户数据
 * @param[in] size 需要发送的用户数据的大小
 * @param[out] *has_send_size 实际发送的数据大小
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 对于阻塞方式，该调用将一直阻塞，直到所有数据发送完成；或者发生错误
 * @remark 对于非阻塞方式，该调用将在发送完所有数据后返回；或者超过指定时间后依然发送失败
 * @remark 接口返回后，实际发送的数据存储在参数*has_send_size中
 */
sfpr_int_t sfpr_socket_send(sfpr_socket_t *sock, const sfpr_char_t *data, sfpr_int_t size, sfpr_int_t *has_send_size);

/**
 *\ingroup network_ability
 * @brief 通过指定socket连接向指定地址发送用户数据；用于UDP协议的socket连接
 * @param[in] *sock 需要发送数据的socket连接
 * @param[in] to 目标地址
 * @param[in] *data 需要发送的用户数据
 * @param[in] size 需要发送的用户数据的大小
 * @param[out] *has_send_size 实际发送的数据大小
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 对于阻塞方式，该调用将一直阻塞，直到所有数据发送完成；或者发生错误
 * @remark 对于非阻塞方式，该调用将在发送完所有数据后返回；或者超过指定时间后依然发送失败
 * @remark 接口返回后，实际发送的数据存储在参数*has_send_size中
 */
sfpr_int_t sfpr_socket_sendto(sfpr_socket_t *sock, struct sockaddr_in *to, const sfpr_char_t *data, sfpr_int_t size, sfpr_int_t *has_send_size);

/**
 *\ingroup network_ability
 * @brief 通过指定socket连接接收用户数据；用于TCP协议的socket连接
 * @param[in] *sock 需要接收数据的socket连接
 * @param[in] *buffer 用来存储接收到的用户数据
 * @param[in] size 接收用户数据的缓冲区的大小
 * @param[out] *real_recv_size 实际接收的数据大小
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 对于阻塞方式，该调用将一直阻塞，直到接收到数据；
 * @remark 对于非阻塞方式，该调用在第一次调用后，如果检测到没有接收数据，则调用poll接口来检测在指定时间内是否有数据可读，如果可读则再次读取，否则返回
 * @remark 对于该接口，参数size为允许用户读取的最大字节数，实际接收的数据存储在参数*real_recv_size中
 */
sfpr_int_t sfpr_socket_recv(sfpr_socket_t *sock, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *real_recv_size);

/**
 *\ingroup network_ability
 * @brief 通过指定socket连接接收指定大小的用户数据；用于TCP协议的socket连接
 * @param[in] *sock 需要接收数据的socket连接
 * @param[in] *buffer 用来存储接收到的用户数据
 * @param[in] size 指定接收的数据大小
 * @param[out] *real_recv_size 实际接收的数据大小
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 对于阻塞模式，如果接收数据没有达到指定大小，则一直阻塞下去
 * @remark 对于非阻塞方式，如果接收数据没有达到指定大小，并且超过设定的超时参数依然没能检测到数据可读，则返回
 * @remark 对于该接口，参数size为要求用户读取的字节数，不过实际读取数据不足该值则认为读取失败，实际接收的数据存储在参数*real_recv_size中
 */
sfpr_int_t sfpr_socket_recv_by_size(sfpr_socket_t *sock, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *real_recv_size);

/**
 *\ingroup network_ability
 * @brief 通过指定socket连接从指定的地址接收用户数据；用于UDP协议的socket连接
 * @param[in] *sock 需要接收数据的socket连接
 * @param[in] *buffer 接收数据的地址源
 * @param[in] *from 用来存储接收到的用户数据
 * @param[in] size 接收用户数据的缓冲区的大小
 * @param[out] *real_recv_size 实际接收的数据大小
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 * @remark 对于阻塞方式，该调用将一直阻塞，直到接收到数据；
 * @remark 对于非阻塞方式，该调用在第一次调用后，如果检测到没有接收数据，则调用poll接口来检测在指定时间内是否有数据可读，如果可读则再次读取，否则返回
 * @remark 对于该接口，参数size为允许用户读取的最大字节数，实际接收的数据存储在参数*real_recv_size中
 */
sfpr_int_t sfpr_socket_recvfrom(sfpr_socket_t *sock, struct sockaddr_in *from, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *real_recv_size);

/**************************************************************************************************/
/**
 *\defgroup network_status 状态设置检测类函数
 *\ingroup network
 **************************************************************************************************/

/**
 *\ingroup network-status
 * @brief 设置socket的参数；该接口只是对api setsockopt的封装
 * @param[in] *sock 需要接收数据的socket连接
 * @param[in] level 参数类型；SOL_SOCKET|IPPROTO_TCP 
 * @param[in] optname 参数名；SO_KEEPALIVE|SO_REUSEADDR|SO_SNDBUF|SO_RCVBUF|SO_LINGER|TCP_NODELAY
 * @param[in] optval 参数值
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 */
sfpr_int_t sfpr_socket_opt_set(sfpr_socket_t *sock, sfpr_int_t level, sfpr_int_t optname, void *optval, sfpr_int_t optlen);

/**
 *\ingroup network-status
 * @brief 获取socket的参数；该接口只是对api setsockopt的封装
 * @param[in] *sock 需要接收数据的socket连接
 * @param[in] level 参数类型
 * @param[in] optname 参数名
 * @param[out] optval 参数值
 * @param[out] optlen 参数值大小
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 */
sfpr_int_t sfpr_socket_opt_get(sfpr_socket_t *sock, sfpr_int_t level, sfpr_int_t optname, void *optval, sfpr_int_t *optlen);

/**
 *\ingroup network-status
 * @brief 设置socket的超时时间
 * @param[in] *sock 需要接收数据的socket连接
 * @param[in] timeout 超时时间值；ms
 * @retval SFPR_SUCCESS 成功
 */
sfpr_int_t sfpr_socket_timeout_set(sfpr_socket_t *sock, sfpr_uint32_t timeout);

/**
 *\ingroup network-status
 * @brief 设置socket为非阻塞模式
 * @param[in] *sock 需要接收数据的socket连接
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 * @remark 如果返回失败，错误号存储在sock中的字段error中
 */
sfpr_int_t sfpr_socket_nonblock_set(sfpr_socket_t *sock);

/**
 *\ingroup network-status
 * @brief 获取主机名
 * @param[in] *host_name 接收主机名的缓冲
 * @param[in] size 接收主机名的缓冲大小
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_socket_hostname_get(sfpr_char_t *host_name, sfpr_int_t size);

/**
 *\ingroup network-status
 * @brief 该接口用来检测指定的socket连接上是否可读/写
 * @param[in] *sock 需要接收数据的socket连接
 * @param[in] for_read 1:检测socket是否有可读数据；0:检测socket可写
 * @retval SFPR_SUCCESS 检测成功; 
 * @retval SFPR_TIMEOUT 超时
 * @retval 其他非0值 poll内部errno
 */
sfpr_int_t sfpr_socket_wait(sfpr_socket_t *sock, sfpr_int_t for_read);


sfpr_char_t *sfpr_iptos(unsigned long in);

unsigned short sfpr_htons(unsigned short inval);
unsigned long sfpr_htonl(unsigned long inval);

sfpr_int_t sfpr_byte_order_check();

sfpr_int_t sfpr_socket_start_up();
sfpr_int_t sfpr_socket_clean_up();
#ifdef __cplusplus
}
#endif

#endif
