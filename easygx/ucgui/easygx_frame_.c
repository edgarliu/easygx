
#include <stdlib.h>
#include <string.h>

#include "easygx_private.h"
#include "easygx.h"
#include "easygx_core.h"
#include "easygx_widget.h"
#include "easygx_frame.h"
#include "easygx_control.h"
#include "easygx_message.h"
#include "easygx_input.h"
#include "easygx_portable.h"
#include "easygx_log.h"

//#include "sfpr_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
  unsigned int nativeVirtualKey;
  int ucguiKey;
}easygx_ucgui_key_t;

static easygx_ucgui_key_t g_keymap[] = {
  {0x26,    GUI_KEY_UP},
  {0x28,    GUI_KEY_DOWN},
  {0x27,    GUI_KEY_RIGHT},
  {0x25,    GUI_KEY_LEFT},
  {0x9,     GUI_KEY_HOME},//Tab 键
  {0x18,    GUI_KEY_END},
  {0x10,    GUI_KEY_SHIFT},
  {0x11,    GUI_KEY_CONTROL},
  {0x8,     GUI_KEY_BACKSPACE},
  {0x2d,    GUI_KEY_INSERT},
  {0x2e,    GUI_KEY_DELETE},
  {0x70,    GUI_KEY_F1},
  {0x71,    GUI_KEY_F2},
  {0x72,    GUI_KEY_F3},
  {0x73,    GUI_KEY_F4},
  {0x74,    GUI_KEY_F5},
  {0x75,    GUI_KEY_F6},
  {0x76,    GUI_KEY_H1},
  {0x77,    GUI_KEY_H2},
  {0,0}
};

static int ucguikey_to_vkey(unsigned int ucguiKey)
{
	int i;
	for (i=0; g_keymap[i].ucguiKey; i++) {
		if (g_keymap[i].ucguiKey == ucguiKey) {
			return g_keymap[i].nativeVirtualKey;
		}
	}
	return ucguiKey;
}


int egx_message_reg_notification_(egx_frame_t *frame,egx_control_t *control,egx_notification_e event)
{
	return 0;
}

unsigned int egx_message_reg_timer_(egx_wnd_t hwnd,unsigned int timer_id,int interval)
{
	sfpr_log(easygx_log,SFPR_LOG_ERROR,"egx_message_reg_timer_() | ucgui library is not support timer");
	return 0;
}

int egx_frame_set_title_(egx_wnd_t hwnd,char *title)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	if(hWnd == 0){
		return -1;
	}
	FRAMEWIN_SetText(hWnd,title);
	return 0;	
}


static void cb_desktop(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_QUIT:{
			easygx_set_quit();
			break;
		}
	}
}

int egx_frame_init_()
{
	WM_SetCallback(WM_HBKWIN,cb_desktop);
	return 0;
}

static void cb_window_client(WM_MESSAGE *pMsg)
{
	WM_HWIN hClient = (FRAMEWIN_Handle)(pMsg->hWin);
	WM_HWIN hWin = WM_GetParent(pMsg->hWin);
	egx_window_t *window = (egx_window_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWin));
	switch (pMsg->MsgId) {
	case WM_PAINT:{
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		egx_message_exec_public_msg((egx_frame_t*)window,egx_message_paint,0,0);
		break;
	}
	default:
		WM_DefaultProc(pMsg);
	}
}

static WM_CALLBACK *cb_window_default = NULL;

static void cb_window_message(WM_MESSAGE *pMsg)
{
	easygx_t *easygx = easygx_instance_get();
	WM_HWIN hWin = (WM_HWIN)(pMsg->hWin);
	egx_window_t *window = (egx_window_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWin));
