
#ifndef SFPR_HASH_H
#define SFPR_HASH_H

/**
 *\file sfpr_hash.h
 *\brief SFPR库的Hash表模块
 *\author 刘晓强
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
 *\defgroup hash 哈希表
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup hash_struct 基本数据结构
 *\ingroup hash
 **************************************************************************************************/

/**
 *\ingroup hash_struct
 *\brief 默认哈希表大小
 */
#define HASH_DEFAULT_SIZE		2048

/**
 *\ingroup hash_struct
 *\brief 哈希表关键字非法长度
 */
#define SPPR_HASH_KEY_STRING	(-1)

/**
 *\ingroup hash_struct
 *\brief 哈希表名称的最大长度
 */
#define HASH_MAX_NAME		20

/**
 *\ingroup hash_struct
 *\brief 哈希表节点的描述
 */
typedef struct sfpr_hash_node_s sfpr_hash_node_t;

/**
 *\ingroup hash_struct
 *\brief 哈希表的哈希函数
 */
typedef sfpr_int_t (*sfpr_hashfunc_t)(const sfpr_void_t *key,sfpr_uint32_t key_size);

/**
 *\ingroup hash_struct
 *\brief 哈希表的比较回调函数
 */
typedef sfpr_int_t (*sfpr_cmpfunc_t)(const sfpr_void_t *keya,const sfpr_void_t *keyb,sfpr_int_t key_size);

/**
 *\ingroup hash_struct
 *\brief 哈希表的删除回调函数
 */
typedef void (*sfpr_hashdelete_t)(sfpr_hash_node_t *node);

/**
 *\ingroup hash_struct
 *\brief 哈希表的遍历回调函数
 */
typedef sfpr_void_t (*sfpr_hashenum_t)(sfpr_hash_node_t *node,sfpr_void_t *param);

/**
 *\ingroup hash_struct
 *\brief 描述一个哈希表结构
 */
typedef struct sfpr_hash_s sfpr_hash_t;

/**************************************************************************************************/
/**
 *\defgroup hash_base 基本函数
 *\ingroup hash
 **************************************************************************************************/

/**
 *\ingroup hash_base
 *\fn sfpr_hash_t *sfpr_hash_new()
 *\brief 创建一个新的哈希表
 *\return sfpr_hash_t*类型指针
 *\retval 非空 成功创建一个新的哈希表,返回哈希表指针。
 *\retval NULL 失败 
 */
sfpr_hash_t *sfpr_hash_new();

/**
 *\ingroup hash_base
 *\brief 销毁一个哈希表
 *\param[in] hash 哈希表二级指针。
 *\return 是否销毁成功
 *\retval 0 成功
 *\retval <0 失败
 */
sfpr_int_t sfpr_hash_destroy(sfpr_hash_t **hash);


/**
 *\ingroup hash_base
 *\brief 初始化一个哈希表
 *\param[in] hash 哈希表指针。			
 *\param[in] name 哈希表名称。			
 *\param[in] size 哈希表大小。			
 *\param[in] hash_func 哈希函数。			
 *\param[in] cmp_func 哈希比较函数。			
 *\return 是否创建成功
 *\retval 0 成功
 *\retval <0 失败
 */
sfpr_int_t sfpr_hash_init(sfpr_hash_t *hash,sfpr_char_t *name,sfpr_int_t size,
						sfpr_hashfunc_t hash_func,sfpr_cmpfunc_t cmp_func,
						sfpr_hashdelete_t del_func);

/**
 *\ingroup hash_base
 *\brief 与初始化对应的操作
 *\param[in] hash 哈希表指针。			
 *\return 是否释放成功
 *\retval 0 成功
 *\retval <0 失败
 */
sfpr_int_t sfpr_hash_finalize(sfpr_hash_t *hash);


/**************************************************************************************************/
/**
 *\defgroup hash_ability 功能函数
 *\ingroup hash
 **************************************************************************************************/

