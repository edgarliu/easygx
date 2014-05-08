
#include "sfel_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

sfel_node_t *sfel_parse_file(char *file,sfel_portable_t *portable)
{
	FILE	*fp;  
	sfel_node_t	*tree;

	fp = fopen(file,"rb");
	tree = mxmlLoadFile(NULL, fp, portable->cb);
  fclose(fp);
	return tree;
}

void sfel_delete(sfel_node_t *node)
{
	mxmlDelete(node);
}

sfel_node_t *sfel_find_element(sfel_node_t *node,const char *name, const char *attr,const char *value)
{
	return mxmlFindElement(node,node,name,attr,value,MXML_DESCEND);
}


sfel_node_t *sfel_get_child(sfel_node_t *node)
{
	sfel_node_t *child = node->child;
	while(child){
		if(child->type == MXML_ELEMENT)
			return child;		
		child = child->next;
	}
	return NULL;
}

sfel_node_t *sfel_get_sibling(sfel_node_t *node)
{
	sfel_node_t *sibling = node->next;
	while(sibling){
		if(sibling->type == MXML_ELEMENT)
			return sibling;		
		sibling = sibling->next;
	}
	return NULL;
}

sfel_node_t *sfel_get_parent(sfel_node_t *node)
{
	return node->parent;
}

sfel_attr_t *sfel_get_attr(sfel_node_t *node,int *attr_num)
{
	
	
	return NULL;
}


#ifdef __cplusplus
}
#endif

