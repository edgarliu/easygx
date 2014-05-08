
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
 * 1. 公共部分
*****************************************************************************************/
static int egx_control_resid = EASYGX_ID_CONTROL;
int egx_control_generate_resid()
{
	return egx_control_resid++;
}

/*****************************************************************************************
 * 2. Label
*****************************************************************************************/
int egx_label_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_label_t *_label=NULL;
	_label = (egx_label_t *)malloc(sizeof(egx_label_t));
	if(!_label)
		return -1;
	memset(_label, 0x0, sizeof(egx_label_t));
	_label->id = egx_string_malloc(id);
	if(!_label->id){
		free(_label);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_label->widget_type = egx_widget_control;
	_label->control_type = egx_control_type_label;
	_label->rect.x = x;
	_label->rect.y = y;
	_label->rect.width = width;
	_label->rect.height = height;
	_label->cur_frame = parent;
	_label->object = egx_label_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_label->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_label_create() |  id:%s  handle:0x%x  res_id:%d\n",_label->id,_label->object,_label->res_id);	
	egx_widget_set_struct_(_label->object,(egx_widget_t*)_label);
	egx_widget_register((egx_widget_t*)_label);
	return 0;
}

int egx_label_set_text(char *id,char *data)
{
	egx_label_t *label = (egx_label_t*)egx_widget_find(id);
	if(!label){
		return -1;
	}
	egx_label_set_text_(label->object,data);
	return 0;	
}

/*****************************************************************************************
 * 3. Button
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
	egx_button_t *_button=NULL;
	_button = (egx_button_t *)malloc(sizeof(egx_button_t));
	if(!_button)
		return -1;
	memset(_button, 0x0, sizeof(egx_button_t));
	_button->id = egx_string_malloc(id);
	if(!_button->id){
		free(_button);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_button->widget_type = egx_widget_control;
	_button->control_type = egx_control_type_button;
	_button->rect.x = x;
	_button->rect.y = y;
	_button->rect.width = width;
	_button->rect.height = height;
	_button->cur_frame = parent;
	_button->object = egx_pushbutton_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_button->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_pushbutton_create() |  id:%s  handle:0x%x  res_id:%d\n",_button->id,_button->object,_button->res_id);
	egx_widget_set_struct_(_button->object,(egx_widget_t*)_button);
	egx_widget_register((egx_widget_t*)_button);
	return 0;	
}

int egx_checkbox_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_checkbox_t *_button=NULL;
	_button = (egx_checkbox_t *)malloc(sizeof(egx_checkbox_t));
	if(!_button)
		return -1;
	memset(_button, 0x0, sizeof(egx_checkbox_t));
	_button->id = egx_string_malloc(id);
	if(!_button->id){
		free(_button);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_button->widget_type = egx_widget_control;
	_button->control_type = egx_control_type_checkbox;
	_button->rect.x = x;
	_button->rect.y = y;
	_button->rect.width = width;
	_button->rect.height = height;
	_button->cur_frame = parent;
	_button->object = egx_checkbox_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_button->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_checkbox_create() |  id:%s  handle:0x%x  res_id:%d\n",_button->id,_button->object,_button->res_id);
	egx_widget_set_struct_(_button->object,(egx_widget_t*)_button);
	egx_widget_register((egx_widget_t*)_button);
	return 0;	
}

int egx_groupbox_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_groupbox_t *_button=NULL;
	_button = (egx_groupbox_t *)malloc(sizeof(egx_groupbox_t));
	if(!_button)
		return -1;
	memset(_button, 0x0, sizeof(egx_groupbox_t));
	_button->id = egx_string_malloc(id);
	if(!_button->id){
		free(_button);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_button->widget_type = egx_widget_control;
	_button->control_type = egx_control_type_groupbox;
	_button->rect.x = x;
	_button->rect.y = y;
	_button->rect.width = width;
	_button->rect.height = height;
	_button->cur_frame = parent;
	_button->object = egx_groupbox_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_button->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_groupbox_create() |  id:%s  handle:0x%x  res_id:%d\n",_button->id,_button->object,_button->res_id);
	egx_widget_set_struct_(_button->object,(egx_widget_t*)_button);
	egx_widget_register((egx_widget_t*)_button);
	return 0;	
}

int egx_radiobutton_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_radiobutton_t *_button=NULL;
	_button = (egx_radiobutton_t *)malloc(sizeof(egx_radiobutton_t));
	if(!_button)
		return -1;
	memset(_button, 0x0, sizeof(egx_radiobutton_t));
	_button->id = egx_string_malloc(id);
	if(!_button->id){
		free(_button);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_button->widget_type = egx_widget_control;
	_button->control_type = egx_control_type_radiobutton;
	_button->rect.x = x;
	_button->rect.y = y;
	_button->rect.width = width;
	_button->rect.height = height;
	_button->cur_frame = parent;
	if(parent && (style & EGX_STYLE_GROUP)){
		parent->curr_radiogroup = egx_radiobutton_begin_(parent->object);
	}
	_button->object = egx_radiobutton_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0,(parent)?(parent->curr_radiogroup):0);
	_button->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_radiobutton_create() |  id:%s  handle:0x%x  res_id:%d\n",_button->id,_button->object,_button->res_id);
	egx_widget_set_struct_(_button->object,(egx_widget_t*)_button);
	egx_widget_register((egx_widget_t*)_button);
	return 0;	
}

/*****************************************************************************************
 * 4. Edit
*****************************************************************************************/
int egx_edit_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id)
{
	egx_frame_t *parent = (egx_frame_t*)egx_widget_find(parent_id);
	egx_edit_t *_edit=NULL;
	_edit = (egx_edit_t *)malloc(sizeof(egx_edit_t));
	if(!_edit)
		return -1;
	memset(_edit, 0x0, sizeof(egx_edit_t));
	_edit->id = egx_string_malloc(id);
	if(!_edit->id){
		free(_edit);
		return -1;
	}
	if(res_id <= 0){
		res_id = egx_control_generate_resid();
	}
	_edit->widget_type = egx_widget_control;
	_edit->control_type = egx_control_type_edit;
	_edit->rect.x = x;
	_edit->rect.y = y;
	_edit->rect.width = width;
	_edit->rect.height = height;
	_edit->cur_frame = parent;
	_edit->object = egx_edit_create_(res_id,name,style,x,y,width,height,(parent)?(parent->object):0);
	_edit->res_id = res_id;
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_edit_create() | id:%s fd:0x%x res_id:%d\n",_edit->id,_edit->object,_edit->res_id);
	egx_widget_set_struct_(_edit->object,(egx_widget_t*)_edit);
	egx_widget_register((egx_widget_t*)_edit);
	return 0;	
}

int egx_edit_get_text(char *id,char *text,int length)
{
	egx_edit_t *edit = (egx_edit_t*)egx_widget_find(id);
	if(!edit){
		return -1;
	}
	egx_edit_get_text_(edit->object,text,length);
	return 0;
}

int egx_edit_set_text(char *id,char *text)
{
	egx_edit_t *edit = (egx_edit_t*)egx_widget_find(id);
	if(!edit){
		return -1;
	}
	egx_edit_set_text_(edit->object,text);
	return 0;	
}

#ifdef __cplusplus
}
#endif
