
#include <string.h>

#include "easygx_private.h"
#include "easygx.h"
#include "easygx_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

int egx_widget_set_struct_(egx_wnd_t hwnd,egx_widget_t *widget)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	if(hWnd == 0){
		return -1;
	}	
	WM_SetUserDataEx(hWnd, (void*)widget);
	return 0;	
}

egx_widget_t *egx_widget_get_struct_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	return (egx_widget_t*)WM_GetUserDataEx(hWnd);
}



int egx_widget_set_parent_(egx_wnd_t hwnd,egx_wnd_t hparent)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_HWIN hParent = GUIWND_TO_HWND(hparent);
	WM_AttachWindow(hWnd,hParent);
	return 0;
}

egx_wnd_t egx_widget_get_parent_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_HWIN hParent = WM_GetParent(hWnd);
	return HWND_TO_GUIWND(hParent);
}

int egx_widget_update_(egx_wnd_t hwnd,int erase)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	if(erase){
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
	}
	WM_InvalidateWindow(hWnd);
	return 0;
}

int egx_widget_update_rect_(egx_wnd_t hwnd, int x, int y, int width, int height,int erase)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	GUI_RECT wrect;
	wrect.x0 = (short int)x;
	wrect.y0 = (short int)y;
	wrect.x1 = (short int)(x + width);
	wrect.y1 = (short int)(y + height);
	WM_InvalidateRect(hWnd,&wrect);
	return 0;
}

int egx_widget_show_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_ShowWindow(hWnd);
	return 0;
}

int egx_widget_hide_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_HideWindow(hWnd);
	return 0;
}

int egx_widget_enabled_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_EnableWindow(hWnd);
	return 0;
}

int egx_widget_disabled_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_DisableWindow(hWnd);
	return 0;
}

int egx_widget_is_enabled_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_IsEnabled(hWnd);
	return 0;
}

int egx_widget_set_focus_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_SetFocus(hWnd);
	return 0;
}

int egx_widget_set_pos_(egx_wnd_t hwnd,int x,int y)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_MoveTo(hWnd,x,y);
//	WM_MoveChildTo(hWnd,x,y);
	return 0;
}

int egx_widget_set_size_(egx_wnd_t hwnd,int width,int height)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_SetSize(hWnd,width,height);
	return 0;
}

int egx_widget_set_rect_(egx_wnd_t hwnd,int x,int y,int width,int height)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	WM_MoveTo(hWnd,x,y);
	WM_SetSize(hWnd,width,height);
 	return 0;
}

int egx_widget_get_rect_(egx_wnd_t hwnd,egx_rect_t *rect)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	rect->x = WM_GetWindowOrgX(hWnd);
	rect->y = WM_GetWindowOrgY(hWnd);
	rect->width = WM_GetWindowSizeX(hWnd);
	rect->height = WM_GetWindowSizeY(hWnd);
/*	GUI_RECT wrect;
	WM_GetWindowRectEx(hWnd,&wrect);
	rect->x = wrect.x0;
	rect->y = wrect.y0;
	rect->width = wrect.x1 - wrect.x0;
	rect->height = wrect.y1 - wrect.y0;
*/
	return 0;
}

int egx_widget_get_client_rect_(egx_wnd_t hwnd,egx_rect_t *rect)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	GUI_RECT wrect;
	WM_GetInsideRectEx(hWnd,&wrect);
	rect->x = wrect.x0;
	rect->y = wrect.y0;
	rect->width = wrect.x1 - wrect.x0;
	rect->height = wrect.y1 - wrect.y0;

	return 0;
}

#ifdef __cplusplus
}
#endif
