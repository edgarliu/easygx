
#include <stdlib.h>
#include <string.h>

#include <QObject>
#include <QtCore>
#include <QMouseEvent>
#include <QPushButton>

#include "easygx_private.h"
#include "easygx.h"
#include "easygx_core.h"
#include "easygx_widget.h"
#include "easygx_frame.h"
#include "easygx_message.h"
#include "easygx_input.h"
#include "easygx_portable.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif
QT_BEGIN_NAMESPACE

int egx_message_reg_notification_(egx_frame_t *frame,egx_control_t *control,egx_notification_e event)
{
	CEgxWindow *gwindow = (CEgxWindow*)(frame->object);
	if(event == egx_notification_pushbutton_click ||
		event == egx_notification_checkbox_check ||
		event == egx_notification_radiobutton_check){
		QAbstractButton *button = (QAbstractButton*)(control->object);
		QObject::connect(button, SIGNAL(clicked(bool)),gwindow, SLOT(on_button_event(bool)));
	}
	return 0;
}

unsigned int egx_message_reg_timer_(egx_wnd_t hwnd,unsigned int timer_id,int interval)
{
	CEgxWindow *gwindow = (CEgxWindow*)(hwnd);
	return gwindow->startTimer(interval);
}

int egx_frame_set_title_(egx_wnd_t hwnd,char *title)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;	
	widget->setWindowTitle(QString::fromLocal8Bit(title));
	return 0;
}

egx_wnd_t egx_window_create_(char *name,egx_uint32_t style,int x,int y,int width,int height)
{
	CEgxWindow *gwindow = new CEgxWindow(); 
	if(!gwindow){
		return 0;
	}
	gwindow->setWindowTitle(QString::fromLocal8Bit(name));
	if(x == 0 || y == 0){
		gwindow->resize(width,height);
	}else{
		gwindow->setGeometry(x,y,width,height);
	}
	gwindow->setObjectName("egxwindow");
	//printf("objectName:%s\n",gwindow->objectName().toLocal8Bit().data());	
	return HWND_TO_GUIWND(gwindow);
}

int egx_window_show_(egx_wnd_t hwnd)
{
	CEgxWindow *gwindow = (CEgxWindow*)(hwnd);
	if(0 == hwnd){
		return -1;
	}
	gwindow->show();
	return 0;	
}

int egx_window_hide_(egx_wnd_t hwnd)
{
	CEgxWindow *gwindow = (CEgxWindow*)(hwnd);
	if(0 == hwnd){
		return -1;
	}
	gwindow->hide();
	return 0;
}

int egx_window_destroy_(egx_wnd_t hwnd)
{
	CEgxWindow *gwindow = (CEgxWindow*)(hwnd);
	if(0 == hwnd){
		return -1;
	}
	delete gwindow;
	return 0;
}

egx_wnd_t egx_dialog_create_(char *name,egx_uint32_t style,int x,int y,int width,int height)
{
	CEgxDialog *dialog = new CEgxDialog(); 
	if(!dialog){
		return 0;
	}
	dialog->setModal(true);
	dialog->setWindowTitle(QString::fromLocal8Bit(name));
	if(x == 0 || y == 0){
		dialog->resize(width,height);
	}else{
		dialog->setGeometry(x,y,width,height);
	}
	dialog->setObjectName("egxdialog");
	//printf("objectName:%s\n",gwindow->objectName().toLocal8Bit().data());	
	return HWND_TO_GUIWND(dialog);
}

int egx_dialog_show_(egx_wnd_t hwnd)
{
	CEgxDialog *dialog = (CEgxDialog*)(hwnd);
	if(0 == hwnd){
		return -1;
	}
	if(dialog->isModal() == true){
		dialog->exec();
	}else{
		dialog->show();
	}
	return 0;	
}

int egx_dialog_hide_(egx_wnd_t hwnd)
{
	CEgxDialog *dialog = (CEgxDialog*)(hwnd);
	if(0 == hwnd){
		return -1;
	}
	dialog->finished(0);
	dialog->hide();
	return 0;
}

