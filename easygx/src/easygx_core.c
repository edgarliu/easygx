
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "easygx.h"
#include "easygx_core.h"
#include "easygx_frame.h"
#include "easygx_sfel.h"
#include "easygx_portable.h"
#include "easygx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

static easygx_t *g_easygx = NULL;
static int inited = 0;

void easygx_instance_set(easygx_t *easygx)
{
	g_easygx = easygx;
}

easygx_t *easygx_instance_get()
{
	return g_easygx;
}

void easygx_instance_quit()
{
	free(g_easygx);
	g_easygx = NULL;
	inited = 0;
}



int easygx_init(easygx_init_conf_t *init_conf)
{
	easygx_t *_easygx = NULL;

	memset(init_conf->error,0,sizeof(init_conf->error));
	if(inited){
		strncpy(init_conf->error,"easygx has inited\n",sizeof(init_conf->error)-1);
		return 1;
	}

	_easygx = (easygx_t*)malloc(sizeof(easygx_t));
	if(!_easygx)
		return -1;

	memset(_easygx,0,sizeof(easygx_t));
	strncpy(_easygx->name,GUI_LIBARY,sizeof(_easygx->name)-1);

	_easygx->widget_hash = sfpr_hash_new();
	if(NULL == _easygx->widget_hash){
		return -2;		
	}
	sfpr_hash_init(_easygx->widget_hash, "widgetHash",4096, NULL, NULL,NULL);
	
	sfpr_stack_create(&(_easygx->showed_frame),128,sfpr_stack_mode_pointer);

	if(sfel_init() < 0){
		free(_easygx);
		return -1;
	}

	easygx_instance_set(_easygx);
	inited = 1;

	if(easygx_init_(init_conf) < 0){
		free(_easygx);
		easygx_instance_quit();
		return -1;
	}
	sfpr_log(easygx_log,SFPR_LOG_INFO,"easygx_init() | easygx library: %s\n",_easygx->name);	
	return 0;
}

int easygx_run()
{
	if(0 == inited || NULL == easygx_instance_get()){
		return -1;
	}
	return easygx_run_();	
}

int easygx_quit()
{
	if(0 == inited || NULL == easygx_instance_get()){
		return -1;
	}
	egx_widget_enum();
	easygx_quit_();
	easygx_instance_quit();
	return 0;
}

void easygx_set_quit()
{
	easygx_t *easygx = easygx_instance_get();
	if(!easygx){
		return;
	}
	easygx->quit_flag = 1;
	easygx_set_quit_();
}

int easygx_is_quit()
{
	easygx_t *easygx = easygx_instance_get();
	if(!easygx){
		return 1;
	}
	return easygx->quit_flag;
}


const char*easygx_name_get()
{
	easygx_t *easygx = easygx_instance_get();
	if(NULL == easygx){
		return NULL;
	}
	return easygx->name;
}

int easygx_application_set(void *application)
{
	easygx_t *easygx = easygx_instance_get();
	if(!easygx){
		return 0;
	}
	sfpr_log(easygx_log,SFPR_LOG_INFO,"easygx_application_set() | %p\n",application);		
	easygx->application = application;
}

void* easygx_application_get()
{
	easygx_t *easygx = easygx_instance_get();
	if(!easygx){
		return NULL;
	}
	sfpr_log(easygx_log,SFPR_LOG_INFO,"easygx_application_get() | %p\n",easygx->application);		
	return easygx->application;
}



int egx_widget_register(egx_widget_t *widget)
{
	easygx_t *easygx = easygx_instance_get();
	int retval = 0;
	if(!widget || !widget->id){
		return -1;
	}
	retval = sfpr_hash_insert(easygx->widget_hash,widget->id,strlen(widget->id),(sfpr_void_t*)widget, 0,0);
	return retval;
}

egx_widget_t *egx_widget_find(const char *id)
{
	easygx_t *easygx = easygx_instance_get();
	if(!id){
		return NULL;
	}
	return sfpr_hash_find(easygx->widget_hash, id, strlen(id));	
}

static void widget_enum_cb(sfpr_hash_node_t *node,void *param)
{
	sfpr_log(easygx_log,SFPR_LOG_INFO,(char*)"egx_widget_enum() | slot:%d  id:%s  hashvalue:%x\n",
			sfpr_hash_node_get_slot(node),sfpr_hash_node_get_key(node),sfpr_hash_node_get_hashvalue(node));		
}

void egx_widget_enum()
{
	easygx_t *easygx = easygx_instance_get();
	sfpr_hash_enum(easygx->widget_hash,widget_enum_cb,NULL);
}

#ifdef __cplusplus
}
#endif
