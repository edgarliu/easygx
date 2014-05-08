#ifndef SFPR_POLL_H
#define SFPR_POLL_H
/**
 * @file sfpr_poll.h
 * @brief SFPR���pollģ��
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
 *\defgroup poll ��·����
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup poll_struct �������ݽṹ
 *\ingroup poll
 **************************************************************************************************/

/**
 * Poll ѡ��
 */
#define SFPR_POLLIN    0x001     /**<��������ݿɶ�*/
#define SFPR_POLLPRI   0x002     /**<Priority data available*/
#define SFPR_POLLOUT   0x004     /**<����д*/
#define SFPR_POLLERR   0x010     /**<Pending error*/
#define SFPR_POLLHUP   0x020     /**<Hangup occurred*/
#define SFPR_POLLNVAL  0x040     /**<Descriptor invalid*/

#define SFPR_MAX_REQ_FD 100

/**
 *\ingroup poll_struct 
 * @brief Poll��Ҫ����socket���ӵ���Ϣ
 */
typedef struct sfpr_pollfd_s {
    sfpr_int_t 		    event;      /**< �¼����ͣ�ͨ��ΪSFPR_POLLIN��SFPR_POLLOUT*/
    sfpr_socket_t 	*sock;            /**<������socket������Ϣ @see  sfpr_socket_t*/
    void       		    *user_data;          /**<�û�����*/
}sfpr_pollfd_t;

/**
 *\ingroup poll_struct
 * @brief poll�ṹ�壻��ǰֻʵ����select��ʽ
 */
typedef struct sfpr_poll_s sfpr_poll_t;

/**************************************************************************************************/
/**
 *\defgroup poll_base ��������
 *\ingroup poll
 **************************************************************************************************/

/**
 *\ingroup poll_base
 * @brief ����һ��pollset������Ҫ�Ƿ����ڴ棬��ʼ�����ֶ�
 * @param[in] *pollset   sfpr_pollset_t�ṹ��
 * @param[in] size �����������ļ�����������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_NOMEM �ڴ����ʧ��
 */
sfpr_int_t sfpr_poll_create(sfpr_poll_t **pollset, sfpr_int_t size);

/**
 *\ingroup poll_base
 * @brief ����һ��pollset����
 * @param[in] *pollset sfpr_pollset_t�ṹ��
 * @retval SFPR_SUCCESS �ɹ�
 */
sfpr_int_t sfpr_poll_destroy(sfpr_poll_t **pollset);

/**************************************************************************************************/
/**
 *\defgroup poll_ability ���ܺ���
 *\ingroup poll
 **************************************************************************************************/

/**
 *\ingroup poll_ability
 * @brief ��pollset���������һ���������ļ���������Ϣ
 * @param[in] *pollset ���ڼ���poll����
 * @param[in] *descriptor ��Ҫ�����ļ���������Ϣ
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_poll_add(sfpr_poll_t *pollset, const sfpr_pollfd_t *descriptor);

/**
 *\ingroup poll_ability
 * @brief ��pollset�������Ƴ�һ�����ļ���������Ϣ
 * @param[in] pollset ���ڼ���poll����
 * @param[in] descriptor ��Ҫ�Ƴ����ļ���������Ϣ
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_poll_remove(sfpr_poll_t *pollset, const sfpr_pollfd_t *descriptor);

/**************************************************************************************************/
/**
 *\defgroup poll_status ״̬�ຯ��
 *\ingroup poll
 **************************************************************************************************/

/**
 *\ingroup poll_status
 * @brief �����ָ����ʱ���ڣ��Ƿ����¼����������ҽ������¼����ļ���������Ϣͨ������descriptors����
 * @param[in] *pollset ���ڼ���poll����
 * @param[in] timeout ��ʱʱ�䣻ms��0:���������������أ�-1:������ֱ�����¼�����������0:��ʱ�ĺ�����
 * @param[out] *num ��⵽���¼�����
 * @param[out] **descriptors ��⵽���ļ���������Ϣ
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_TIMEOUT ��ʱ
 * @retval ������0ֵ �����
 */
sfpr_int_t sfpr_poll_wait(sfpr_poll_t *pollset, sfpr_int_t timeout, sfpr_int_t *num, sfpr_pollfd_t **descriptors);


#ifdef __cplusplus
}
#endif

#endif