int egx_dialog_destroy_(egx_wnd_t hwnd)
{
	CEgxDialog *gwindow = (CEgxDialog*)(hwnd);
	if(0 == hwnd){
		return -1;
	}
	delete gwindow;
	return 0;
}


egx_wnd_t egx_view_create_(egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	CEgxWindow *gwindow = (CEgxWindow*)(parent);
	CEgxView *view = new CEgxView(gwindow); 
	if(!view){
		return 0;
	}
	view->resize(width,height);
	view->setObjectName("egxview");
	return HWND_TO_GUIWND(view);
}

int egx_view_show_(egx_wnd_t hwnd)
{
	CEgxView *view = (CEgxView*)hwnd;
	view->show(); 
	view->setFocus();//获取焦点，这样才能显示出来
	return 0;
}

int egx_view_hide_(egx_wnd_t hwnd)
{
	CEgxView *view = (CEgxView*)hwnd;
	view->hide(); 
	return 0;	
}



void CEgxWindow::closeEvent(QCloseEvent * event)
{
	egx_window_t *window = (egx_window_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this)); //(this->frame);
	egx_window_hide(window->id);
}

void CEgxWindow::paintEvent(QPaintEvent * event)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	egx_message_exec_public_msg(frame,egx_message_paint,0,0);
}

void CEgxWindow::timerEvent(QTimerEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));//(this->frame);
	egx_message_exec_timer(frame,event->timerId());
}

void CEgxWindow::keyPressEvent(QKeyEvent *event)
{
	egx_uint32_t key = event->nativeVirtualKey();
	egx_window_t *window = (egx_window_t*)egx_widget_get_struct_(HWND_TO_GUIWND(this));
//    printf("keyPressEvent 0x%x  0x%x  0x%x\n",event->key(),event->nativeVirtualKey(),event->nativeScanCode());
	//sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"cb_window_message() | window:%s   key down\n",window->id);
	if((window->vs_type == egx_vs_type_list) && (key == EASYGX_KEY_PGDOWN)){
		egx_view_t *view = window?(window->current_view):NULL;
		egx_view_hide((view?(view->id):NULL),egx_view_action_next);
	}else if((window->vs_type == egx_vs_type_list) && (key == EASYGX_KEY_PGUP)){
		egx_view_t *view = window?(window->current_view):NULL;
		egx_view_hide((view?(view->id):NULL),egx_view_action_next);
	}else{
		egx_message_exec_public_msg((egx_frame_t*)window,egx_message_keydown,key,0);
	}
}

void CEgxWindow::keyReleaseEvent(QKeyEvent *event)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	egx_message_exec_public_msg(frame,egx_message_keyup,event->nativeVirtualKey(),0);
}

void CEgxWindow::mousePressEvent(QMouseEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_widget_set_focus_(frame->object);		
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg(frame,egx_message_lbuttondown,pos,0);
    }else if(button == Qt::RightButton){
	}else if(button == Qt::MidButton){
		egx_message_exec_public_msg(frame,egx_message_mbuttondown,pos,0);
	}	
}

void CEgxWindow::mouseReleaseEvent(QMouseEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_widget_set_focus_(frame->object);		
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg(frame,egx_message_lbuttonup,pos,0);
    }else if(button == Qt::RightButton){
		egx_window_hide(frame->id);
	}else if(button == Qt::MidButton){
		egx_message_exec_public_msg(frame,egx_message_mbuttonup,pos,0);
	}
}

void	CEgxWindow::mouseDoubleClickEvent(QMouseEvent * event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_widget_set_focus_(frame->object);		
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg(frame,egx_message_lbuttondbclk,pos,0);
    }	
}

void	CEgxWindow::wheelEvent(QWheelEvent * event)
{
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    egx_uint32_t delta = event->delta();
	egx_message_exec_public_msg(frame,egx_message_mousewheel,pos,delta);	
}

