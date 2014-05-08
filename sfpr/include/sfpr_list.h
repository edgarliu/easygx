#ifndef SFPR_LIST_H
#define SFPR_LIST_H

/**
 *\file sfpr_list.h
 *\brief SFPR库的list 模块
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
 *\defgroup list 链表
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup list_struct 基本数据结构
 *\ingroup list
 **************************************************************************************************/

/**
 *\ingroup list_struct
 *\brief 链表数据节点的描述
 */
typedef struct sfpr_list_node_s sfpr_list_node_t;

/**
 *\ingroup list_struct
 *\brief 链表结构的描述
 */
typedef struct sfpr_list_s sfpr_list_t;

/**************************************************************************************************/
/**
 *\defgroup list_base 基本函数
 *\ingroup list
 **************************************************************************************************/

/**
 *\ingroup list_base
 *\brief 创建一个新的链表,并初始化。
*\param[in] double_list 建立一个双向链表	
*\param[in] use_mem_pool 使用内存池标识
*\param[in] *mem_keeper 内存池容器,如果非空，则使用内存池
*\return  NULL 表示失败， 非空 表示成功。
 */
sfpr_list_t *sfpr_list_new(int double_list,sfpr_mem_keeper_t *mem_keeper);

/**
 *\ingroup list_base
*\brief 销毁链表
*\param[in] *list 指向链表的指针		
*\return SFPR_SUCCESS 成功， SFPR_ERROR 失败
*/
int sfpr_list_destroy(sfpr_list_t *list);

/**
 *\ingroup list_base
*\brief 清空链表，不销毁，保留链表结构
*\param[in] *list 指向链表的指针
*\param[in] clear_cb 清空的回调函数
*\param[in] *param 传递给回调函数的值		
*\return SFPR_SUCCESS 成功， SFPR_ERROR 失败
*/
int sfpr_list_clear(sfpr_list_t *list,void(*clear_cb)(void *data,void *param),void *param);


/**************************************************************************************************/
/**
 *\defgroup list_ins_del 插入和删除函数
 *\ingroup list
 **************************************************************************************************/

/**
 *\ingroup list_ins_del 
 *\brief 向链表末尾插入节点
 *\param[in] *list 指向链表的指针		
 *\param[in] *data 要插入的到链表中的数据	
 *\return SFPR_SUCCESS表示正常插入，SFPR_ERROR表示当前链表插入失败。
 */
sfpr_list_node_t *sfpr_list_append(sfpr_list_t *list,void *data);

/**
 *\ingroup list_ins_del 
 *\brief 向链表末尾插入节点
 *\param[in] *list 指向链表的指针		
 *\param[in] *data 要插入的到链表中的数据
 *\param[in] *append_cb 回调函数指针
 *\return SFPR_SUCCESS表示正常插入，SFPR_ERROR表示当前链表插入失败。
 */
sfpr_list_node_t *sfpr_list_append_with_cb(sfpr_list_t *list,void(*append_cb)(void*data, void* param),void* param,void *data);

/**
 *\ingroup list_ins_del 
 *\brief 向链表头部插入节点
 *\param[in] *list 指向链表的指针		
 *\param[in] *data 要插入的到链表中的数据	
 *\return SFPR_SUCCESS表示正常插入，SFPR_ERROR表示当前链表插入失败。
 */
sfpr_list_node_t *sfpr_list_prepend(sfpr_list_t *list,void *data);

/**
 *\ingroup list_ins_del 
 *\brief 删除链表中的某些节点
 *\param[in] *list 指向链表的指针	
 *\param[in] cmp_cb 比较回调函数
 *\param[in] *cmp_param 比较回调函数的参数
 *\param[in] *delete_cb	删除的回调函数
 *\param[in] *delete_param 删除回调函数的参数
 *\return SFPR_SUCCESS表示删除成功，SFPR_ERROR表示当前链表删除失败。
 */	
int sfpr_list_delete(sfpr_list_t *list, int(*cmp_cb)(void  *src, void *param), void *cmp_param,
					void(*delete_cb)(void *data,void *delete_param),void *delete_param);

/**************************************************************************************************/
/**
 *\defgroup list_get 获取类函数
 *\ingroup list
 **************************************************************************************************/
