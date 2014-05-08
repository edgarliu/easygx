
#ifndef SFPR_QUEUE_H
#define SFPR_QUEUE_H

/**
 *\file sfpr_queue.h
 *\brief SFPR库的queue 模块
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
 *\defgroup queue 队列
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup queue_struct 基本数据结构
 *\ingroup queue
 **************************************************************************************************/

/**
 *\ingroup queue_struct
 *\brief 队列节点已用标志
 */
#define SFPR_QUEUE_NODE_USED		0x00000001

/**
 *\ingroup queue_struct
 *\brief 队列数据节点的描述
 */
typedef struct sfpr_queue_node_s sfpr_queue_node_t;

/**
 *\ingroup queue_struct
 *\brief 级别个数
 */
#define SFPR_QUEUE_MAX_LEVEL	8

/**
 *\ingroup queue_struct
 *\brief 默认队列长度
 */
#define SFPR_DEFAULT_QUEUE_LENGTH		128

/**
 *\ingroup queue_struct
 *\brief 最大队列长度
 */
#define SFPR_MAX_QUEUE_LENGTH		1024

/**
 *\ingroup queue_struct
 *\brief 队列初始化成功标志
 */
#define SFPR_QUEUE_INIT_OK		0x00000001

/**
 *\ingroup queue_struct
 *\brief 队列的整体描述
 */
typedef struct sfpr_queue_s sfpr_queue_t;

/**************************************************************************************************/
/**
 *\defgroup queue_base 基本函数
 *\ingroup queue
 **************************************************************************************************/

/**
 *\ingroup queue_base
 *\brief 创建一个新的队列
 *\return  NULL 表示失败， 非空 表示成功。
 */
sfpr_queue_t *sfpr_queue_create();

/**
 *\ingroup queue_base
 *\brief 销毁队列
 *\param[in] *queue 指向队列的指针		
 *\return SFPR_SUCCESS 成功， SFPR_ERROR 失败
 */
int sfpr_queue_destroy(sfpr_queue_t *queue);

/**
 *\ingroup queue_base
 *\brief 初始化队列
 *\param[in] *queue 指向队列的指针			
 *\return SFPR_SUCCESS 成功 SFPR_ERROR 失败
 */
int sfpr_queue_init(sfpr_queue_t *queue);

/**************************************************************************************************/
/**
 *\defgroup queue_ability 功能函数
 *\ingroup queue
 **************************************************************************************************/

/**
 *\ingroup queue_ability
 *\brief 向队列插入节点
 *\note 将对象存入当前队列，如果当前队列为空，
 *同时也有get操作被挂起，在put成功后将唤醒其中一个挂起的get操作。
 *\param[in] *queue 指向队列的指针		
 *\param[in] *data 要插入队列的数据		
 *\return SFPR_SUCCESS表示正常存入，SFPR_ERROR表示当前队列存入失败。
 */
int sfpr_queue_put(sfpr_queue_t *queue,void *data);

/**
 *\ingroup queue_ability
 *\brief 向队列插入节点
 *\note 将对象存入当前队列，如果当前队列为空，
 *同时也有get操作被挂起，在put成功后将唤醒其中一个挂起的get操作。
 *\param[in] *queue 指向队列的指针
 *\param[in] *data 要插入队列的数据
 *\param[in] level 要插入队列的数据的优先级，范围[0-7],数值越小优先级越高，0为最低优先级
 *\return SFPR_SUCCESS表示正常存入，SFPR_ERROR表示当前队列存入失败。
 */
int sfpr_queue_put_with_level(sfpr_queue_t *queue,void *data,int level);

/**
 *\ingroup queue_ability
 *\brief 从队列取数据并从队列中删除该节点
 *\param[in] *queue 指向队列的指针
 *\return ret	返回已经取得的对象。
 */
 void *sfpr_queue_get(sfpr_queue_t *queue);


/**************************************************************************************************/
/**
 *\defgroup queue_status 状态获取类函数
 *\ingroup queue
 **************************************************************************************************/

/**
 *\ingroup queue_status
 *\brief 获取队列数据个数
 *\param[in] *queue 指向队列的指针
 *\return 成功返回数据个数 
 */
int sfpr_queue_size(sfpr_queue_t *queue);


/**
 *\ingroup queue_status
 *\brief 判断队列的数据是否为空
 *\param[in] *queue 指向队列的指针
 *\return =1 队列不为空，0 队列为空
 */
int sfpr_queue_is_empty(sfpr_queue_t *queue);

/**
 *\ingroup queue_base
 *\brief 删除队列的节点，清空队列计数
 *\param[in] *queue 指向队列的指针			
 *\return SFPR_SUCCESS 成功 SFPR_ERROR 失败
 */
int sfpr_queue_clear(sfpr_queue_t *queue);

/**
 *\ingroup queue_base
 *\brief 遍历队列
 *\param[in] *queue 指向队列的指针			
 *\return SFPR_SUCCESS 成功 SFPR_ERROR 失败
 */
int sfpr_queue_enum(sfpr_queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif

