
#include "easygx_private.h"

#include "easygx.h"

#ifdef __cplusplus
extern "C" {
#endif

egx_painter_t egx_paint_begin_(egx_wnd_t hwnd)
{
	WM_HWIN hWnd = GUIWND_TO_HWND(hwnd);
	return 1;
}

int egx_paint_end_(egx_wnd_t hwnd,egx_painter_t painter)
{
	return 0;
}

int egx_draw_line_(egx_painter_t painter,int x0,int y0,int x1,int y1)
{
	GUI_SetColor(GUI_BLACK);
	GUI_DrawLine(x0,y0,x1,y1);
	return 0;
}

int egx_draw_ellipse_(egx_painter_t painter,int x0,int y0,int width,int height)
{
	int hr = width/2,vr = height/2;
	int x = x0 + hr,y = y0+ vr;
	GUI_SetColor(GUI_BLACK);
	if(width == height){
		GUI_DrawCircle(x,y,hr);
	}else{
		GUI_DrawEllipse(x,y,hr,vr);
	}
	return 0;
}

#ifdef __cplusplus
}
#endif
