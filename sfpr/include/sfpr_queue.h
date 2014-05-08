
#ifndef SFPR_QUEUE_H
#define SFPR_QUEUE_H

/**
 *\file sfpr_queue.h
 *\brief SFPR���queue ģ��
 *\author bijing
 *\version 0.1.0
 *\date 2012-02-06
 */
 
#include "sfpr_thread_mutex.h"

#include "sfpr_thread_cond.h"


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup queue ����
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup queue_struct �������ݽṹ
 *\ingroup queue
 **************************************************************************************************/

/**
 *\ingroup queue_struct
 *\brief ���нڵ����ñ�־
 */
#define SFPR_QUEUE_NODE_USED		0x00000001

/**
 *\ingroup queue_struct
 *\brief �������ݽڵ������
 */
typedef struct sfpr_queue_node_s sfpr_queue_node_t;

/**
 *\ingroup queue_struct
 *\brief �������
 */
#define SFPR_QUEUE_MAX_LEVEL	8

/**
 *\ingroup queue_struct
 *\brief Ĭ�϶��г���
 */
#define SFPR_DEFAULT_QUEUE_LENGTH		128

/**
 *\ingroup queue_struct
 *\brief �����г���
 */
#define SFPR_MAX_QUEUE_LENGTH		1024

/**
 *\ingroup queue_struct
 *\brief ���г�ʼ���ɹ���־
 */
#define SFPR_QUEUE_INIT_OK		0x00000001

/**
 *\ingroup queue_struct
 *\brief ���е���������
 */
typedef struct sfpr_queue_s sfpr_queue_t;

/**************************************************************************************************/
/**
 *\defgroup queue_base ��������
 *\ingroup queue
 **************************************************************************************************/

/**
 *\ingroup queue_base
 *\brief ����һ���µĶ���
 *\return  NULL ��ʾʧ�ܣ� �ǿ� ��ʾ�ɹ���
 */
sfpr_queue_t *sfpr_queue_create();

/**
 *\ingroup queue_base
 *\brief ���ٶ���
 *\param[in] *queue ָ����е�ָ��		
 *\return SFPR_SUCCESS �ɹ��� SFPR_ERROR ʧ��
 */
int sfpr_queue_destroy(sfpr_queue_t *queue);

/**
 *\ingroup queue_base
 *\brief ��ʼ������
 *\param[in] *queue ָ����е�ָ��			
 *\return SFPR_SUCCESS �ɹ� SFPR_ERROR ʧ��
 */
int sfpr_queue_init(sfpr_queue_t *queue);

/**************************************************************************************************/
/**
 *\defgroup queue_ability ���ܺ���
 *\ingroup queue
 **************************************************************************************************/

/**
 *\ingroup queue_ability
 *\brief ����в���ڵ�
 *\note ��������뵱ǰ���У������ǰ����Ϊ�գ�
 *ͬʱҲ��get������������put�ɹ��󽫻�������һ�������get������
 *\param[in] *queue ָ����е�ָ��		
 *\param[in] *data Ҫ������е�����		
 *\return SFPR_SUCCESS��ʾ�������룬SFPR_ERROR��ʾ��ǰ���д���ʧ�ܡ�
 */
int sfpr_queue_put(sfpr_queue_t *queue,void *data);

/**
 *\ingroup queue_ability
 *\brief ����в���ڵ�
 *\note ��������뵱ǰ���У������ǰ����Ϊ�գ�
 *ͬʱҲ��get������������put�ɹ��󽫻�������һ�������get������
 *\param[in] *queue ָ����е�ָ��
 *\param[in] *data Ҫ������е�����
 *\param[in] level Ҫ������е����ݵ����ȼ�����Χ[0-7],��ֵԽС���ȼ�Խ�ߣ�0Ϊ������ȼ�
 *\return SFPR_SUCCESS��ʾ�������룬SFPR_ERROR��ʾ��ǰ���д���ʧ�ܡ�
 */
int sfpr_queue_put_with_level(sfpr_queue_t *queue,void *data,int level);

/**
 *\ingroup queue_ability
 *\brief �Ӷ���ȡ���ݲ��Ӷ�����ɾ���ýڵ�
 *\param[in] *queue ָ����е�ָ��
 *\return ret	�����Ѿ�ȡ�õĶ���
 */
 void *sfpr_queue_get(sfpr_queue_t *queue);


/**************************************************************************************************/
/**
 *\defgroup queue_status ״̬��ȡ�ຯ��
 *\ingroup queue
 **************************************************************************************************/

/**
 *\ingroup queue_status
 *\brief ��ȡ�������ݸ���
 *\param[in] *queue ָ����е�ָ��
 *\return �ɹ��������ݸ��� 
 */
int sfpr_queue_size(sfpr_queue_t *queue);


/**
 *\ingroup queue_status
 *\brief �ж϶��е������Ƿ�Ϊ��
 *\param[in] *queue ָ����е�ָ��
 *\return =1 ���в�Ϊ�գ�0 ����Ϊ��
 */
int sfpr_queue_is_empty(sfpr_queue_t *queue);

/**
 *\ingroup queue_base
 *\brief ɾ�����еĽڵ㣬��ն��м���
 *\param[in] *queue ָ����е�ָ��			
 *\return SFPR_SUCCESS �ɹ� SFPR_ERROR ʧ��
 */
int sfpr_queue_clear(sfpr_queue_t *queue);

/**
 *\ingroup queue_base
 *\brief ��������
 *\param[in] *queue ָ����е�ָ��			
 *\return SFPR_SUCCESS �ɹ� SFPR_ERROR ʧ��
 */
int sfpr_queue_enum(sfpr_queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif

