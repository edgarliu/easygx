#ifndef SFPR_NETWORK_H
#define SFPR_NETWORK_H
/**
 * @file sfpr_network.h
 * @brief SFPR���networkģ��
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
 *\defgroup network ����
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup network_struct �������ݽṹ
 *\ingroup network
 **************************************************************************************************/
/**
 *\ingroup network_struct
 * @brief socket shutdown��ʽ
 */
typedef enum
{
	sfpr_sd_receive = 0,
	sfpr_sd_send,
	sfpr_sd_both
}sfpr_sd_type_t;

/**
 *\ingroup network_struct
 * @brief socketЭ��ö��
 */
typedef enum
{
	sfpr_socket_protocol_tcp, /**<TCP��ʽ*/
	sfpr_socket_protocol_udp  /**<UDP��ʽ*/
}sfpr_socket_protocol_t;

/**
 *\ingroup network_struct
 * @brief �ֽ���
 */
typedef enum
{
	sfpr_little_end = 1, /**<С��*/
	sfpr_big_end  /**<���*/
}sfpr_byte_order_t;


/**
 *\ingroup network_struct
 * @brief socketģʽö��
 */
typedef enum
{
	sfpr_socket_mode_client = 0, /**<�����*/
	sfpr_socket_mode_server  /**<�ͻ���*/
}sfpr_socket_mode_t;

/**
 *\ingroup network_struct
 * @brief socket ������ʽ��
 */
typedef enum
{
	sfpr_socket_block = 0, /**<������ʽ*/
	sfpr_socket_noblock /**<��������ʽ*/
}sfpr_socket_block_t;

/**
 *\ingroup network_struct
 * @brief �����ʶ
 */
#define SFPR_SOCKET_FLAG_MUTEX	0x80000000

/**
 *\ingroup network_struct
 * @brief ����һ��socket�ṹ
 */
typedef struct sfpr_socket_s
{
    sfpr_int_t       					sock; /**<socket�ļ�������*/
	sfpr_socket_protocol_t	protocol;/**<socketЭ�鷽ʽ��[default]tcp @see sfpr_socket_protocol_t*/
	sfpr_socket_mode_t		mode;/**<socketģʽ��[default]�ͻ��� @see sfpr_socket_mode_t*/
	sfpr_int_t							timeout;/**<��ʱʱ�䣻ms*/
	sfpr_int_t       					error; /**<socket���������������Ĵ����*/
	sfpr_uint32_t       				flag;  /**<���Ӳ������糤����/������*/
	sfpr_socket_block_t       no_block; /**<socket������ʽ��[default]������ʽ @see sfpr_socket_block_t*/
	struct sockaddr_in 			sockaddr;
	sfpr_int_t							sockaddr_len;	
	union{
		struct socket_server_s{
			sfpr_mutex_t 			mutex;/**<������ @remark ���ڷ���˷�������ʱͬ�� @see sfpr_mutex_t*/
			sfpr_int_t       			port; /**<����˼����˿�*/
		}server;
		struct socket_client_s{
		  sfpr_char_t      			*server_host;/**<����˵�ַ*/
			sfpr_int_t       			server_port;/**<����˼����˿�*/
		}client;
	}end;
	sfpr_uint32_t 					user_flag;/**<�û���ʶ*/
}sfpr_socket_t;

/**************************************************************************************************/
/**
 *\defgroup network_base ��������
 *\ingroup network
 **************************************************************************************************/

/**
 *\ingroup network_base
 * @brief ����socket�ṹ�������ڴ�
 * @param[out] **sock sfpr_socket_t�ṹ��
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_NOMEM ʧ��
 */
sfpr_int_t sfpr_socket_create(sfpr_socket_t **sock);

