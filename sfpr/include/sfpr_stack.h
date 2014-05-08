/***  
* @file sfpr_stack.h 
* @brief 动态顺序栈的定义  
* @author nahaitao
* @version 0.1.0
* @date 2012-03-01
*/ 
#ifndef SFPR_STACK_H
#define SFPR_STACK_H

#include "sfpr_global.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\defgroup sfpr_stack 栈模块
 *
 */

/**
 *\defgroup sfpr_stack_struct sfpr_stack模块的数据结构
 *\ingroup sfpr_stack
 */

/**
 *\defgroup sfpr_stack_base sfpr_stack模块的基础功能接口
 *\ingroup sfpr_stack
 */

/**
 *\ingroup  sfpr_stack_struct
 * @brief 栈中节点数据结构
 */
typedef struct sfpr_stack_node_s
{
    int flag; /**<附件参数，如节点数据类型*/
    int size; /**<用户数据大小*/
    void *data; /**<用户数据*/
}sfpr_stack_node_t;

/**
 *\ingroup  sfpr_stack_struct
 * @brief 栈节点数据结构大小
 */
#define SFPR_STACK_NODE_SIZE sizeof(sfpr_stack_node_t)

enum{
    sfpr_stack_mode_data = 0,
    sfpr_stack_mode_pointer
};

/**
 *\ingroup  sfpr_stack_struct
 *@brief 栈结构的定义  
 */ 
typedef struct sfpr_stack_s sfpr_stack_t;

/**
 *\ingroup  sfpr_stack_base
 *@brief 初始化栈，构造一个空的栈
 *@param **stack 栈结构体实例
 *@param size 栈中内存空间大小
 *@param mode 0:用户数据采用栈中内存；1:用户数据由用户外部分配空间，栈中只是保存指针
 *@retval 0 成功 
 *@retval -1 失败
 */ 
int sfpr_stack_create(sfpr_stack_t **stack, int size, int mode);
/**
 *\ingroup  sfpr_stack_base
 *@brief 销毁栈
 *@param **stack 栈结构体实例	
 */ 
void sfpr_stack_destroy(sfpr_stack_t **stack);  
/**
 *\ingroup  sfpr_stack_base
 *@brief 判断栈是否为空
 *@param *stack 栈结构体实例	
 *@retval 0 空
 *@retval -1 非空
 */ 
int sfpr_stack_is_empty(sfpr_stack_t *stack); 
/**
 *\ingroup  sfpr_stack_base
 *@brief 获取栈剩余可用空间
 *@param *stack 栈结构体实例	
 *@return 剩余可用空间大小
 */ 
int sfpr_stack_get_free_size(sfpr_stack_t *stack);  
/**
 *\ingroup  sfpr_stack_base
 *@brief 获取栈中用户数据个数
 *@param *stack 栈结构体实例	
 *@return 用户数据个数
 */ 
int sfpr_stack_get_count(sfpr_stack_t *stack);  

/**
 *\ingroup  sfpr_stack_base
 *@brief 入栈
 *@param *stack 栈结构体实例	
 *@retval 0 空
 *@retval -1 非空
 */ 
int sfpr_stack_push(sfpr_stack_t *stack, void *data, int size, int flag);  
/**
 *\ingroup  sfpr_stack_base
 *@brief 弹出栈中的数据
 *@param[in] *stack 栈结构体实例	
 *@param[out] *size 用户数据大小 
 *@param[out] *flag 用户数据标志
 *@return 返回用户数据指针
 *@remark 该接口使用与mode=1的情况
 */ 
void *sfpr_stack_pop_pointer(sfpr_stack_t *stack, int *size, int *flag);  


void *sfpr_stack_get_top_pointer(sfpr_stack_t *stack, int *size, int *flag);  

void *sfpr_stack_get_bottom_pointer(sfpr_stack_t *stack, int *size, int *flag);

/**
 *\ingroup  sfpr_stack_base
 *@brief 弹出栈中的数据
 *@param[in] *stack 栈结构体实例	
 *@param[out] *data 用户数据容器 
 *@param[out] *size 用户数据大小 
 *@param[out] *flag 用户数据标志
 *@return 用户存储在栈中的节点信息
 *@remark 该接口使用与mode=0的情况
 *@remark 该接口要求用户提供一块内存，可以接收保存在栈中的用户数据
 */ 
int sfpr_stack_pop_data(sfpr_stack_t *stack, void *data, int *size, int *flag);

/**
 *\ingroup sfpr_stack_base
 *\brief 清空栈
 *\param[in] stack 栈		
 *\param[in] delete_cb 删除的回调函数
 *\return 是否清空成功
 *\retval 0 成功
 *\retval <0 失败
 */
int sfpr_stack_clear(sfpr_stack_t *stack, void(*delete_cb)(sfpr_stack_node_t *node));

#ifdef __cplusplus
}
#endif

#endif 

