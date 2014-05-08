
#include <string.h>

#include "easygx.h"
#include "easygx_control.h"
#include "easygx_log.h"

#include "easygx_private.h"

#include "Windowsx.h"

#ifdef __cplusplus
extern "C" {
#endif

egx_wnd_t egx_label_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	//sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_label_create_() | parent: 0x%x ---> 0x%x\n",parent,GUIWND_TO_HWND(parent));
	hWnd = CreateWindow("STATIC",name,WS_CHILD|WS_VISIBLE,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

int egx_label_set_text_(egx_wnd_t hwnd,char *text)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	SetWindowText(hWnd,text);
}



int egx_button_enable_(egx_wnd_t hwnd,int enable)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	Button_Enable(hWnd,enable);
	return 0;
}

int egx_button_checked_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	int state = 0;
	state = Button_GetCheck(hWnd);
	return state;
}

egx_wnd_t egx_pushbutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

egx_wnd_t egx_checkbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|WS_BORDER|BS_AUTOCHECKBOX,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

egx_wnd_t egx_groupbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

egx_wnd_t egx_radiobutton_begin_(egx_wnd_t parent)
{
	return 0;
}

egx_wnd_t egx_radiobutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	if(style & EGX_STYLE_GROUP){
		hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_GROUP,
					x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	}else{
		hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
					x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);		
	}
	return HWND_TO_GUIWND(hWnd);	
}



egx_wnd_t egx_edit_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|WS_BORDER,
				x,y,width,height,
				(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, NULL);
	//GetClassName(hWnd,buff,31);
	return HWND_TO_GUIWND(hWnd);
}

int egx_edit_get_text_(egx_wnd_t hwnd,char *text,int length)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	*((LPWORD)text) = length;
	return SendMessage(hWnd,EM_GETLINE,0,(LPARAM)text);
}

int egx_edit_set_text_(egx_wnd_t hwnd,char *text)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	SetWindowText(hWnd,text);
}

#ifdef __cplusplus
}
#endif
