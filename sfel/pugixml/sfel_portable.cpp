
#include <string.h>
#include <stdio.h>

#include <iostream>

#include "sfel_xml.h"
#include "sfel_global.h"

#include "pugixml.hpp"

using namespace std;
using namespace pugi;

#ifdef __cplusplus
extern "C" {
#endif


/*
typedef sfel_document_t xml_node_struct;
typedef sfel_node_t xml_node_struct;
typedef sfel_attr_t xml_attribute_struct;
*/

/**************************************************************************************************
 1. 文档操作函数
 **************************************************************************************************/

sfel_document_t *sfel_xml_create_document()
{
	xml_document *doc = new xml_document();
	return doc->internal_object();
}

sfel_document_t *sfel_xml_parse_file(const sfpr_char_t *sfel_file, sfel_portable_t *portable)
{
	if(!sfel_file){
		return NULL;
	}
	xml_document *doc = new xml_document();
	doc->load_file(sfel_file);
	return doc->internal_object();
}

sfel_document_t *sfel_xml_parse_string(sfpr_char_t *sfel_data, sfel_portable_t *portable)
{
	if(!sfel_data){
		return NULL;
	}
	xml_document *doc = new xml_document();
	doc->load(sfel_data);
	return doc->internal_object();
}

static xml_document* sfel_xml_get_xmldoc(sfel_document_t *sfel_document)
{
	if(!sfel_document){
		return NULL;
	}
	xml_node node((xml_node_struct*)sfel_document);
	if(node.type() != node_document){
		return NULL;
	}
	xml_document *pdoc = (xml_document*)&node;
	return (xml_document*)pdoc->get_xmldoc();
}

sfpr_int_t sfel_xml_declare(sfel_document_t *sfel_document, const sfpr_char_t *version, const sfpr_char_t *encode)
{
	xml_document *doc = sfel_xml_get_xmldoc(sfel_document);
	if(!doc){
		return -1;
	}
    xml_node decl = doc->prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = version;
    decl.append_attribute("encoding") = encode;//"UTF-8";
	return 0;
}

sfpr_int_t sfel_xml_save(sfel_document_t *sfel_document, const sfpr_char_t *sfel_file)
{
	if(!sfel_document || !sfel_file){
		return -1;
	}
	xml_document *doc = sfel_xml_get_xmldoc(sfel_document);
	if(!doc){
		return -1;
	}
	sfel_xml_declare(sfel_document,"1.0","gb2312");
	doc->save_file(sfel_file);
	return 0;
}

sfpr_int_t sfel_xml_doc_print(sfel_document_t *sfel_document, sfpr_char_t *output,int size)
{
	xml_document *doc = sfel_xml_get_xmldoc(sfel_document);
	if(!doc){
		return -1;
	}
	return -1;
}

sfpr_int_t sfel_xml_destroy(sfel_document_t *sfel_document)
{
	xml_document *doc = sfel_xml_get_xmldoc(sfel_document);
	if(!doc){
		return -1;
	}
//	doc->reset();
	delete doc;
	return 0;
}

sfel_node_t *sfel_xml_get_root(sfel_document_t *sfel_document)
{
	//! 获取根节点
	xml_document *doc = sfel_xml_get_xmldoc(sfel_document);
	if(!doc){
		return NULL;
	}
	return doc->document_element().internal_object();
}

/**************************************************************************************************
 2. 节点操作函数
 **************************************************************************************************/
const sfpr_int_t sfel_xml_node_get_type(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return -1;
	}
	xml_node node((xml_node_struct *)sfel_node);
	return (sfpr_int_t)(node.type() - 1);
}

const sfpr_char_t *sfel_xml_node_get_name(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	return node.name();
}

const sfpr_char_t *sfel_xml_node_get_value(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	return node.child_value();
}

sfel_document_t *sfel_xml_node_get_document(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	return node.root().internal_object();
}

/**
 * brief 获取第一个子节点
 */
sfel_node_t *sfel_xml_node_get_child(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	return node.first_child().internal_object();
}

/**
 * brief 获取下一个相邻的节点
 */
