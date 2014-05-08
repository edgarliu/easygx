
#include <string.h>
#include <iostream>
#include "sfel_xml.h"
#include "sfel_global.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 1. 文档操作函数
 **************************************************************************************************/
sfel_document_t *sfel_xml_create_document()
{
	xml_document<> *doc;
	try
	{
		doc = new xml_document<>;
		doc->m_filedoc = NULL;
		return doc;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_parse_file | error | %s\n", error.what());
		#endif
		sfel_xml_destroy(doc);
		return NULL;
	}
	catch(...)
	{
		sfel_xml_destroy(doc);
		return NULL;
	}
}

sfel_document_t *sfel_xml_parse_file(const sfpr_char_t *sfel_file, sfel_portable_t *portable)
{
	xml_document<> *doc = new xml_document<>;
	try
	{
		file<> *fdoc = NULL;
		fdoc = new file<>(sfel_file);
		#ifdef LOG_DEBUG
		fprintf(stderr, "sfel_xml_parse_file | opened:%d  size:%d\n",fdoc->is_open(),fdoc->size());
		#endif
		if(fdoc->is_open() == 0){
			delete fdoc;
			return NULL;
		}
		doc->parse<1>(fdoc->data());
		doc->m_filedoc = fdoc;
		return doc;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_parse_file | error | %s\n", error.what());
		#endif
		sfel_xml_destroy(doc);
		return NULL;
	}
	catch(...)
	{
		sfel_xml_destroy(doc);
		return NULL;
	}
}

sfel_document_t *sfel_xml_parse_string(sfpr_char_t *sfel_data, sfel_portable_t *portable)
{
	xml_document<> *doc = new xml_document<>;;	
	try
	{
		doc->parse<1>(sfel_data);
		doc->m_filedoc = NULL;
		return doc;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_parse_string | error | %s\n", error.what());
		#endif
		sfel_xml_destroy(doc);
		return NULL;
	}
	catch(...)
	{
		sfel_xml_destroy(doc);
		return NULL;
	}
}

