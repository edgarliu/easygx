
#include <QPainter>
#include "easygx_private.h"

#include "easygx.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

egx_painter_t egx_paint_begin_(egx_wnd_t hwnd)
{
	QPainter *_painter = new QPainter((QWidget*)hwnd);
	return HWND_TO_GUIWND(_painter);
}

int egx_paint_end_(egx_wnd_t hwnd,egx_painter_t painter)
{
	QPainter *_painter = (QPainter*)painter;
	delete _painter;
	return 0;
}

int egx_draw_line_(egx_painter_t painter,int x0,int y0,int x1,int y1)
{
	QPainter *_painter = (QPainter*)painter;
	_painter->drawLine(x0,y0,x1,y1);
	return 0;
}

int egx_draw_ellipse_(egx_painter_t painter,int x0,int y0,int width,int height)
{
	QPainter *_painter = (QPainter*)painter;
	_painter->drawEllipse(x0,y0,width,height);
	return 0;
}

#ifdef __cplusplus
}
#endif
