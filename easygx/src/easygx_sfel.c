
#include <stdlib.h>
#include <string.h>

#include "easygx.h"
#include "easygx_core.h"
#include "easygx_frame.h"
#include "easygx_sfel.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

struct egx_exec_param_s{
	char *curr_frame;
	char *curr_window;
};

static int create_desktop(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	easygx_t *easygx = easygx_instance_get();
	char *pwidth = NULL,*pheight = NULL;
	if(status == SFEL_NODE_END){
		easygx_t *easygx = easygx_instance_get();
		easygx->firstwnd = sfel_xml_attr_get_value(node,"firstwnd");
		return 0;
	}
	sfpr_log(easygx_log,SFPR_LOG_INFO,"easygx_sfel_create() |  node:%s  id:%s  name:%s\n",
			sfel_xml_node_get_name(node),sfel_xml_attr_get_value(node,"id"),sfel_xml_attr_get_value(node,"name"));
	
	pwidth = sfel_xml_attr_get_value(node,"width");
	pheight = sfel_xml_attr_get_value(node,"height");
	easygx->width = (pwidth)?atoi(pwidth):320;
	easygx->height = (pheight)?atoi(pheight):240;
	return 0;
}

static int create_window(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	easygx_t *easygx = easygx_instance_get();
	char *id = NULL,*name = NULL,*view_show = NULL;
	if(status == SFEL_NODE_END){
		egx_param->curr_frame = NULL;
		egx_param->curr_window = NULL;
		return 0;
	}
	id = sfel_xml_attr_get_value(node,"id");
	name = sfel_xml_attr_get_value(node,"name");
//	sfpr_log(easygx_log,SFPR_LOG_INFO,"easygx_sfel_create() |  node:%s  id:%s  name:%s\n",
//			sfel_xml_node_get_name(node),id,name);
	egx_window_create(id,egx_window_type_general,name,0,0,0,easygx->width,easygx->height);
	view_show = sfel_xml_attr_get_value(node,"view_show");
	if(view_show && 0 == strcmp(view_show,"stack")){
		egx_window_set_stack(id);
	}
	egx_param->curr_frame = id;
	egx_param->curr_window = id;
	return 0;
}

static int create_dialog(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	
}

static int create_view(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	easygx_t *easygx = easygx_instance_get();
	char *id = NULL,*name = NULL;
	if(status == SFEL_NODE_END){
		egx_param->curr_frame = egx_param->curr_window;
		return 0;
	}
	id = sfel_xml_attr_get_value(node,"id");
	name = sfel_xml_attr_get_value(node,"name");
//	sfpr_log(easygx_log,SFPR_LOG_INFO,"easygx_sfel_create() |  node:%s  id:%s  name:%s\n",
//			sfel_xml_node_get_name(node),id,name);
	egx_view_create(id,egx_view_type_general,name,egx_param->curr_window);
	egx_param->curr_frame = id;
	return 0;
}

static int create_title(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	char *name = NULL;
	if(status == SFEL_NODE_END){
		return 0;
	}
	name = sfel_xml_attr_get_value(node,"name");
	if(name){
		egx_frame_set_title(egx_param->curr_frame,name);
	}
	return 0;
}

static int create_status(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	char *name = NULL;
	if(status == SFEL_NODE_END){
		return 0;
	}
	return 0;
}

