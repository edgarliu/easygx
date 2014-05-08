
#include <string.h>

#include "easygx.h"
#include "easygx_core.h"
#include "easygx_widget.h"
#include "easygx_portable.h"
#include "easygx_utils.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

int egx_widget_set_parent(egx_widget_t *widget,egx_widget_t *parent)
{
	if(NULL == widget || NULL == parent){
		return -1;
	}
	return egx_widget_set_parent_(widget->object,parent->object);
}

egx_widget_t *egx_widget_get_parent(egx_widget_t *widget)
{
	egx_widget_t *wparent = NULL;
	egx_wnd_t hparent;
	if(NULL == widget){
		return NULL;
	}
	hparent = egx_widget_get_parent_(widget->object);
	if(0 == hparent){
		return NULL;
	}
	return egx_widget_get_struct_(hparent);
}

int egx_widget_update(egx_widget_t *widget,int erase)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_update_(widget->object,erase);
}

int egx_widget_update_rect(egx_widget_t *widget, int x, int y, int width, int height,int erase)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_update_rect_(widget->object,x,y,width,height,erase);
}

int egx_widget_show(egx_widget_t *widget)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_show_(widget->object);
}

int egx_widget_hide(egx_widget_t *widget)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_hide_(widget->object);
}

int egx_widget_enabled(egx_widget_t *widget)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_enabled_(widget->object);
}

int egx_widget_disabled(egx_widget_t *widget)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_disabled_(widget->object);
}

int egx_widget_is_enabled(egx_widget_t *widget)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_is_enabled_(widget->object);
}

int egx_widget_set_focus(egx_widget_t *widget)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_set_focus_(widget->object);
}

int egx_widget_set_pos(egx_widget_t *widget,int x,int y)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_set_pos_(widget->object,x,y);
}

int egx_widget_set_size(egx_widget_t *widget,int width,int height)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_set_size_(widget->object,width,height);
}

int egx_widget_set_rect(egx_widget_t *widget,int x,int y,int width,int height)
{
	if(NULL == widget){
		return -1;
	}
	return egx_widget_set_rect_(widget->object,x,y,width,height);
}

int egx_widget_get_rect(egx_widget_t *widget,egx_rect_t *rect)
{
	if(NULL == widget || NULL == rect){
		return -1;
	}
	return egx_widget_get_rect_(widget->object,rect);
}

int egx_widget_get_client_rect(egx_widget_t *widget,egx_rect_t *rect)
{
	if(NULL == widget || NULL == rect){
		return -1;
	}
	return egx_widget_get_client_rect_(widget->object,rect);
}

#ifdef __cplusplus
}
#endif
