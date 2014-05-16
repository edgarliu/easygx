/**
 *\file easygx_class.h
 *\brief easygx¼Ì³ÐqtµÄÀà
 *\version 3.0.0
 *\date 2012
 */

#ifndef EASYGX_CLASS_H
#define EASYGX_CLASS_H

#include "config.h"

#ifdef ENABLE_QT4
#include <QObject>
#include <QFrame>
#endif

#ifdef ENABLE_QT5
#include <QMainWindow>
#include <QFrame>
#include <QDialog>
#include <QAbstractButton>
#include <QString>
#include <QDebug>
#endif

#include "easygx.h"

QT_BEGIN_NAMESPACE

class CEgxWindow: public QFrame //QMainWindow
{
    Q_OBJECT
	public:	
	CEgxWindow():QFrame(0){};
    ~CEgxWindow(){};
	protected:
    void closeEvent(QCloseEvent * event);
	void paintEvent(QPaintEvent * event);
    void timerEvent(QTimerEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);	
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
	void	mouseDoubleClickEvent(QMouseEvent * event); 
    void	wheelEvent(QWheelEvent * event);
//	signals:
	public slots:
	void on_button_event(bool checked);
};

class CEgxDialog: public QDialog
{
    Q_OBJECT
    public:
	CEgxDialog():QDialog(0){};
	~CEgxDialog(){};
	protected:
    void closeEvent(QCloseEvent * event);
	void paintEvent(QPaintEvent * event);
    void timerEvent(QTimerEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
	void	mouseDoubleClickEvent(QMouseEvent * event); 
    void	wheelEvent(QWheelEvent * event);
//	signals:
	public slots:
	void on_button_event(bool checked);	
};

class CEgxView: public QFrame //QMainWindow
{
    Q_OBJECT
    public:
	CEgxView(CEgxWindow *parent =0):QFrame(parent){};
	protected:
	void paintEvent(QPaintEvent * event);
    void timerEvent(QTimerEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);	
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
	void	mouseDoubleClickEvent(QMouseEvent * event); 
    void	wheelEvent(QWheelEvent * event);
	public slots:
	void on_button_event(bool checked);
};

QT_END_NAMESPACE

#endif 
