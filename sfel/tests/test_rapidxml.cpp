// testrapidxml.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "sfel_portable.h"
#include "sfel.h"

using namespace std;
using namespace rapidxml;

int main(int argc, char* argv[])
{
	file<> fdoc("client.xml");
	std::cout<<fdoc.data()<<std::endl; 

	xml_document<>  doc;    
	doc.parse<0>(fdoc.data());
	std::cout<<doc.name()<<std::endl;

	//! ��ȡ���ڵ�
	xml_node<>* root = doc.first_node();
	std::cout<<root->name()<<std::endl;
	//! ��ȡ���ڵ��һ���ڵ�
	xml_node<>* node1 = root->first_node();
	std::cout<<node1->name()<<std::endl;
	xml_node<>* node11 = node1->first_node();
	std::cout<<node11->name()<<std::endl;
	std::cout<<node11->value()<<std::endl;

	//! �޸�֮���ٴα���
	xml_node<>* size = root->first_node("size");
	size->append_node(doc.allocate_node(node_element,"w","0"));
	size->append_node(doc.allocate_node(node_element,"h","0"));
	std::string text;  
	rapidxml::print(std::back_inserter(text),doc,0);  

	std::cout<<text<<std::endl; 

	std::ofstream out("client.xml");
	out << doc;
	system("PAUSE");

	return EXIT_SUCCESS;
}

