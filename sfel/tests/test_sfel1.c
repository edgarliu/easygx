#include <stdio.h>

#include "sfel_xml.h"
#include "sfel.h"

static attr_handle(sfel_node_t *node, sfel_attr_t *attr, void *param)
{
	printf("attr name:%s, value:%s\n", sfel_xml_attr_get_aname(attr), sfel_xml_attr_get_avalue(attr));
}

static node_handle(sfel_node_t *node, void *param)
{
	//printf("node:%p\n", node);
	printf("node name:%s, value:%s, type:%d\n", sfel_xml_node_get_name(node), sfel_xml_node_get_value(node),sfel_xml_node_get_type(node));
	if(sfel_xml_node_get_type(node) == sfel_node_data)
	{
		sfel_document_t *doc = sfel_xml_node_get_document(node);
		sfel_xml_node_set_value(node, "test update");
		printf("node new value:%s\n", sfel_xml_node_get_value(node));
	}
#if 0
	//sfel_attr_t *attr = sfel_xml_attr_get(node, "id");
	//printf("attr name:%s, value:%s\n", sfel_xml_attr_get_aname(attr), sfel_xml_attr_get_avalue(attr));

	//sfel_node_t *tmp = sfel_xml_attr_get_parent(attr);
	//printf("get parent node of attr:%p\n", tmp);
	printf("parent node name:%s, value:%s, type:%d\n", sfel_xml_node_get_name(tmp), sfel_xml_node_get_value(tmp),sfel_xml_node_get_type(tmp));
#endif
}

int main(int argc, char* argv[])
{
	sfel_document_t *doc = NULL;
	sfel_node_t *root = NULL;
	sfel_node_t *node = NULL;
	int counter = 0;
	int ret = 0;
	char output[100*1024];
	
	doc = sfel_xml_parse_file("client.xml", NULL);

    /** 查询节点***/
	root = sfel_xml_get_root(doc);
	printf("Root name:%s, value:%s\n", sfel_xml_node_get_name(root), sfel_xml_node_get_value(root));
#if 0

	node  = sfel_xml_node_get_child(root);
	printf("first child of Root name:%s, value:%s\n", sfel_xml_node_get_name(node), sfel_xml_node_get_value(node));

	node  = sfel_xml_node_get_sibling(node);
	printf("second child of Root name:%s, value:%s\n", sfel_xml_node_get_name(node), sfel_xml_node_get_value(node));
	node  = sfel_xml_node_get_sibling(node);
	printf("third child of Root:%p\n", node);

    //counter = sfel_xml_node_find(node, NULL, /*"id"*/NULL, /*"third"*/NULL, node_handle, NULL);
	//printf("there are nodes:%d\n", counter);

	//node  = sfel_xml_node_get_child(node);
	//printf("child node name:%s, value:%s\n", sfel_xml_node_get_name(node), sfel_xml_node_get_value(node));

    /** 查询属性****/
	//sfel_attr_t *attr = sfel_xml_attr_get_by_name(node, "id");
	//printf("attr name:%s, value:%s\n", sfel_xml_attr_get_name(attr), sfel_xml_attr_get_value(attr));

	counter = sfel_xml_node_enum_attr(node, NULL, "request", attr_handle, NULL);
	printf("there are attrs:%d\n", counter);
#endif
#if 0
	/***修改节点*****/
	ret = sfel_xml_node_set_value(node, "update");
	printf("update node:%d\n", ret);

	/***修改属性******/
	//ret = sfel_xml_attr_update(node, "method", "update");

	/***删除节点*****/
	ret = sfel_xml_node_delete(node);

	/***删除属性******/
	//ret = sfel_xml_attr_delete(node, "method");

	/***增加节点*****/
	//ret = sfel_xml_node_append_child_by_name(node, "add", "test");

	/***增加属性******/
	//ret = sfel_xml_attr_add(node, "method", "get");
	sfel_xml_save(doc, "client.xml");
#endif

#if 1
	//sfel_xml_node_find_all(root, 0, NULL, NULL,NULL, node_handle, NULL);
	node  = sfel_xml_node_get_child(root);
	node  = sfel_xml_node_get_child(node);
	node  = sfel_xml_node_get_sibling(node);
	//node  = sfel_xml_node_get_sibling(node);
	printf("node:%s\n", sfel_xml_node_get_name(node));
	//sfel_xml_node_enum_attr(node,NULL,NULL,attr_handle,NULL);
#endif

	memset(output, 0x0, sizeof(output));
	//sfel_xml_doc_print(doc, output,sizeof(output));
	sfel_xml_node_print(node, output,sizeof(output));
	printf("xml content:%s\n", output);
	
	sfel_xml_destroy(doc);
	printf("end main.\n");
	return 0;
}

