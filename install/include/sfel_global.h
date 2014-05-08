
#ifndef SFEL_GLOBAL_H
#define SFEL_GLOBAL_H

#include "sfpr_global.h"

/**************************************************************************************************/
/**
 *\defgroup sfel_global sfelȫ����������
 *
 **************************************************************************************************/

/**
 *\ingroup sfel_global
 * @brief �������xml��Ϣ
 */
typedef sfpr_void_t sfel_document_t;

/**
 *\ingroup sfel_global
 * @brief xml�ڵ�
 */
typedef sfpr_void_t sfel_node_t;

/**
 *\ingroup sfel_global
 * @brief xml����
 */
typedef sfpr_void_t sfel_attr_t;

/**
 *\ingroup sfel_global
 * @brief int����
 */
typedef sfpr_int_t sfel_type_t;

/**
 *\ingroup sfel_global
 * @brief ��ʱ����
 */
typedef struct sfel_portable_s{
	sfpr_void_t *reserve;
}sfel_portable_t;

/**
 *\ingroup sfel_global
 * @brief xml�ڵ㴦��ص�����
 */
typedef sfpr_int_t (*sfel_xml_node_cb_t)(sfel_node_t *node, sfpr_void_t *param);

/**
 *\ingroup sfel_global
 * @brief xml���Դ���ص�����
 */
typedef sfpr_int_t (*sfel_xml_attr_cb_t)(sfel_node_t *node, sfel_attr_t *attr, sfpr_void_t *param);

/**
 *\ingroup sfel_global
 * @brief sax�ص�����
 */
typedef sfpr_int_t (*sfel_sax_cb_t)(sfel_node_t *node, sfpr_void_t *param);

/**
 *\ingroup sfel_global
 * @brief �ڵ㿪ʼ
 */
#define SFEL_NODE_START  0

/**
 *\ingroup sfel_global
 * @brief �ڵ����
 */
#define SFEL_NODE_END    1

/**
 *\ingroup sfel_tags
 *\brief ѭ������action�����ķ���ֵ��־
 */
#define SFEL_NODE_LOOP_BEGIN  0x6f4f2fef

/**
 *\ingroup sfel_tags
 *\brief ѭ������action�����ķ���ֵ��־
 */
#define SFEL_NODE_LOOP_END  0x6f4f2fdf

/**
 *\ingroup sfel_tags
 *\brief ѭ������action�����ķ���ֵ��־
 */
#define SFEL_NODE_LOOP  0x6f4f2fff

/**
 *\ingroup sfel_tags
 *\brief �����ӽڵ�action�����ķ���ֵ��־
 */
#define SFEL_NODE_SKIP_CHILD  0x6f4f2f1f

#endif
