
#ifndef SFEL_XML_H
#define SFEL_XML_H

/**
 * @file sfel_xml.h
 * @brief SFEL����ʵ��xml�ӿ�
 * @author nahaitao
 * @version 0.1.0
 * @date 2012-02-21
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "sfel_global.h"


/*
	DOM ��ʽ
*/

/**************************************************************************************************/
/**
 *\defgroup sfel_xml xml�ļ�����ģ��
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup sfel_xml_doc �ĵ���������
 *\ingroup sfel_xml
 **************************************************************************************************/
 
/**
 *\ingroup sfel_xml_doc
 * @brief ����document
 * @return sfel_document_tָ��
 */
sfel_document_t *sfel_xml_create_document();

/**
 *\ingroup sfel_xml_doc
 * @brief ����xml�ļ�
 * @param[in] sfel_file sfel_file_tָ�룬ָ��򿪵�xml�ļ�
 * @param[in] portable ��ʱδʹ��
 * @return sfel_document_tָ�룬ָ��������xml��Ϣ
 */
sfel_document_t *sfel_xml_parse_file(const sfpr_char_t *sfel_file, sfel_portable_t *portable);

/**
 *\ingroup sfel_xml_doc
 * @brief ����xml�ַ���
 * @param[in] sfel_data xml�ļ���ʽ���ַ�������
 * @param[in] portable ��ʱδʹ��
 * @return sfel_document_tָ�룬ָ��������xml��Ϣ
 */
sfel_document_t *sfel_xml_parse_string(sfpr_char_t *sfel_data, sfel_portable_t *portable);

/**
 *\ingroup sfel_xml_doc
 * @brief �������
 * @param[in] sfel_document sfel_document_tָ�룬ָ��������xml��Ϣ
 * @param[in] version �汾,��"1.0"
 * @param[in] encode ���룬��"gb2312"
 * @return 0:�ɹ�; ��0:ʧ��
 */
sfpr_int_t sfel_xml_declare(sfel_document_t *sfel_document, const sfpr_char_t *version,const sfpr_char_t *encode);

/**
 *\ingroup sfel_xml_doc
 * @brief ����xml�ļ�
 * @param[in] sfel_document sfel_document_tָ�룬ָ��������xml��Ϣ
 * @param[in] sfel_file xml�ļ�·��
 * @return 0:�ɹ�; ��0:ʧ��
 */
sfpr_int_t sfel_xml_save(sfel_document_t *sfel_document, const sfpr_char_t *sfel_file);

/**
 *\ingroup sfel_xml_doc
 * @brief ��sfel�����ļ����������뵽һ��������
 * @param[in] *doc ָ��������xml��Ϣ
 * @param[in] *output ������
 * @param[in] size ��������С
 * @remark ����outputҪ���û����ⲿ�������ݣ����ڴ��㹻��
 */
sfpr_int_t sfel_xml_doc_print(sfel_document_t *doc, sfpr_char_t *output,int size);

/**
 *\ingroup sfel_xml_doc
 * @brief �ͷ�sfel_document_tָ��ָ�����Դ
 * @param[in] sfel_document sfel_document_tָ�룬ָ��������xml��Ϣ
 * @return 0:�ɹ�; ��0:ʧ��
 */
sfpr_int_t sfel_xml_destroy(sfel_document_t *sfel_document);

/**
 *\ingroup sfel_xml_doc
 * @brief ����xml�ַ���
 * @param[in] sfel_document sfel_document_tָ�룬ָ��������xml��Ϣ
 * @return sfel_node_tָ�룬ָ��xml�ĸ��ڵ�
 */
sfel_node_t *sfel_xml_get_root(sfel_document_t *sfel_document);

/**************************************************************************************************/
/**
 *\defgroup sfel_xml_node �ڵ��������
 *\ingroup sfel_xml
 **************************************************************************************************/

/**
 *\ingroup sfel_xml_node
 * @brief ��ȡ�ڵ������
 * @param[in] sfel_node xml�ڵ�ʵ��
 * @remark 
 *	 0: xml�ļ�
 *	 1: xml��ǩ
 *	 2: xml��ǩ����ı�
 *	 3: CDATA�ڵ�
 *	 4: xmlע��
 *	 5: xml���� <?xml version="1.0" encoding="ISO-8859-1"?>
 *	 6: DOCTYPE�ڵ�
 *	 7: PI�ڵ�
 */
