/**
 *\file easygx_frame.h
 *\brief  easygx��װ�Ĵ��嶨�弰�ӿ�
 *\version 3.0.0
 *\date 2014.04.03
 */
#ifndef EASYGX_FRAME_H
#define EASYGX_FRAME_H

#include <string.h>

#include "easygx.h"
#include "easygx_widget.h"
#include "easygx_message.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	egx_frame_unknown = 0,
	egx_frame_window,
	egx_frame_dialog,
	egx_frame_view
}egx_frame_type_e;

#define EGX_FRAME  \
	EGX_WIDGET;\
	egx_frame_type_e frame_type;\
	egx_msgs_public_t public_msg;\
	egx_msgs_private_t  private_msg;\
	egx_msgs_notification_t  notification;\
	egx_msgs_timer_t      timer;\
	egx_painter_t painter;\
	egx_wnd_t curr_radiogroup


/**
 *\brief ���ڽṹ����
 */
struct egx_frame_s
{
	EGX_FRAME;
};


typedef enum{
	egx_window_type_general = 0,
	egx_window_type_welcomewnd,
	egx_window_type_mainwnd,
	egx_window_type_popupwnd,
	egx_window_type_mainmenu,
	egx_window_type_menu
}egx_window_type_e;

//��ͼ��ʾ��ʽ
typedef enum{
	egx_vs_type_list = 0,
	egx_vs_type_stack
}egx_vs_type_e;

#define EGX_WINDOW_MAX_STACK	16

/**
 *\brief ���ڽṹ����
 */
struct egx_window_s
{
	EGX_FRAME;
	egx_window_type_e window_type;
	egx_uint32_t have_view;
	egx_vs_type_e vs_type;//��ͼ����ʾ��ʽ
	//�������е���ͼ��һ�����ڵ����е���ͼ�γ�һ��˫������
	egx_view_t *head,*tail;
	//����ģʽ�£����浱ǰ��ͼ
	egx_view_t *current_view;
	//ջģʽ�£�������������ʾ��ͼ
	egx_view_t *stack_view[EGX_WINDOW_MAX_STACK];
	int stack_top;
};


typedef enum{
	egx_dialog_type_general
}egx_dialog_type_e;

/**
 *\brief �Ի���ṹ����
 */
struct egx_dialog_s
{
	EGX_FRAME;
	egx_dialog_type_e dialog_type;
};


typedef enum{
	egx_view_type_general
}egx_view_type_e;

typedef enum{
	egx_view_action_next = 1,
	egx_view_action_prep,
	egx_view_action_pop
}egx_view_action_e;

/**
 *\brief ��ͼ�ṹ����
 */
struct egx_view_s
{
	EGX_FRAME;
	egx_view_type_e view_type;
	egx_window_t *cur_window;
	egx_view_t *next;
	egx_view_t *prep;
};


int egx_frame_set_title(char *frame_id,char *title);

int egx_window_create(char *id,egx_window_type_e type,char *name,egx_uint32_t style,int x,int y,int width,int height);

int egx_window_show(char *id);

int egx_window_hide(char *id);

int egx_window_close(char *id);

int egx_window_destroy(char *id);

int egx_window_set_stack(char *id);


int egx_view_create(char *view_id,egx_view_type_e type,char *name,char *parent_id);

int egx_view_show(char *view_id);

int egx_view_hide(char *view_id,egx_view_action_e action);

int egx_view_destroy(char *view_id);


#ifdef __cplusplus
}
#endif

#endif

