
#include <stdlib.h>
#include <string.h>

#include "easygx.h"
#include "easygx_core.h"
#include "easygx_frame.h"
#include "easygx_portable.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

int egx_message_reg_public_msg(char *frame_id,egx_msg_public_e msg_id,egx_msg_cb_t msg_cb,void *userdata)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_find(frame_id);
	egx_msg_desc_t *msg_desc = NULL;
	if(!frame){
		return -11;
	}
	if(msg_id < 0 || msg_id >= egx_message_number){
		return -12;
	}
	msg_desc = &(frame->public_msg.message[msg_id]);
	msg_desc->msg_id = msg_id;
	msg_desc->msg_cb = msg_cb;
	msg_desc->userdata = userdata;
	return 0;
}

int egx_message_reg_notification(char *control_id,egx_notification_e event,egx_msg_cb_t msg_cb,egx_notification_attr_u *attr,void *userdata)
{
	egx_control_t *control = (egx_control_t*)egx_widget_find(control_id);
	egx_frame_t *frame = NULL;
	egx_notification_desc_t *notification = NULL;
	int ii = 0;
	if(!control){
		return -11;
	}
	if(control->widget_type != egx_widget_control){
		return -12;
	}
	if(control->res_id == 0){
		return -13;
	}
	if(control->cur_frame == NULL){
		return -14;
	}
	if(event == egx_notification_unknown){
		return -15;
	}
	if(msg_cb == NULL){
		return -16;
	}
//	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_message_reg_notification() | res_id:%d  event:%d\n",control->res_id,event);	
	frame = control->cur_frame;
	notification = frame->notification.message;
	for(ii=0;ii<EGX_MSG_MAX_NOTIFICATION;ii++){
		if(notification[ii].event == egx_notification_unknown){
			notification[ii].control = control;
			notification[ii].event = event;
			notification[ii].msg_cb = msg_cb;
			notification[ii].userdata = userdata;
			if(attr){
				memcpy(&(notification[ii].attr),attr,sizeof(egx_notification_attr_u));
				notification[ii].attr_valid = 1;
			}else{
				memset(&(notification[ii].attr),0,sizeof(egx_notification_attr_u));
				notification[ii].attr_valid = 0;
			}
			break;
		}
	}
	if(ii == EGX_MSG_MAX_NOTIFICATION){
		return -16;
	}
	egx_message_reg_notification_(frame,control,event);	
	return 0;
}

unsigned int egx_message_reg_timer(char *frame_id,unsigned int timer_id,int interval,egx_msg_cb_t msg_cb,void *userdata)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_find(frame_id);
	egx_timer_desc_t *timer = NULL;
	int ii;
	if(!frame){
		return -1;
	}
	timer = frame->timer.message;
	for(ii=0;ii<EGX_MSG_MAX_TIMER;ii++){
		if(timer[ii].id == 0){
			timer[ii].id = timer_id;
			timer[ii].interval = interval;
			timer[ii].msg_cb = msg_cb;
			timer[ii].userdata = userdata;
			break;
		}
	}
	if(ii == EGX_MSG_MAX_TIMER){
		return -16;
	}
	timer[ii].id = egx_message_reg_timer_(frame->object,timer_id,interval);
	return timer[ii].id;
}



int egx_message_exec_public_msg(egx_frame_t *frame,egx_msg_public_e msg_id,egx_uint32_t param1,egx_uint32_t param2)
{
	egx_msg_desc_t *msg_desc = NULL;
	if(!frame){
		return -11;
	}
	if(msg_id < 0 || msg_id >= egx_message_number){
		return -12;
	}
	msg_desc = &(frame->public_msg.message[msg_id]);
	if(msg_desc->msg_cb){
		msg_desc->msg_cb((egx_widget_t*)frame,msg_id,param1,param2,msg_desc->userdata);
	}
	return 0;
}


int egx_message_exec_notification(egx_frame_t *frame,egx_notification_e event,int res_id,char *id)
{
	int ii,ret = -1;
	egx_notification_desc_t *notification = frame->notification.message;
	egx_notification_attr_u attr;
	for(ii=0;ii<EGX_MSG_MAX_NOTIFICATION;ii++){
		egx_control_t *control = notification[ii].control;
		if(!control){
			continue;
		}
		memset(&attr,0,sizeof(egx_notification_attr_u));
		memcpy(&attr,&(notification[ii].attr),sizeof(egx_notification_attr_u));
		//res_id 和 id 只要有一项满足，就执行
		if(((res_id >0) &&  (res_id == control->res_id)) || ((id != NULL) && (0 == strcmp(id,control->id)))){
//			sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_message_exec_notification() | control: %d %s  current:%d %s\n",
//				control->res_id,control->id,res_id,id);
			if((event == notification[ii].event) || (event == egx_notification_unknown)){
				notification[ii].msg_cb((egx_widget_t*)control,control->control_type,
					((notification[ii].attr_valid)?(egx_uint32_t)&attr:0),0,notification[ii].userdata);
				ret = 0;
			}
			break;
		}
	}
	return ret;
}


