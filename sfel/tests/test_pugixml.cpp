// testrapidxml.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include "pugixml/pugixml.hpp"

#include "sfel.h"

using namespace std;
using namespace pugi;

int main(int argc, char* argv[])
{

	const char *file_name = "client.xml";
	xml_document *doc = new xml_document();
	xml_parse_result result = doc->load_file(file_name);
	if(result.status != status_ok){
		printf("open file:%s failed,status:%d",file_name,result.status);
	}
	printf("open file:%s success\n",file_name);

	//! 获取根节点
	xml_node root_node = doc->document_element();
//	xml_node_struct* root_node = doc->internal_object();
//	xml_node new_doc = xml_node(root_node);
	printf("root:%s \n",root_node.name());
	
//    pugi::xml_node decl = doc->prepend_child(pugi::node_declaration);
//    decl.append_attribute("version") = "1.0";
//    decl.append_attribute("encoding") = "UTF-8";

	xml_node child_node = root_node.child("desktop");
	xml_node root = child_node.root();
	xml_document *pdoc = (xml_document*)&root;

	printf("doc:%p   %p    %p  %p\n",doc,doc->internal_object(),pdoc->get_xmldoc(),root.internal_object());

	pdoc->save_file("client_save1.xml");
	doc->save_file("client_save.xml");

	delete (xml_document*)pdoc->get_xmldoc();
//	delete doc;

/*
	xml_document *doc = new xml_document();

	doc->append_child("xqliu");
	
	doc->save_file("client_save.xml");
	delete doc;
*/
	return EXIT_SUCCESS;
}

