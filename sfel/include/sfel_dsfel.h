
#ifndef SFEL_DSFEL_H
#define SFEL_DSFEL_H

/**
 *\file sfel_dsfel.h
 *\brief 实现动态sfel功能，支持在sfel配置文件中嵌套动态脚本
 *\author 那海涛
 *\version 0.1.0
 *\date 2012-02-27
 */

/**************************************************************************************************
 * DSFEL: 动态SFEL
***************************************************************************************************/
#include "sfel.h"
#include "sfel_xml.h"
#include "sfpr_stack.h"
#include "sfpr_global.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>

/**************************************************************************************************/
/**
 *\defgroup dsfel 动态sfel
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup dsfel_base 基本函数
 *\ingroup dsfel
 **************************************************************************************************/


/**
 *\ingroup dsfel_base
 * @brief 动态sfel中脚本函数的最大参数个数
 */
#define SFEL_DSFEL_MAX_ARGV	10
#define SFEL_DSFEL_MAX_RETURN_SIZE 4096

/**
 *\ingroup dsfel_base
 * @brief dsfel执行结果类型
 */
typedef enum dsfel_result_type_s{
	dsfel_result_int = 0,/**<整型*/
	dsfel_result_pointer,/**<指针*/
	dsfel_result_string/**<字符串*/
}dsfel_result_type_t;

/**
 *\ingroup dsfel_base
 * @brief dsfel执行结果
 */
typedef struct dsfel_handle_result_s
{
    dsfel_result_type_t type;/**<返回值类型*/
	   sfpr_int_t ret; /**<整型返回值*/
	   sfpr_void_t *pointer;/**<指针返回值*/
	   sfpr_char_t *string;/**<字符串*/
	sfpr_int_t size; /**<string字段的最大值*/
}dsfel_handle_result_t;

/**
 *\ingroup dsfel_base
 * @brief 动态sfel语言中脚本函数的格式
 */
typedef sfpr_int_t (*dsfel_cb_t)(sfel_document_t *doc, sfpr_void_t *argv[SFEL_DSFEL_MAX_ARGV],dsfel_handle_result_t *dsfel_result);

/**
 *\ingroup dsfel_base
 * @brief 动态sfel
 */
typedef struct dsfel_s{
	sfpr_char_t *name; /**<dsfel元素名称*/
	sfpr_int_t type;/**<dsfel元素类型*/
	union{
		dsfel_cb_t cb;/**<函数元素对应的回调函数*/
		sfpr_void_t *var;/**<变量元素对应的变量*/
	}body;
	sfpr_void_t *argv[SFEL_DSFEL_MAX_ARGV];/**<参数*/
}dsfel_t;

int dsfel_func_register(char *name, dsfel_cb_t cb);

/**
 *\ingroup dsfel_base
 * @brief dsfel元素类型
 */
enum dsfel_type_s{
	dsfel_varient = 0,/**<变量*/
	dsfel_function/**<函数*/
}dsfel_type_t;

/**
 *\ingroup dsfel_base
 * @brief 解析动态sfel脚本
 * @param instance sfel_instance_t实例
 * @param script dsfel脚本
 * @param *output 执行结果，由用户在接口外部分配好内存
 * @return 0-成功; -1-失败
 * @remark 该接口要求dsfel脚本满足以下规约:
 * \n
 *         1. 目前只支持函数、属性
 * \n
 *         2. 函数的引用格式:${func(param,param,...,param)}
 * \n
 *                           ${func(param,param,...,param).property}
 * \n  
 *		   3. 函数相互嵌套时，必须使用完全引用，如下:
 * \n
 *            ${func(param,${func1()},...,param)}
 * \n
 *            ${func(param,${func1()},...,param).property}
 * \n
 *         4. 如果在dsfel中存在符号"."，则变量或者函数返回为sfel_node_t类型，可以获取节点的attribute值
 * \n
 *         5. 如果需要使用属性，只支持一层属性调用, 如sfel_node.attr;
              不支持多层属性，如sfel_node.attr.attr1
 */
sfpr_int_t dsfel_compile(sfel_document_t *sfel_document, const sfpr_char_t *script, sfpr_char_t *output,int outsz);

/**
 *\ingroup dsfel_base
 *@brief 判断指定的脚本是否有效的dsfel脚本
 *@param script 待检测字符串
 *@retval 0: 有效的dsfel脚本
 *@retval -1:非dsfel脚本
 */
sfpr_int_t dsfel_isvalid(const sfpr_char_t *script);

#ifdef __cplusplus
}
#endif

#endif
