
#ifndef SFEL_H
#define SFEL_H

/**
 *\file sfel.h
 *\brief SFEL在XML库之上再封装一层，实现SFEL语言\n
 *XML解析库实现SFEL的语法\n
 *本库实现SFEL的语义
 *\author 刘晓强
 *\version 0.1.0
 *\date 2012-02-12
 */

#include <stdio.h>
#include <stdarg.h>

#include "sfpr_hash.h"
#include "sfpr_global.h"
#include "sfel_xml.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup sfel sfel主模块
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup sfel_tags SFEL标签描述
 *\ingroup sfel
 **************************************************************************************************/

/**
 *\ingroup sfel_tags
 *\brief 标签属性
 */
typedef struct sfel_tag_attr_s{
	sfpr_char_t *name;/**<属性名*/
	sfpr_uint32_t type;/**<属性的数据类型*/
	sfpr_uint32_t flags;/**<参数，暂时保留*/
	sfpr_char_t *desc;/**<属性描述*/
}sfel_tag_attr_t;

/**
 *\ingroup sfel_tags
 * @brief 标签上下文回调函数的参数
 */ 
#define ACTION_ARGS (sfel_node_t *node,sfpr_void_t *param,sfpr_int_t depth, sfpr_int_t status)

/**
 *\ingroup sfel_tags
 * @brief 标签上下文回调函数
 * @param status 0:节点开始时; 1:节点开始后
 */ 
typedef sfpr_int_t(*tag_action_t)ACTION_ARGS;

/**
 *\ingroup sfel_tags
 * @brief 已经废弃的一个宏
 */ 
#define REGISTER_ACTION(tag_class,tag_name)  sfel_action_register(#tag_name,tag_class->context,tag_class->tag_##tag_name)

/**
 *\ingroup sfel_tags
 *\brief 标签的回调函数
 */
typedef struct sfel_tag_cb_s{
	const sfpr_char_t *context;/**<回调的上下文环境*/
	tag_action_t action;/**<上下文处理函数*/	
}sfel_tag_cb_t;

/**
 *\ingroup sfel_tags
 *\brief 标签的描述
 */
typedef struct sfel_tag_s{
	sfpr_char_t *name; /**<标签名*/
	sfpr_char_t *class_name; /**<类名*/
	sfpr_char_t *subclass;/**<子类名*/
	sfpr_uint32_t flag;/**<其他参数，暂时保留*/
	sfpr_uint32_t event;/**<事件*/
	sfel_tag_attr_t *attr;/**<属性定义信息*/
	sfpr_char_t *desc;/**<描述信息*/
	tag_action_t default_action;/**<默认上下文回调函数*/
	sfpr_int_t action_count;/**<当前注册的上下文处理函数数量*/
}sfel_tag_t;

enum sfel_node_type
{
	sfel_node_document = 0,      //!< A document node. Name and value are empty.
	sfel_node_element,       //!< An element node. Name contains element name. Value contains text of first data node.
	sfel_node_data,          //!< A data node. Name is empty. Value contains data text.
	sfel_node_cdata,         //!< A CDATA node. Name is empty. Value contains data text.
	sfel_node_comment,       //!< A comment node. Name is empty. Value contains comment text.
	sfel_node_declaration,   //!< A declaration node. Name and value are empty. Declaration parameters (version, encoding and standalone) are in node attributes.
	sfel_node_doctype,       //!< A DOCTYPE node. Name is empty. Value contains DOCTYPE text.
	sfel_node_pi             //!< A PI node. Name contains target. Value contains instructions.
};

/**
 *\ingroup sfel_tags
 *\brief 注册标签回调函数时用到的数据结构
 */
typedef struct sfel_action_s{
	char *tag;
	tag_action_t action;/**<上下文处理函数*/
}sfel_action_t;

/**************************************************************************************************/
/**
 *\defgroup sfel_tags_manager SFEL标签管理
 *\ingroup sfel
 **************************************************************************************************/

/**
 *\ingroup sfel_tags_manager
 *@brief sfel语言的初始化
 *@retval 0 成功
 *@retval -1 失败 
 */
sfpr_int_t sfel_init();
/**
 *\ingroup sfel_tags_manager
 *@brief sfel语言的资源销毁
 *@retval 0 成功
 *@retval -1 失败 
 */
sfpr_int_t sfel_quit();

int sfel_tags_register(char *classname,sfel_tag_t *list);

/**
 *\ingroup sfel_tags_manager
 *@brief 遍历所有的标签
 *@retval 0 成功
 *@retval -1 失败 
 *@remark 暂时用于测试，输出信息到终端
 */
sfpr_int_t sfel_tag_enum();
/**
 *\ingroup sfel_tags_manager
 *@brief 遍历指定标签的所有属性
 *@retval 0 成功
 *@retval -1 失败 
 *@remark 暂时用于测试，输出信息到终端
 */
sfpr_int_t sfel_tag_attr_enum(sfpr_char_t *tagname);

/**
 *\ingroup sfel_tags_manager
 *@brief 根据名称获取指定标签
 *@param tagname 标签名称
 *@return 标签实例
 */
sfel_tag_t *sfel_tag_get(const sfpr_char_t *tagname);

/**
 *\ingroup sfel_tags_manager
 * @brief 给指定标签注册指定上下文下的处理函数
 * @param tagname 标签名称
 * @param context 上下文描述
 * @param action 处理函数
 *@retval 0 成功
 *@retval -1 失败 
 */
sfpr_int_t sfel_action_register(sfpr_char_t *tagname,const sfpr_char_t *context,tag_action_t action);