const sfpr_int_t sfel_xml_node_get_type(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief ��ȡָ���ڵ������
 * @param[in] sfel_node ָ���ڵ�
 * @return ָ���ڵ������
 */
const sfpr_char_t *sfel_xml_node_get_name(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_base
 * @brief ��ȡָ���ڵ��ֵ
 * @param[in] sfel_node ָ���ڵ�
 * @return ָ���ڵ��ֵ
 * @remark ͨ��ͨ����ȡ�ڵ�������ӽڵ㷽ʽ����ȡ�ڵ�����ݣ������滻�ýӿ�
 */
const sfpr_char_t *sfel_xml_node_get_value(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief ��ȡָ���ڵ��document
 * @param[in] sfel_node ָ���ڵ�
 * @return ָ��document
 */
sfel_document_t *sfel_xml_node_get_document(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief ��ȡָ���ڵ�ĵ�һ���ӽڵ�
 * @param[in] sfel_node ָ���ڵ�
 * @return ָ���ڵ�ĵ�һ���ӽڵ�
 */
sfel_node_t *sfel_xml_node_get_child(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief ��ȡָ���ڵ����һ�����ڽڵ�
 * @param[in] sfel_node ָ���ڵ�
 * @return ָ���ڵ����һ�����ڽڵ�
 */
sfel_node_t *sfel_xml_node_get_sibling(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief ��ȡָ���ڵ�ĸ��ڵ�
 * @param[in] sfel_node ָ���ڵ�
 * @return ָ���ڵ�ĸ��ڵ�
 */
sfel_node_t *sfel_xml_node_get_parent(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief �����趨����������ָ���ڵ�������ӽڵ㼰�ӽڵ���ӽڵ�;���ҵ����û�����ص�����
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] depth �ݹ���ȣ�0:�����ӽڵ㣻1:ָ���ڵ���ӽڵ�...
 * @param[in] node_name �����ҽڵ����ƣ���ΪNULL������������ӽڵ�
 * @param[in] attr_name �������ƣ���ΪNULL���򲻶����Խ�����֤
 * @param[in] attr_value ����ֵ����ΪNULL������֤����ֵ
 * @param[in] node_handle �û��ص�����
 * @param[in] param �ص���������
 * @return �ҵ��Ľڵ����
 */
sfpr_int_t sfel_xml_node_find_all(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_node_cb_t node_handle, void *param);

/**
 *\ingroup sfel_xml_node
 * @brief �����趨����������ָ���ڵ�������ӽڵ㼰�ӽڵ���ӽڵ㣬�������������Ľڵ�
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] depth �ݹ���ȣ�0:�����ӽڵ㣻1:ָ���ڵ���ӽڵ�...
 * @param[in] node_name �����ҽڵ����ƣ���ΪNULL������������ӽڵ�
 * @param[in] attr_name �������ƣ���ΪNULL���򲻶����Խ�����֤
 * @param[in] attr_value ����ֵ����ΪNULL������֤����ֵ
 * @return ���������Ľڵ����NULL
 */
sfel_node_t *sfel_xml_node_find(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value);

/**
 *\ingroup sfel_xml_node
 * @brief ����ָ����������ָ���ڵ�֮�����һ�����������ı�ǩ
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] node_name �����ҽڵ����ƣ���ΪNULL������������ӽڵ�
 * @param[in] attr_name �������ƣ���ΪNULL���򲻶����Խ�����֤
 * @param[in] attr_value ����ֵ����ΪNULL������֤����ֵ
 * @return �ҵ��Ľڵ�
 */
sfel_node_t* sfel_xml_node_find_next(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value);

/**
 *\ingroup sfel_xml_node
 * @brief ����ָ����������ָ���ڵ�������ӽڵ㣬��ִ�лص�����
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] node_name �����ҽڵ����ƣ���ΪNULL������������ӽڵ�
 * @param[in] attr_name �������ƣ���ΪNULL���򲻶����Խ�����֤
 * @param[in] attr_value ����ֵ����ΪNULL������֤����ֵ
 * @param[in] node_handle �û��ص�����
 * @param[in] param �ص���������
 * @return �ҵ��Ľڵ����
 */
sfpr_int_t sfel_xml_node_enum_child(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_node_cb_t node_handle, void *param);

/**
 *\ingroup sfel_xml_node
 * @brief ����ָ���������������ԣ��������û��ص�����
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] attr_name �������ƣ���ΪNULL���������������
 * @param[in] attr_value ����ֵ����ΪNULL������֤����ֵ
 * @param[in] attr_handle �û��ص�����
 * @param[in] param �ص���������
 * @return �ҵ������Ը���
 */
sfpr_int_t sfel_xml_node_enum_attr(sfel_node_t *sfel_node,const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_attr_cb_t attr_handle, void *param);

/**
 *\ingroup sfel_xml_node
 * @brief �����µĽڵ�
 * @param[in] sfel_node �������ڵ�
 * @param[in] node_name �½ڵ�����
 * @param[in] node_value �½ڵ�ֵ
 * @return �����ӵĽڵ�
 */
sfel_node_t *sfel_xml_node_append_child_by_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value);

/**
 *\ingroup sfel_xml_node
 * @brief �����µĽڵ�
 * @param[in] doc ָ��������xml��Ϣ
 * @param[in] sfel_node �������ڵ�
 * @param[in] child_node �½ڵ�����
 * @return �����ӵĽڵ�
 */
sfel_node_t *sfel_xml_node_append_child_by_node(sfel_node_t *sfel_node, sfel_node_t *child_node);

/**
 *\ingroup sfel_xml_node
 * @brief ��һ���ڵ㼰����������Ϣ��ӵ���һ���ڵ���
 * @param sfel_node Ŀ��ڵ�
 * @param child_node �ӽڵ�
 * @return �����ӵĽڵ�
 */
sfel_node_t* sfel_xml_node_copy_to_child(sfel_node_t *sfel_node, sfel_node_t *child_node);

/**
 *\ingroup sfel_xml_node
 * @brief �����µĽڵ�,��Ϊ���ڵ�ĵ�һ����ǩ
 * @param[in] doc ָ��������xml��Ϣ
 * @param[in] sfel_node �������ڵ�
 * @param[in] node_name �½ڵ�����
 * @param[in] node_value �½ڵ�ֵ
 * @return �����ӵĽڵ�
 */
sfel_node_t *sfel_xml_node_prepend_child_by_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value);

/**
 *\ingroup sfel_xml_node
 * @brief �����µĽڵ�
 * @param[in] sfel_node ������ڵ�λ��
 * @param[in] new_node �½ڵ�
 * @return �����ӵĽڵ�
 */
sfel_node_t *sfel_xml_node_insert_before(sfel_node_t *sfel_node, sfel_node_t *new_node);

/**
 *\ingroup sfel_xml_node
 * @brief �޸�ָ���ڵ������
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] node_name �ڵ�ֵ
 * @return �޸ĺ�Ľڵ�
 * @remark �ýӿ�ֻ������node_element���͵ı�ǩ
 */
sfel_node_t *sfel_xml_node_set_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name);

/**
 *\ingroup sfel_xml_node
 * @brief �޸�ָ���ڵ��ֵ
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] node_value �ڵ�ֵ
 * @return �޸ĺ�Ľڵ�
 * @remark �ýӿ�ֻ������node_element���͵ı�ǩ
 */
sfel_node_t *sfel_xml_node_set_value(sfel_node_t *sfel_node, const sfpr_char_t *node_value);

/**
 *\ingroup sfel_xml_node
 * @brief ��¡ָ���ڵ�
 * @param[in]*doc ָ��������xml��Ϣ
 * @param[in] sfel_node ����¡�Ľڵ�
 * @return ��¡��Ľڵ�
 */
sfel_node_t *sfel_xml_node_after_clone(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node
 * @brief ���ڵ���������뵽һ��������
 * @param[in] *doc ָ��������xml��Ϣ
 * @param[in] *output ת���������
 * @param[in] size ��������С
 * @remark ����outputҪ���û����ⲿ�������ݣ����ڴ��㹻��
 */
sfpr_int_t sfel_xml_node_print(sfel_node_t *sfel_node, sfpr_char_t *output,int size);

/**
 *\ingroup sfel_xml_node
 * @brief �Ƴ�ָ���ڵ�
 * @param[in] sfel_node ��ɾ���Ľڵ�
 * @return 0:�ɹ�; ��0:ʧ��
 */
sfpr_int_t sfel_xml_node_delete(sfel_node_t *sfel_node);

/**************************************************************************************************/
/**
 *\defgroup sfel_xml_node_attr �ڵ����Բ�������
 *\ingroup sfel_xml
 **************************************************************************************************/

/**
 *\ingroup sfel_xml_node_attr
 * @brief ��ȡָ���ڵ�ĵ�һ������
 * @param sfel_node ָ���ڵ�
 * @return ָ���ڵ�ĵ�һ�����ԣ����û����ΪNULL
 */
sfel_attr_t *sfel_xml_node_get_first_attr(sfel_node_t *sfel_node);

/**
 *\ingroup sfel_xml_node_attr
 * @brief ��ȡָ�����Ե���һ����������
 * @param[in] sfel_attr ָ������
 * @return ָ�����Ե���һ�����ԣ����û����ΪNULL
 */
sfel_attr_t *sfel_xml_attr_get_sibling(sfel_attr_t *sfel_attr);

/**
 *\ingroup sfel_xml_node_attr
 * @brief ����ָ�����Ƶ�����
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] attr_name ��������
 * @return �ҵ�������
 */
sfel_attr_t *sfel_xml_attr_get(sfel_node_t *sfel_node, const sfpr_char_t *attr_name);

/**
 *\ingroup sfel_xml_node_attr
 * @brief ��ѯָ�����Ե�����
 * @param[in] sfel_attr sfel_attr_tָ��
 * @return ��������
 */
sfpr_char_t *sfel_xml_attr_get_aname(sfel_attr_t *sfel_attr);

/**
 *\ingroup sfel_xml_node_attr
 * @brief ��ѯָ�����Ե�ֵ
 * @param[in] sfel_attr sfel_attr_tָ��
 * @return ����ֵ
 */
sfpr_char_t *sfel_xml_attr_get_avalue(sfel_attr_t *sfel_attr);

/**
 *\ingroup sfel_xml_node_attr
 * @brief ����ָ�����Ƶ����Ե�ֵ
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] attr_name ��������
 * @return ����ֵ����NULL
 */
sfpr_char_t *sfel_xml_attr_get_value(sfel_node_t *sfel_node, const sfpr_char_t *attr_name);

/**
 *\ingroup sfel_xml_node_attr
 * @brief �����µ�����
 * @param[in] sfel_node �������ڵ�
 * @param[in] attr_name ����������
 * @param[in] attr_value ������ֵ
 * @return �����ӵ�����
 */
sfel_attr_t *sfel_xml_attr_add(sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *attr_value);

/**
 *\ingroup sfel_xml_node_attr
 * @brief �޸�ָ�����Ե�ֵ
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] attr_name ������
 * @param[in] new_attr_value ���޸�ֵ
 * @return �޸ĺ������
 */
sfel_attr_t *sfel_xml_attr_update(sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *new_attr_value);

/**
 *\ingroup sfel_xml_node_attr
 * @brief ɾ��ָ�����Ƶ�����
 * @param[in] sfel_node ָ���ڵ�
 * @param[in] attr_name ��ɾ����������
 * @return 0:�ɹ�; ��0:ʧ��
 */
sfpr_int_t sfel_xml_attr_delete(sfel_node_t *sfel_node, const sfpr_char_t *attr_name);

/**************************************************************************************************/
/**
 *\defgroup sfel_xml_sax SAX����
 *\ingroup sfel_xml
 **************************************************************************************************/
sfpr_int_t sfel_sax_parse_file(sfpr_char_t *file,sfel_sax_cb_t sax,void *sax_data,sfel_portable_t *portable);
sfpr_int_t sfel_sax_parse_string(sfpr_char_t *file,sfel_sax_cb_t sax,void *sax_data,sfel_portable_t *portable);


#ifdef __cplusplus
}
#endif

#endif