sfel_node_t *sfel_xml_node_get_sibling(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	return node.next_sibling().internal_object();
}

/**
 * brief 获取父节点
 */
sfel_node_t *sfel_xml_node_get_parent(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	return node.parent().internal_object();
}

sfpr_int_t sfel_xml_node_find_all(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_node_cb_t node_handle, void *param)
{
	if(!sfel_node)
		return -1;

	sfpr_int_t idepth = depth-1;	
	xml_node node((xml_node_struct *)sfel_node);
	xml_node tmp_node = node.first_child();
	if(tmp_node.empty())
		return -1;
	xml_node_struct *tmp_node_struct = tmp_node.internal_object();
	while(tmp_node_struct){
		xml_node curr_node(tmp_node_struct);
		if((NULL == node_name) || (node_name && strcmp(node_name, curr_node.name())==0)){		
			if(attr_name){
				if(sfel_xml_node_enum_attr(tmp_node_struct, attr_name, attr_value, NULL, NULL) > 0){
					if(node_handle){
						node_handle(tmp_node_struct, param);
					}
				}
			}
			else{
				if(node_handle){
					node_handle(tmp_node_struct, param);
				}
			}
		}
		if(idepth){
			sfel_xml_node_find_all(tmp_node_struct, idepth, node_name, attr_name,  attr_value,node_handle, param);
		}
		xml_node next_node = curr_node.next_sibling();
		if(next_node.empty()){
			break;
		}
		tmp_node_struct = next_node.internal_object();
	}
	return 0;
}

sfel_node_t *sfel_xml_node_find(sfel_node_t *sfel_node, sfpr_int_t depth, const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value)
{
	if(!sfel_node)
		return NULL;

	sfpr_int_t idepth = depth-1;
	xml_node node((xml_node_struct *)sfel_node);
	xml_node tmp_node = node.first_child();
	if(tmp_node.empty())
		return NULL;
	xml_node_struct *tmp_node_struct = tmp_node.internal_object();
	while(tmp_node_struct){
		xml_node curr_node(tmp_node_struct);
		if((NULL == node_name) || (node_name && strcmp(node_name, curr_node.name())==0) ){
			if(attr_name){
				if(sfel_xml_node_enum_attr(tmp_node_struct, attr_name, attr_value, NULL, NULL) > 0){
					return tmp_node_struct;
				}
			}
			else{
					return tmp_node_struct;
			}
		}
		if(idepth){
			sfel_node_t *ret_node = sfel_xml_node_find(tmp_node_struct, idepth, node_name, attr_name,  attr_value);
			if(ret_node){
				return ret_node;
			}
		}
		xml_node next_node = curr_node.next_sibling();
		if(next_node.empty()){
			break;
		}
		tmp_node_struct = next_node.internal_object();
	}
	return NULL;
}

sfel_node_t* sfel_xml_node_find_next(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value)
{
	if(!sfel_node)
		return NULL;

	xml_node node((xml_node_struct *)sfel_node);
	xml_node tmp_node = node_name?node.next_sibling(node_name):node.next_sibling();
	if(tmp_node.empty())
		return NULL;
	xml_node_struct *tmp_node_struct = tmp_node.internal_object();
	while(tmp_node_struct){
		xml_node curr_node(tmp_node_struct);
		if(attr_name){
			if(sfel_xml_node_enum_attr(curr_node.internal_object(), attr_name, attr_value, NULL, NULL) > 0){
				return tmp_node_struct;
			}
		}
		else{
			return tmp_node_struct;
		}
		xml_node next_node = node_name?curr_node.next_sibling(node_name):curr_node.next_sibling();
		if(next_node.empty()){
			break;
		}
		tmp_node_struct = next_node.internal_object();
	}
	return NULL;
}

