/***  
* @file sfpr_stack.h 
* @brief ��̬˳��ջ�Ķ���  
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
 *\defgroup sfpr_stack ջģ��
 *
 */

/**
 *\defgroup sfpr_stack_struct sfpr_stackģ������ݽṹ
 *\ingroup sfpr_stack
 */

/**
 *\defgroup sfpr_stack_base sfpr_stackģ��Ļ������ܽӿ�
 *\ingroup sfpr_stack
 */

/**
 *\ingroup  sfpr_stack_struct
 * @brief ջ�нڵ����ݽṹ
 */
typedef struct sfpr_stack_node_s
{
    int flag; /**<������������ڵ���������*/
    int size; /**<�û����ݴ�С*/
    void *data; /**<�û�����*/
}sfpr_stack_node_t;

/**
 *\ingroup  sfpr_stack_struct
 * @brief ջ�ڵ����ݽṹ��С
 */
#define SFPR_STACK_NODE_SIZE sizeof(sfpr_stack_node_t)

enum{
    sfpr_stack_mode_data = 0,
    sfpr_stack_mode_pointer
};

/**
 *\ingroup  sfpr_stack_struct
 *@brief ջ�ṹ�Ķ���  
 */ 
typedef struct sfpr_stack_s sfpr_stack_t;

/**
 *\ingroup  sfpr_stack_base
 *@brief ��ʼ��ջ������һ���յ�ջ
 *@param **stack ջ�ṹ��ʵ��
 *@param size ջ���ڴ�ռ��С
 *@param mode 0:�û����ݲ���ջ���ڴ棻1:�û��������û��ⲿ����ռ䣬ջ��ֻ�Ǳ���ָ��
 *@retval 0 �ɹ� 
 *@retval -1 ʧ��
 */ 
int sfpr_stack_create(sfpr_stack_t **stack, int size, int mode);
/**
 *\ingroup  sfpr_stack_base
 *@brief ����ջ
 *@param **stack ջ�ṹ��ʵ��	
 */ 
void sfpr_stack_destroy(sfpr_stack_t **stack);  
/**
 *\ingroup  sfpr_stack_base
 *@brief �ж�ջ�Ƿ�Ϊ��
 *@param *stack ջ�ṹ��ʵ��	
 *@retval 0 ��
 *@retval -1 �ǿ�
 */ 
int sfpr_stack_is_empty(sfpr_stack_t *stack); 
/**
 *\ingroup  sfpr_stack_base
 *@brief ��ȡջʣ����ÿռ�
 *@param *stack ջ�ṹ��ʵ��	
 *@return ʣ����ÿռ��С
 */ 
int sfpr_stack_get_free_size(sfpr_stack_t *stack);  
/**
 *\ingroup  sfpr_stack_base
 *@brief ��ȡջ���û����ݸ���
 *@param *stack ջ�ṹ��ʵ��	
 *@return �û����ݸ���
 */ 
int sfpr_stack_get_count(sfpr_stack_t *stack);  

/**
 *\ingroup  sfpr_stack_base
 *@brief ��ջ
 *@param *stack ջ�ṹ��ʵ��	
 *@retval 0 ��
 *@retval -1 �ǿ�
 */ 
int sfpr_stack_push(sfpr_stack_t *stack, void *data, int size, int flag);  
/**
 *\ingroup  sfpr_stack_base
 *@brief ����ջ�е�����
 *@param[in] *stack ջ�ṹ��ʵ��	
 *@param[out] *size �û����ݴ�С 
 *@param[out] *flag �û����ݱ�־
 *@return �����û�����ָ��
 *@remark �ýӿ�ʹ����mode=1�����
 */ 
void *sfpr_stack_pop_pointer(sfpr_stack_t *stack, int *size, int *flag);  


void *sfpr_stack_get_top_pointer(sfpr_stack_t *stack, int *size, int *flag);  

void *sfpr_stack_get_bottom_pointer(sfpr_stack_t *stack, int *size, int *flag);

/**
 *\ingroup  sfpr_stack_base
 *@brief ����ջ�е�����
 *@param[in] *stack ջ�ṹ��ʵ��	
 *@param[out] *data �û��������� 
 *@param[out] *size �û����ݴ�С 
 *@param[out] *flag �û����ݱ�־
 *@return �û��洢��ջ�еĽڵ���Ϣ
 *@remark �ýӿ�ʹ����mode=0�����
 *@remark �ýӿ�Ҫ���û��ṩһ���ڴ棬���Խ��ձ�����ջ�е��û�����
 */ 
int sfpr_stack_pop_data(sfpr_stack_t *stack, void *data, int *size, int *flag);

/**
 *\ingroup sfpr_stack_base
 *\brief ���ջ
 *\param[in] stack ջ		
 *\param[in] delete_cb ɾ���Ļص�����
 *\return �Ƿ���ճɹ�
 *\retval 0 �ɹ�
 *\retval <0 ʧ��
 */
int sfpr_stack_clear(sfpr_stack_t *stack, void(*delete_cb)(sfpr_stack_node_t *node));

#ifdef __cplusplus
}
#endif

#endif 

