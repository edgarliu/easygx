#ifndef SFPR_MEMORY_POOL_H
#define SFPR_MEMORY_POOL_H

/**
 * @file sfpr_memory_pool.h
 * @brief SFPR库的memory模块
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
 *\defgroup memory_pool 内存池模块
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup memory_pool_struct 基本数据结构
 *\ingroup memory_pool
 **************************************************************************************************/

/**
 *\ingroup memory_pool_struct
 * brief 内存对齐
 */
#define SFPR_ALIGN(size, boundary) \
    (((size) + ((boundary) - 1)) & ~((boundary) - 1))

/**
 *\ingroup memory_pool_struct
 *\brief 内存池中内存槽的基数
 */
#define SFPR_MEM_POOL_SLOT_BASE_SIZE	1024

/**
 *\ingroup memory_pool_struct
 *\brief 内存块的最大有效字节数，超过该值则从0级内存槽上分配内存
 */
#define SFPR_MEM_POOL_MEM_NODE_MAX_SIZE (512*1024)

/**
 *\ ingroup memory_pool_struct
 * 最大内存槽级别
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
 *\ brief 最大允许分配的内存块个数*/
#define SFPR_MEM_POOL_MAX_ALLOCED_NUMBER	1024

/**
 *\ ingroup memory_pool_struct
 * @brief 内存池结构
 */
typedef struct sfpr_mem_pool_s sfpr_mem_pool_t;

/**
 *\ ingroup memory_pool_struct
 * @brief 内存池簿，该结构为内存池的集合
 */
typedef struct sfpr_mem_keeper_s sfpr_mem_keeper_t;


/**************************************************************************************************/
/**
 *\defgroup memory_pool_base 基本函数
 *\ingroup memory_pool
 **************************************************************************************************/

/**
 *\ingroup memory_pool_base
 * @brief 创建内存簿，分配内存
 * @return 创建好的内存簿，如果创建失败，则返回NULL
 */
sfpr_mem_keeper_t *sfpr_mem_pool_create();

/**
 *\ingroup memory_pool_base
 * @brief 销毁内存簿，同时销毁该内存簿中的所有内存池
 * @retval SFPR_SUCCESS 成功
 */
sfpr_int_t sfpr_mem_pool_destroy(sfpr_mem_keeper_t **keeper);

/**
 *\ingroup memory_pool_base
 * @brief 在内存簿中创建一个新的内存池
 * @param keeper 内存簿
 * @param name 内存池名称
 * @return 创建好的内存池，如果创建失败，则返回NULL
 */
sfpr_mem_pool_t *sfpr_mem_pool_new(sfpr_mem_keeper_t *keeper,const sfpr_char_t *name);

/**
 *\ingroup memory_pool_base
 * @brief 从内存簿中删除指定的内存池
 * @param keeper 内存簿
 * @param pool 待删除的内存池
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_mem_pool_delete(sfpr_mem_keeper_t *keeper,sfpr_mem_pool_t *pool);

/**************************************************************************************************/
/**
 *\defgroup memory_pool_ability 功能函数
 *\ingroup memory_pool
 **************************************************************************************************/
/**
 *\ingroup memory_pool_ability
 * @brief 从内存簿中删除指定名称的内存池
 * @param keeper 内存簿
 * @param name 待删除的内存池名称
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_mem_pool_delete_byname(sfpr_mem_keeper_t *keeper,const sfpr_char_t *name);

/**
 *\ingroup memory_pool_ability
 * @brief 从指定内存池分配内存单元
 * @param pool 内存池
 * @param size 需要申请的内存块大小
 * @return 申请的内存单元
 */
sfpr_void_t* sfpr_mem_malloc(sfpr_mem_pool_t *pool,sfpr_int_t size);

/**
 *\ingroup memory_pool_ability
 * @brief 按照指定的大小，重新分配内存单元
 * @param pool 内存池
 * @param addr 当前用户内存单元地址
 * @param old_size 需要重新申请的内存块大小
 * @param new_size 原内存单元中字节数
 * @return 新的内存单元
 * @remark 重新分配内存后，用户数据保留
 */
sfpr_void_t* sfpr_mem_realloc(sfpr_mem_pool_t *pool, sfpr_void_t *addr, sfpr_int_t old_size, sfpr_int_t new_size);

/**
 *\ingroup memory_pool_ability
 * @brief 释放用户内存单元
 * @param pool 内存池
 * @param addr 当前用户内存单元地址
 * @retval SFPR_SUCCESS 成功
 * @retval SFPR_ERROR 失败
 */
sfpr_int_t sfpr_mem_free(sfpr_mem_pool_t *pool,sfpr_void_t *addr);

/**
 *\ingroup memory_pool_ability
 * @brief 从内存簿中获取指定名称的内存池
 * @param keeper 内存簿
 * @param name 内存池名称
 * @return 指定的内存，如果不存在，则返回NULL
 */
sfpr_mem_pool_t *sfpr_mem_pool_get(sfpr_mem_keeper_t *keeper,const sfpr_char_t *name);

/**************************************************************************************************/
/**
 *\defgroup memory_pool_status 状态获取类函数
 *\ingroup memory_pool
 **************************************************************************************************/

/**
 *\ingroup memory_pool_status
 * @brief 获取内存簿中所有内存池的状态信息
 * @retval SFPR_SUCCESS 成功
 * @remark 当前只是将内存池状态信息打印到终端上；以后可以对该接口进行扩展，专门定义一个获取内存池状态信息的结构
 */
sfpr_int_t sfpr_mem_pool_statall(sfpr_mem_keeper_t *keeper);

/**
 *\ingroup memory_pool_status
 * @brief 获取指定内存池的状态信息
 * @param pool 内存池
 * @retval SFPR_SUCCESS 成功
 * @remark 暂时没有定义
 */
sfpr_int_t sfpr_mem_pool_stat(sfpr_mem_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif
