
#include <stdlib.h>
#include <string.h>

#include "easygx.h"
#include "easygx_core.h"
#include "easygx_widget.h"
#include "easygx_frame.h"
#include "easygx_input.h"
#include "easygx_portable.h"
#include "easygx_log.h"

#include "easygx_private.h"

#include "Windowsx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EASYGX_WINDOW_CLASS "easygx_window"
#define EASYGX_VIEW_CLASS "easygx_view"


int egx_message_reg_notification_(egx_frame_t *frame,egx_control_t *control,egx_notification_e event)
{
	return 0;
}

static VOID CALLBACK egx_timer_cb(HWND hWnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWnd));
	egx_message_exec_timer(frame,idEvent);
}

unsigned int egx_message_reg_timer_(egx_wnd_t hwnd,unsigned int timer_id,int interval)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	return SetTimer(hWnd,timer_id,interval,egx_timer_cb);
}

int egx_frame_set_title_(egx_wnd_t hwnd,char *title)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	if(hWnd == 0){
		return -1;
	}
	SetWindowText(hWnd,title);
	return 0;	
}


static LRESULT cb_window_syscommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	wmId    = LOWORD(wParam); 
	wmEvent = HIWORD(wParam); 

	wmId &= 0xfff0;//低4位是保留的
	switch (wmId){//使用了WS_EX_TOOLWINDOW之后就没有最小化和最大化按钮了
		case SC_MINIMIZE:{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		case SC_MAXIMIZE:
			return -1;
			return DefWindowProc(hWnd, message, wParam, lParam);
		case SC_RESTORE:
			return DefWindowProc(hWnd, message, wParam, lParam);
		case SC_CLOSE:{
			egx_window_t *window = (egx_window_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWnd));
			egx_window_close(window->id);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		case SC_MOVE://注意：SC_MOVE会拦截WM_LBUTTONUP消息
			return DefWindowProc(hWnd, message, wParam, lParam);
		case SC_SIZE://屏蔽了此消息，就不能改变窗口大小了
			return -1;
		case SC_MOUSEMENU:
		case SC_HOTKEY:
		case SC_VSCROLL:
		case SC_HSCROLL:
		case SC_DEFAULT:
		case SC_KEYMENU:
		case SC_NEXTWINDOW:
		case SC_PREVWINDOW:
		case SC_CONTEXTHELP:
		case SC_MONITORPOWER:
		case SC_TASKLIST:
		case SC_SCREENSAVE:
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}	
}

static LRESULT CALLBACK cb_window_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWnd));
	switch (message) {
	case WM_PAINT:{
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		egx_message_exec_public_msg(frame,egx_message_paint,0,0);
		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd,hdc);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
    case WM_KEYDOWN:{
		DWORD key = (DWORD)wParam;
		egx_window_t *window = (egx_window_t*)frame;
		//sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"cb_window_message() | window:%s\n",frame->id);		
		if((window->vs_type == egx_vs_type_list) && (key == EASYGX_KEY_PGDOWN)){
			egx_view_t *view = window?(window->current_view):NULL;
			egx_view_hide((view?(view->id):NULL),egx_view_action_next);
		}else if((window->vs_type == egx_vs_type_list) && (key == EASYGX_KEY_PGUP)){
			egx_view_t *view = window?(window->current_view):NULL;
			egx_view_hide((view?(view->id):NULL),egx_view_action_next);
		}else{
			egx_message_exec_public_msg(frame,egx_message_keydown,key,0);
		}
		break;
	}
    case WM_KEYUP:{
		DWORD key = (DWORD)wParam;
		egx_message_exec_public_msg(frame,egx_message_keyup,key,0);
		break;
	}
	case WM_LBUTTONDOWN:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);		
		egx_message_exec_public_msg(frame,egx_message_lbuttondown,pos,0);
		break;
	}
	case WM_LBUTTONUP:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);
		egx_message_exec_public_msg(frame,egx_message_lbuttonup,pos,0);		
		break;
	}
	case WM_LBUTTONDBLCLK:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);		
		egx_message_exec_public_msg(frame,egx_message_lbuttondbclk,pos,0);
		break;
	}
	case WM_RBUTTONDOWN:{
		//右键除了隐藏当前窗口外，什么都不干
		break;
	}
	case WM_RBUTTONUP:{
		//右键表示隐藏当前窗口
		egx_window_t *window = (egx_window_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWnd));
		egx_window_hide(window->id);
		return 0;
	}
	case WM_MBUTTONDOWN:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);		
		egx_message_exec_public_msg(frame,egx_message_mbuttondown,pos,0);		
		break;
	}
	case WM_MBUTTONUP:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);		
		egx_message_exec_public_msg(frame,egx_message_mbuttonup,pos,0);
		break;
	}
	case WM_MOUSEWHEEL:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_uint32_t fwKeys = LOWORD(wParam);
		egx_uint32_t zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		egx_message_exec_public_msg(frame,egx_message_mousewheel,pos,zDelta);		
		break;
	}
	case WM_COMMAND:{
		egx_window_t *window = (egx_window_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWnd));
		int res_id = LOWORD(wParam); 
		int event = HIWORD(wParam); 
		egx_message_exec_notification((egx_frame_t*)window,egx_notification_unknown,res_id,NULL);
		break;
	}
	case WM_SYSCOMMAND:
		return cb_window_syscommand(hWnd, message, wParam, lParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}	
}

