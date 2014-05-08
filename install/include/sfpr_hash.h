
#ifndef SFPR_HASH_H
#define SFPR_HASH_H

/**
 *\file sfpr_hash.h
 *\brief SFPR���Hash��ģ��
 *\author ����ǿ
 *\version 0.1.0
 *\date 2012-02-12
 */


#include "sfpr_global.h"
#include "sfpr_memory_pool.h"


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup hash ��ϣ��
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup hash_struct �������ݽṹ
 *\ingroup hash
 **************************************************************************************************/

/**
 *\ingroup hash_struct
 *\brief Ĭ�Ϲ�ϣ���С
 */
#define HASH_DEFAULT_SIZE		2048

/**
 *\ingroup hash_struct
 *\brief ��ϣ��ؼ��ַǷ�����
 */
#define SPPR_HASH_KEY_STRING	(-1)

/**
 *\ingroup hash_struct
 *\brief ��ϣ�����Ƶ���󳤶�
 */
#define HASH_MAX_NAME		20

/**
 *\ingroup hash_struct
 *\brief ��ϣ��ڵ������
 */
typedef struct sfpr_hash_node_s sfpr_hash_node_t;

/**
 *\ingroup hash_struct
 *\brief ��ϣ��Ĺ�ϣ����
 */
typedef sfpr_int_t (*sfpr_hashfunc_t)(const sfpr_void_t *key,sfpr_uint32_t key_size);

/**
 *\ingroup hash_struct
 *\brief ��ϣ��ıȽϻص�����
 */
typedef sfpr_int_t (*sfpr_cmpfunc_t)(const sfpr_void_t *keya,const sfpr_void_t *keyb,sfpr_int_t key_size);

/**
 *\ingroup hash_struct
 *\brief ��ϣ���ɾ���ص�����
 */
typedef void (*sfpr_hashdelete_t)(sfpr_hash_node_t *node);

/**
 *\ingroup hash_struct
 *\brief ��ϣ��ı����ص�����
 */
typedef sfpr_void_t (*sfpr_hashenum_t)(sfpr_hash_node_t *node,sfpr_void_t *param);

/**
 *\ingroup hash_struct
 *\brief ����һ����ϣ��ṹ
 */
typedef struct sfpr_hash_s sfpr_hash_t;

/**************************************************************************************************/
/**
 *\defgroup hash_base ��������
 *\ingroup hash
 **************************************************************************************************/

/**
 *\ingroup hash_base
 *\fn sfpr_hash_t *sfpr_hash_new()
 *\brief ����һ���µĹ�ϣ��
 *\return sfpr_hash_t*����ָ��
 *\retval �ǿ� �ɹ�����һ���µĹ�ϣ��,���ع�ϣ��ָ�롣
 *\retval NULL ʧ�� 
 */
sfpr_hash_t *sfpr_hash_new();

/**
 *\ingroup hash_base
 *\brief ����һ����ϣ��
 *\param[in] hash ��ϣ�����ָ�롣
 *\return �Ƿ����ٳɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_destroy(sfpr_hash_t **hash);


/**
 *\ingroup hash_base
 *\brief ��ʼ��һ����ϣ��
 *\param[in] hash ��ϣ��ָ�롣			
 *\param[in] name ��ϣ�����ơ�			
 *\param[in] size ��ϣ���С��			
 *\param[in] hash_func ��ϣ������			
 *\param[in] cmp_func ��ϣ�ȽϺ�����			
 *\return �Ƿ񴴽��ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_init(sfpr_hash_t *hash,sfpr_char_t *name,sfpr_int_t size,
						sfpr_hashfunc_t hash_func,sfpr_cmpfunc_t cmp_func,
						sfpr_hashdelete_t del_func);

/**
 *\ingroup hash_base
 *\brief ���ʼ����Ӧ�Ĳ���
 *\param[in] hash ��ϣ��ָ�롣			
 *\return �Ƿ��ͷųɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_finalize(sfpr_hash_t *hash);


/**************************************************************************************************/
/**
 *\defgroup hash_ability ���ܺ���
 *\ingroup hash
 **************************************************************************************************/

/**
 *\ingroup hash_ability
 *\brief ���ϣ�����ڵ�
 *\param[in] hash ��ϣ��ָ�롣			
 *\param[in] key �ؼ���		
 *\param[in] key_size �ؼ��ֳ���			
 *\param[in] value ��ϣ����		
 *\param[in] group �������� 
 *\param[in] flags ��־��Ϣ 		
 *\return ���ز���valueֵ
 *\retval <0 ����ʧ��
 *\retval >=0 �ɹ�
 */
sfpr_int_t sfpr_hash_insert(sfpr_hash_t *hash,void *key,sfpr_uint32_t key_size,
						void *value,sfpr_int_t group,sfpr_uint32_t flags);

