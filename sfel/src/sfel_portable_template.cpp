
#include <string.h>
#include <iostream>

#include "sfel_xml.h"
#include "sfel_global.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

sfel_document_t *sfel_xml_create_document()
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_document_t *sfel_xml_parse_file(sfpr_char_t *sfel_file, sfel_portable_t *portable)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_document_t *sfel_xml_parse_string(sfpr_char_t *sfel_data, sfel_portable_t *portable)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_int_t sfel_xml_declare(sfel_document_t *sfel_document, const sfpr_char_t *version, const sfpr_char_t *encode)
{
	try
	{
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

sfpr_int_t sfel_xml_save(sfel_document_t *sfel_document, sfpr_char_t *sfel_file)
{
	try
	{
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

sfpr_int_t sfel_xml_destroy(sfel_document_t *sfel_document)
{
	try
	{
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

sfel_node_t *sfel_xml_get_root(sfel_document_t *sfel_document)
{
	//! 获取根节点
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_int_t sfel_xml_node_get_type(sfel_node_t *sfel_node)
{
	try
	{
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfpr_char_t *sfel_xml_node_get_name(sfel_node_t *sfel_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_document_t *sfel_xml_node_get_document(sfel_node_t *sfel_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_char_t *sfel_xml_node_get_value(sfel_node_t *sfel_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}


/**
 * brief 获取第一个子节点
 */
sfel_node_t *sfel_xml_node_get_child(sfel_node_t *sfel_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

/**
 * brief 获取下一个相邻的节点
 */
sfel_node_t *sfel_xml_node_get_sibling(sfel_node_t *sfel_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

/**
 * brief 获取父节点
 */
sfel_node_t *sfel_xml_node_get_parent(sfel_node_t *sfel_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}


sfel_node_t *sfel_xml_node_find(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_int_t sfel_xml_node_find_all(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_node_cb_t node_handle, void *param)
{
	try
	{
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfel_node_t* sfel_xml_node_find_next(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_int_t sfel_xml_node_enum_child(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_node_cb_t node_handle, void *param)
{
	try
	{
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

sfpr_int_t sfel_xml_node_enum_attr(sfel_node_t *sfel_node, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_attr_cb_t attr_handle, void *param)
{
	try
	{
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfel_node_t *sfel_xml_node_add(sfel_document_t *sfel_document, sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_preadd(sfel_document_t *sfel_document, sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value)
{
	try
	{
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_nadd(sfel_document_t *sfel_document, sfel_node_t *sfel_node, sfel_node_t *new_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_ninsert(sfel_node_t *sfel_node, sfel_node_t *new_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_update(sfel_document_t *sfel_document, sfel_node_t *sfel_node, const sfpr_char_t *new_node_value)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_eupdate(sfel_document_t *sfel_document, sfel_node_t *sfel_node, const char *new_value)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_int_t sfel_xml_node_delete(sfel_node_t *sfel_node)
{
	try
	{
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfel_node_t *sfel_xml_node_clone(sfel_document_t *sfel_document, sfel_node_t *sfel_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

static sfpr_int_t sfel_xml_attr_cb(sfel_node_t *node, sfel_attr_t *attr, sfpr_void_t *param)
{
	sfel_node_t *instance = param;
	sfel_document_t *doc;
	doc = sfel_xml_node_get_document(instance);
	if(!instance || !doc)
		return -1;
	sfel_xml_attr_add(doc, instance, sfel_xml_attr_get_aname(attr),sfel_xml_attr_get_avalue(attr));
	return 0;
}
static sfpr_int_t sfel_xml_node_cb(sfel_node_t *node, sfpr_void_t *param)
{
	sfel_node_t *instance = param;
	if(!instance)
		return -1;
	sfel_xml_node_copy(instance, node);
	return 0;
}

sfel_node_t* sfel_xml_node_copy(sfel_node_t *instance, sfel_node_t *resource)
{
	sfel_node_t *node;
	sfel_document_t *doc;
	doc = sfel_xml_node_get_document(instance);
	if(!doc)
		return NULL;
	
	//增加当前标签，及所有属性
	node = sfel_xml_node_add(doc, instance, sfel_xml_node_get_name(resource),sfel_xml_node_get_value(resource));
	sfel_xml_node_enum_attr(resource, NULL,NULL, sfel_xml_attr_cb, node);
	
	//遍历下一级子标签及属性
	sfel_xml_node_enum_child(resource, NULL, NULL,NULL, sfel_xml_node_cb, node);
	return node;
}

sfel_attr_t *sfel_xml_node_get_first_attr(sfel_node_t *sfel_node)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_attr_t *sfel_xml_attr_get_sibling(sfel_attr_t *sfel_attr)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_attr_t *sfel_xml_attr_get(sfel_node_t *sfel_node, const sfpr_char_t *attr_name)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_char_t *sfel_xml_attr_get_aname(sfel_attr_t *sfel_attr)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_char_t *sfel_xml_attr_get_avalue(sfel_attr_t *sfel_attr)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_char_t *sfel_xml_attr_get_value(sfel_node_t *sfel_node, const sfpr_char_t *attr_name)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_attr_get_parent_node(sfel_attr_t *sfel_attr)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_attr_t *sfel_xml_attr_add(sfel_document_t *sfel_document, sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *attr_value)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_attr_t *sfel_xml_attr_update(sfel_document_t *sfel_document, sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *new_attr_value)
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_int_t sfel_xml_attr_delete(sfel_node_t *sfel_node, const sfpr_char_t *attr_name)
{
	try
	{
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}


sfpr_int_t sfel_xml_convert_to_string(sfel_document_t *sfel_document, sfpr_char_t *output)
{
	try
	{
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfpr_int_t sfel_xml_convert_to_string1(sfel_node_t *sfel_node, sfpr_char_t *output)
{
	try
	{
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

#ifdef __cplusplus
}
#endif
