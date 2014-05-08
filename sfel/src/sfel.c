
#include <stdio.h>

#include "sfpr_thread_mutex.h"

#include "sfel.h"
#include "sfel_ref.h"

#ifdef __cplusplus
extern "C" {
#endif

extern sfel_tag_t sfel_tags_basic[];

#define MAX_TAGS		399
typedef struct sfel_s{
	sfpr_hash_t *tag_hash;
	sfpr_hash_t *action_hash;
	sfpr_int_t size;//最大标签数
	sfpr_int_t status;
	sfpr_mutex_t mutex;
}sfel_t;

enum sfel_status{ss_unused = 0,ss_initing,ss_usable};

static sfel_t g_sfel;

static sfpr_int_t sfel_tag_default_action(sfel_node_t *node,sfpr_void_t *param,sfpr_int_t depth,sfpr_int_t status)
{
	sfpr_char_t *attr_value = sfel_xml_attr_get_value(node,"id");
	fprintf(stderr,"%*s sfel_tag_default_action() | Status:%d  Node Name:%s  Id:%s  Value:%s\n",
			depth*4," ",status,sfel_xml_node_get_name(node),
			attr_value?attr_value:"",sfel_xml_node_get_value(node));	
}

sfpr_int_t sfel_init()
{
	if(g_sfel.status != ss_unused)
		return 1;

	g_sfel.status = ss_initing;

	#ifdef LOG_NOTICE
	fprintf(stderr,"sfel_init() | notice | begin\n");
	#endif

	sfpr_mutex_create(&(g_sfel.mutex),0);
	g_sfel.size = MAX_TAGS;
	g_sfel.tag_hash = sfpr_hash_new();
	if(sfpr_hash_init(g_sfel.tag_hash,"Sfel_Hash",g_sfel.size,NULL,NULL,NULL)<0){
		#ifdef LOG_ERROR
		fprintf(stderr,"sfel_init() | error | sfpr_hash_init() failed\n");
		#endif
		g_sfel.status = ss_unused;
		return -1;
	}

	sfel_tags_register("basic",sfel_tags_basic);

	g_sfel.action_hash = sfpr_hash_new();
	if(sfpr_hash_init(g_sfel.action_hash,"Sfel_Action_Hash",4096,NULL,NULL,NULL)<0){
		#ifdef LOG_ERROR
		fprintf(stderr,"sfel_init() | error | sfpr_hash_init() failed\n");
		#endif
		g_sfel.status = ss_unused;
		return -1;
	}

	g_sfel.status = ss_usable;
	return 0;
}

static sfpr_void_t sfel_tag_clear_cb(sfpr_hash_node_t *node)
{
}

sfpr_int_t sfel_quit()
{
	if(g_sfel.status != ss_usable)
		return -1;

	sfpr_hash_clear(g_sfel.tag_hash,sfel_tag_clear_cb);
	sfpr_hash_finalize(g_sfel.tag_hash);
	sfpr_hash_destroy(&(g_sfel.tag_hash));
	sfpr_hash_clear(g_sfel.action_hash,sfel_tag_clear_cb);
	sfpr_hash_finalize(g_sfel.action_hash);
	sfpr_hash_destroy(&(g_sfel.action_hash));
	sfpr_mutex_destroy(&(g_sfel.mutex));
	g_sfel.status = ss_unused;
	return 0;
}

int sfel_tags_register(char *classname,sfel_tag_t *list)
{
	sfel_tag_t *tag = NULL;	
	sfpr_int_t j=0;
	
	while(list[j].name){
		tag = &(list[j]);
		#ifdef LOG_DEBUG
		fprintf(stderr,"sfel_init() | Tag | Name: %-12s  Desc: %s\n",tag->name,tag->desc);
		#endif
		#if 1
		tag->default_action = NULL;
		#else
		tag->default_action = sfel_tag_default_action;
		#endif
		tag->action_count = 0;
		sfpr_hash_insert(g_sfel.tag_hash,tag->name,strlen(tag->name),tag,0,0);
		j++;
	}
	return 0;
}

sfel_tag_t *sfel_tag_get(const sfpr_char_t *tagname)
{
	if(g_sfel.status != ss_usable)
		return NULL;
	if(!tagname)
		return NULL;
	
	return (sfel_tag_t*)sfpr_hash_find(g_sfel.tag_hash,tagname,strlen(tagname));
}

static sfpr_void_t sfel_tag_enum_cb(sfpr_hash_node_t *node,sfpr_void_t *param)
{
	sfel_tag_t *tag = (sfel_tag_t*)sfpr_hash_node_get_value(node);
	sfpr_int_t i=0;
	
	printf("sfel_tag_enum_cb(): slot:%-5d hash:%-13d key:%-15s   Desc:%s\n",
				sfpr_hash_node_get_slot(node),
				sfpr_hash_node_get_hashvalue(node),
				sfpr_hash_node_get_key(node),
				tag->desc);
	if(!tag->attr){
		return;
	}
	while(tag->attr[i].name){
		fprintf(stderr,"\t\tAttrbute  Name:%-15s  Desc:%s\n",tag->attr[i].name,tag->attr[i].desc);	
		i++;
	}
}

sfpr_int_t sfel_tag_enum()
{
	if(g_sfel.status != ss_usable)
		return -1;

	sfpr_hash_enum(g_sfel.tag_hash,sfel_tag_enum_cb,NULL);
	
	#ifdef LOG_NOTICE
	fprintf(stderr,"sfel_tag_enum() | notice | Hash Count: %d\n",sfpr_hash_count(g_sfel.tag_hash));
	#endif
	
	return 0;
}

sfpr_int_t sfel_tag_attr_enum(sfpr_char_t *tagname)
{
	sfel_tag_t *tag = NULL;
	sfpr_int_t i=0;
	if(g_sfel.status != ss_usable)
		return -1;
	if(!tagname)
		return -1;
	
	tag = (sfel_tag_t*)sfpr_hash_find(g_sfel.tag_hash,tagname,strlen(tagname));
	if(!tag)
		return -1;
	
	while(tag->attr[i].name){
		fprintf(stderr,"Tag Attr | Name:%s  Desc:%s\n",tag->attr[i].name,tag->attr[i].desc);	
		i++;
	}
	return 0;
}




typedef struct{
	char key[128];
	tag_action_t action;/**<上下文处理函数*/
}sfel_inner_action_t;

//注册标签
sfpr_int_t sfel_action_register(sfpr_char_t *tagname,const sfpr_char_t *context,tag_action_t action)
{
	sfel_tag_t *tag = NULL;
	sfel_inner_action_t *inner_action = NULL;

	if(g_sfel.status != ss_usable)
		return -1;
	if(!tagname || !action)
		return -2;
	tag = (sfel_tag_t*)sfpr_hash_find(g_sfel.tag_hash,tagname,strlen(tagname));
	if(!tag)
		return -3;

	sfpr_mutex_lock(&(g_sfel.mutex));
	if(!context){
		tag->default_action = action;
		sfpr_mutex_unlock(&(g_sfel.mutex));
		return 0;
	}

	inner_action = (sfel_inner_action_t*)malloc(sizeof(sfel_inner_action_t));
	if(NULL == inner_action){
		sfpr_mutex_unlock(&(g_sfel.mutex));
		return -4;		
	}
	memset(inner_action,0,sizeof(sfel_inner_action_t));
	sprintf(inner_action->key,"%s->%s",tagname,context);
	inner_action->action = action;
	sfpr_hash_insert(g_sfel.action_hash,inner_action->key,strlen(inner_action->key),
			inner_action,0,0);

	tag->action_count++;	
	sfpr_mutex_unlock(&(g_sfel.mutex));
	return 0;
}


sfpr_int_t sfel_action_registers(const sfpr_char_t *context,sfel_action_t *list)
{
	int i = 0,ret;
	if(!list)
		return -1;

	if(g_sfel.status != ss_usable)
		return -1;
	
	while(list[i].tag && list[i].action){
		ret = sfel_action_register(list[i].tag,context,list[i].action);
		if(ret < 0){
			fprintf(stderr,"sfel_action_registers() | error | tag: %s register failed:%d\n",list[i].tag,ret);
		}else{
			#ifdef LOG_NOTICE
			fprintf(stderr,"sfel_action_registers() | notice | tag: %s\n",list[i].tag);
			#endif
		}
		i++;
	}

	return 0;
}

tag_action_t sfel_action_get(const sfpr_char_t *tagname,const sfpr_char_t *context)
{
	char key[128];
	sfel_tag_t *tag = NULL;
	sfel_inner_action_t *inner_action = NULL;

	if(g_sfel.status != ss_usable)
		return NULL;
	if(!tagname || !context)
		return NULL;
	
	tag = (sfel_tag_t*)sfpr_hash_find(g_sfel.tag_hash,tagname,strlen(tagname));
	if(!tag)
		return NULL;

	memset(key,0,sizeof(key));
	sprintf(key,"%s->%s",tagname,context);
	inner_action = (sfel_inner_action_t*)sfpr_hash_find(g_sfel.action_hash,key,strlen(key));
	if(NULL == inner_action){
		return NULL;
	}
	return inner_action->action;
}




static sfpr_int_t sfel_xml_attr_cb(sfel_node_t *node, sfel_attr_t *attr, sfpr_void_t *param)
{
	sfel_node_t *new_node = param;
	if(!new_node)
		return -1;
	sfel_xml_attr_add(new_node, sfel_xml_attr_get_aname(attr),sfel_xml_attr_get_avalue(attr));
	return 0;
}

static sfpr_int_t sfel_xml_node_cb(sfel_node_t *node, sfpr_void_t *param)
{
	sfel_node_t *new_node = param;
	if(!new_node)
		return -1;
	sfel_xml_node_copy_to_child(new_node, node);
	return 0;
}

sfel_node_t* sfel_xml_node_copy_to_child(sfel_node_t *sfel_node, sfel_node_t *child_node)
{
	sfel_node_t *new_node;
	
	//增加当前标签，及所有属性
	new_node = sfel_xml_node_append_child_by_name(sfel_node, sfel_xml_node_get_name(child_node),sfel_xml_node_get_value(child_node));
	sfel_xml_node_enum_attr(child_node, NULL,NULL, sfel_xml_attr_cb, new_node);
	
	//遍历下一级子标签及属性
	sfel_xml_node_enum_child(child_node, NULL, NULL,NULL, sfel_xml_node_cb, new_node);
	return new_node;
}

#ifdef __cplusplus
}
#endif