//	sfpr_log(easygx_log,SFPR_LOG_DEBUG,"cb_window_message() | Message:%d",pMsg->MsgId);
	switch (pMsg->MsgId) {
		case WM_KEY:{
			int key = ((const WM_KEY_INFO*)(pMsg->Data.p))->Key;
			int pressedCnt = ((const WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt;
			if(key == GUI_KEY_ESCAPE) {
				egx_window_hide(window->id);
			}else if((window->vs_type ==  egx_vs_type_list) && (key == EASYGX_KEY_PGDOWN)){
				egx_view_t *view = window?(window->current_view):NULL;
				egx_view_hide((view?(view->id):NULL),egx_view_action_next);
			}else if((window->vs_type ==  egx_vs_type_list) && (key == EASYGX_KEY_PGUP)){ 
				egx_view_t *view = window?(window->current_view):NULL;
				egx_view_hide((view?(view->id):NULL),egx_view_action_next);
			}else{
				key = ucguikey_to_vkey(key);
				//sfpr_log(easygx_log,SFPR_LOG_INFO,"egxwnd_wm_proc() | key:0x%02x",key);
				egx_message_exec_public_msg((egx_frame_t*)window,egx_message_keydown,key,0);
			}
			return;
		}
		case WM_TIMER:{
			egx_message_exec_public_msg((egx_frame_t*)window,egx_message_timer,0,0);
			break;
		}
		case WM_NOTIFY_PARENT:{
			int res_id = WM_GetId(pMsg->hWinSrc);
			int event = pMsg->Data.v;
			switch (event){
				case WM_NOTIFICATION_RELEASED:{
					egx_message_exec_notification((egx_frame_t*)window,egx_notification_pushbutton_click,res_id,NULL);
					break;
				}
				case WM_NOTIFICATION_VALUE_CHANGED:{
				
					break;
				}
			}
		}
	}
	if(cb_window_default){
		cb_window_default(pMsg);
	}
	return;
}

static WM_CALLBACK *cb_view_default = NULL;

static void cb_view_message(WM_MESSAGE *pMsg)
{
	WM_HWIN hWin = (WM_HWIN)(pMsg->hWin);
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWin));	
	switch (pMsg->MsgId) {
		case WM_KEY:{
			int key = ((const WM_KEY_INFO*)(pMsg->Data.p))->Key;
			int pressedCnt = ((const WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt;
			WM_HWIN hParent = WM_GetParent(hWin);
			egx_window_t *window = (egx_window_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hParent));
			if(key == GUI_KEY_ESCAPE) {
				if(window->vs_type == egx_vs_type_stack){
					egx_view_hide(frame->id,egx_view_action_pop);
				}else{
					egx_window_hide(window->id);
				}
			}else if((window->vs_type ==  egx_vs_type_list) && (key == EASYGX_KEY_PGDOWN)){
				egx_view_hide((frame?(frame->id):NULL),egx_view_action_next);
			}else if((window->vs_type ==  egx_vs_type_list) && (key == EASYGX_KEY_PGUP)){ 
				egx_view_hide((frame?(frame->id):NULL),egx_view_action_next);
			}else{
				key = ucguikey_to_vkey(key);
				//sfpr_log(easygx_log,SFPR_LOG_INFO,"egxwnd_wm_proc() | key:0x%02x",key);
				egx_message_exec_public_msg(frame,egx_message_keydown,key,0);
			}
			return;
		}
		case WM_TIMER:{
			egx_message_exec_public_msg(frame,egx_message_timer,0,0);
			break;
		}
		case WM_NOTIFY_PARENT:{
			int res_id = WM_GetId(pMsg->hWinSrc);
			int event = pMsg->Data.v;
			switch (event){
				case WM_NOTIFICATION_RELEASED:{
					egx_message_exec_notification(frame,egx_notification_pushbutton_click,res_id,NULL);
					break;
				}
				case WM_NOTIFICATION_VALUE_CHANGED:{
				
					break;
				}
			}
		}
	}	
	if(cb_view_default){
		cb_view_default(pMsg);
	}
	return;	
}

egx_wnd_t egx_window_create_(char *name,egx_uint32_t style,int x,int y,int width,int height)
{
	WM_HWIN hWnd,hClient;
	WM_CALLBACK *old_cb = NULL;
	hWnd = FRAMEWIN_Create(name,0,0, x, y,width,height);
	FRAMEWIN_AddCloseButton(hWnd,FRAMEWIN_BUTTON_RIGHT,0);
	FRAMEWIN_AddMinButton(hWnd,FRAMEWIN_BUTTON_RIGHT,0);
	old_cb = WM_SetCallback(hWnd,cb_window_message);
	if(cb_window_default == NULL){
		cb_window_default = old_cb;
	}
	hClient = WM_GetClientWindow(hWnd);
	WM_SetCallback(hClient,cb_window_client);
	return HWND_TO_GUIWND(hWnd);
}

int egx_window_show_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_ShowWindow(hWnd);	
	WM_BringToTop(hWnd);
	WM_SetFocus(hWnd);
	return 0;
}

int egx_window_hide_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_HideWindow(hWnd);
	return 0;
}

int egx_window_destroy_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_DeleteWindow(hWnd);
	return 0;
}

egx_wnd_t egx_view_create_(egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	WM_CALLBACK *old_cb = NULL;
	WM_HWIN hParent = GUIWND_TO_HWND(parent);
	WM_HWIN hScrollArea = SCROLLAREA_CreateEx(x,y,width,height,0,0,hParent,WM_CF_HIDE,0);
	old_cb = WM_SetCallback(hScrollArea,cb_view_message);
	if(cb_view_default == NULL){
		cb_view_default = old_cb;
	}
	return HWND_TO_GUIWND(hScrollArea);
}

int egx_view_show_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_ShowWindow(hWnd);
	return 0;
}

int egx_view_hide_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_HideWindow(hWnd);
	return 0;	
}


#ifdef __cplusplus
}
#endif
