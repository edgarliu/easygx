
#include <stdlib.h>
#include <string.h>

#include <QApplication>
#include <QTextCodec>

#include "easygx_private.h"
#include "easygx.h"

#ifdef __cplusplus
extern "C" {
#endif

int easygx_init_(easygx_init_conf_t *init_conf)
{
	int ret = -1;
	int argc = 1;
	char *argv[] = {(char*)"easygx",(char*)""};
	QApplication *app;

	app = new QApplication(argc,argv); 
	
/*	
	QList<QByteArray> list = QTextCodec::availableCodecs();
	for(int i=0;i<list.size();i++){
		QByteArray name = list.at(i);
		if(strstr(name.data(),"gb") || strstr(name.data(),"GB") )
		printf("======%s\n",name.data());
	}

	foreach (int mib,QTextCodec::availableMibs()){
		QTextCodec *codec = QTextCodec::codecForMib(mib);
		if(0 == strcmp(codec->name().data(),"GBK")){
			printf("======%s\n",codec->name().data());
			QTextCodec::setCodecForLocale(codec);
		}
	}
*/
	easygx_application_set(app);
	return 0;
}

int easygx_run_()
{
	QApplication *app = (QApplication *)easygx_application_get();
	return app->exec();	
}

int easygx_quit_()
{
	QApplication *app = (QApplication *)easygx_application_get();
	delete app;
	return 0;
}

void easygx_set_quit_()
{
	QApplication *app = (QApplication *)easygx_application_get();
	app->exit(0);
}

#ifdef __cplusplus
}
#endif