sfpr_int_t sfel_xml_node_enum_child(sfel_node_t *sfel_node,const sfpr_char_t *node_name, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_node_cb_t node_handle, void *param)
{
	sfpr_int_t iflg = 0;
	xml_node node((xml_node_struct *)sfel_node);
	if(!sfel_node)
		return 0;
	xml_node tmp_node = node_name?node.child(node_name):node.first_child();
	if(tmp_node.empty())
		return 0;
	xml_node_struct *tmp_node_struct = tmp_node.internal_object();
	while(tmp_node_struct){
		if(attr_name){
			if(sfel_xml_node_enum_attr(tmp_node_struct, attr_name, attr_value, NULL, NULL) > 0){
				if(node_handle){
					node_handle(tmp_node_struct, param);
				}
				iflg++;
			}
		}
		else{
			if(node_handle){
				node_handle(tmp_node_struct, param);
			}
			iflg++;
		}
		xml_node curr_node(tmp_node_struct);
		xml_node next_node = node_name?curr_node.next_sibling(node_name):curr_node.next_sibling();
		if(next_node.empty()){
			break;
		}
		tmp_node_struct = next_node.internal_object();
	}
	return iflg;
}

sfpr_int_t sfel_xml_node_enum_attr(sfel_node_t *sfel_node, const sfpr_char_t *attr_name,const sfpr_char_t *attr_value, sfel_xml_attr_cb_t attr_handle, void *param)
{
	sfpr_int_t iflg = 0;
	xml_node node((xml_node_struct *)sfel_node);
	if(!sfel_node)
		return 0;
	xml_attribute attr = node.first_attribute();
	if(attr.empty())
		return 0;
	xml_attribute_struct *tmp_attr_struct = attr.internal_object();
	while(tmp_attr_struct){
		xml_attribute curr_attr(tmp_attr_struct);
		if(attr_name){
			if(strcmp(curr_attr.name(), attr_name)){//如果不相等，则进入下一次循环
				xml_attribute next_attr = curr_attr.next_attribute();
				if(next_attr.empty()){
					break;
				}
				tmp_attr_struct = next_attr.internal_object();	
				continue;
			}
		}
		if(attr_value){
			if(!strcmp(curr_attr.value(), attr_value)){
				if(attr_handle){
					attr_handle(sfel_node, curr_attr.internal_object(), param);
				}
				iflg++;
			}
		}
		else{
			if(attr_handle){
				attr_handle(sfel_node, curr_attr.internal_object(), param);
			}
			iflg++;
		}
		xml_attribute next_attr = curr_attr.next_attribute();
		if(next_attr.empty()){
			break;
		}
		tmp_attr_struct = next_attr.internal_object();
	}
	return iflg;
}

sfel_node_t *sfel_xml_node_append_child_by_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value)
{
	if(!sfel_node || !node_name){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_node child_node = node.append_child(node_name);
	if(node_value){
		child_node.set_value(node_value);
	}
	return child_node.internal_object();
}

sfel_node_t *sfel_xml_node_append_child_by_node(sfel_node_t *sfel_node, sfel_node_t *child_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_node child_node_((xml_node_struct *)child_node);	
	xml_node child_new = node.append_copy(child_node_);
	return child_new.internal_object();
}

sfel_node_t *sfel_xml_node_after_clone(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_node parent = node.parent();
	xml_node new_node = parent.insert_copy_after(node,node);
	return new_node.internal_object();
}

sfel_node_t *sfel_xml_node_prepend_child_by_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name, const sfpr_char_t *node_value)
{
	if(!sfel_node || !node_name){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_node child_node = node.prepend_child(node_name);
	if(node_value){
		child_node.set_value(node_value);
	}
	return child_node.internal_object();	
}

sfel_node_t *sfel_xml_node_insert_before(sfel_node_t *sfel_node, sfel_node_t *new_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_node new_node_((xml_node_struct *)new_node);	

	xml_node parent_node = node.parent();
	xml_node child_new = parent_node.insert_copy_before(node,new_node_);
	return child_new.internal_object();
}

sfel_node_t *sfel_xml_node_set_name(sfel_node_t *sfel_node, const sfpr_char_t *node_name)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	node.set_name(node_name);
	return sfel_node;
}

sfel_node_t *sfel_xml_node_set_value(sfel_node_t *sfel_node, const sfpr_char_t *node_value)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	node.set_value(node_value);
	return sfel_node;	
}

