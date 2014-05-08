
#include <stdio.h>
#include <string.h>

#include "sfel.h"

int tag_sfel_action_init(sfel_node_t *node,void *param,int depth, int status)
{
	
	printf("%*s init node name:%s, value:%s\n",
			depth*4," ",sfel_xml_node_get_name(node),  sfel_xml_node_get_value(node));
}


int tag_sfel_action_update(sfel_node_t *node,void *param,int depth, int status)
{
	printf("%*s update node name:%s, value:%s\n",
			depth*4," ",sfel_xml_node_get_name(node),  sfel_xml_node_get_value(node));
}

int main(int argc,char *argv[])
{
	sfel_instance_t *instance = NULL;
	sfel_node_t *node = NULL;


	sfel_init();
	
	sfel_tag_enum();
	
	sfel_action_register("sfel","Init",tag_sfel_action_init);
	sfel_action_register("sfel","Update",tag_sfel_action_update);
	sfel_action_register("desktop","Init",tag_sfel_action_init);
	sfel_action_register("desktop","Update",tag_sfel_action_update);
	sfel_action_register("window","Init",tag_sfel_action_init);
	sfel_action_register("window","Update",tag_sfel_action_update);
	sfel_action_register("dialog","Init",tag_sfel_action_init);
	sfel_action_register("dialog","Update",tag_sfel_action_update);
	sfel_action_register("panel","Init",tag_sfel_action_init);
	sfel_action_register("panel","Update",tag_sfel_action_update);
	sfel_action_register("panelbuttons","Init",tag_sfel_action_init);
	sfel_action_register("panelbuttons","Update",tag_sfel_action_update);
	sfel_action_register("lamps","Init",tag_sfel_action_init);
	sfel_action_register("lamps","Update",tag_sfel_action_update);

	sfel_tag_attr_enum("window");
	
	sfel_quit();
	
	return 0;	
}