/**
 *\ingroup network_base
 * @brief ��ʼ��sfpr_socket_tʵ��������api sock�����ҳ�ʼ������Э�顢ģʽ����ַ���˿�
 * @param[in] *sock sfpr_socket_t�ṹ��
 * @param[in] host socket����˵�ַ; ���ڷ����ģʽ����������ΪNULL
 * @param[in] port socket����˼����˿�
 * @param[in] mode socketģʽ�������/�ͻ���
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 */
sfpr_int_t sfpr_socket_init(sfpr_socket_t *sock, sfpr_socket_protocol_t protocol, sfpr_char_t *host, sfpr_int_t port, sfpr_socket_mode_t mode);
/**
 *\ingroup network_base
 * @brief �ж�socket�Ƿ���Ч
 * @retval SFPR_SUCCESS ��Ч
 * @retval SFPR_ERROR ��Ч
 */
sfpr_int_t sfpr_socket_isvalid(sfpr_socket_t *sock);

/**
 *\ingroup network_base
 * @brief ����һ����־ֵ��������ʾ����״̬���糤����/������
 * @param[in] *sock sfpr_socket_t�ṹ��
 * @param[in] flag �û����Ӳ������糤����/������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_socket_set_flag(sfpr_socket_t *sock, sfpr_int_t flag);

/**
 *\ingroup network_base
 * @brief ��ȡ�û����õı�־�ֶ�
 * @param[in] *sock sfpr_socket_t�ṹ��
 * @return �û����õı�ǲ���
 */
sfpr_int_t sfpr_socket_get_flag(sfpr_socket_t *sock);

/**
 *\ingroup network_base
 * @brief ����socket���
 * @param[in] *sock sfpr_socket_t�ṹ��
 * @param[in] protocol socket���õ�Э�鷽ʽ
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_NOMEM ʧ��
 */
sfpr_int_t sfpr_socket_socket(sfpr_socket_t *sock, sfpr_socket_protocol_t protocol);

/**
 *\ingroup network_base
 * @brief ����socket�ṹ���ͷ��ڴ�
 * @param **sock sfpr_socket_t�ṹ�壻���ٺ�ò���ΪNULL
 * @retval SFPR_SUCCESS �ɹ�
 */
sfpr_int_t sfpr_socket_destroy(sfpr_socket_t **sock);

/**
 *\ingroup network_base
 * @brief �ر�socket����
 * @param[in] *sock sfpr_socket_t�ṹ��
 * @param[in] how �رշ�ʽ
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 */
sfpr_int_t sfpr_socket_shutdown(sfpr_socket_t *sock, sfpr_sd_type_t how);

/**************************************************************************************************/
/**
 *\defgroup network_ability ���ܺ���
 *\ingroup network
 **************************************************************************************************/

/**
 *\ingroup network_ability
 * @brief �ر�socket���ӣ�ͬʱ��socket���Ϊ-1
 * @param[in] *sock sfpr_socket_t�ṹ��
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 */
sfpr_int_t sfpr_socket_close(sfpr_socket_t *sock);

/**
 *\ingroup network_ability
 * @brief ��socket���ӣ�����socket�����
 * @param[in] *sock sfpr_socket_t�ṹ��
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 */
sfpr_int_t sfpr_socket_bind(sfpr_socket_t *sock);

/**
 *\ingroup network_ability
 * @brief ����socket����������socket�����
 * @param[in] *sock sfpr_socket_t�ṹ��
 * @param[in] max_num ͬʱ���������ͻ���������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 */
sfpr_int_t sfpr_socket_listen(sfpr_socket_t *sock, sfpr_int_t max_num);

/**
 *\ingroup network_ability
 * @brief ����ͻ��˵���������
 * @param[in] *sock �����socket
 * @param[in] **sock_client �����Ŀͻ������Ӷ�Ӧ��socket��Ϣ
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark ������óɹ���ͨ������sock_client���ؿͻ���������Ϣ���ýṹ�ں����ж�̬�����ڴ棬�û���ʹ�ý�������Ҫ�ֶ�����
 */
sfpr_int_t sfpr_socket_accept(sfpr_socket_t *sock, sfpr_socket_t **sock_client);