struct xml_memory_writer: pugi::xml_writer
{
    char* buffer;
    size_t capacity;
    size_t result;

    xml_memory_writer(): buffer(0), capacity(0), result(0)
    {
    }

    xml_memory_writer(char* buffer, size_t capacity): buffer(buffer), capacity(capacity), result(0)
    {
    }

    size_t written_size() const
    {
        return result < capacity ? result : capacity;
    }

    virtual void write(const void* data, size_t size)
    {
        if (result < capacity)
        {
            size_t chunk = (capacity - result < size) ? capacity - result : size;

            memcpy(buffer + result, data, chunk);
        }

        result += size;
    }
};

sfpr_int_t sfel_xml_node_print(sfel_node_t *sfel_node, sfpr_char_t *output,int size)
{
	if(!sfel_node){
		return -1;
	}
	xml_node node((xml_node_struct *)sfel_node);	
    xml_memory_writer writer(output, size - 1);
    node.print(writer);
    output[writer.written_size()] = 0;
    return 0;
}

sfpr_int_t sfel_xml_node_delete(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return -1;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_node parent = node.parent();
	parent.remove_child(node);
	return 0;
}


/**************************************************************************************************
 3. 节点属性操作函数
 **************************************************************************************************/
sfel_attr_t *sfel_xml_node_get_first_attr(sfel_node_t *sfel_node)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	return node.first_attribute().internal_object();
}

sfel_attr_t *sfel_xml_attr_get_sibling(sfel_attr_t *sfel_attr)
{
	if(!sfel_attr){
		return NULL;
	}
	xml_attribute attr((xml_attribute_struct *)sfel_attr);
	return attr.next_attribute().internal_object();
}

sfel_attr_t *sfel_xml_attr_get(sfel_node_t *sfel_node, const sfpr_char_t *attr_name)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_attribute attr = node.attribute(attr_name);
	if(attr.empty()){
		return NULL;
	}
	return attr.internal_object();
}

sfpr_char_t *sfel_xml_attr_get_aname(sfel_attr_t *sfel_attr)
{
	if(!sfel_attr){
		return NULL;
	}
	xml_attribute attr((xml_attribute_struct *)sfel_attr);
	return (sfpr_char_t*)attr.name();
}

sfpr_char_t *sfel_xml_attr_get_avalue(sfel_attr_t *sfel_attr)
{
	if(!sfel_attr){
		return NULL;
	}
	xml_attribute attr((xml_attribute_struct *)sfel_attr);
	return (sfpr_char_t*)attr.value();
}

sfpr_char_t *sfel_xml_attr_get_value(sfel_node_t *sfel_node, const sfpr_char_t *attr_name)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_attribute attr = node.attribute(attr_name);
	if(attr.empty()){
		return NULL;
	}
//	printf("sfel_xml_attr_get_value() node:%s  attr_name=%s  attr_value=%s\n",node.name(),attr_name,attr.value());
	return (sfpr_char_t*)attr.value();
}

sfel_attr_t *sfel_xml_attr_add(sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *attr_value)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_attribute attr = node.append_attribute(attr_name);
	attr.set_value(attr_value);
	return attr.internal_object();
}

sfel_attr_t *sfel_xml_attr_update(sfel_node_t *sfel_node, const sfpr_char_t *attr_name, const sfpr_char_t *new_attr_value)
{
	if(!sfel_node){
		return NULL;
	}
	xml_node node((xml_node_struct *)sfel_node);
	xml_attribute attr = node.attribute(attr_name);
	if(attr.empty()){
		attr = node.append_attribute(attr_name);
		attr.set_value(new_attr_value);
		return attr.internal_object();
	}
	attr.set_value(new_attr_value);
	return attr.internal_object();
}

sfpr_int_t sfel_xml_attr_delete(sfel_node_t *sfel_node, const sfpr_char_t *attr_name)
{
	if(!sfel_node){
		return -1;
	}
	xml_node node((xml_node_struct *)sfel_node);
	node.remove_attribute(attr_name);
	return 0;
}

#ifdef __cplusplus
}
#endif
