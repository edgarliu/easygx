
#ifndef SFEL_DSFEL_H
#define SFEL_DSFEL_H

/**
 *\file sfel_dsfel.h
 *\brief ʵ�ֶ�̬sfel���ܣ�֧����sfel�����ļ���Ƕ�׶�̬�ű�
 *\author �Ǻ���
 *\version 0.1.0
 *\date 2012-02-27
 */

/**************************************************************************************************
 * DSFEL: ��̬SFEL
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
 *\defgroup dsfel ��̬sfel
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup dsfel_base ��������
 *\ingroup dsfel
 **************************************************************************************************/


/**
 *\ingroup dsfel_base
 * @brief ��̬sfel�нű�����������������
 */
#define SFEL_DSFEL_MAX_ARGV	10
#define SFEL_DSFEL_MAX_RETURN_SIZE 4096

/**
 *\ingroup dsfel_base
 * @brief dsfelִ�н������
 */
typedef enum dsfel_result_type_s{
	dsfel_result_int = 0,/**<����*/
	dsfel_result_pointer,/**<ָ��*/
	dsfel_result_string/**<�ַ���*/
}dsfel_result_type_t;

/**
 *\ingroup dsfel_base
 * @brief dsfelִ�н��
 */
typedef struct dsfel_handle_result_s
{
    dsfel_result_type_t type;/**<����ֵ����*/
	   sfpr_int_t ret; /**<���ͷ���ֵ*/
	   sfpr_void_t *pointer;/**<ָ�뷵��ֵ*/
	   sfpr_char_t *string;/**<�ַ���*/
	sfpr_int_t size; /**<string�ֶε����ֵ*/
}dsfel_handle_result_t;

/**
 *\ingroup dsfel_base
 * @brief ��̬sfel�����нű������ĸ�ʽ
 */
typedef sfpr_int_t (*dsfel_cb_t)(sfel_document_t *doc, sfpr_void_t *argv[SFEL_DSFEL_MAX_ARGV],dsfel_handle_result_t *dsfel_result);

/**
 *\ingroup dsfel_base
 * @brief ��̬sfel
 */
typedef struct dsfel_s{
	sfpr_char_t *name; /**<dsfelԪ������*/
	sfpr_int_t type;/**<dsfelԪ������*/
	union{
		dsfel_cb_t cb;/**<����Ԫ�ض�Ӧ�Ļص�����*/
		sfpr_void_t *var;/**<����Ԫ�ض�Ӧ�ı���*/
	}body;
	sfpr_void_t *argv[SFEL_DSFEL_MAX_ARGV];/**<����*/
}dsfel_t;

int dsfel_func_register(char *name, dsfel_cb_t cb);

/**
 *\ingroup dsfel_base
 * @brief dsfelԪ������
 */
enum dsfel_type_s{
	dsfel_varient = 0,/**<����*/
	dsfel_function/**<����*/
}dsfel_type_t;

/**
 *\ingroup dsfel_base
 * @brief ������̬sfel�ű�
 * @param instance sfel_instance_tʵ��
 * @param script dsfel�ű�
 * @param *output ִ�н�������û��ڽӿ��ⲿ������ڴ�
 * @return 0-�ɹ�; -1-ʧ��
 * @remark �ýӿ�Ҫ��dsfel�ű��������¹�Լ:
 * \n
 *         1. Ŀǰֻ֧�ֺ���������
 * \n
 *         2. ���������ø�ʽ:${func(param,param,...,param)}
 * \n
 *                           ${func(param,param,...,param).property}
 * \n  
 *		   3. �����໥Ƕ��ʱ������ʹ����ȫ���ã�����:
 * \n
 *            ${func(param,${func1()},...,param)}
 * \n
 *            ${func(param,${func1()},...,param).property}
 * \n
 *         4. �����dsfel�д��ڷ���"."����������ߺ�������Ϊsfel_node_t���ͣ����Ի�ȡ�ڵ��attributeֵ
 * \n
 *         5. �����Ҫʹ�����ԣ�ֻ֧��һ�����Ե���, ��sfel_node.attr;
              ��֧�ֶ�����ԣ���sfel_node.attr.attr1
 */
sfpr_int_t dsfel_compile(sfel_document_t *sfel_document, const sfpr_char_t *script, sfpr_char_t *output,int outsz);

/**
 *\ingroup dsfel_base
 *@brief �ж�ָ���Ľű��Ƿ���Ч��dsfel�ű�
 *@param script ������ַ���
 *@retval 0: ��Ч��dsfel�ű�
 *@retval -1:��dsfel�ű�
 */
sfpr_int_t dsfel_isvalid(const sfpr_char_t *script);

#ifdef __cplusplus
}
#endif

#endif