/**
 *\ingroup network_ability
 * @brief �����˷�����������
 * @param[in] *sock �����socket
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark ����������ʽ���õ��ý�������ֱ�����ӳɹ����߳�ʱ[�ó�ʱΪϵͳĬ��]
 * @remark ���ڷ�������ʽ��connect���÷��غ���Ҫͨ��sfpr_socket_wait�����ָ��ʱ�����Ƿ��д
 * @remark �����д��ͨ������0�ֽ���������ⷢ���socket�����Ƿ���Ч
 */
sfpr_int_t sfpr_socket_connect(sfpr_socket_t *sock);

/**
 *\ingroup network_ability
 * @brief ͨ��ָ��socket���ӷ����û����ݣ�����TCPЭ���socket����
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] *data ��Ҫ���͵��û�����
 * @param[in] size ��Ҫ���͵��û����ݵĴ�С
 * @param[out] *has_send_size ʵ�ʷ��͵����ݴ�С
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark ����������ʽ���õ��ý�һֱ������ֱ���������ݷ�����ɣ����߷�������
 * @remark ���ڷ�������ʽ���õ��ý��ڷ������������ݺ󷵻أ����߳���ָ��ʱ�����Ȼ����ʧ��
 * @remark �ӿڷ��غ�ʵ�ʷ��͵����ݴ洢�ڲ���*has_send_size��
 */
sfpr_int_t sfpr_socket_send(sfpr_socket_t *sock, const sfpr_char_t *data, sfpr_int_t size, sfpr_int_t *has_send_size);

/**
 *\ingroup network_ability
 * @brief ͨ��ָ��socket������ָ����ַ�����û����ݣ�����UDPЭ���socket����
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] to Ŀ���ַ
 * @param[in] *data ��Ҫ���͵��û�����
 * @param[in] size ��Ҫ���͵��û����ݵĴ�С
 * @param[out] *has_send_size ʵ�ʷ��͵����ݴ�С
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark ����������ʽ���õ��ý�һֱ������ֱ���������ݷ�����ɣ����߷�������
 * @remark ���ڷ�������ʽ���õ��ý��ڷ������������ݺ󷵻أ����߳���ָ��ʱ�����Ȼ����ʧ��
 * @remark �ӿڷ��غ�ʵ�ʷ��͵����ݴ洢�ڲ���*has_send_size��
 */
sfpr_int_t sfpr_socket_sendto(sfpr_socket_t *sock, struct sockaddr_in *to, const sfpr_char_t *data, sfpr_int_t size, sfpr_int_t *has_send_size);

/**
 *\ingroup network_ability
 * @brief ͨ��ָ��socket���ӽ����û����ݣ�����TCPЭ���socket����
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] *buffer �����洢���յ����û�����
 * @param[in] size �����û����ݵĻ������Ĵ�С
 * @param[out] *real_recv_size ʵ�ʽ��յ����ݴ�С
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark ����������ʽ���õ��ý�һֱ������ֱ�����յ����ݣ�
 * @remark ���ڷ�������ʽ���õ����ڵ�һ�ε��ú������⵽û�н������ݣ������poll�ӿ��������ָ��ʱ�����Ƿ������ݿɶ�������ɶ����ٴζ�ȡ�����򷵻�
 * @remark ���ڸýӿڣ�����sizeΪ�����û���ȡ������ֽ�����ʵ�ʽ��յ����ݴ洢�ڲ���*real_recv_size��
 */
sfpr_int_t sfpr_socket_recv(sfpr_socket_t *sock, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *real_recv_size);

/**
 *\ingroup network_ability
 * @brief ͨ��ָ��socket���ӽ���ָ����С���û����ݣ�����TCPЭ���socket����
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] *buffer �����洢���յ����û�����
 * @param[in] size ָ�����յ����ݴ�С
 * @param[out] *real_recv_size ʵ�ʽ��յ����ݴ�С
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark ��������ģʽ�������������û�дﵽָ����С����һֱ������ȥ
 * @remark ���ڷ�������ʽ�������������û�дﵽָ����С�����ҳ����趨�ĳ�ʱ������Ȼû�ܼ�⵽���ݿɶ����򷵻�
 * @remark ���ڸýӿڣ�����sizeΪҪ���û���ȡ���ֽ���������ʵ�ʶ�ȡ���ݲ����ֵ����Ϊ��ȡʧ�ܣ�ʵ�ʽ��յ����ݴ洢�ڲ���*real_recv_size��
 */
