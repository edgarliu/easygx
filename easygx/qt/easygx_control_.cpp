
#include <string.h>

#include <QLabel>
#include <QAbstractButton>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QString>
#include <QLineEdit>
#include <QGroupBox>
#include <QButtonGroup>
#include "easygx_private.h"

#include "easygx.h"
#include "easygx_control.h"


#ifdef __cplusplus
extern "C" {
#endif

egx_wnd_t egx_label_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	QLabel *label = new QLabel((QWidget*)(parent));
	label->setText(QString::fromLocal8Bit(name));
	if(x == 0 || y == 0){
		label->resize(width,height);
	}else{
		label->setGeometry(x,y,width,height);
	}
	return HWND_TO_GUIWND(label);
}

int egx_label_destroy_(egx_wnd_t hwnd)
{
	QLabel *label = (QLabel*)(hwnd);
	delete label;
	return 0;
}

int egx_label_set_text_(egx_wnd_t hwnd,char *text)
{
	QLabel *label = (QLabel*)(hwnd);
	label->setText(QString::fromLocal8Bit(text));
	return 0;
}

int egx_label_get_text_(egx_wnd_t hwnd, char *text, int length)
{
	QLabel *label = (QLabel*)(hwnd);
	QString qText = label->text();
	strncpy(text,qText.toLocal8Bit().data(),length - 1);
	return 0;
}

int egx_button_enable_(egx_wnd_t hwnd,int enable)
{
	if(0 == hwnd){
		return -1;
	}
	QWidget *widget = (QWidget*)hwnd;
	if(enable){
		widget->setEnabled(true);
	}else{
		widget->setEnabled(false);		
	}
	return 0;
}

int egx_button_checked_(egx_wnd_t hwnd)
{
	QAbstractButton *button = (QAbstractButton*)hwnd;
	if(!button){
		return -1;
	}
	return button->isChecked();
}

egx_wnd_t egx_pushbutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	QPushButton *button = new QPushButton((QWidget*)(parent));
	button->setText(QString::fromLocal8Bit(name));
	if(x == 0 || y == 0){
		button->resize(width,height);
	}else{
		button->setGeometry(x,y,width,height);
	}
	return HWND_TO_GUIWND(button);
}

egx_wnd_t egx_checkbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	QCheckBox *button = new QCheckBox((QWidget*)(parent));
	button->setText(QString::fromLocal8Bit(name));
	if(x == 0 || y == 0){
		button->resize(width,height);
	}else{
		button->setGeometry(x,y,width,height);
	}
	return HWND_TO_GUIWND(button);
}

egx_wnd_t egx_groupbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	QGroupBox *button = new QGroupBox(QString::fromLocal8Bit(name),(QWidget*)(parent));
	if(x == 0 || y == 0){
		button->resize(width,height);
	}else{
		button->setGeometry(x,y,width,height);
	}
	return HWND_TO_GUIWND(button);
}

egx_wnd_t egx_radiobutton_begin_(egx_wnd_t parent)
{
	QButtonGroup *button = new QButtonGroup((QWidget*)(parent));	
	return HWND_TO_GUIWND(button);
}

egx_wnd_t egx_radiobutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent,egx_wnd_t radiogroup)
{
	QRadioButton *button = new QRadioButton((QWidget*)(parent));
	button->setText(QString::fromLocal8Bit(name));
	if(x == 0 || y == 0){
		button->resize(width,height);
	}else{
		button->setGeometry(x,y,width,height);
	}
	QButtonGroup *_radiogroup = (QButtonGroup*)radiogroup;
	if(_radiogroup){
		_radiogroup->addButton(button);
	}
	return HWND_TO_GUIWND(button);
}



egx_wnd_t egx_edit_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	QLineEdit *edit = new QLineEdit((QWidget*)(parent));
	edit->setText(QString::fromLocal8Bit(name));
	if(x == 0 || y == 0){
		edit->resize(width,height);
	}else{
		edit->setGeometry(x,y,width,height);
	}
	return HWND_TO_GUIWND(edit);
}

int egx_edit_get_text_(egx_wnd_t hwnd,char *text,int length)
{
	QLineEdit *edit = (QLineEdit*)(hwnd);
	QString qText = edit->text();
	strncpy(text,qText.toLocal8Bit().data(),length - 1);	
	return 0;
}

int egx_edit_set_text_(egx_wnd_t hwnd,char *text)
{
	QLineEdit *edit = (QLineEdit*)(hwnd);
	edit->setText(QString::fromLocal8Bit(text));
	return 0;
}


#ifdef __cplusplus
}
#endif
