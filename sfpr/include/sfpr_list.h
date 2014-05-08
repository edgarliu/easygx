#ifndef SFPR_LIST_H
#define SFPR_LIST_H

/**
 *\file sfpr_list.h
 *\brief SFPR���list ģ��
 *\author dulh
 *\version 0.1.0
 *\date 2012-02-07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sfpr_global.h"
#include "sfpr_thread_mutex.h"
#include "sfpr_memory_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup list ����
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup list_struct �������ݽṹ
 *\ingroup list
 **************************************************************************************************/

/**
 *\ingroup list_struct
 *\brief �������ݽڵ������
 */
typedef struct sfpr_list_node_s sfpr_list_node_t;

/**
 *\ingroup list_struct
 *\brief ����ṹ������
 */
typedef struct sfpr_list_s sfpr_list_t;

/**************************************************************************************************/
/**
 *\defgroup list_base ��������
 *\ingroup list
 **************************************************************************************************/

/**
 *\ingroup list_base
 *\brief ����һ���µ�����,����ʼ����
*\param[in] double_list ����һ��˫������	
*\param[in] use_mem_pool ʹ���ڴ�ر�ʶ
*\param[in] *mem_keeper �ڴ������,����ǿգ���ʹ���ڴ��
*\return  NULL ��ʾʧ�ܣ� �ǿ� ��ʾ�ɹ���
 */
sfpr_list_t *sfpr_list_new(int double_list,sfpr_mem_keeper_t *mem_keeper);

/**
 *\ingroup list_base
*\brief ��������
*\param[in] *list ָ�������ָ��		
*\return SFPR_SUCCESS �ɹ��� SFPR_ERROR ʧ��
*/
int sfpr_list_destroy(sfpr_list_t *list);

/**
 *\ingroup list_base
*\brief ������������٣���������ṹ
*\param[in] *list ָ�������ָ��
*\param[in] clear_cb ��յĻص�����
*\param[in] *param ���ݸ��ص�������ֵ		
*\return SFPR_SUCCESS �ɹ��� SFPR_ERROR ʧ��
*/
int sfpr_list_clear(sfpr_list_t *list,void(*clear_cb)(void *data,void *param),void *param);


/**************************************************************************************************/
/**
 *\defgroup list_ins_del �����ɾ������
 *\ingroup list
 **************************************************************************************************/

/**
 *\ingroup list_ins_del 
 *\brief ������ĩβ����ڵ�
 *\param[in] *list ָ�������ָ��		
 *\param[in] *data Ҫ����ĵ������е�����	
 *\return SFPR_SUCCESS��ʾ�������룬SFPR_ERROR��ʾ��ǰ�������ʧ�ܡ�
 */
sfpr_list_node_t *sfpr_list_append(sfpr_list_t *list,void *data);

/**
 *\ingroup list_ins_del 
 *\brief ������ĩβ����ڵ�
 *\param[in] *list ָ�������ָ��		
 *\param[in] *data Ҫ����ĵ������е�����
 *\param[in] *append_cb �ص�����ָ��
 *\return SFPR_SUCCESS��ʾ�������룬SFPR_ERROR��ʾ��ǰ�������ʧ�ܡ�
 */
sfpr_list_node_t *sfpr_list_append_with_cb(sfpr_list_t *list,void(*append_cb)(void*data, void* param),void* param,void *data);

/**
 *\ingroup list_ins_del 
 *\brief ������ͷ������ڵ�
 *\param[in] *list ָ�������ָ��		
 *\param[in] *data Ҫ����ĵ������е�����	
 *\return SFPR_SUCCESS��ʾ�������룬SFPR_ERROR��ʾ��ǰ�������ʧ�ܡ�
 */
sfpr_list_node_t *sfpr_list_prepend(sfpr_list_t *list,void *data);

/**
 *\ingroup list_ins_del 
 *\brief ɾ�������е�ĳЩ�ڵ�
 *\param[in] *list ָ�������ָ��	
 *\param[in] cmp_cb �Ƚϻص�����
 *\param[in] *cmp_param �Ƚϻص������Ĳ���
 *\param[in] *delete_cb	ɾ���Ļص�����
 *\param[in] *delete_param ɾ���ص������Ĳ���
 *\return SFPR_SUCCESS��ʾɾ���ɹ���SFPR_ERROR��ʾ��ǰ����ɾ��ʧ�ܡ�
 */	
int sfpr_list_delete(sfpr_list_t *list, int(*cmp_cb)(void  *src, void *param), void *cmp_param,
					void(*delete_cb)(void *data,void *delete_param),void *delete_param);

/**************************************************************************************************/
/**
 *\defgroup list_get ��ȡ�ຯ��
 *\ingroup list
 **************************************************************************************************/