static int create_control(sfel_node_t *node,struct egx_exec_param_s *egx_param,sfpr_int_t depth,egx_control_type_e control_type)
{
	char *id = NULL,*name = NULL;
	char *px = NULL,*py = NULL,*pwidth = NULL,*pheight = NULL;
	int x=0,y=0,width = 0,height = 0;
	egx_uint32_t style = 0;
	
	id = sfel_xml_attr_get_value(node,"id");
	name = sfel_xml_attr_get_value(node,"name");
	px = sfel_xml_attr_get_value(node,"x");
	py = sfel_xml_attr_get_value(node,"y");
	pwidth = sfel_xml_attr_get_value(node,"width");
	pheight = sfel_xml_attr_get_value(node,"height");
	x = (px)?atoi(px):0;
	y = (py)?atoi(py):0;
	width = (pwidth)?atoi(pwidth):0;
	height = (pheight)?atoi(pheight):0;
//	sfpr_log(easygx_log,SFPR_LOG_INFO,"easygx_sfel_create() |  node:%s  id:%s  name:%s\n",
//			sfel_xml_node_get_name(node),id,name);
	switch(control_type){
		case egx_control_type_label:{
			egx_label_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_button:{
			egx_pushbutton_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			char *jump = sfel_xml_attr_get_value(node,"jump");
			char *data_src = sfel_xml_attr_get_value(node,"data_src");
			char *data_dest = sfel_xml_attr_get_value(node,"data_dest");
			if(jump){
				egx_message_pushbutton_jump(id,jump,NULL);
			}else if(data_src && data_dest){
				egx_message_pushbutton_trans(id,data_src,data_dest,NULL);				
			}
			break;
		}
		case egx_control_type_checkbox:{
			egx_checkbox_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_groupbox:{
			egx_groupbox_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_radiobutton:{
			sfel_node_t *first_node = sfel_xml_node_get_child(sfel_xml_node_get_parent(node));
			if(first_node == node){
				style |= EGX_STYLE_GROUP;
			}
			egx_radiobutton_create(id,0,name,style,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_edit:{
			egx_edit_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_multiedit:{
			egx_multiedit_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_listbox:{
			egx_listbox_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_combobox:{
			egx_combobox_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_tree:{
			egx_tree_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_list:{
			egx_list_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_table:{
			egx_table_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_textarea:{
			egx_textarea_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_image:{
			egx_image_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_progressbar:{
			egx_progressbar_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_datetime:{
			egx_datetime_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
		case egx_control_type_ipaddr:{
			egx_ipaddr_create(id,0,name,0,x,y,width,height,egx_param->curr_frame);
			break;
		}
	}
	return 0;
}

static int create_button(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_button);
}

static int create_checkbox(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_checkbox);
}

static int create_groupbox(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_groupbox);
}

static int create_radiobutton(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_radiobutton);
}

static int create_edit(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_edit);
}

static int create_multiedit(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_multiedit);
}

static int create_listbox(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_listbox);
}

static int create_combox(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_combobox);
}

static int create_label(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_label);
}

static int create_textarea(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_textarea);
}

static int create_list(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_list);
}

static int create_table(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_table);
}

static int create_progressbar(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_progressbar);
}

static int create_datetime(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_datetime);
}

static int create_ipaddr(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_ipaddr);
}

static int create_image(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	if(status == SFEL_NODE_END){
		return 0;
	}
	return create_control(node,(struct egx_exec_param_s*)param,depth,egx_control_type_image);
}

static sfel_action_t easygx_create_action[] = {
	{"desktop", 	create_desktop},	
	{"window", 	create_window},
	{"dialog", 	    create_dialog},
	{"view", 	    create_view},
	{"title", 	    create_title},
	{"status", 	create_status},
	{"button", 	create_button},
	{"checkbox", 	create_checkbox},
	{"groupbox", 	create_groupbox},
	{"radiobutton", 	create_radiobutton},
	{"edit", 	    create_edit},
	{"multiedit", 	    create_multiedit},
	{"listbox", 	    create_listbox},
	{"combox", 	    create_combox},
	{"label", 		create_label},	
	{"textarea", 	    create_textarea},
	{"list", 	    create_list},
	{"table", 	    create_table},
	{"progressbar", 	    create_progressbar},
	{"datetime", 	    create_datetime},
	{"ipaddr", 	    create_ipaddr},
	{"image", 	    create_image},
//	{"", 	    create_},
	{NULL,NULL}	
};


static int destroy_desktop(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	if(status == SFEL_NODE_END){

		return 0;
	}
//	sfpr_log(easygx_log,SFPR_LOG_INFO,"easygx_sfel_destroy() |  node:%s  id:%s  name:%s\n",
//			sfel_xml_node_get_name(node),sfel_xml_attr_get_value(node,"id"),sfel_xml_attr_get_value(node,"name"));
	return 0;
}

static int destroy_window(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	if(status == SFEL_NODE_END){

		return 0;
	}

	return 0;
}

static int destroy_label(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	if(status == SFEL_NODE_END){

		return 0;
	}

	return 0;
}

static int destroy_button(sfel_node_t *node,sfpr_void_t *param, sfpr_int_t depth, sfpr_int_t status)
{
	struct egx_exec_param_s *egx_param = (struct egx_exec_param_s*)param;	
	if(status == SFEL_NODE_END){
		return 0;
	}

	return 0;
}

static sfel_action_t easygx_destroy_action[] = {
	{"desktop", 	destroy_desktop},	
	{"window", 	destroy_window},	
	{"label", 		destroy_label},	
	{"button", 	destroy_button},	
	{NULL,NULL}	
};

int easygx_sfel_create(sfel_node_t *node)
{
	struct egx_exec_param_s egx_param;
	sfel_exec_mode_t mode;
	if(!node){
		return -1;
	}

	//注册sfel节点处理函数
	sfel_action_registers("Create",easygx_create_action);

	//执行节点的回调函数
	memset(&mode, 0x0, sizeof(mode));
	mode.dsfel_on = 0;
	mode.default_action_on = 0;
	mode.pattern = "*";
	mode.recursive = -1;
	memset(&egx_param,0,sizeof(egx_param));
	return sfel_inst_exec(node,"Create",&egx_param,&mode);
}

int easygx_sfel_show(sfel_node_t *node)
{
	return 0;	
}

int easygx_sfel_destroy(sfel_node_t *node)
{
	struct egx_exec_param_s egx_param;
	sfel_exec_mode_t mode;
	if(!node){
		return -1;
	}

	//注册sfel节点处理函数
	sfel_action_registers("Destroy",easygx_destroy_action);

	//执行节点的回调函数
	memset(&mode, 0x0, sizeof(mode));
	mode.dsfel_on = 0;
	mode.default_action_on = 0;
	mode.pattern = "*";
	mode.recursive = -1;
	memset(&egx_param,0,sizeof(egx_param));
	return sfel_inst_exec(node,"Destroy",&egx_param,&mode);
}

int easygx_sfel_file_register(const char *sfel_file)
{
	easygx_t *easygx = easygx_instance_get();
	sfel_instance_t *instance = NULL;
	if(!easygx){
		return -1;
	}
	if(easygx->sfel_inst){
		return -2;
	}
	instance = sfel_inst_create((char*)sfel_file, 0, NULL);
	if(NULL == instance){
		return -3;
	}
	easygx->sfel_inst = instance;
	easygx_sfel_create(instance->root);
	return 0;
}

int easygx_sfel_file_show()
{
	easygx_t *easygx = easygx_instance_get();
	if(!easygx){
		return -1;
	}
	if(NULL == easygx->sfel_inst){
		return -2;
	}
	egx_window_show(easygx->firstwnd);
	return 0;
}

int easygx_sfel_file_unregister()
{
	easygx_t *easygx = easygx_instance_get();
	if(!easygx){
		return -1;
	}
	if(NULL == easygx->sfel_inst){
		return -2;
	}
	easygx_sfel_destroy(easygx->sfel_inst->root);	
	sfel_inst_destroy(&(easygx->sfel_inst));
	easygx->sfel_inst = NULL;
	return 0;
}


#ifdef __cplusplus
}
#endif
