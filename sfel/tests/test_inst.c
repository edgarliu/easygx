
#include <stdio.h>
#include <string.h>

#include "sfel.h"


int tag_sfel_script_init(sfel_node_t *node,void *param,int depth,int status)
{
	char errstr[256];
	char buff[256];
	int nret = 0;
	char *style = NULL;
	if(status == SFEL_NODE_END){
		return 0;
	}

	style = sfel_xml_attr_get_value(node,"style");
	if(style == NULL){
		return 0;
	}
	if(0 == strcmp(style,"ii>is")){		
		if(sfel_lua_call_func(NULL,node,errstr,"ii>is",2,7,&nret,buff) < 0){
			printf("error: %s\n",errstr);
			return 0;
		}
		printf("tag_sfel_script_init %d  %s\n",nret,buff);
	}
	return 0;
}

int tag_sfel_action_init(sfel_node_t *node,void *param,int depth,int status)
{
	int counter = 0;
#if 1
	printf("%*s init node name:%s, value:%s\n",
			depth*4," ",sfel_xml_node_get_name(node),  sfel_xml_node_get_value(node));
#endif
}


int tag_sfel_action_update(sfel_node_t *node,void *param,int depth,int status)
{
#if 1	
	printf("%*s update node name:%s, value:%s\n",
			depth*4," ",sfel_xml_node_get_name(node),  sfel_xml_node_get_value(node));
#endif
}

#ifdef VXWORKS
int inst_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	sfel_instance_t *instance = NULL;
	sfel_node_t *node = NULL;
	sfel_exec_mode_t mode;

	sfel_init();
	
	sfel_action_register("script","Init",tag_sfel_script_init);
	#if 0
	sfel_action_register("request","Init",tag_sfel_action_init);
	sfel_action_register("desktop","Init",tag_sfel_action_init);
	sfel_action_register("window","Init",tag_sfel_action_init);
	sfel_action_register("dialog","Init",tag_sfel_action_init);
	sfel_action_register("panel","Init",tag_sfel_action_init);
	sfel_action_register("panelbuttons","Init",tag_sfel_action_init);
	sfel_action_register("lamps","Init",tag_sfel_action_init);

	sfel_action_register("sfel","Update",tag_sfel_action_update);
	sfel_action_register("desktop","Update",tag_sfel_action_update);
	sfel_action_register("window","Update",tag_sfel_action_update);
	sfel_action_register("dialog","Update",tag_sfel_action_update);
	sfel_action_register("panel","Update",tag_sfel_action_update);
	sfel_action_register("panelbuttons","Update",tag_sfel_action_update);
	sfel_action_register("lamps","Update",tag_sfel_action_update);
	#endif

	#ifdef VXWORKS
	instance = sfel_inst_create("/ata0a/client.xml", 0, NULL);
	#else
	instance = sfel_inst_create("client.xml", 0, NULL);
	#endif
	if(NULL == instance){
		printf("create instance failed\n");
		sfel_quit();
		return 0;
	}
	printf("create instance success.\n");

	//执行节点的回调函数
	printf("begin sfel_inst_exec......\n");

	memset(&mode, 0x0, sizeof(mode));
	mode.dsfel_on = 1;
	mode.default_action_on = 0;
	mode.pattern = "*";
	mode.recursive = -1;
	sfel_inst_exec(instance->root,"Init",NULL,&mode);
	
	//销毁一个实例
	sfel_inst_destroy(&instance);

	/********************测试文件管理********************************/	
	sfel_quit();
	
	return 0;	
}
