
#include <stdlib.h>
#include <string.h>

#include <QObject>
#include <QVariant>

#include "sfpr_hash.h"

#include "easygx_private.h"
#include "easygx_class.h"

#include "easygx.h"
#include "easygx_widget.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

QT_BEGIN_NAMESPACE
int egx_widget_set_struct_(egx_wnd_t hwnd,egx_widget_t *widget)
{
	QObject *object = (QObject*)hwnd;
	if(!object){
		return -1;
	}
	object->setProperty("BelongsWidget",(unsigned int)widget);
	return 0;
}

egx_widget_t *egx_widget_get_struct_(egx_wnd_t hwnd)
{
	egx_widget_t *widget = NULL;
	if(0 == hwnd){
		return NULL;
	}
	QObject *object = (QObject*)hwnd;
	QVariant variant = object->property("BelongsWidget");
	widget = (egx_widget_t*)variant.toUInt();
	return widget;
}

int egx_widget_set_parent_(egx_wnd_t hwnd,egx_wnd_t hparent)
{
	if(0 == hwnd || 0 == hparent){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	QWidget *parent = (QWidget*)hparent;	
	widget->setParent(parent);
	return 0;
}

egx_wnd_t egx_widget_get_parent_(egx_wnd_t hwnd)
{
	if(0 == hwnd){
		return 0;
	}
	QWidget *widget = (QWidget*)hwnd;	
	QWidget *parent = NULL;	
	parent = widget->parentWidget();
	return HWND_TO_GUIWND(parent);
}

int egx_widget_update_(egx_wnd_t hwnd,int erase)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;
	widget->repaint();
	//widget->update();
	return 0;
}

int egx_widget_update_rect_(egx_wnd_t hwnd, int x, int y, int width, int height,int erase)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->update(x,y,width,height);	
	return 0;
}

int egx_widget_show_(egx_wnd_t hwnd)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->show();
	return 0;
}

int egx_widget_hide_(egx_wnd_t hwnd)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->hide();	
	return 0;
}

int egx_widget_enabled_(egx_wnd_t hwnd)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->setEnabled(true);		
	return 0;
}

int egx_widget_disabled_(egx_wnd_t hwnd)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->setEnabled(false);	
	return 0;
}

int egx_widget_is_enabled_(egx_wnd_t hwnd)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	if(widget->isEnabled() == true){
		return 1;
	}
	return 0;
}

int egx_widget_set_focus_(egx_wnd_t hwnd)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->setFocus();
	return 0;
}

int egx_widget_get_rect_(egx_wnd_t hwnd,egx_rect_t *rect)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	QRect wrect = widget->geometry();
	rect->x = wrect.left();
	rect->y = wrect.top();
	rect->width = wrect.width();
	rect->height = wrect.height();
	return 0;
}

int egx_widget_get_client_rect_(egx_wnd_t hwnd,egx_rect_t *rect)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	QRect wrect = widget->geometry();
	rect->x = wrect.left();
	rect->y = wrect.top();
	rect->width = wrect.width();
	rect->height = wrect.height();
	return 0;
}

int egx_widget_set_rect_(egx_wnd_t hwnd,int x,int y,int width,int height)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;
	widget->setGeometry(x,y,width,height);
	//widget->move(x,y);
	//widget->resize(width,height);
	return 0;
}

int egx_widget_set_pos_(egx_wnd_t hwnd,int x,int y)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->move(x,y);
	return 0;
}

int egx_widget_set_size_(egx_wnd_t hwnd,int width,int height)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->resize(width,height);	
	return 0;
}

QT_END_NAMESPACE
#ifdef __cplusplus
}
#endif
