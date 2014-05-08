/**
 *\file easygx_message.h
 *\brief easygx封装的消息出来结构定义及相关接口
 *\version 3.0.0
 *\date 2012
 */

#ifndef EASYGX_MESSAGE_H
#define EASYGX_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "easygx.h"
#include "easygx_control.h"

#define EGX_MSG_MAX_NOTIFICATION     16
#define EGX_MSG_MAX_PRIVATE          8
#define EGX_MSG_MAX_TIMER            4

typedef enum{
	egx_message_paint = 0,
	egx_message_notification,
	egx_message_timer,
	egx_message_keydown,
	egx_message_keyup,
	egx_message_lbuttondown,
	egx_message_lbuttonup,
	egx_message_lbuttondbclk,
//	egx_message_rbuttondown, //easygx 不支持右键弹出菜单功能，如果用户按下右键，则隐藏当前窗口
//	egx_message_rbuttonup,
	egx_message_mbuttondown,
	egx_message_mbuttonup,
	egx_message_mousewheel,
	egx_message_number
}egx_msg_public_e;

typedef enum{
	egx_notification_unknown = 0,
	egx_notification_pushbutton_click,
	egx_notification_checkbox_check,
	egx_notification_radiobutton_check,
	egx_notification_
}egx_notification_e;

typedef int (*egx_msg_cb_t)(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata);

typedef struct{
	int msg_id;
	egx_msg_cb_t msg_cb;
	void *userdata;
}egx_msg_desc_t;

typedef union{
	struct{
		const char *jump_id;
		const char *data_src;
		const char *data_dest;
	}button;
	struct{
		char *default_data;
	}edit;
}egx_notification_attr_u;

typedef struct{
	egx_control_t *control;
	egx_notification_e event;
	egx_msg_cb_t msg_cb;
	int attr_valid;
	egx_notification_attr_u attr;
	void *userdata;
}egx_notification_desc_t;

typedef struct{
	unsigned int id;
	int interval;
	egx_msg_cb_t msg_cb;	
	void *userdata;	
}egx_timer_desc_t;

typedef struct{
	egx_msg_desc_t message[egx_message_number];
}egx_msgs_public_t;

typedef struct{
	egx_msg_desc_t message[EGX_MSG_MAX_PRIVATE];
}egx_msgs_private_t;

typedef struct{
	egx_notification_desc_t message[EGX_MSG_MAX_NOTIFICATION];
}egx_msgs_notification_t;

typedef struct{
	egx_timer_desc_t message[EGX_MSG_MAX_TIMER];
}egx_msgs_timer_t;


int egx_message_reg_public_msg(char *frame_id,egx_msg_public_e msg_id,egx_msg_cb_t msg_cb,void *userdata);

int egx_message_reg_private_msg(char *frame_id,int msg_id,egx_msg_cb_t msg_cb,void *userdata);

int egx_message_reg_notification(char *control_id,egx_notification_e event,egx_msg_cb_t msg_cb,egx_notification_attr_u *attr,void *userdata);

unsigned int egx_message_reg_timer(char *frame_id,unsigned int timer_id,int interval,egx_msg_cb_t msg_cb,void *userdata);

int egx_message_pushbutton_jump(char *button_id,const char *target_id,void *userdata);

int egx_message_pushbutton_trans(char *button_id,const char *data_src,const char *data_dest,void *userdata);


int egx_message_exec_public_msg(egx_frame_t *frame,egx_msg_public_e msg_id,egx_uint32_t param1,egx_uint32_t param2);

int egx_message_exec_private_msg(egx_frame_t *frame,int msg_id);

int egx_message_exec_notification(egx_frame_t *frame,egx_notification_e event,int res_id,char *id);

int egx_message_exec_timer(egx_frame_t *frame,int timer_id);

#ifdef __cplusplus
}
#endif

#endif
