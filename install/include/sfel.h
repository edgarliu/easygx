
#ifndef SFEL_H
#define SFEL_H

/**
 *\file sfel.h
 *\brief SFEL��XML��֮���ٷ�װһ�㣬ʵ��SFEL����\n
 *XML������ʵ��SFEL���﷨\n
 *����ʵ��SFEL������
 *\author ����ǿ
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
 *\defgroup sfel sfel��ģ��
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup sfel_tags SFEL��ǩ����
 *\ingroup sfel
 **************************************************************************************************/

/**
 *\ingroup sfel_tags
 *\brief ��ǩ����
 */
typedef struct sfel_tag_attr_s{
	sfpr_char_t *name;/**<������*/
	sfpr_uint32_t type;/**<���Ե���������*/
	sfpr_uint32_t flags;/**<��������ʱ����*/
	sfpr_char_t *desc;/**<��������*/
}sfel_tag_attr_t;

/**
 *\ingroup sfel_tags
 * @brief ��ǩ�����Ļص������Ĳ���
 */ 
#define ACTION_ARGS (sfel_node_t *node,sfpr_void_t *param,sfpr_int_t depth, sfpr_int_t status)

/**
 *\ingroup sfel_tags
 * @brief ��ǩ�����Ļص�����
 * @param status 0:�ڵ㿪ʼʱ; 1:�ڵ㿪ʼ��
 */ 
typedef sfpr_int_t(*tag_action_t)ACTION_ARGS;

/**
 *\ingroup sfel_tags
 * @brief �Ѿ�������һ����
 */ 
#define REGISTER_ACTION(tag_class,tag_name)  sfel_action_register(#tag_name,tag_class->context,tag_class->tag_##tag_name)

/**
 *\ingroup sfel_tags
 *\brief ��ǩ�Ļص�����
 */
typedef struct sfel_tag_cb_s{
	const sfpr_char_t *context;/**<�ص��������Ļ���*/
	tag_action_t action;/**<�����Ĵ�����*/	
}sfel_tag_cb_t;

/**
 *\ingroup sfel_tags
 *\brief ��ǩ������
 */
