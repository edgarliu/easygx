
#ifndef SFEL_XML_H
#define SFEL_XML_H

/**
 * @file sfel_xml.h
 * @brief SFEL库中实现xml接口
 * @author nahaitao
 * @version 0.1.0
 * @date 2012-02-21
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "sfel_global.h"


/*
	DOM 方式
*/

/**************************************************************************************************/
/**
 *\defgroup sfel_xml xml文件解析模块
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup sfel_xml_doc 文档操作函数
 *\ingroup sfel_xml
 **************************************************************************************************/
 
/**
 *\ingroup sfel_xml_doc
 * @brief 创建document
 * @return sfel_document_t指针
 */
sfel_document_t *sfel_xml_create_document();

/**
 *\ingroup sfel_xml_doc
 * @brief 解析xml文件
 * @param[in] sfel_file sfel_file_t指针，指向打开的xml文件
 * @param[in] portable 暂时未使用
 * @return sfel_document_t指针，指向解析后的xml信息
 */
sfel_document_t *sfel_xml_parse_file(const sfpr_char_t *sfel_file, sfel_portable_t *portable);

/**
 *\ingroup sfel_xml_doc
 * @brief 解析xml字符串
 * @param[in] sfel_data xml文件格式的字符串缓冲
 * @param[in] portable 暂时未使用
 * @return sfel_document_t指针，指向解析后的xml信息
 */
sfel_document_t *sfel_xml_parse_string(sfpr_char_t *sfel_data, sfel_portable_t *portable);

/**
 *\ingroup sfel_xml_doc
 * @brief 添加声明
 * @param[in] sfel_document sfel_document_t指针，指向解析后的xml信息
 * @param[in] version 版本,如"1.0"
 * @param[in] encode 编码，如"gb2312"
 * @return 0:成功; 非0:失败
 */
sfpr_int_t sfel_xml_declare(sfel_document_t *sfel_document, const sfpr_char_t *version,const sfpr_char_t *encode);

/**
 *\ingroup sfel_xml_doc
 * @brief 保存xml文件
 * @param[in] sfel_document sfel_document_t指针，指向解析后的xml信息
 * @param[in] sfel_file xml文件路径
 * @return 0:成功; 非0:失败
 */
sfpr_int_t sfel_xml_save(sfel_document_t *sfel_document, const sfpr_char_t *sfel_file);

/**
 *\ingroup sfel_xml_doc
 * @brief 将sfel配置文件的内容输入到一个缓冲中
 * @param[in] *doc 指向解析后的xml信息
 * @param[in] *output 缓冲区
 * @param[in] size 缓冲区大小
 * @remark 参数output要求用户在外部分配内容，且内存足够大
 */
sfpr_int_t sfel_xml_doc_print(sfel_document_t *doc, sfpr_char_t *output,int size);

/**
 *\ingroup sfel_xml_doc
 * @brief 释放sfel_document_t指针指向的资源
 * @param[in] sfel_document sfel_document_t指针，指向解析后的xml信息
 * @return 0:成功; 非0:失败
 */
sfpr_int_t sfel_xml_destroy(sfel_document_t *sfel_document);

/**
 *\ingroup sfel_xml_doc
 * @brief 解析xml字符串
 * @param[in] sfel_document sfel_document_t指针，指向解析后的xml信息
 * @return sfel_node_t指针，指向xml的根节点
 */
sfel_node_t *sfel_xml_get_root(sfel_document_t *sfel_document);

/**************************************************************************************************/
/**
 *\defgroup sfel_xml_node 节点操作函数
 *\ingroup sfel_xml
 **************************************************************************************************/

/**
 *\ingroup sfel_xml_node
 * @brief 获取节点的类型
 * @param[in] sfel_node xml节点实例
 * @remark 
 *	 0: xml文件
 *	 1: xml标签
 *	 2: xml标签间的文本
 *	 3: CDATA节点
 *	 4: xml注释
 *	 5: xml声明 <?xml version="1.0" encoding="ISO-8859-1"?>
 *	 6: DOCTYPE节点
 *	 7: PI节点
 */
const sfpr_int_t sfel_xml_node_get_type(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief 获取指定节点的名称
 * @param[in] sfel_node 指定节点
 * @return 指定节点的名称
 */
const sfpr_char_t *sfel_xml_node_get_name(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_base
 * @brief 获取指定节点的值
 * @param[in] sfel_node 指定节点
 * @return 指定节点的值
 * @remark 通常通过获取节点的数据子节点方式来获取节点的内容，用来替换该接口
 */
const sfpr_char_t *sfel_xml_node_get_value(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief 获取指定节点的document
 * @param[in] sfel_node 指定节点
 * @return 指定document
 */
sfel_document_t *sfel_xml_node_get_document(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief 获取指定节点的第一个子节点
 * @param[in] sfel_node 指定节点
 * @return 指定节点的第一个子节点
 */
sfel_node_t *sfel_xml_node_get_child(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief 获取指定节点的下一个相邻节点
 * @param[in] sfel_node 指定节点
 * @return 指定节点的下一个相邻节点
 */
sfel_node_t *sfel_xml_node_get_sibling(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief 获取指定节点的父节点
 * @param[in] sfel_node 指定节点
 * @return 指定节点的父节点
 */
sfel_node_t *sfel_xml_node_get_parent(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief 按照设定的条件查找指定节点的所有子节点及子节点的子节点;并且调用用户处理回调函数
 * @param[in] sfel_node 指定节点
 * @param[in] depth 递归深度，0:所有子节点；1:指定节点的子节点...
 * @param[in] node_name 待查找节点名称，若为NULL，则查找所有子节点
 * @param[in] attr_name 属性名称，若为NULL，则不对属性进行验证
 * @param[in] attr_value 属性值，若为NULL，则不验证属性值
 * @param[in] node_handle 用户回调函数
 * @param[in] param 回调函数参数
 * @return 找到的节点个数
 */
sfpr_int_t sfel_xml_node_find_all(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_node_cb_t node_handle, void *param);

/**
 *\ingroup sfel_xml_node
 * @brief 按照设定的条件查找指定节点的所有子节点及子节点的子节点，返回满足条件的节点
 * @param[in] sfel_node 指定节点
 * @param[in] depth 递归深度，0:所有子节点；1:指定节点的子节点...
 * @param[in] node_name 待查找节点名称，若为NULL，则查找所有子节点
 * @param[in] attr_name 属性名称，若为NULL，则不对属性进行验证
 * @param[in] attr_value 属性值，若为NULL，则不验证属性值
 * @return 满足条件的节点或者NULL
 */
sfel_node_t *sfel_xml_node_find(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value);

/**
 *\ingroup sfel_xml_node
 * @brief 按照指定条件查找指定节点之后的下一个满足条件的标签
 * @param[in] sfel_node 指定节点
 * @param[in] node_name 待查找节点名称，若为NULL，则查找所有子节点
 * @param[in] attr_name 属性名称，若为NULL，则不对属性进行验证
 * @param[in] attr_value 属性值，若为NULL，则不验证属性值
 * @return 找到的节点
 */
sfel_node_t* sfel_xml_node_find_next(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value);

/**
 *\ingroup sfel_xml_node
 * @brief 按照指定条件查找指定节点的所有子节点，并执行回调函数
 * @param[in] sfel_node 指定节点
 * @param[in] node_name 待查找节点名称，若为NULL，则查找所有子节点
 * @param[in] attr_name 属性名称，若为NULL，则不对属性进行验证
 * @param[in] attr_value 属性值，若为NULL，则不验证属性值
 * @param[in] node_handle 用户回调函数
 * @param[in] param 回调函数参数
 * @return 找到的节点个数
 */
sfpr_int_t sfel_xml_node_enum_child(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_node_cb_t node_handle, void *param);

/**
 *\ingroup sfel_xml_node
 * @brief 按照指定的条件查找属性，并调用用户回调函数
 * @param[in] sfel_node 指定节点
 * @param[in] attr_name 属性名称，若为NULL，则查找所有属性
 * @param[in] attr_value 属性值，若为NULL，则不验证属性值
 * @param[in] attr_handle 用户回调函数
 * @param[in] param 回调函数参数
 * @return 找到的属性个数
 */
sfpr_int_t sfel_xml_node_enum_attr(sfel_node_t *sfel_node,const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_attr_cb_t attr_handle, void *param);

/**
 *\ingroup sfel_xml_node
 * @brief 增加新的节点
 * @param[in] sfel_node 所属父节点
 * @param[in] node_name 新节点名称
 * @param[in] node_value 新节点值
 * @return 新增加的节点
 */
sfel_node_t *sfel_xml_node_append_child_by_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value);

/**
 *\ingroup sfel_xml_node
 * @brief 增加新的节点
 * @param[in] doc 指向解析后的xml信息
 * @param[in] sfel_node 所属父节点
 * @param[in] child_node 新节点名称
 * @return 新增加的节点
 */
sfel_node_t *sfel_xml_node_append_child_by_node(sfel_node_t *sfel_node, sfel_node_t *child_node);

/**
 *\ingroup sfel_xml_node
 * @brief 将一个节点及其所有子信息添加到另一个节点中
 * @param sfel_node 目标节点
 * @param child_node 子节点
 * @return 新增加的节点
 */
sfel_node_t* sfel_xml_node_copy_to_child(sfel_node_t *sfel_node, sfel_node_t *child_node);

/**
 *\ingroup sfel_xml_node
 * @brief 增加新的节点,作为父节点的第一个标签
 * @param[in] doc 指向解析后的xml信息
 * @param[in] sfel_node 所属父节点
 * @param[in] node_name 新节点名称
 * @param[in] node_value 新节点值
 * @return 新增加的节点
 */
sfel_node_t *sfel_xml_node_prepend_child_by_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value);

/**
 *\ingroup sfel_xml_node
 * @brief 插入新的节点
 * @param[in] sfel_node 待插入节点位置
 * @param[in] new_node 新节点
 * @return 新增加的节点
 */
sfel_node_t *sfel_xml_node_insert_before(sfel_node_t *sfel_node, sfel_node_t *new_node);

/**
 *\ingroup sfel_xml_node
 * @brief 修改指定节点的名称
 * @param[in] sfel_node 指定节点
 * @param[in] node_name 节点值
 * @return 修改后的节点
 * @remark 该接口只适用于node_element类型的标签
 */
sfel_node_t *sfel_xml_node_set_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name);

/**
 *\ingroup sfel_xml_node
 * @brief 修改指定节点的值
 * @param[in] sfel_node 指定节点
 * @param[in] node_value 节点值
 * @return 修改后的节点
 * @remark 该接口只适用于node_element类型的标签
 */
sfel_node_t *sfel_xml_node_set_value(sfel_node_t *sfel_node, const sfpr_char_t *node_value);

/**
 *\ingroup sfel_xml_node
 * @brief 克隆指定节点
 * @param[in]*doc 指向解析后的xml信息
 * @param[in] sfel_node 待克隆的节点
 * @return 克隆后的节点
 */
sfel_node_t *sfel_xml_node_after_clone(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief 将节点的内容输入到一个缓冲中
 * @param[in] *doc 指向解析后的xml信息
 * @param[in] *output 转换后的数据
 * @param[in] size 缓冲区大小
 * @remark 参数output要求用户在外部分配内容，且内存足够大
 */
sfpr_int_t sfel_xml_node_print(sfel_node_t *sfel_node, sfpr_char_t *output,int size);

/**
 *\ingroup sfel_xml_node
 * @brief 移除指定节点
 * @param[in] sfel_node 待删除的节点
 * @return 0:成功; 非0:失败
 */
sfpr_int_t sfel_xml_node_delete(sfel_node_t *sfel_node);

/**************************************************************************************************/
/**
 *\defgroup sfel_xml_node_attr 节点属性操作函数
 *\ingroup sfel_xml
 **************************************************************************************************/

/**
 *\ingroup sfel_xml_node_attr
 * @brief 获取指定节点的第一个属性
 * @param sfel_node 指定节点
 * @return 指定节点的第一个属性，如果没有则为NULL
 */
sfel_attr_t *sfel_xml_node_get_first_attr(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node_attr
 * @brief 获取指定属性的下一个相邻属性
 * @param[in] sfel_attr 指定属性
 * @return 指定属性的下一个属性，如果没有则为NULL
 */
sfel_attr_t *sfel_xml_attr_get_sibling(sfel_attr_t *sfel_attr);

/**
 *\ingroup sfel_xml_node_attr
 * @brief 查找指定名称的属性
 * @param[in] sfel_node 指定节点
 * @param[in] attr_name 属性名称
 * @return 找到的属性
 */
sfel_attr_t *sfel_xml_attr_get(sfel_node_t *sfel_node, const sfpr_char_t *attr_name);

/**
 *\ingroup sfel_xml_node_attr
 * @brief 查询指定属性的名称
 * @param[in] sfel_attr sfel_attr_t指针
 * @return 属性名称
 */
sfpr_char_t *sfel_xml_attr_get_aname(sfel_attr_t *sfel_attr);

/**
 *\ingroup sfel_xml_node_attr
 * @brief 查询指定属性的值
 * @param[in] sfel_attr sfel_attr_t指针
 * @return 属性值
 */
sfpr_char_t *sfel_xml_attr_get_avalue(sfel_attr_t *sfel_attr);

/**
 *\ingroup sfel_xml_node_attr
 * @brief 查找指定名称的属性的值
 * @param[in] sfel_node 指定节点
 * @param[in] attr_name 属性名称
 * @return 属性值或者NULL
 */
sfpr_char_t *sfel_xml_attr_get_value(sfel_node_t *sfel_node, const sfpr_char_t *attr_name);

/**
 *\ingroup sfel_xml_node_attr
 * @brief 增加新的属性
 * @param[in] sfel_node 所属父节点
 * @param[in] attr_name 新属性名称
 * @param[in] attr_value 新属性值
 * @return 新增加的属性
 */
sfel_attr_t *sfel_xml_attr_add(sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *attr_value);

/**
 *\ingroup sfel_xml_node_attr
 * @brief 修改指定属性的值
 * @param[in] sfel_node 指定节点
 * @param[in] attr_name 属性名
 * @param[in] new_attr_value 待修改值
 * @return 修改后的属性
 */
sfel_attr_t *sfel_xml_attr_update(sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *new_attr_value);

/**
 *\ingroup sfel_xml_node_attr
 * @brief 删除指定名称的属性
 * @param[in] sfel_node 指定节点
 * @param[in] attr_name 待删除属性名称
 * @return 0:成功; 非0:失败
 */
sfpr_int_t sfel_xml_attr_delete(sfel_node_t *sfel_node, const sfpr_char_t *attr_name);

/**************************************************************************************************/
/**
 *\defgroup sfel_xml_sax SAX函数
 *\ingroup sfel_xml
 **************************************************************************************************/
sfpr_int_t sfel_sax_parse_file(sfpr_char_t *file,sfel_sax_cb_t sax,void *sax_data,sfel_portable_t *portable);
sfpr_int_t sfel_sax_parse_string(sfpr_char_t *file,sfel_sax_cb_t sax,void *sax_data,sfel_portable_t *portable);


#ifdef __cplusplus
}
#endif

#endif