/**
 *\ingroup list_get
 *\brief 获取链表节点的个数
 *\param[in] *list 指向链表的指针
 *\return 成功返回节点的个数 
 */
int sfpr_list_count(sfpr_list_t *list);

/**
 *\ingroup list_get 
 *\brief 通过索引获取相应的数据
 *\param[in] *list 指向链表的指针
 *\param[in] index 索引值
 *\return NULL: 没有找到 非NULL: 找到并且返回所需数据
 */
void* sfpr_list_index(sfpr_list_t* list,int index);

/**
 *\ingroup list_get 
 *\brief 获取链表头结点
 *\param[in] list 目标链表指针
 *\return NULL: 没有找到 非NULL: 找到并且返回所需数据
 */
sfpr_list_node_t *sfpr_list_get_head(sfpr_list_t *list);

/**
 *\ingroup list_get 
 *\brief 获取链表尾结点
 *\param[in] list 目标链表指针
 *\return NULL: 没有找到 非NULL: 找到并且返回所需数据
 */
sfpr_list_node_t *sfpr_list_get_tail(sfpr_list_t *list);

/**
 *\ingroup list_get 
 *\brief 获取下一个结点
 *\param[in] node 当前节点
 *\return NULL: 没有找到 非NULL: 找到并且返回所需数据
 */
sfpr_list_node_t *sfpr_list_get_next(sfpr_list_node_t *node);

/**
 *\ingroup list_get 
 *\brief 获取前一个结点
 *\param[in] node 当前节点
 *\return NULL: 没有找到 非NULL: 找到并且返回所需数据
 */
sfpr_list_node_t *sfpr_list_get_prep(sfpr_list_node_t *node);

/**
 *\ingroup list_get 
 *\brief 获取数据结点
 *\param[in] node 当前节点
 *\return NULL: 没有找到 非NULL: 找到并且返回所需数据
 */
void* sfpr_list_node_get_data(sfpr_list_node_t *node);

/**
 *\ingroup list_get 
 *\brief 从链表中查找所需的数据
 *\param[in] list 目标链表指针
 *\param[in] cmp_cb 比较函数,返回-1 表示没找到,返回0表示找到
 *\param[in] param 回调函数所需参数
 *\return NULL: 没有找到 非NULL: 找到并且返回所需数据
 */
void* sfpr_list_find(sfpr_list_t* list,int(*cmp_cb)(void* data,void* param),void* param);

/**
 *\ingroup list_get 
 *\brief 找到指定索引处的数据
 *\param[in] list 目标链表指针
 *\param[in] count 遍历个数
 *\param[in] handle_cb 回调函数
 *\param[in] param 回调函数所需参数
 *\return SFPR_SUCCESS成功 SFPR_ERROR 失败
 */
int sfpr_list_find_index(sfpr_list_t* list,int index, int(*handle_cb)(void *data,int index,void *param),void *param);

/**
 *\ingroup list_get 
 *\brief 从指定索引开始读取固定数量的数据
 *\param[in] list 目标链表指针
 *\param[in] startIndex 比开始位置
 *\param[in] count 遍历个数
 *\param[in] handle_cb 回调函数
 *\param[in] param 回调函数所需参数
 *\return SFPR_SUCCESS成功 SFPR_ERROR 失败
 */
int sfpr_list_enum_count(sfpr_list_t* list,int startIndex,int count,void(*handle_cb)(void *data,int index,void *param),void *param);

/**
 *\ingroup list_get 
 *\brief 正向遍历整个链表
 *\param[in] *list 指向链表的指针
 *\param[in] handle_cb 遍历的回调函数
 *\param[in] param 回调函数的参数
 *\return SFPR_SUCCESS成功 SFPR_ERROR 失败
 */
int sfpr_list_all(sfpr_list_t *list, void(*handle_cb)(void *data,void *param),void *param);

/**
 *\ingroup list_get 
 *\brief 反向遍历整个链表
 *\param[in] *dlist 指向链表的指针
 *\param[in] handle_cb 遍历的回调函数
 *\param[in] param 回调函数的参数
 *\return SFPR_SUCCESS成功 SFPR_ERROR 失败
 */
int sfpr_list_reverse_all(sfpr_list_t *list, void(*handle_cb)(void *data,void *param),void *param);

#ifdef __cplusplus
}
#endif

#endif
