
#include <string.h>

#include "easygx.h"
#include "easygx_widget.h"
#include "easygx_frame.h"
#include "easygx_control.h"
#include "easygx_portable.h"
#include "easygx_utils.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
 * 0. 公共部分
*****************************************************************************************/
static int egx_control_resid = EASYGX_ID_CONTROL;
int egx_control_generate_resid()
{
	return egx_control_resid++;
}

/*****************************************************************************************
 * 1. Label
*****************************************************************************************/
int egx_label_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_label_t *_ctrl=NULL;
	_ctrl = (egx_label_t *)malloc(sizeof(egx_label_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_label_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_label;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_label_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_label_create() |  id:%s  handle:0x%x  res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);	
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;
}

int egx_label_set_text(char *id,char *data)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	egx_label_set_text_(ctrl->object,data);
	return 0;	
}

/*****************************************************************************************
 * 2. Button
*****************************************************************************************/
int egx_button_enable(char *id,int enable)
{
	egx_control_t *control = (egx_control_t*)egx_widget_find(id);
	if(!control){
		return -1;
	}
	return egx_button_enable_(control->object,enable);	
}

int egx_button_checked(char *id)
{
	egx_control_t *control = (egx_control_t*)egx_widget_find(id);
	if(!control){
		return -1;
	}
	return egx_button_checked_(control->object);	
}

int egx_pushbutton_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_button_t *_ctrl=NULL;
	_ctrl = (egx_button_t *)malloc(sizeof(egx_button_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_button_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_button;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_pushbutton_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_pushbutton_create() |  id:%s  handle:0x%x  res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

int egx_checkbox_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_checkbox_t *_ctrl=NULL;
	_ctrl = (egx_checkbox_t *)malloc(sizeof(egx_checkbox_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_checkbox_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_checkbox;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_checkbox_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_checkbox_create() |  id:%s  handle:0x%x  res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

int egx_groupbox_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_groupbox_t *_ctrl=NULL;
	_ctrl = (egx_groupbox_t *)malloc(sizeof(egx_groupbox_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_groupbox_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_groupbox;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_groupbox_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_groupbox_create() |  id:%s  handle:0x%x  res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

int egx_radiobutton_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_radiobutton_t *_ctrl=NULL;
	_ctrl = (egx_radiobutton_t *)malloc(sizeof(egx_radiobutton_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_radiobutton_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_radiobutton;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	if(parent && (style & EGX_STYLE_GROUP)){
		parent->curr_radiogroup = egx_radiobutton_begin_(parent->object);
	}
	_ctrl->object = egx_radiobutton_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0,(parent)?(parent->curr_radiogroup):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_radiobutton_create() |  id:%s  handle:0x%x  res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

/*****************************************************************************************
 * 3. Edit
*****************************************************************************************/
int egx_edit_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_edit_t *_ctrl=NULL;
	_ctrl = (egx_edit_t *)malloc(sizeof(egx_edit_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_edit_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_edit;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_edit_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_edit_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

int egx_edit_get_text(char *id,char *text,int length)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	egx_edit_get_text_(ctrl->object,text,length);
	return 0;
}

int egx_edit_set_text(char *id,char *text)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	egx_edit_set_text_(ctrl->object,text);
	return 0;	
}

int egx_multiedit_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_multiedit_t *_ctrl=NULL;
	_ctrl = (egx_multiedit_t *)malloc(sizeof(egx_multiedit_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_multiedit_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_multiedit;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_multiedit_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_edit_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

/*****************************************************************************************
 * 4. ListBox
*****************************************************************************************/
int egx_listbox_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_listbox_t *_ctrl=NULL;
	_ctrl = (egx_listbox_t *)malloc(sizeof(egx_listbox_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_listbox_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_listbox;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_listbox_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_listbox_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;		
}

int egx_listbox_get_count(char *id)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	return egx_listbox_get_count_(ctrl->object);	
}

int egx_listbox_add_item(char *id,char *name,void* data)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	egx_listbox_add_item_(ctrl->object,name,data);
	return 0;
}

int egx_listbox_get_selected_item(char *id)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	return egx_listbox_get_selected_item_(ctrl->object);
}

int egx_listbox_get_selected_name(char *id,char *buffer,int length)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	egx_listbox_get_selected_name_(ctrl->object,buffer,length);
	return 0;
}

void* egx_listbox_get_selected_data(char *id)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return NULL;
	}
	return egx_listbox_get_selected_data_(ctrl->object);
}


/*****************************************************************************************
 * 5. ComboBox
*****************************************************************************************/
int egx_combobox_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_combobox_t *_ctrl=NULL;
	_ctrl = (egx_combobox_t *)malloc(sizeof(egx_combobox_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_combobox_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_combobox;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_combobox_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_combobox_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

int egx_combobox_get_count(char *id)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	return egx_combobox_get_count_(ctrl->object);	
}

int egx_combobox_add_item(char *id,char *name,void* data)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	egx_combobox_add_item_(ctrl->object,name,data);
	return 0;
}

int egx_combobox_set_selected_item(char *id,int index)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	return egx_combobox_set_selected_item_(ctrl->object,index);
}

int egx_combobox_get_selected_item(char *id)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	return egx_combobox_get_selected_item_(ctrl->object);		
}

int egx_combobox_get_selected_name(char *id,char *buffer,int length)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	egx_combobox_get_selected_name_(ctrl->object,buffer,length);		
	return 0;
}

void* egx_combobox_get_selected_data(char *id)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return NULL;
	}
	return egx_combobox_get_selected_data_(ctrl->object);		
}

