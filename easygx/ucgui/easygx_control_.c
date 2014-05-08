
#include <string.h>

#include "easygx_private.h"
#include "easygx.h"
#include "easygx_portable.h"
#include "easygx_control.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

egx_wnd_t egx_label_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	TEXT_Handle hText;
	if(parent){
		hText = TEXT_CreateAsChild(x, y, width, height,GUIWND_TO_HWND(parent),res_id,WM_CF_SHOW, name,GUI_TA_LEFT);
	}else{
		hText = TEXT_Create(x, y, width, height,res_id,WM_CF_HIDE,name,GUI_TA_LEFT);
	}
	TEXT_SetTextAlign(hText,GUI_TA_LEFT);
	return HWND_TO_GUIWND(hText);
}

int egx_label_set_text_(egx_wnd_t hwnd,char *text)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);	
	TEXT_SetText(hWnd, text);
	return 0;
}

egx_wnd_t egx_pushbutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	BUTTON_Handle hButton;
	if(parent){
		hButton = BUTTON_CreateAsChild(x, y, width, height,GUIWND_TO_HWND(parent),res_id,WM_CF_SHOW);
	}else{
		hButton = BUTTON_Create(x, y, width, height,res_id,WM_CF_SHOW);
	}
	BUTTON_SetText(hButton, name);
	return HWND_TO_GUIWND(hButton);
}

egx_wnd_t egx_edit_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	EDIT_Handle hEdit;
	if(parent){
		hEdit = EDIT_CreateAsChild(x, y, width,height,GUIWND_TO_HWND(parent), res_id,WM_CF_SHOW,256);
	}else{
		hEdit = EDIT_Create(x, y, width,height, res_id, 256, WM_CF_SHOW );		
	}
	EDIT_SetInsertMode(hEdit, 0);
	return HWND_TO_GUIWND(hEdit);
}

int egx_edit_get_text_(egx_wnd_t hwnd,char *text,int length)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);	
	EDIT_GetText(hWnd,text,length);
	return 0;
}

int egx_edit_set_text_(egx_wnd_t hwnd,char *text)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);	
	EDIT_SetText(hWnd, text);
	return 0;
}


#ifdef __cplusplus
}
#endif