sfpr_int_t sfel_xml_declare(sfel_document_t *sfel_document, const sfpr_char_t *version, const sfpr_char_t *encode)
{
	try
	{
		xml_document<> *doc = (xml_document<> *)sfel_document;
		xml_node<>* mynode=doc->allocate_node(node_declaration);
		sfel_attr_t *attr = NULL;
		if(mynode != NULL){
			doc->insert_node(doc->first_node(), mynode);
			attr = sfel_xml_attr_add(mynode, "version", version);
			if(!attr){
				return -1;
			}
			attr = sfel_xml_attr_add(mynode, "encoding", encode);
			if(!attr){
				return -2;
			}
			return 0;
		}
		return -3;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_get_root | error | %s\n", error.what());
		#endif
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfpr_int_t sfel_xml_save(sfel_document_t *sfel_document, const sfpr_char_t *sfel_file)
{
	try
	{
		xml_document<> *pdoc = (xml_document<> *)sfel_document;
		FILE *fp;
		int ret;
		if(!sfel_document)
			return -11;

		ret = sfel_xml_declare(sfel_document, "1.0", "gb2312");
		if(ret < 0)
			return ret;
		
		if(( fp = fopen(sfel_file,"w"))< 0 ){
			return -12;
		}
		rapidxml::print(fp, *pdoc, 0);
		fclose(fp);
		return 0;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_save | error | %s\n", error.what());
		#endif
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfpr_int_t sfel_xml_doc_print(sfel_document_t *sfel_document, sfpr_char_t *output,int size)
{
	try
	{
		xml_document<> *doc;
		if(!output)
			return -1;

		if(sfel_document)
		{
			doc = (xml_document<> *)sfel_document;
			rapidxml::print(output, *doc, 0);  
			return 0;
		}
		return -1;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_convert_to_string | error | %s\n", error.what());
		#endif
		return -1;
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
		xml_document<> *doc;
		if(sfel_document)
		{
			doc = (xml_document<> *)sfel_document;
			if(doc->m_filedoc){
				file<> *fdoc = (file<>*)doc->m_filedoc;
				delete fdoc;
			}
			delete doc;
		}
		return 0;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_destroy | error | %s\n", error.what());
		#endif
		return -1;
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
		xml_node<>* root;
		xml_document<> *doc = (xml_document<> *)sfel_document;
		if(!sfel_document)
			return NULL;
		root = doc->first_node();
		return root;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_get_root | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

/**************************************************************************************************
 2. 节点操作函数
 **************************************************************************************************/
const sfpr_int_t sfel_xml_node_get_type(sfel_node_t *sfel_node)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!node)
			return -1;
		return node->type();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_get_type | error | %s\n", error.what());
		#endif
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

const sfpr_char_t *sfel_xml_node_get_name(sfel_node_t *sfel_node)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!sfel_node)
			return NULL;
		return node->name();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_get_name | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

const sfpr_char_t *sfel_xml_node_get_value(sfel_node_t *sfel_node)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!sfel_node)
			return NULL;
		return node->value();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_get_value | error | %s\n", error.what());
		#endif
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
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!sfel_node)
			return NULL;
		if(node->type() == node_document){
			return sfel_node;
		}
		return node->document();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_get_document | error | %s\n", error.what());
		#endif
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
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!sfel_node)
			return NULL;
		return node->first_node();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_get_child | error | %s\n", error.what());
		#endif
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
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!sfel_node)
			return NULL;
		return node->next_sibling();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_get_sibling | error | %s\n", error.what());
		#endif
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
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!node)
			return NULL;
		return node->parent();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_get_parent | error | %s\n", error.what());
		#endif
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
		xml_node<>* tmp;
		sfpr_int_t idepth = depth-1;
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_node<>* sential;

		if(!sfel_node)
			return -1;

		tmp = node->first_node(NULL);
		if(!tmp)
			return -1;
		for (sential = tmp->next_sibling(NULL); tmp; tmp = sential, (sential==NULL)?(sential=NULL):(sential = sential->next_sibling(NULL)))
		{
			if((node_name && strcmp(node_name, tmp->name())==0) || !node_name)
			{
				if(attr_name)
				{
					if(sfel_xml_node_enum_attr(tmp, attr_name, attr_value, NULL, NULL) > 0)
					{
						if(node_handle)
						{
							node_handle(tmp, param);
						}
					}
				}
				else
				{
					if(node_handle)
					{
						node_handle(tmp, param);
					}
				}
			}
			if(idepth)
			{
				sfel_xml_node_find_all(tmp, idepth, node_name, attr_name,  attr_value, node_handle, param);
			}
		}
		return 0;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_find_all | error | %s\n", error.what());
		#endif
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfel_node_t *sfel_xml_node_find(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value)
{
	try
	{
		xml_node<>* tmp;
		xml_node<>* ret;
		sfpr_int_t idepth = depth-1;
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_node<>* sential;
		if(!sfel_node)
			return NULL;

		tmp = node->first_node(NULL);
		if(!tmp)
			return NULL;
		for (sential = tmp->next_sibling(NULL); tmp; tmp = sential, (sential==NULL)?(sential=NULL):(sential = sential->next_sibling(NULL)))
		{
			if((node_name && strcmp(node_name, tmp->name())==0) || !node_name)
			{
				if(attr_name)
				{
					if(sfel_xml_node_enum_attr(tmp, attr_name, attr_value, NULL, NULL) > 0)
					{
						return tmp;
					}
				}
				else
				{
					return tmp;
				}
			}
			if(idepth)
			{
				ret = (xml_node<>*)sfel_xml_node_find(tmp, idepth, node_name, attr_name, attr_value);
				if(ret)
					return ret;
			}
		}
		return NULL;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_find | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t* sfel_xml_node_find_next(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value)
{
	try
	{
		xml_node<>* tmp;
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_node<>* sential;
		if(!sfel_node)
			return NULL;
		tmp = node->next_sibling(node_name);
		if(!tmp)
			return NULL;
		for (sential = tmp->next_sibling(node_name); tmp; tmp = sential, (sential==NULL)?(sential=NULL):(sential = sential->next_sibling(node_name)))
		{
			if(attr_name)
			{
				if(sfel_xml_node_enum_attr(tmp, attr_name, attr_value, NULL, NULL) > 0)
				{
					return tmp;
				}
			}
			else
			{
				return tmp;
			}
		}
		return NULL;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_enum_child | error | %s\n", error.what());
		#endif
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
		xml_node<>* tmp;
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_node<>* sential;
		sfpr_int_t iflg = 0;
		if(!sfel_node)
			return -1;
		tmp = node->first_node(node_name);
		if(!tmp)
			return 0;
		for (sential = tmp->next_sibling(node_name); tmp; tmp = sential, (sential==NULL)?(sential=NULL):(sential = sential->next_sibling(node_name)))
		{
			if(attr_name)
			{
				if(sfel_xml_node_enum_attr(tmp, attr_name, attr_value, NULL, NULL) > 0)
				{
					if(node_handle)
					{
						node_handle(tmp, param);
					}
					iflg++;
				}
			}
			else
			{
				if(node_handle)
				{
					node_handle(tmp, param);
				}
				iflg++;
			}
		}
		return iflg;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_enum_child | error | %s\n", error.what());
		#endif
		return -1;
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
		xml_attribute<> *attr;
		xml_attribute<> *sential;
		sfpr_int_t iflg = 0;
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!sfel_node)
			return -1;
		attr = node->first_attribute(attr_name);

		if(!attr)
			return -1;
		
		for (sential = attr->next_attribute(attr_name); attr; attr = sential, (sential==NULL)?(sential=NULL):(sential = sential->next_attribute(attr_name)))
		{
			if(attr_value)
			{
				if(!strcmp(attr->value(), attr_value/*, strlen(attr_value)*/))
				{
					if(attr_handle)
					{
						attr_handle(node, attr, param);
					}
					iflg++;
				}
			}
			else
			{
				if(attr_handle)
				{
					attr_handle(node, attr, param);
				}
				iflg++;
			}
		}

		return iflg;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_enum_attr | error | %s\n", error.what());
		#endif
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfel_node_t *sfel_xml_node_append_child_by_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_document<>* doc = (xml_document<>*)sfel_xml_node_get_document(sfel_node);
		if(!doc){
			return NULL;
		}
		sfpr_char_t *_name = doc->allocate_string(node_name);
		sfpr_char_t *_value = doc->allocate_string(node_value);
		xml_node<>* mynode=doc->allocate_node(node_element,_name,_value);
		node->append_node(mynode);
		return mynode;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_add | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_append_child_by_node(sfel_node_t *sfel_node, sfel_node_t *child_node)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_node<>* pchild_node = (xml_node<>*)child_node;
		
		if(node)
		{
			node->append_node(pchild_node);
			return child_node;
		}
		else
			return NULL;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_nadd | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_after_clone(sfel_node_t *sfel_node)
{
	try
	{
		xml_node<>* node_clone;
		xml_node<>* node_parent;
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_document<>* doc = (xml_document<>*)sfel_xml_node_get_document(sfel_node);
		if(!doc){
			return NULL;
		}
		node_clone = doc->clone_node(node);
		node_parent = node->parent();
		node_parent->append_node(node_clone);
		return node_clone;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_clone | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_prepend_child_by_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_document<>* doc = (xml_document<>*)sfel_xml_node_get_document(sfel_node);
		if(!doc){
			return NULL;
		}
		sfpr_char_t *_name = doc->allocate_string(node_name);
		sfpr_char_t *_value = doc->allocate_string(node_value);
		xml_node<>* mynode=doc->allocate_node(node_element,_name,_value);
		node->prepend_node(mynode);
		return mynode;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_add | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_insert_before(sfel_node_t *sfel_node, sfel_node_t *new_node)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_node<>* parent;
		xml_node<>* mynode = NULL;

		if(!sfel_node || !new_node)
			return node;		
		
		parent = node->parent();
		mynode= (xml_node<>*)new_node;
		parent->insert_node(node, mynode);
		return mynode;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_ninsert | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_node_t *sfel_xml_node_set_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_document<>* doc = (xml_document<>*)sfel_xml_node_get_document(sfel_node);
		sfpr_char_t *_name = NULL;
		if(!doc || !sfel_node || !node_name)
			return node;
		_name = doc->allocate_string(node_name);
		node->name(_name);
		return node;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_set_name | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}	
}

sfel_node_t *sfel_xml_node_set_value(sfel_node_t *sfel_node, const sfpr_char_t *node_value)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_document<>* doc = (xml_document<>*)sfel_xml_node_get_document(sfel_node);
		sfpr_char_t *_value = NULL;
		if(!doc || !sfel_node || !node_value)
			return node;
		_value = doc->allocate_string(node_value);
		node->value(_value);
		return node;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_set_value | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfpr_int_t sfel_xml_node_print(sfel_node_t *sfel_node, sfpr_char_t *output,int size)
{
	try
	{
		xml_node<>* node;
		if(!output)
			return -1;

		if(sfel_node)
		{
			node = (xml_node<> *)sfel_node;
			rapidxml::print(output, *node, 0);  
			return 0;
		}
		return -1;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_convert_to_string | error | %s\n", error.what());
		#endif
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}

sfpr_int_t sfel_xml_node_delete(sfel_node_t *sfel_node)
{
	try
	{
		xml_node<>* parent;
		xml_node<>* node = (xml_node<>*)sfel_node;	
		if(!sfel_node)
			return -1;
		parent = node->parent();
		if(parent)
			parent->remove_node(node);
		else
			return -1;
		return 0;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_delete | error | %s\n", error.what());
		#endif
		return -1;
	}
	catch(...)
	{
		return -1;
	}
}


/**************************************************************************************************
 3. 节点属性操作函数
 **************************************************************************************************/
sfel_attr_t *sfel_xml_node_get_first_attr(sfel_node_t *sfel_node)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!node)
			return NULL;
		
		return node->first_attribute();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_node_get_first_attr | error | %s\n", error.what());
		#endif
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
		xml_attribute<>* attr = (xml_attribute<>*)sfel_attr;
		if(!attr)
			return NULL;
		
		return attr->next_attribute();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_attr_get_by_name | error | %s\n", error.what());
		#endif
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
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!sfel_node)
			return NULL;
		
		return node->first_attribute(attr_name);
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_attr_get_by_name | error | %s\n", error.what());
		#endif
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
		xml_attribute<>* attr = (xml_attribute<>*)sfel_attr;
		if(!attr)
			return NULL;
		return attr->name();
			
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_attr_get_name | error | %s\n", error.what());
		#endif
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
		xml_attribute<>* attr = (xml_attribute<>*)sfel_attr;
		if(!sfel_attr)
			return NULL;
		return attr->value();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_attr_get_value | error | %s\n", error.what());
		#endif
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
		xml_attribute<>* attr = (xml_attribute<>*)sfel_xml_attr_get(sfel_node, attr_name);
		if(!attr)
			return NULL;
		return attr->value();
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_attr_get_value | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_attr_t *sfel_xml_attr_add(sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *attr_value)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_document<>* doc = (xml_document<>*)sfel_xml_node_get_document(sfel_node);
		if(!doc){
			return NULL;
		}
		sfpr_char_t *_name = doc->allocate_string(attr_name);
		sfpr_char_t *_value = doc->allocate_string(attr_value);
		xml_attribute<>* myattr=doc->allocate_attribute(_name, _value);
		node->append_attribute(myattr);
		return myattr;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_attr_add | error | %s\n", error.what());
		#endif
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

sfel_attr_t *sfel_xml_attr_update(sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *new_attr_value)
{
	try
	{
		xml_node<>* node = (xml_node<>*)sfel_node;
		xml_document<>* doc = (xml_document<>*)sfel_xml_node_get_document(sfel_node);
		xml_attribute<>* attr = node->first_attribute(attr_name);
		if(!doc){
			return NULL;
		}
		sfpr_char_t *_value = doc->allocate_string(new_attr_value);
		if(attr)
		{
			attr->value(_value);
		}
		return attr;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_attr_update | error | %s\n", error.what());
		#endif
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
		sfpr_int_t ret = 0;
		xml_attribute<>* attr;	
		xml_node<>* node = (xml_node<>*)sfel_node;
		if(!node)
			return -1;
		attr = node->first_attribute(attr_name);
		if(attr)
		{
			node->remove_attribute(attr);//先删除节点
		}	
		return ret;
	}
	catch(parse_error error)
	{
		#ifdef LOG_ERROR
		fprintf(stderr, "sfel_xml_attr_delete | error | %s\n", error.what());
		#endif
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