/*****************************************************************************************
 * 6. Tree
*****************************************************************************************/

int egx_tree_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_tree_t *_ctrl=NULL;
	_ctrl = (egx_tree_t *)malloc(sizeof(egx_tree_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_tree_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_tree;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_tree_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_tree_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;		
}

int egx_tree_insert_item(char *id,int level,char *text)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	return egx_tree_insert_item_(ctrl->object,level,text);
}

/*****************************************************************************************
 * 7. List
*****************************************************************************************/

int egx_list_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_list_t *_ctrl=NULL;
	_ctrl = (egx_list_t *)malloc(sizeof(egx_list_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_list_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_list;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_list_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_list_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

/*****************************************************************************************
 * 8. Table
*****************************************************************************************/

int egx_table_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_table_t *_ctrl=NULL;
	_ctrl = (egx_table_t *)malloc(sizeof(egx_table_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_table_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_table;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_table_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_table_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

int egx_table_init_column(char *id,int column,int width,char *name)
{
	egx_table_t *ctrl = (egx_table_t*)egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	ctrl->columns++;
	return egx_table_init_column_(ctrl->object,column,width,name);		
}

int egx_table_init_item(char *id,int rows)
{
	egx_table_t *ctrl = (egx_table_t*)egx_widget_find(id);
	int line;
	if(!ctrl){
		return -1;
	}
	ctrl->rows = rows;
	for(line=0;line<rows;line++){
		egx_table_init_item_(ctrl->object,line,ctrl->columns);
	}
	return 0;
}


int egx_table_set_item_data(char *id,int row,int column,char *text)
{
	egx_widget_t *ctrl = egx_widget_find(id);
	if(!ctrl){
		return -1;
	}
	return egx_table_set_item_data_(ctrl->object,row,column,text);
}

int egx_table_set_item(char *id,int row,char *text[])
{
	egx_table_t *ctrl = (egx_table_t*)egx_widget_find(id);
	int count;
	if(!ctrl){
		return -1;
	}
	for(count = 0;count < ctrl->columns;count++){
		if(text[count]){
			egx_table_set_item_data_(ctrl->object,row,count,text[count]);
		}
	}
	return 0;
}

int egx_table_set_line(char *id,int row,char *text,char *sep)
{
	egx_table_t *ctrl = (egx_table_t*)egx_widget_find(id);
	char *array[32];
	int num = 0,count = 0;
	if(!ctrl){
		return -1;
	}
	memset(array,0,sizeof(array));
	num = sfpr_str_split(text,array,32,sep);
	for(count = 0;count < ctrl->columns;count++){
		if(array[count]){
			egx_table_set_item_data_(ctrl->object,row,count,array[count]);
		}
	}
}

/*****************************************************************************************
 * 9. TextArea
*****************************************************************************************/

int egx_textarea_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_textarea_t *_ctrl=NULL;
	_ctrl = (egx_textarea_t *)malloc(sizeof(egx_textarea_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_textarea_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_textarea;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_textarea_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_textarea_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;		
}

/*****************************************************************************************
 * 10. Image
*****************************************************************************************/

int egx_image_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_image_t *_ctrl=NULL;
	_ctrl = (egx_image_t *)malloc(sizeof(egx_image_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_image_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_image;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_image_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_image_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;	
}

/*****************************************************************************************
 * 11. ProgressBar
*****************************************************************************************/

int egx_progressbar_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_progressbar_t *_ctrl=NULL;
	_ctrl = (egx_progressbar_t *)malloc(sizeof(egx_progressbar_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_progressbar_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_progressbar;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_progressbar_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_progressbar_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;		
}

/*****************************************************************************************
 * 12. DateTime
*****************************************************************************************/

int egx_datetime_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_datetime_t *_ctrl=NULL;
	_ctrl = (egx_datetime_t *)malloc(sizeof(egx_datetime_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_datetime_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_datetime;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_datetime_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_datetime_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;		
}

/*****************************************************************************************
 * 13. IpAddr
*****************************************************************************************/

int egx_ipaddr_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_ipaddr_t *_ctrl=NULL;
	_ctrl = (egx_ipaddr_t *)malloc(sizeof(egx_ipaddr_t));
	if(!_ctrl)
		return -1;
	memset(_ctrl, 0x0, sizeof(egx_ipaddr_t));
	_ctrl->id = egx_string_malloc(id);
	if(!_ctrl->id){
		free(_ctrl);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_ctrl->widget_type = egx_widget_control;
	_ctrl->control_type = egx_control_type_ipaddr;
	_ctrl->rect.x = x;
	_ctrl->rect.y = y;
	_ctrl->rect.width = width;
	_ctrl->rect.height = height;
	_ctrl->cur_frame = parent;
	_ctrl->object = egx_ipaddr_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_ctrl->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_ipaddr_create() | id:%s fd:0x%x res_id:%d\n",_ctrl->id,_ctrl->object,_ctrl->res_id);
	egx_widget_set_struct_(_ctrl->object,(egx_widget_t*)_ctrl);
	egx_widget_register((egx_widget_t*)_ctrl);
	return 0;		
}

#ifdef __cplusplus
}
#endif