/**
 *\ingroup list_get
 *\brief ��ȡ����ڵ�ĸ���
 *\param[in] *list ָ�������ָ��
 *\return �ɹ����ؽڵ�ĸ��� 
 */
int sfpr_list_count(sfpr_list_t *list);

/**
 *\ingroup list_get 
 *\brief ͨ��������ȡ��Ӧ������
 *\param[in] *list ָ�������ָ��
 *\param[in] index ����ֵ
 *\return NULL: û���ҵ� ��NULL: �ҵ����ҷ�����������
 */
void* sfpr_list_index(sfpr_list_t* list,int index);

/**
 *\ingroup list_get 
 *\brief ��ȡ����ͷ���
 *\param[in] list Ŀ������ָ��
 *\return NULL: û���ҵ� ��NULL: �ҵ����ҷ�����������
 */
sfpr_list_node_t *sfpr_list_get_head(sfpr_list_t *list);

/**
 *\ingroup list_get 
 *\brief ��ȡ����β���
 *\param[in] list Ŀ������ָ��
 *\return NULL: û���ҵ� ��NULL: �ҵ����ҷ�����������
 */
sfpr_list_node_t *sfpr_list_get_tail(sfpr_list_t *list);

/**
 *\ingroup list_get 
 *\brief ��ȡ��һ�����
 *\param[in] node ��ǰ�ڵ�
 *\return NULL: û���ҵ� ��NULL: �ҵ����ҷ�����������
 */
sfpr_list_node_t *sfpr_list_get_next(sfpr_list_node_t *node);

/**
 *\ingroup list_get 
 *\brief ��ȡǰһ�����
 *\param[in] node ��ǰ�ڵ�
 *\return NULL: û���ҵ� ��NULL: �ҵ����ҷ�����������
 */
sfpr_list_node_t *sfpr_list_get_prep(sfpr_list_node_t *node);

/**
 *\ingroup list_get 
 *\brief ��ȡ���ݽ��
 *\param[in] node ��ǰ�ڵ�
 *\return NULL: û���ҵ� ��NULL: �ҵ����ҷ�����������
 */
void* sfpr_list_node_get_data(sfpr_list_node_t *node);

/**
 *\ingroup list_get 
 *\brief �������в������������
 *\param[in] list Ŀ������ָ��
 *\param[in] cmp_cb �ȽϺ���,����-1 ��ʾû�ҵ�,����0��ʾ�ҵ�
 *\param[in] param �ص������������
 *\return NULL: û���ҵ� ��NULL: �ҵ����ҷ�����������
 */
void* sfpr_list_find(sfpr_list_t* list,int(*cmp_cb)(void* data,void* param),void* param);

/**
 *\ingroup list_get 
 *\brief �ҵ�ָ��������������
 *\param[in] list Ŀ������ָ��
 *\param[in] count ��������
 *\param[in] handle_cb �ص�����
 *\param[in] param �ص������������
 *\return SFPR_SUCCESS�ɹ� SFPR_ERROR ʧ��
 */
int sfpr_list_find_index(sfpr_list_t* list,int index, int(*handle_cb)(void *data,int index,void *param),void *param);

/**
 *\ingroup list_get 
 *\brief ��ָ��������ʼ��ȡ�̶�����������
 *\param[in] list Ŀ������ָ��
 *\param[in] startIndex �ȿ�ʼλ��
 *\param[in] count ��������
 *\param[in] handle_cb �ص�����
 *\param[in] param �ص������������
 *\return SFPR_SUCCESS�ɹ� SFPR_ERROR ʧ��
 */
int sfpr_list_enum_count(sfpr_list_t* list,int startIndex,int count,void(*handle_cb)(void *data,int index,void *param),void *param);

/**
 *\ingroup list_get 
 *\brief ���������������
 *\param[in] *list ָ�������ָ��
 *\param[in] handle_cb �����Ļص�����
 *\param[in] param �ص������Ĳ���
 *\return SFPR_SUCCESS�ɹ� SFPR_ERROR ʧ��
 */
int sfpr_list_all(sfpr_list_t *list, void(*handle_cb)(void *data,void *param),void *param);

/**
 *\ingroup list_get 
 *\brief ���������������
 *\param[in] *dlist ָ�������ָ��
 *\param[in] handle_cb �����Ļص�����
 *\param[in] param �ص������Ĳ���
 *\return SFPR_SUCCESS�ɹ� SFPR_ERROR ʧ��
 */
int sfpr_list_reverse_all(sfpr_list_t *list, void(*handle_cb)(void *data,void *param),void *param);

#ifdef __cplusplus
}
#endif

#endif