sfpr_int_t sfpr_socket_recv_by_size(sfpr_socket_t *sock, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *real_recv_size);

/**
 *\ingroup network_ability
 * @brief ͨ��ָ��socket���Ӵ�ָ���ĵ�ַ�����û����ݣ�����UDPЭ���socket����
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] *buffer �������ݵĵ�ַԴ
 * @param[in] *from �����洢���յ����û�����
 * @param[in] size �����û����ݵĻ������Ĵ�С
 * @param[out] *real_recv_size ʵ�ʽ��յ����ݴ�С
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 * @remark ����������ʽ���õ��ý�һֱ������ֱ�����յ����ݣ�
 * @remark ���ڷ�������ʽ���õ����ڵ�һ�ε��ú������⵽û�н������ݣ������poll�ӿ��������ָ��ʱ�����Ƿ������ݿɶ�������ɶ����ٴζ�ȡ�����򷵻�
 * @remark ���ڸýӿڣ�����sizeΪ�����û���ȡ������ֽ�����ʵ�ʽ��յ����ݴ洢�ڲ���*real_recv_size��
 */
sfpr_int_t sfpr_socket_recvfrom(sfpr_socket_t *sock, struct sockaddr_in *from, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *real_recv_size);

/**************************************************************************************************/
/**
 *\defgroup network_status ״̬���ü���ຯ��
 *\ingroup network
 **************************************************************************************************/

/**
 *\ingroup network-status
 * @brief ����socket�Ĳ������ýӿ�ֻ�Ƕ�api setsockopt�ķ�װ
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] level �������ͣ�SOL_SOCKET|IPPROTO_TCP 
 * @param[in] optname ��������SO_KEEPALIVE|SO_REUSEADDR|SO_SNDBUF|SO_RCVBUF|SO_LINGER|TCP_NODELAY
 * @param[in] optval ����ֵ
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 */
sfpr_int_t sfpr_socket_opt_set(sfpr_socket_t *sock, sfpr_int_t level, sfpr_int_t optname, void *optval, sfpr_int_t optlen);

/**
 *\ingroup network-status
 * @brief ��ȡsocket�Ĳ������ýӿ�ֻ�Ƕ�api setsockopt�ķ�װ
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] level ��������
 * @param[in] optname ������
 * @param[out] optval ����ֵ
 * @param[out] optlen ����ֵ��С
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 */
sfpr_int_t sfpr_socket_opt_get(sfpr_socket_t *sock, sfpr_int_t level, sfpr_int_t optname, void *optval, sfpr_int_t *optlen);

/**
 *\ingroup network-status
 * @brief ����socket�ĳ�ʱʱ��
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] timeout ��ʱʱ��ֵ��ms
 * @retval SFPR_SUCCESS �ɹ�
 */
sfpr_int_t sfpr_socket_timeout_set(sfpr_socket_t *sock, sfpr_uint32_t timeout);

/**
 *\ingroup network-status
 * @brief ����socketΪ������ģʽ
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 * @remark �������ʧ�ܣ�����Ŵ洢��sock�е��ֶ�error��
 */
sfpr_int_t sfpr_socket_nonblock_set(sfpr_socket_t *sock);

/**
 *\ingroup network-status
 * @brief ��ȡ������
 * @param[in] *host_name �����������Ļ���
 * @param[in] size �����������Ļ����С
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_socket_hostname_get(sfpr_char_t *host_name, sfpr_int_t size);

/**
 *\ingroup network-status
 * @brief �ýӿ��������ָ����socket�������Ƿ�ɶ�/д
 * @param[in] *sock ��Ҫ�������ݵ�socket����
 * @param[in] for_read 1:���socket�Ƿ��пɶ����ݣ�0:���socket��д
 * @retval SFPR_SUCCESS ���ɹ�; 
 * @retval SFPR_TIMEOUT ��ʱ
 * @retval ������0ֵ poll�ڲ�errno
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
