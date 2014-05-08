
#include "easygx.h"
#include "easygx_frame.h"
#include "easygx_portable.h"

#ifdef __cplusplus
extern "C" {
#endif

int egx_paint_begin(egx_frame_t *frame)
{
	if(!frame){
		return 0;
	}
	frame->painter = egx_paint_begin_(frame->object);
	return 0;
}

int egx_paint_end(egx_frame_t *frame)
{
	int ret;
	if(!frame){
		return 0;
	}
	ret = egx_paint_end_(frame->object,frame->painter);
	frame->painter = 0;
	return ret;
}

int egx_draw_line(egx_frame_t *frame,int x0,int y0,int x1,int y1)
{
	int inner_begin = 0;
	if(frame->painter == 0){
		egx_paint_begin(frame);
		inner_begin = 1;
	}
	egx_draw_line_(frame->painter,x0,y0,x1,y1);
	if(inner_begin){
		egx_paint_end(frame);
	}
	return 0;
}

int egx_draw_ellipse(egx_frame_t *frame,int x0,int y0,int width,int height)
{
	int inner_begin = 0;
	if(frame->painter == 0){
		egx_paint_begin(frame);
		inner_begin = 1;
	}
	egx_draw_ellipse_(frame->painter,x0,y0,width,height);
	if(inner_begin){
		egx_paint_end(frame);
	}
	return 0;	
}

#ifdef __cplusplus
}
#endif