void CEgxWindow::on_button_event(bool checked)
{
	char className[64];
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	QObject *sender = dynamic_cast<QObject*>(QObject::sender());
	egx_widget_t *widget = (egx_widget_t *)egx_widget_get_struct_(HWND_TO_GUIWND(sender));
	memset(className,0,sizeof(className));
	strncpy(className,sender->metaObject()->className(),sizeof(className) - 1);
//	printf("on_button_event class:%s  %d\n",className,checked);
	if(0 == strcmp(className,"QPushButton")){
		egx_message_exec_notification(frame,egx_notification_pushbutton_click,0,widget->id);		
	}
	else if(0 == strcmp(className,"QCheckBox")){
		egx_message_exec_notification(frame,egx_notification_checkbox_check,0,widget->id);		
	}
	else if(0 == strcmp(className,"QRadioButton")){
		egx_message_exec_notification(frame,egx_notification_radiobutton_check,0,widget->id);		
	}
}


void CEgxDialog::closeEvent(QCloseEvent * event)
{
	egx_dialog_t *dialog = (egx_dialog_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this)); //(this->frame);
	egx_dialog_hide(dialog->id);
}

void CEgxDialog::paintEvent(QPaintEvent * event)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	egx_message_exec_public_msg(frame,egx_message_paint,0,0);
}

void CEgxDialog::timerEvent(QTimerEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));//(this->frame);
	egx_message_exec_timer(frame,event->timerId());
}

void CEgxDialog::keyPressEvent(QKeyEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));//(this->frame);
	egx_message_exec_public_msg(frame,egx_message_keydown,event->nativeVirtualKey(),0);
}

void CEgxDialog::keyReleaseEvent(QKeyEvent *event)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	egx_message_exec_public_msg(frame,egx_message_keyup,event->nativeVirtualKey(),0);
}

void CEgxDialog::mousePressEvent(QMouseEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_widget_set_focus_(frame->object);		
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg(frame,egx_message_lbuttondown,pos,0);
    }else if(button == Qt::RightButton){
	}else if(button == Qt::MidButton){
		egx_message_exec_public_msg(frame,egx_message_mbuttondown,pos,0);
	}	
}

void CEgxDialog::mouseReleaseEvent(QMouseEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_widget_set_focus_(frame->object);
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg(frame,egx_message_lbuttonup,pos,0);
    }else if(button == Qt::RightButton){
		egx_dialog_hide(frame->id);
	}else if(button == Qt::MidButton){
		egx_message_exec_public_msg(frame,egx_message_mbuttonup,pos,0);
	}
}

void	CEgxDialog::mouseDoubleClickEvent(QMouseEvent * event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_widget_set_focus_(frame->object);		
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg(frame,egx_message_lbuttondbclk,pos,0);
    }	
}

void	CEgxDialog::wheelEvent(QWheelEvent * event)
{
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    egx_uint32_t delta = event->delta();
	egx_message_exec_public_msg(frame,egx_message_mousewheel,pos,delta);	
}

void CEgxDialog::on_button_event(bool checked)
{
	char className[64];
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	QObject *sender = dynamic_cast<QObject*>(QObject::sender());
	egx_widget_t *widget = (egx_widget_t *)egx_widget_get_struct_(HWND_TO_GUIWND(sender));
	memset(className,0,sizeof(className));
	strncpy(className,sender->metaObject()->className(),sizeof(className) - 1);
//	printf("on_button_event class:%s  %d\n",className,checked);
	if(0 == strcmp(className,"QPushButton")){
		egx_message_exec_notification(frame,egx_notification_pushbutton_click,0,widget->id);		
	}
	else if(0 == strcmp(className,"QCheckBox")){
		egx_message_exec_notification(frame,egx_notification_checkbox_check,0,widget->id);		
	}
	else if(0 == strcmp(className,"QRadioButton")){
		egx_message_exec_notification(frame,egx_notification_radiobutton_check,0,widget->id);		
	}
}


void CEgxView::paintEvent(QPaintEvent * event)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	egx_message_exec_public_msg(frame,egx_message_paint,0,0);
}

void CEgxView::timerEvent(QTimerEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	egx_message_exec_timer(frame,event->timerId());
}