int egx_message_exec_timer(egx_frame_t *frame,int timer_id)
{
	int ii,ret = -1;
	egx_timer_desc_t *timer = NULL;
	if(!frame || timer_id <= 0){
		return -11;
	}
	timer = frame->timer.message;
	for(ii=0;ii<EGX_MSG_MAX_TIMER;ii++){
		if(timer[ii].id == timer_id){
			timer[ii].msg_cb((egx_widget_t*)frame,egx_message_timer,timer_id,0,timer[ii].userdata);
			ret = 0;
			break;
		}
	}
	return ret;
}

static int egx_button_jump_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	egx_frame_t *frame = NULL;
	egx_notification_attr_u *attr = (egx_notification_attr_u*)param1;
	if(!attr){
		return -1;
	}
	frame = (egx_frame_t*)egx_widget_find(attr->button.jump_id);
	if(!frame){
		return -2;
	}
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_button_jump() |  pushbutton:%s ---> show:%s\n",
			widget->id,frame->id);
	if(frame->widget_type == egx_widget_frame && frame->frame_type == egx_frame_window){
		egx_window_show(frame->id);
	}
	if(frame->widget_type == egx_widget_frame && frame->frame_type == egx_frame_view){
		egx_view_show(frame->id);
	}
	return 0;
}

int egx_message_pushbutton_jump(char *button_id,const char *target_id,void *userdata)
{
	egx_notification_attr_u attr;
	if(!button_id || !target_id){
		return -1;
	}
	memset(&attr,0,sizeof(egx_notification_attr_u));
	attr.button.jump_id = target_id;
	return egx_message_reg_notification(button_id,egx_notification_pushbutton_click,egx_button_jump_cb,&attr,userdata);
}


static int egx_button_trans_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	char buffer[512];
	egx_control_t *control_src = NULL,*control_dest = NULL;
	egx_notification_attr_u *attr = (egx_notification_attr_u*)param1;
	if(!attr){
		return -1;
	}
	control_src = (egx_control_t*)egx_widget_find(attr->button.data_src);
	control_dest = (egx_control_t*)egx_widget_find(attr->button.data_dest);
	if(!control_src || !control_dest){
		return -2;
	}
	if(control_src->widget_type != egx_widget_control || control_dest->widget_type != egx_widget_control){
		return -3;
	}
	memset(buffer,0,sizeof(buffer));
	if(control_src->control_type == egx_control_type_edit){
		egx_edit_get_text_(control_src->object,buffer,sizeof(buffer));
	}
	
	if(control_dest->control_type == egx_control_type_label){
		egx_label_set_text_(control_dest->object,buffer);
	}
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_button_trans() |  data src:%s  <%s>---> data dest:%s\n",
			control_src->id,buffer,control_dest->id);
	return 0;
}

int egx_message_pushbutton_trans(char *button_id,const char *data_src,const char *data_dest,void *userdata)
{
	egx_notification_attr_u attr;
	if(!button_id || !data_src || !data_dest){
		return -1;
	}
	memset(&attr,0,sizeof(egx_notification_attr_u));
	attr.button.data_src = data_src;
	attr.button.data_dest = data_dest;
	return egx_message_reg_notification(button_id,egx_notification_pushbutton_click,egx_button_trans_cb,&attr,userdata);
}



static char *keycode_desc[256] = {
	"","","","","","","","",//0--7
	"Backspace","TAB","LF","","","ENTER","","",//8--f
	"Shift","Ctrl","Alt","Pause","CapsLock","","","",//0x10--0x17
	"","","","ESCAPE","","","","",//0x18--0x1f
	"Space","PageUp","PageDown","End","Home","Left","Up","Right",//0x20--0x27
	"Down","","","","PrintScreen","Insert","Delete","",//0x28---0x2f
	"0","1","2","3","4","5","6","7",//0x30--0x37
	"8","9","","","","","","",//0x38---0x3f
	"","A","B","C","D","E","F","G",//0x40--0x47
	"H","I","J","K","L","M","N","O",//0x48---0x4f
	"P","Q","R","S","T","U","V","W",//0x50--0x57
	"X","Y","Z","LWIN","RWIN","APPS","","",//0x58---0x5f
	"","","","","","","","",//0x60--0x67
	"","","","","","","","",//0x68---0x6f
	"F1","F2","F3","F4","F5","F6","F7","F8",//0x70--0x77
	"F9","F10","F11","F12","","","","",//0x78---0x7f
	"","","","","","","","",//0x80--0x87
	"","","","","","","","",//0x88---0x8f
	"NumLock","Scroll Lock","","","","","","",//0x90--0x97
	"","","","","","","","",//0x98---0x9f
	"","","","","","","","",//0xa0--0xa7
	"","","","","","","","",//0xa8---0xaf
	"","","","","","","","",//0xb0--0xb7
	"","",";","=",",","-",".","/",//0xb8---0xbf
	"`","","","","","","","",//0xc0--0xc7
	"","","","","","","","",//0xc8---0xcf
	"","","","","","","","",//0xd0--0xd7
	"","","","[","\\","]","'","",//0xd8---0xdf
	"","","","","","","","",//0xe0--0xe7
	"","","","","","","","",//0xe8---0xef
	"","","","","","","","",//0xf0--0xf7
	"","","","","","","","Fn"//0xf8---0xff
};

const char *egx_keycode_desc(unsigned char keycode)
{
	return keycode_desc[keycode];
}

#ifdef __cplusplus
}
#endif