/**
 *\ingroup hash_ability
 *\brief 向哈希表插入节点
 *\param[in] hash 哈希表指针。			
 *\param[in] key 关键字		
 *\param[in] key_size 关键字长度			
 *\param[in] value 哈希数据		
 *\param[in] group 所属分组 
 *\param[in] flags 标志信息 		
 *\return 返回参数value值
 *\retval <0 插入失败
 *\retval >=0 成功
 */
sfpr_int_t sfpr_hash_insert(sfpr_hash_t *hash,void *key,sfpr_uint32_t key_size,
						void *value,sfpr_int_t group,sfpr_uint32_t flags);

/**
 *\ingroup hash_ability
 *\brief 从哈希表删除节点
 *\param[in] hash 哈希表指针。			
 *\param[in] key 关键字		
 *\param[in] key_size 关键字长度
 *\param[in] delete_cb 删除的回调函数
 *\return 是否删除成功
 *\retval 0 成功
 *\retval <0 失败
 */
sfpr_int_t sfpr_hash_delete(sfpr_hash_t *hash,const sfpr_void_t *key,sfpr_uint32_t key_size,
			sfpr_hashdelete_t delete_cb);

/**
 *\ingroup hash_ability
 *\brief 清空哈希表,删除所有节点
 *\param[in] hash 哈希表指针。			
 *\param[in] delete_cb 删除的回调函数
 *\return 是否清空成功
 *\retval 0 成功
 *\retval <0 失败
 */
sfpr_int_t sfpr_hash_clear(sfpr_hash_t *hash,sfpr_hashdelete_t delete_cb);


/**
 *\ingroup hash_ability
 *\brief 查找节点
 *\param[in] hash 哈希表指针。			
 *\param[in] key 关键字		
 *\param[in] key_size 关键字长度			
 *\return 返回查到到的节点的数据的指针
 *\retval 非空 数据指针
 *\retval NULL 没找到
 */
sfpr_void_t *sfpr_hash_find(sfpr_hash_t *hash,const sfpr_void_t *key,sfpr_uint32_t key_size);

/**
 *\ingroup hash_ability
 *\brief 在组中查找节点
 *\param[in] hash 哈希表指针。			
 *\param[in] key 关键字		
 *\param[in] key_size 关键字长度			
 *\param[in] group 组ID		
 *\return 返回查到到的节点的数据的指针
 *\retval 非空 数据指针
 *\retval NULL 没找到
 */
sfpr_void_t *sfpr_hash_group_find(sfpr_hash_t *hash,const void *key,sfpr_uint32_t key_size,sfpr_int_t group);

/**
 *\ingroup hash_ability
 *\brief 遍历哈希表
 *\param[in] hash 哈希表指针
 *\param[in] enum_cb 遍历的回调函数
 *\param[in] param 回调函数的参数
 *\return 是否遍历成功
 *\retval 0 成功
 *\retval <0 失败
 */
sfpr_int_t sfpr_hash_enum(sfpr_hash_t *hash,
				sfpr_hashenum_t enum_cb,sfpr_void_t *param);

/**
 *\ingroup hash_ability
 *\brief 按照组遍历哈希表
 *\param[in] hash 哈希表指针
 *\param[in] group 组id
 *\param[in] enum_cb 遍历的回调函数
 *\param[in] param 回调函数的参数
 *\return 是否遍历成功
 *\retval 0 成功
 *\retval <0 失败
 */
sfpr_int_t sfpr_hash_group_enum(sfpr_hash_t *hash,sfpr_int_t group,
				sfpr_hashenum_t enum_cb,void *param);

/**************************************************************************************************/
/**
 *\defgroup hash_status 状态获取类函数
 *\ingroup hash
 **************************************************************************************************/

/**
 *\ingroup hash_status
 *\brief 获取当前节点数
 *\param[in] hash 哈希表指针
 *\return 返回节点个数
 *\retval >=0 当前节点数
 *\retval <0 失败
 */
sfpr_int_t sfpr_hash_count(sfpr_hash_t *hash);


/**
 *\ingroup hash_status
 *\brief 哈希表是否可用
 *\param[in] hash 哈希表指针
 *\return 返回哈希表状态
 *\retval 0 成功
 *\retval <0 失败
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