/**
 *\ingroup sfel_tags_manager
 *@brief 用指定的上下文下注册标签列表
 *@param context 上下文描述
 *@param list 标签及处理函数数组
 *@retval 0 成功
 *@retval -1 失败 
 */
sfpr_int_t sfel_action_registers(const sfpr_char_t *context,sfel_action_t *list);

/**
 *\ingroup sfel_tags_manager
 *@brief 获取指定标签注册的与上下文匹配的功能函数
 *@param tagname 标签名称
 *@param context 上下文信息
 *return 标签在指定上下文下的功能函数
 */
tag_action_t sfel_action_get(const sfpr_char_t *tagname,const sfpr_char_t *context);

/**
 *\defgroup sfel_inst_manager SFEL文件管理
 *\ingroup sfel
***************************************************************************************************/
/**
 *\ingroup sfel_inst_manager
 * @brief sfel执行注册函数的参数
 */
typedef struct sfel_exec_mode_s{
	sfpr_int_t recursive; /**<递归深度，0-只处理当前节点；1-处理当前节点及其子节点；2-处理当前节点及其子节点，以及所有子节点的子节点；依次类推*/
	sfpr_char_t *pattern; /**<模式匹配，允许出现空格，目前支持三种类型:
						 	1. name1,name2,...,nameN 只处理模式中指定名称的这些节点
						 	2. * 处理所有的节点
						 	3. *,!name1,!name2,...,!name3 处理除模式指定名称外的所有节点
						 */
	sfpr_int_t dsfel_on;/**1:启用dsfel功能; 0:禁用<*/
	sfpr_int_t default_action_on;/**1:启用默认的注册函数; 0:禁用<*/
	sfpr_int_t action_retval;/**1回调函数的返回值<*/
	sfpr_int_t action_error_omit;/**1:忽略标签回调函数的错误; 1:不忽略，遇到错误就返回<*/
	sfpr_int_t max_loop;/** 节点循环执行的最大次数<*/
	sfpr_int_t node_depth_looped;/** 执行循环的节点的深度<*/
}sfel_exec_mode_t;


/**
 *\ingroup sfel_inst_manager
 * @brief sfel文件实例
 */
typedef struct sfel_instance_s{
	sfel_document_t *doc;/**<指向解析后的sfel信息*/
	sfel_node_t *root;/**<sfel的根节点*/
	sfpr_char_t path[512]; /**<sfel配置文件路径，支持相对路径*/
	sfpr_int_t save; /**<1: 实例销毁时要保存sfel配置文件; 0:使用后不需要保存*/
	void *script_handle;
}sfel_instance_t;

/**
 *\ingroup sfel_inst_manager
 * @brief 创建一个SFEL实例
 * @param sfel_file sfel文件路径，支持相对路径
 * @param save 1:保存; 0:不保存
 * @param portable 保留参数
 * @return 创建好的sfel实例，如果失败返回NULL
 * @remark 该接口执行成功时，会分配内存，用户使用后需要调用sfel_inst_distroy接口来销毁资源
 */
sfel_instance_t *sfel_inst_create(sfpr_char_t *sfel_file,sfpr_int_t save, sfel_portable_t *portable);

/**
 *\ingroup sfel_inst_manager
 * @brief 创建一个SFEL实例
 * @param sfel_string sfel内容
 * @param portable 保留参数
 * @return 创建好的sfel实例，如果失败返回NULL
 * @remark 该接口执行成功时，会分配内存，用户使用后需要调用sfel_inst_distroy接口来销毁资源
 */
sfel_instance_t *sfel_inst_create_by_string(sfpr_char_t *sfel_string, sfel_portable_t *portable);


/**
 *\ingroup sfel_inst_manager
 * @brief 根据参数指定上下文信息、递归深度、匹配模式来执行相关节点的注册函数
 * @param node 待操作的sfel_node_t实例
 * @param context 用户注册函数时的上下文信息
 * @param param 用户注册函数需要的附加参数
 * @param recursive 
 * @param pattern 模式匹配，允许出现空格，目前支持三种类型:
 *                1. name1,name2,...,nameN 只处理模式中指定名称的这些节点
 *                2. * 处理所有的节点
 *                3. *,!name1,!name2,...,!name3 处理除模式指定名称外的所有节点
 */
sfpr_int_t sfel_inst_exec(sfel_node_t *node,sfpr_char_t *context,sfpr_void_t *param, sfel_exec_mode_t *exec_mode);


/**
 *\ingroup sfel_inst_manager
 * @brief 销毁SFEL实例
 * @param **sfel_inst sfel实例
 * @retval 0 成功
 * @retval -1 失败
 */
sfpr_int_t sfel_inst_destroy(sfel_instance_t **sfel_inst);



/**************************************************************************************************
 * 10. lua函数
***************************************************************************************************/
int sfel_lua_call_func(void *lstate,sfel_node_t *node,char *errcode,char *fmt,...);


/**************************************************************************************************
 * 11. 一些通用宏
***************************************************************************************************/
#define TAG_ATTR_COMMON \
	{"id",				0,0,		"节点唯一ID"},\
	{"name",			0,0,		"节点名称"},\
	{"desc",			0,0,		"节点描述"},\
	{"value",			0,0,		"节点值"},\
	{"datatype",	0,0,		"节点值的数据类型"},\
	{"userdata",	0,0,		"用户数据"},

#define TAG_ATTR_COMMON_WIDGET \
	{"x",				0,0,		"控件位置的X坐标"},\
	{"y",			0,0,		"控件位置的Y坐标"},\
	{"width",			0,0,		"控件宽度，0表示自适应"},\
	{"height",			0,0,		"控件高度，0表示自适应"},

#ifdef __cplusplus
}
#endif

#endif

