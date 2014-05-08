#ifndef SFPR_MEMORY_POOL_H
#define SFPR_MEMORY_POOL_H

/**
 * @file sfpr_memory_pool.h
 * @brief SFPR���memoryģ��
 * @author nahaitao
 * @version 0.1.0
 * @date 2012-02-09
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sfpr_global.h"
#include "sfpr_thread_mutex.h"


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup memory_pool �ڴ��ģ��
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup memory_pool_struct �������ݽṹ
 *\ingroup memory_pool
 **************************************************************************************************/

/**
 *\ingroup memory_pool_struct
 * brief �ڴ����
 */
#define SFPR_ALIGN(size, boundary) \
    (((size) + ((boundary) - 1)) & ~((boundary) - 1))

/**
 *\ingroup memory_pool_struct
 *\brief �ڴ�����ڴ�۵Ļ���
 */
#define SFPR_MEM_POOL_SLOT_BASE_SIZE	1024

/**
 *\ingroup memory_pool_struct
 *\brief �ڴ��������Ч�ֽ�����������ֵ���0���ڴ���Ϸ����ڴ�
 */
#define SFPR_MEM_POOL_MEM_NODE_MAX_SIZE (512*1024)

/**
 *\ ingroup memory_pool_struct
 * ����ڴ�ۼ���
 *
 * n		size: 1024*2^(n-1)
 * 1		size: 1024*2^(1-1)		1024
 * 2		size: 1024*2^(2-1)		2048
 * 3		size: 								4096
 * 4		size: 								8192
 * 5		size: 								16384(16K)
 * 6		size:									32K
 * 7		size: 								64K
 * 8		size: 								128K
 * 0		size: 								more than 128K
 */
#define SFPR_MEM_POOL_MAX_SLOT		9
/**
 *\ ingroup memory_pool_struct
 *\ brief ������������ڴ�����*/
#define SFPR_MEM_POOL_MAX_ALLOCED_NUMBER	1024

/**
 *\ ingroup memory_pool_struct
 * @brief �ڴ�ؽṹ
 */
typedef struct sfpr_mem_pool_s sfpr_mem_pool_t;

/**
 *\ ingroup memory_pool_struct
 * @brief �ڴ�ز����ýṹΪ�ڴ�صļ���
 */
typedef struct sfpr_mem_keeper_s sfpr_mem_keeper_t;


/**************************************************************************************************/
/**
 *\defgroup memory_pool_base ��������
 *\ingroup memory_pool
 **************************************************************************************************/

/**
 *\ingroup memory_pool_base
 * @brief �����ڴ沾�������ڴ�
 * @return �����õ��ڴ沾���������ʧ�ܣ��򷵻�NULL
 */
sfpr_mem_keeper_t *sfpr_mem_pool_create();

/**
 *\ingroup memory_pool_base
 * @brief �����ڴ沾��ͬʱ���ٸ��ڴ沾�е������ڴ��
 * @retval SFPR_SUCCESS �ɹ�
 */
sfpr_int_t sfpr_mem_pool_destroy(sfpr_mem_keeper_t **keeper);

/**
 *\ingroup memory_pool_base
 * @brief ���ڴ沾�д���һ���µ��ڴ��
 * @param keeper �ڴ沾
 * @param name �ڴ������
 * @return �����õ��ڴ�أ��������ʧ�ܣ��򷵻�NULL
 */
sfpr_mem_pool_t *sfpr_mem_pool_new(sfpr_mem_keeper_t *keeper,const sfpr_char_t *name);

/**
 *\ingroup memory_pool_base
 * @brief ���ڴ沾��ɾ��ָ�����ڴ��
 * @param keeper �ڴ沾
 * @param pool ��ɾ�����ڴ��
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_mem_pool_delete(sfpr_mem_keeper_t *keeper,sfpr_mem_pool_t *pool);

/**************************************************************************************************/
/**
 *\defgroup memory_pool_ability ���ܺ���
 *\ingroup memory_pool
 **************************************************************************************************/
/**
 *\ingroup memory_pool_ability
 * @brief ���ڴ沾��ɾ��ָ�����Ƶ��ڴ��
 * @param keeper �ڴ沾
 * @param name ��ɾ�����ڴ������
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_mem_pool_delete_byname(sfpr_mem_keeper_t *keeper,const sfpr_char_t *name);

/**
 *\ingroup memory_pool_ability
 * @brief ��ָ���ڴ�ط����ڴ浥Ԫ
 * @param pool �ڴ��
 * @param size ��Ҫ������ڴ���С
 * @return ������ڴ浥Ԫ
 */
sfpr_void_t* sfpr_mem_malloc(sfpr_mem_pool_t *pool,sfpr_int_t size);

/**
 *\ingroup memory_pool_ability
 * @brief ����ָ���Ĵ�С�����·����ڴ浥Ԫ
 * @param pool �ڴ��
 * @param addr ��ǰ�û��ڴ浥Ԫ��ַ
 * @param old_size ��Ҫ����������ڴ���С
 * @param new_size ԭ�ڴ浥Ԫ���ֽ���
 * @return �µ��ڴ浥Ԫ
 * @remark ���·����ڴ���û����ݱ���
 */
sfpr_void_t* sfpr_mem_realloc(sfpr_mem_pool_t *pool, sfpr_void_t *addr, sfpr_int_t old_size, sfpr_int_t new_size);

/**
 *\ingroup memory_pool_ability
 * @brief �ͷ��û��ڴ浥Ԫ
 * @param pool �ڴ��
 * @param addr ��ǰ�û��ڴ浥Ԫ��ַ
 * @retval SFPR_SUCCESS �ɹ�
 * @retval SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_mem_free(sfpr_mem_pool_t *pool,sfpr_void_t *addr);

/**
 *\ingroup memory_pool_ability
 * @brief ���ڴ沾�л�ȡָ�����Ƶ��ڴ��
 * @param keeper �ڴ沾
 * @param name �ڴ������
 * @return ָ�����ڴ棬��������ڣ��򷵻�NULL
 */
sfpr_mem_pool_t *sfpr_mem_pool_get(sfpr_mem_keeper_t *keeper,const sfpr_char_t *name);

/**************************************************************************************************/
/**
 *\defgroup memory_pool_status ״̬��ȡ�ຯ��
 *\ingroup memory_pool
 **************************************************************************************************/

/**
 *\ingroup memory_pool_status
 * @brief ��ȡ�ڴ沾�������ڴ�ص�״̬��Ϣ
 * @retval SFPR_SUCCESS �ɹ�
 * @remark ��ǰֻ�ǽ��ڴ��״̬��Ϣ��ӡ���ն��ϣ��Ժ���ԶԸýӿڽ�����չ��ר�Ŷ���һ����ȡ�ڴ��״̬��Ϣ�Ľṹ
 */
sfpr_int_t sfpr_mem_pool_statall(sfpr_mem_keeper_t *keeper);

/**
 *\ingroup memory_pool_status
 * @brief ��ȡָ���ڴ�ص�״̬��Ϣ
 * @param pool �ڴ��
 * @retval SFPR_SUCCESS �ɹ�
 * @remark ��ʱû�ж���
 */
sfpr_int_t sfpr_mem_pool_stat(sfpr_mem_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif
