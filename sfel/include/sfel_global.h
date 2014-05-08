
#ifndef SFEL_GLOBAL_H
#define SFEL_GLOBAL_H

#include "sfpr_global.h"

/**************************************************************************************************/
/**
 *\defgroup sfel_global sfel全局数据类型
 *
 **************************************************************************************************/

/**
 *\ingroup sfel_global
 * @brief 解析后的xml信息
 */
typedef sfpr_void_t sfel_document_t;

/**
 *\ingroup sfel_global
 * @brief xml节点
 */
typedef sfpr_void_t sfel_node_t;

/**
 *\ingroup sfel_global
 * @brief xml属性
 */
typedef sfpr_void_t sfel_attr_t;

/**
 *\ingroup sfel_global
 * @brief int类型
 */
typedef sfpr_int_t sfel_type_t;

/**
 *\ingroup sfel_global
 * @brief 暂时保留
 */
typedef struct sfel_portable_s{
	sfpr_void_t *reserve;
}sfel_portable_t;

/**
 *\ingroup sfel_global
 * @brief xml节点处理回调函数
 */
typedef sfpr_int_t (*sfel_xml_node_cb_t)(sfel_node_t *node, sfpr_void_t *param);

/**
 *\ingroup sfel_global
 * @brief xml属性处理回调函数
 */
typedef sfpr_int_t (*sfel_xml_attr_cb_t)(sfel_node_t *node, sfel_attr_t *attr, sfpr_void_t *param);

/**
 *\ingroup sfel_global
 * @brief sax回调函数
 */
typedef sfpr_int_t (*sfel_sax_cb_t)(sfel_node_t *node, sfpr_void_t *param);

/**
 *\ingroup sfel_global
 * @brief 节点开始
 */
#define SFEL_NODE_START  0

/**
 *\ingroup sfel_global
 * @brief 节点结束
 */
#define SFEL_NODE_END    1

/**
 *\ingroup sfel_tags
 *\brief 循环调用action函数的返回值标志
 */
#define SFEL_NODE_LOOP_BEGIN  0x6f4f2fef

/**
 *\ingroup sfel_tags
 *\brief 循环调用action函数的返回值标志
 */
#define SFEL_NODE_LOOP_END  0x6f4f2fdf

/**
 *\ingroup sfel_tags
 *\brief 循环调用action函数的返回值标志
 */
#define SFEL_NODE_LOOP  0x6f4f2fff

/**
 *\ingroup sfel_tags
 *\brief 忽略子节点action函数的返回值标志
 */
#define SFEL_NODE_SKIP_CHILD  0x6f4f2f1f

#endif
