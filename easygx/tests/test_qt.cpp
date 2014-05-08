
#include <QApplication>
#include <QTextCodec>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

typedef unsigned long egx_wnd_t;

void *test_init()
{
	int argc = 1;
	char *argv[] = {(char*)"easygx",(char*)""};	
	QApplication *app;
	app = new QApplication(argc,argv); 
	
	return app;
}


int test_run(void *papp)
{
	QApplication *app = (QApplication *)papp;
	return app->exec();		
}

int test_quit(void *papp)
{
	QApplication *app = (QApplication *)papp;
	delete app;
	return 0;
}

egx_wnd_t window_create_(char *name,egx_wnd_t style,int x,int y,int width,int height)
{
	QWidget *gwindow = new QWidget(); 
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
	return egx_wnd_t(gwindow);
}

int window_show_(egx_wnd_t hwnd)
{
	QWidget *gwindow = (QWidget*)(hwnd);
	gwindow->show();
	return 0;	
}

int main(int argc, char* argv[])
{
	void *app = test_init();
	egx_wnd_t window = window_create_((char*)"Test Qt Hahaha",0,0,0,640,480);
	window_show_(window);
	test_run(app);
	test_quit(app);
	return 0;
}