static LRESULT CALLBACK cb_view_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(hWnd));
	switch (message) {
	case WM_PAINT:{
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		egx_message_exec_public_msg(frame,egx_message_paint,0,0);
		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd,hdc);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
    case WM_KEYDOWN:{
		DWORD key = (DWORD)wParam;
		HWND hParent = GetParent(hWnd);
		egx_window_t *window = (egx_window_t*)egx_widget_get_struct_((HWND_TO_GUIWND(hParent)));
		//sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"cb_view_message() | view:%s\n",frame->id);		
		if((window->vs_type == egx_vs_type_list) && (key == EASYGX_KEY_PGDOWN)){
			egx_view_hide((frame?(frame->id):NULL),egx_view_action_next);
		}else if((window->vs_type == egx_vs_type_list) && (key == EASYGX_KEY_PGUP)){
			egx_view_hide((frame?(frame->id):NULL),egx_view_action_next);
		}else{
			egx_message_exec_public_msg(frame,egx_message_keydown,key,0);
		}
		break;
	}
    case WM_KEYUP:{
		DWORD key = (DWORD)wParam;
		egx_message_exec_public_msg(frame,egx_message_keyup,key,0);
		break;
	}
	case WM_LBUTTONDOWN:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);		
		egx_message_exec_public_msg(frame,egx_message_lbuttondown,pos,0);
		break;
	}
	case WM_LBUTTONUP:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);		
		egx_message_exec_public_msg(frame,egx_message_lbuttonup,pos,0);		
		break;
	}
	case WM_LBUTTONDBLCLK:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);			
		egx_message_exec_public_msg(frame,egx_message_lbuttondbclk,pos,0);
		break;
	}
	case WM_RBUTTONUP:{
		//右键表示隐藏当前窗口
		HWND hParent = GetParent(hWnd);
		egx_window_t *window = (egx_window_t*)egx_widget_get_struct_((HWND_TO_GUIWND(hParent)));
		//sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"cb_view_message() | window:%s 0x%x\n",window->id,window->flag);
		if(window->vs_type == egx_vs_type_stack){
			egx_view_hide(frame->id,egx_view_action_pop);
		}else{
			egx_window_hide(window->id);
		}
		return 0;
	}
	case WM_MBUTTONDOWN:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);			
		egx_message_exec_public_msg(frame,egx_message_mbuttondown,pos,0);		
		break;
	}
	case WM_MBUTTONUP:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_widget_set_focus_(frame->object);			
		egx_message_exec_public_msg(frame,egx_message_mbuttonup,pos,0);
		break;
	}
	case WM_MOUSEWHEEL:{
		egx_uint32_t x = GET_X_LPARAM(lParam); 
		egx_uint32_t y = GET_Y_LPARAM(lParam); 
		egx_uint32_t pos = x + (y<<16);
		egx_uint32_t fwKeys = LOWORD(wParam);
		egx_uint32_t zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		egx_message_exec_public_msg(frame,egx_message_mousewheel,pos,zDelta);		
		break;
	}
	case WM_COMMAND:{
		int res_id = LOWORD(wParam); 
		int event = HIWORD(wParam); 
		egx_message_exec_notification((egx_frame_t*)frame,egx_notification_unknown,res_id,NULL);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

int egx_frame_init_()
{
	WNDCLASSEX wcex;
	wcex.cbSize 		= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)cb_window_message;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= NULL;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= EASYGX_WINDOW_CLASS;
	wcex.hIconSm		= NULL;
	if(0 == RegisterClassEx(&wcex)){
		sfpr_log(easygx_log,SFPR_LOG_ERROR,(char*)"egx_frame_init_() | RegisterClassEx %s failed\n",EASYGX_WINDOW_CLASS);
		return -1;
	}

	wcex.cbSize 		= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)cb_view_message;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= NULL;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= EASYGX_VIEW_CLASS;
	wcex.hIconSm		= NULL;
	if(0 == RegisterClassEx(&wcex)){
		sfpr_log(easygx_log,SFPR_LOG_ERROR,(char*)"egx_frame_init_() | RegisterClassEx %s failed\n",EASYGX_VIEW_CLASS);
		return -1;
	}

	return 0;
}

egx_wnd_t egx_window_create_(char *name,egx_uint32_t style,int x,int y,int width,int height)
{
	HWND hWnd;
	hWnd = CreateWindowEx(0,EASYGX_WINDOW_CLASS,name,WS_OVERLAPPEDWINDOW,//|WS_HSCROLL|WS_VSCROLL,
				CW_USEDEFAULT,CW_USEDEFAULT,width,height,
				NULL, NULL, NULL, NULL);
	//sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_window_create_() | 0x%x ---> 0x%x\n",hWnd,HWND_TO_GUIWND(hWnd));				
	return HWND_TO_GUIWND(hWnd);
}

int egx_window_show_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	ShowWindow(hWnd, SW_SHOW);
	return 0;
}

int egx_window_hide_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	ShowWindow(hWnd, SW_HIDE);
	return 0;	
}

int egx_window_close_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	CloseWindow(hWnd);
	return 0;	
}

int egx_window_destroy_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	DestroyWindow(hWnd);
	return 0;	
}



egx_wnd_t egx_view_create_(egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{/*
	hWnd = CreateWindowEx(0L,"SCROLLBAR",NULL,WS_CHILD|WS_VISIBLE|SBS_HORZ,//|SBS_VERT,
				x,y,CW_USEDEFAULT,CW_USEDEFAULT,
				(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)NULL, 0, 0);
	*/
	HWND hWnd;
	hWnd = CreateWindowEx(0L,EASYGX_VIEW_CLASS,NULL,WS_CHILD|WS_VISIBLE,
				x,y,width,height,
				(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)NULL, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

int egx_view_show_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	ShowWindow(hWnd, SW_SHOW);
	return 0;
}

int egx_view_hide_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	ShowWindow(hWnd, SW_HIDE);
	return 0;	
}


#ifdef __cplusplus
}
#endif
