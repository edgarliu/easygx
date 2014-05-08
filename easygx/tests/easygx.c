#include <stdio.h>
#include <string.h>

#include "easygx.h"
#include "easygx_log.h"

int main(int argc, char* argv[])
{
	easygx_init_conf_t init_conf;

	#ifdef WIN32
	LoadLibraryA("exchndl.dll");
	#endif

	easygx_log_init("stderr",SFPR_LOG_INFO);	
	if(easygx_init(&init_conf) < 0){
		fprintf(stderr,"easygx_init() | error: %s\n",init_conf.error);
		return 1;
	}	

	if(easygx_sfel_file_register("easygx.xml") < 0){
		easygx_quit();
		return 1;
	}
	easygx_sfel_file_show();
	easygx_run();
	easygx_sfel_file_unregister();
	easygx_quit();
	return 0;
}