/**
 *\ingroup hash_ability
 *\brief �ӹ�ϣ��ɾ���ڵ�
 *\param[in] hash ��ϣ��ָ�롣			
 *\param[in] key �ؼ���		
 *\param[in] key_size �ؼ��ֳ���
 *\param[in] delete_cb ɾ���Ļص�����
 *\return �Ƿ�ɾ���ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_delete(sfpr_hash_t *hash,const sfpr_void_t *key,sfpr_uint32_t key_size,
			sfpr_hashdelete_t delete_cb);

/**
 *\ingroup hash_ability
 *\brief ��չ�ϣ��,ɾ�����нڵ�
 *\param[in] hash ��ϣ��ָ�롣			
 *\param[in] delete_cb ɾ���Ļص�����
 *\return �Ƿ���ճɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_clear(sfpr_hash_t *hash,sfpr_hashdelete_t delete_cb);


/**
 *\ingroup hash_ability
 *\brief ���ҽڵ�
 *\param[in] hash ��ϣ��ָ�롣			
 *\param[in] key �ؼ���		
 *\param[in] key_size �ؼ��ֳ���			
 *\return ���ز鵽���Ľڵ�����ݵ�ָ��
 *\retval �ǿ� ����ָ��
 *\retval NULL û�ҵ�
 */
sfpr_void_t *sfpr_hash_find(sfpr_hash_t *hash,const sfpr_void_t *key,sfpr_uint32_t key_size);

/**
 *\ingroup hash_ability
 *\brief �����в��ҽڵ�
 *\param[in] hash ��ϣ��ָ�롣			
 *\param[in] key �ؼ���		
 *\param[in] key_size �ؼ��ֳ���			
 *\param[in] group ��ID		
 *\return ���ز鵽���Ľڵ�����ݵ�ָ��
 *\retval �ǿ� ����ָ��
 *\retval NULL û�ҵ�
 */
sfpr_void_t *sfpr_hash_group_find(sfpr_hash_t *hash,const void *key,sfpr_uint32_t key_size,sfpr_int_t group);

/**
 *\ingroup hash_ability
 *\brief ������ϣ��
 *\param[in] hash ��ϣ��ָ��
 *\param[in] enum_cb �����Ļص�����
 *\param[in] param �ص������Ĳ���
 *\return �Ƿ�����ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_enum(sfpr_hash_t *hash,
				sfpr_hashenum_t enum_cb,sfpr_void_t *param);

/**
 *\ingroup hash_ability
 *\brief �����������ϣ��
 *\param[in] hash ��ϣ��ָ��
 *\param[in] group ��id
 *\param[in] enum_cb �����Ļص�����
 *\param[in] param �ص������Ĳ���
 *\return �Ƿ�����ɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_group_enum(sfpr_hash_t *hash,sfpr_int_t group,
				sfpr_hashenum_t enum_cb,void *param);

/**************************************************************************************************/
/**
 *\defgroup hash_status ״̬��ȡ�ຯ��
 *\ingroup hash
 **************************************************************************************************/

/**
 *\ingroup hash_status
 *\brief ��ȡ��ǰ�ڵ���
 *\param[in] hash ��ϣ��ָ��
 *\return ���ؽڵ����
 *\retval >=0 ��ǰ�ڵ���
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_count(sfpr_hash_t *hash);


/**
 *\ingroup hash_status
 *\brief ��ϣ���Ƿ����
 *\param[in] hash ��ϣ��ָ��
 *\return ���ع�ϣ��״̬
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
sfpr_int_t sfpr_hash_can_use(sfpr_hash_t *hash);


sfpr_int_t sfpr_hash_set_userdata(sfpr_hash_t *hash,sfpr_void_t *userdata);

sfpr_void_t *sfpr_hash_get_userdata(sfpr_hash_t *hash);

const sfpr_char_t *sfpr_hash_get_name(sfpr_hash_t *hash);

const sfpr_int_t sfpr_hash_get_size(sfpr_hash_t *hash);

sfpr_int_t sfpr_hash_get_node_size();


sfpr_void_t *sfpr_hash_node_get_key(sfpr_hash_node_t *node);

sfpr_int_t sfpr_hash_node_get_key_size(sfpr_hash_node_t *node);

sfpr_void_t *sfpr_hash_node_get_value(sfpr_hash_node_t *node);

int sfpr_hash_node_set_value(sfpr_hash_node_t *node,sfpr_void_t *new_value);

sfpr_void_t *sfpr_hash_node_get_value_addr(sfpr_hash_node_t *node);


sfpr_int_t sfpr_hash_node_get_slot(sfpr_hash_node_t *node);

sfpr_int_t sfpr_hash_node_get_hashvalue(sfpr_hash_node_t *node);

sfpr_int_t sfpr_hash_node_get_groupid(sfpr_hash_node_t *node);


#ifdef __cplusplus
}
#endif

#endif