typedef struct sfel_tag_s{
	sfpr_char_t *name; /**<��ǩ��*/
	sfpr_char_t *class_name; /**<����*/
	sfpr_char_t *subclass;/**<������*/
	sfpr_uint32_t flag;/**<������������ʱ����*/
	sfpr_uint32_t event;/**<�¼�*/
	sfel_tag_attr_t *attr;/**<���Զ�����Ϣ*/
	sfpr_char_t *desc;/**<������Ϣ*/
	tag_action_t default_action;/**<Ĭ�������Ļص�����*/
	sfpr_int_t action_count;/**<��ǰע��������Ĵ���������*/
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
 *\brief ע���ǩ�ص�����ʱ�õ������ݽṹ
 */
typedef struct sfel_action_s{
	char *tag;
	tag_action_t action;/**<�����Ĵ�����*/
}sfel_action_t;

/**************************************************************************************************/
/**
 *\defgroup sfel_tags_manager SFEL��ǩ����
 *\ingroup sfel
 **************************************************************************************************/

/**
 *\ingroup sfel_tags_manager
 *@brief sfel���Եĳ�ʼ��
 *@retval 0 �ɹ�
 *@retval -1 ʧ�� 
 */
sfpr_int_t sfel_init();
/**
 *\ingroup sfel_tags_manager
 *@brief sfel���Ե���Դ����
 *@retval 0 �ɹ�
 *@retval -1 ʧ�� 
 */
sfpr_int_t sfel_quit();

int sfel_tags_register(char *classname,sfel_tag_t *list);

/**
 *\ingroup sfel_tags_manager
 *@brief �������еı�ǩ
 *@retval 0 �ɹ�
 *@retval -1 ʧ�� 
 *@remark ��ʱ���ڲ��ԣ������Ϣ���ն�
 */
sfpr_int_t sfel_tag_enum();
/**
 *\ingroup sfel_tags_manager
 *@brief ����ָ����ǩ����������
 *@retval 0 �ɹ�
 *@retval -1 ʧ�� 
 *@remark ��ʱ���ڲ��ԣ������Ϣ���ն�
 */
sfpr_int_t sfel_tag_attr_enum(sfpr_char_t *tagname);

/**
 *\ingroup sfel_tags_manager
 *@brief �������ƻ�ȡָ����ǩ
 *@param tagname ��ǩ����
 *@return ��ǩʵ��
 */
sfel_tag_t *sfel_tag_get(const sfpr_char_t *tagname);

/**
 *\ingroup sfel_tags_manager
 * @brief ��ָ����ǩע��ָ���������µĴ�����
 * @param tagname ��ǩ����
 * @param context ����������
 * @param action ������
 *@retval 0 �ɹ�
 *@retval -1 ʧ�� 
 */
sfpr_int_t sfel_action_register(sfpr_char_t *tagname,const sfpr_char_t *context,tag_action_t action);

/**
 *\ingroup sfel_tags_manager
 *@brief ��ָ������������ע���ǩ�б�
 *@param context ����������
 *@param list ��ǩ������������
 *@retval 0 �ɹ�
 *@retval -1 ʧ�� 
 */
sfpr_int_t sfel_action_registers(const sfpr_char_t *context,sfel_action_t *list);

/**
 *\ingroup sfel_tags_manager
 *@brief ��ȡָ����ǩע�����������ƥ��Ĺ��ܺ���
 *@param tagname ��ǩ����
 *@param context ��������Ϣ
 *return ��ǩ��ָ���������µĹ��ܺ���
 */
tag_action_t sfel_action_get(const sfpr_char_t *tagname,const sfpr_char_t *context);

/**
 *\defgroup sfel_inst_manager SFEL�ļ�����
 *\ingroup sfel
***************************************************************************************************/
/**
 *\ingroup sfel_inst_manager
 * @brief sfelִ��ע�ắ���Ĳ���
 */
typedef struct sfel_exec_mode_s{
	sfpr_int_t recursive; /**<�ݹ���ȣ�0-ֻ����ǰ�ڵ㣻1-����ǰ�ڵ㼰���ӽڵ㣻2-����ǰ�ڵ㼰���ӽڵ㣬�Լ������ӽڵ���ӽڵ㣻��������*/
	sfpr_char_t *pattern; /**<ģʽƥ�䣬������ֿո�Ŀǰ֧����������:
						 	1. name1,name2,...,nameN ֻ����ģʽ��ָ�����Ƶ���Щ�ڵ�
						 	2. * �������еĽڵ�
						 	3. *,!name1,!name2,...,!name3 �����ģʽָ������������нڵ�
						 */
	sfpr_int_t dsfel_on;/**1:����dsfel����; 0:����<*/
	sfpr_int_t default_action_on;/**1:����Ĭ�ϵ�ע�ắ��; 0:����<*/
	sfpr_int_t action_retval;/**1�ص������ķ���ֵ<*/
	sfpr_int_t action_error_omit;/**1:���Ա�ǩ�ص������Ĵ���; 1:�����ԣ���������ͷ���<*/
	sfpr_int_t max_loop;/** �ڵ�ѭ��ִ�е�������<*/
	sfpr_int_t node_depth_looped;/** ִ��ѭ���Ľڵ�����<*/
}sfel_exec_mode_t;


/**
 *\ingroup sfel_inst_manager
 * @brief sfel�ļ�ʵ��
 */
typedef struct sfel_instance_s{
	sfel_document_t *doc;/**<ָ��������sfel��Ϣ*/
	sfel_node_t *root;/**<sfel�ĸ��ڵ�*/
	sfpr_char_t path[512]; /**<sfel�����ļ�·����֧�����·��*/
	sfpr_int_t save; /**<1: ʵ������ʱҪ����sfel�����ļ�; 0:ʹ�ú���Ҫ����*/
	void *script_handle;
}sfel_instance_t;

/**
 *\ingroup sfel_inst_manager
 * @brief ����һ��SFELʵ��
 * @param sfel_file sfel�ļ�·����֧�����·��
 * @param save 1:����; 0:������
 * @param portable ��������
 * @return �����õ�sfelʵ�������ʧ�ܷ���NULL
 * @remark �ýӿ�ִ�гɹ�ʱ��������ڴ棬�û�ʹ�ú���Ҫ����sfel_inst_distroy�ӿ���������Դ
 */
sfel_instance_t *sfel_inst_create(sfpr_char_t *sfel_file,sfpr_int_t save, sfel_portable_t *portable);

/**
 *\ingroup sfel_inst_manager
 * @brief ����һ��SFELʵ��
 * @param sfel_string sfel����
 * @param portable ��������
 * @return �����õ�sfelʵ�������ʧ�ܷ���NULL
 * @remark �ýӿ�ִ�гɹ�ʱ��������ڴ棬�û�ʹ�ú���Ҫ����sfel_inst_distroy�ӿ���������Դ
 */
sfel_instance_t *sfel_inst_create_by_string(sfpr_char_t *sfel_string, sfel_portable_t *portable);


/**
 *\ingroup sfel_inst_manager
 * @brief ���ݲ���ָ����������Ϣ���ݹ���ȡ�ƥ��ģʽ��ִ����ؽڵ��ע�ắ��
 * @param node ��������sfel_node_tʵ��
 * @param context �û�ע�ắ��ʱ����������Ϣ
 * @param param �û�ע�ắ����Ҫ�ĸ��Ӳ���
 * @param recursive 
 * @param pattern ģʽƥ�䣬������ֿո�Ŀǰ֧����������:
 *                1. name1,name2,...,nameN ֻ����ģʽ��ָ�����Ƶ���Щ�ڵ�
 *                2. * �������еĽڵ�
 *                3. *,!name1,!name2,...,!name3 �����ģʽָ������������нڵ�
 */
sfpr_int_t sfel_inst_exec(sfel_node_t *node,sfpr_char_t *context,sfpr_void_t *param, sfel_exec_mode_t *exec_mode);


/**
 *\ingroup sfel_inst_manager
 * @brief ����SFELʵ��
 * @param **sfel_inst sfelʵ��
 * @retval 0 �ɹ�
 * @retval -1 ʧ��
 */
sfpr_int_t sfel_inst_destroy(sfel_instance_t **sfel_inst);



/**************************************************************************************************
 * 10. lua����
***************************************************************************************************/
int sfel_lua_call_func(void *lstate,sfel_node_t *node,char *errcode,char *fmt,...);


/**************************************************************************************************
 * 11. һЩͨ�ú�
***************************************************************************************************/
#define TAG_ATTR_COMMON \
	{"id",				0,0,		"�ڵ�ΨһID"},\
	{"name",			0,0,		"�ڵ�����"},\
	{"desc",			0,0,		"�ڵ�����"},\
	{"value",			0,0,		"�ڵ�ֵ"},\
	{"datatype",	0,0,		"�ڵ�ֵ����������"},\
	{"userdata",	0,0,		"�û�����"},

#define TAG_ATTR_COMMON_WIDGET \
	{"x",				0,0,		"�ؼ�λ�õ�X����"},\
	{"y",			0,0,		"�ؼ�λ�õ�Y����"},\
	{"width",			0,0,		"�ؼ���ȣ�0��ʾ����Ӧ"},\
	{"height",			0,0,		"�ؼ��߶ȣ�0��ʾ����Ӧ"},

#ifdef __cplusplus
}
#endif

#endif

