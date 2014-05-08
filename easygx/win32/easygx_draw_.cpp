
#include "easygx.h"

#include "easygx_private.h"

#ifdef __cplusplus
extern "C" {
#endif

egx_painter_t egx_paint_begin_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	HDC hdc;
	hdc = GetWindowDC(hWnd);
//	hdc = GetDC(hWnd);
	SetBkMode(hdc,TRANSPARENT);
	return (egx_painter_t)hdc;
}

int egx_paint_end_(egx_wnd_t hwnd,egx_painter_t painter)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);	
	HDC hdc = (HDC)painter;	
	ReleaseDC(hWnd,hdc);
}

int egx_draw_line_(egx_painter_t painter,int x0,int y0,int x1,int y1)
{
	HDC hdc = (HDC)painter;	
	MoveToEx(hdc,x0,y0,NULL);
	LineTo(hdc,x1,y1);
	return 0;
}

int egx_draw_ellipse_(egx_painter_t painter,int x0,int y0,int width,int height)
{
	HDC hdc = (HDC)painter;	
	Ellipse(hdc,x0,y0,x0+width,y0+height);
	return 0;
}

#ifdef __cplusplus
}
#endif
