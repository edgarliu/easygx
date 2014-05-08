
#include <stdlib.h>
#include <string.h>

#include "easygx.h"
#include "easygx_widget.h"

#include "easygx_private.h"

#ifdef __cplusplus
extern "C" {
#endif

int egx_widget_set_struct_(egx_wnd_t hwnd,egx_widget_t *widget)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	if(hWnd == 0){
		return -1;
	}
	SetWindowLong(hWnd,GWL_USERDATA,(LONG)widget);
	return 0;	
}

egx_widget_t *egx_widget_get_struct_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	return (egx_widget_t*)GetWindowLong(hWnd,GWL_USERDATA);
}


int egx_widget_set_parent_(egx_wnd_t hwnd,egx_wnd_t hparent)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	HWND hParent = GUIWND_TO_HWND(hparent);
	SetParent(hWnd,hParent);
	return 0;
}

egx_wnd_t egx_widget_get_parent_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	HWND hParent;
	hParent = GetParent(hWnd);
	return HWND_TO_GUIWND(hParent);
}

int egx_widget_update_(egx_wnd_t hwnd,int erase)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	InvalidateRect(hWnd,NULL,erase?TRUE:FALSE);	
	return 0;
}

int egx_widget_update_rect_(egx_wnd_t hwnd, int x, int y, int width, int height,int erase)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	RECT wrect;
	wrect.left = x;
	wrect.top = y;
	wrect.right = x + width;
	wrect.bottom = y + height;
	InvalidateRect(hWnd,&wrect,erase?TRUE:FALSE);
	return 0;
}

int egx_widget_show_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	ShowWindow(hWnd, SW_SHOW);	
	return 0;
}

int egx_widget_hide_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	ShowWindow(hWnd, SW_HIDE);	
	return 0;
}

int egx_widget_enabled_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	EnableWindow(hWnd,TRUE);
	return 0;
}

int egx_widget_disabled_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	EnableWindow(hWnd,FALSE);	
	return 0;
}

int egx_widget_is_enabled_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	return IsWindowEnabled(hWnd);
}

int egx_widget_set_focus_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	SetFocus(hWnd);
	return 0;
}

int egx_widget_set_pos_(egx_wnd_t hwnd,int x,int y)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	RECT wrect;
	if(0 == GetWindowRect(hWnd,&wrect)){
		return -2;
	}
	MoveWindow(hWnd,x,y,wrect.right - wrect.left,wrect.bottom - wrect.top,TRUE);
	return 0;
}

int egx_widget_set_size_(egx_wnd_t hwnd,int width,int height)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	RECT wrect;
	if(0 == GetWindowRect(hWnd,&wrect)){
		return -2;
	}
	MoveWindow(hWnd,wrect.left,wrect.top,width,height,TRUE);	
	return 0;
}

int egx_widget_set_rect_(egx_wnd_t hwnd,int x,int y,int width,int height)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	MoveWindow(hWnd,x,y,width,height,TRUE);	
	return 0;
}

int egx_widget_get_rect_(egx_wnd_t hwnd,egx_rect_t *rect)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	RECT wrect;
	if(!rect){
		return -1;
	}
	if(0 == GetWindowRect(hWnd,&wrect)){
		return -2;
	}
	rect->x = wrect.left;
	rect->y = wrect.top;
	rect->width = wrect.right - wrect.left;
	rect->height = wrect.bottom - wrect.top;
	return 0;
}

int egx_widget_get_client_rect_(egx_wnd_t hwnd,egx_rect_t *rect)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	RECT wrect;
	if(!rect){
		return -1;
	}
	if(0 == GetClientRect(hWnd,&wrect)){
		return -2;
	}
	rect->x = wrect.left;
	rect->y = wrect.top;
	rect->width = wrect.right - wrect.left;
	rect->height = wrect.bottom - wrect.top;
	return 0;
}

#ifdef __cplusplus
}
#endif