void CEgxView::keyPressEvent(QKeyEvent *event)
{
	egx_uint32_t key = event->nativeVirtualKey();
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	CEgxWindow *parent = (CEgxWindow*)this->parentWidget();
	egx_window_t *window = (egx_window_t *)egx_widget_get_struct_(HWND_TO_GUIWND(parent));
	//sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"cb_view_message() | window:%s\n",window->id);	
	if((window->vs_type == egx_vs_type_list) && (key == EASYGX_KEY_PGDOWN)){
		egx_view_hide((frame?(frame->id):NULL),egx_view_action_next);
	}else if((window->vs_type == egx_vs_type_list) && (key == EASYGX_KEY_PGUP)){
		egx_view_hide((frame?(frame->id):NULL),egx_view_action_next);
	}else{
		egx_message_exec_public_msg(frame,egx_message_keydown,key,0);
	}
}

void CEgxView::keyReleaseEvent(QKeyEvent *event)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	egx_message_exec_public_msg(frame,egx_message_keyup,event->nativeVirtualKey(),0);
}

void CEgxView::mousePressEvent(QMouseEvent *event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_widget_set_focus_(frame->object);		
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg(frame,egx_message_lbuttondown,pos,0);
    }else if(button == Qt::RightButton){
	}else if(button == Qt::MidButton){
		egx_message_exec_public_msg(frame,egx_message_mbuttondown,pos,0);
	}	
}

void CEgxView::mouseReleaseEvent(QMouseEvent *event)
{
	egx_view_t *view = (egx_view_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	CEgxWindow *parent = (CEgxWindow*)this->parentWidget();
	egx_window_t *window = (egx_window_t *)egx_widget_get_struct_(HWND_TO_GUIWND(parent));
    egx_widget_set_focus_(view->object);	
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg((egx_frame_t*)view,egx_message_lbuttonup,pos,0);
    }else if(button == Qt::RightButton){
		if(window->vs_type ==egx_vs_type_stack){
			egx_view_hide(view->id,egx_view_action_pop);
		}else{
			egx_window_hide(window->id);
		}
	}else if(button == Qt::MidButton){
		egx_message_exec_public_msg((egx_frame_t*)view,egx_message_mbuttonup,pos,0);
	}
}

void	CEgxView::mouseDoubleClickEvent(QMouseEvent * event)
{
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    unsigned long button = event->button();
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_widget_set_focus_(frame->object);		
    if(button == Qt::LeftButton){
		egx_message_exec_public_msg(frame,egx_message_lbuttondbclk,pos,0);
    }	
}

void	CEgxView::wheelEvent(QWheelEvent * event)
{
    egx_uint32_t x = event->x();
    egx_uint32_t y = event->y();
    egx_uint32_t pos = x + (y<<16);
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
    egx_uint32_t delta = event->delta();
	egx_message_exec_public_msg(frame,egx_message_mousewheel,pos,delta);	
}

void CEgxView::on_button_event(bool checked)
{
	char className[64];
	egx_frame_t *frame = (egx_frame_t *)egx_widget_get_struct_(HWND_TO_GUIWND(this));
	QObject *sender = dynamic_cast<QObject*>(QObject::sender());
	egx_widget_t *widget = (egx_widget_t *)egx_widget_get_struct_(HWND_TO_GUIWND(sender));
	memset(className,0,sizeof(className));
	strncpy(className,sender->metaObject()->className(),sizeof(className) - 1);
//	printf("on_button_event class:%s  %s\n",className,frame->id);
	if(0 == strcmp(className,"QPushButton")){
		egx_message_exec_notification(frame,egx_notification_pushbutton_click,0,widget->id);		
	}
	else if(0 == strcmp(className,"QCheckBox")){
		egx_message_exec_notification(frame,egx_notification_checkbox_check,0,widget->id);		
	}
	else if(0 == strcmp(className,"QRadioButton")){
		egx_message_exec_notification(frame,egx_notification_radiobutton_check,0,widget->id);		
	}	
}

QT_END_NAMESPACE

#ifdef __cplusplus
}
#endif
