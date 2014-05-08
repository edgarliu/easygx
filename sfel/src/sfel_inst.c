
#include <stdio.h>

#include "sfpr_string.h"

#include "sfel.h"
#include "sfel_dsfel.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ����sfel�����ļ�������ֵ�е�dsfel�ű�
 */
static sfpr_int_t attr_handle_dsfel(sfel_node_t *node, sfel_attr_t *attr, sfpr_void_t *param)
{
	sfpr_int_t ret = -1;
	sfel_attr_t *attr_ret;
	sfpr_char_t *string = NULL;
	sfel_document_t *doc = NULL;
	doc = sfel_xml_node_get_document(node);
	if(!doc)
		return -1;
	if(!dsfel_isvalid(sfel_xml_attr_get_avalue(attr)))
	{
		string = malloc(SFEL_DSFEL_MAX_RETURN_SIZE);
		memset(string, 0x0, SFEL_DSFEL_MAX_RETURN_SIZE);
		ret = dsfel_compile(doc, sfel_xml_attr_get_avalue(attr), string,SFEL_DSFEL_MAX_RETURN_SIZE - 1);
		if(ret == 0)
		{
			#ifdef LOG_DEBUG
			printf("attr name:%s, old value:%s new value:%s\n", sfel_xml_attr_get_aname(attr), sfel_xml_attr_get_avalue(attr), string);
			#endif
			//�޸�����ֵ�����ýű�ִ�н���滻dsfel���
			attr_ret = sfel_xml_attr_update(node,sfel_xml_attr_get_aname(attr),string);
			if(attr_ret)
				ret = 0;
			else
				ret = -1;
		}
		free(string);
	}
	return ret;

}

static sfpr_int_t node_handle_dsfel(sfel_node_t *node, sfpr_void_t *param)
{
	sfpr_int_t ret = -1;
	sfpr_char_t *string = NULL;
	sfel_document_t *doc = NULL;
	doc = sfel_xml_node_get_document(node);
	if(!doc)
		return -1;
	if(sfel_xml_node_get_type(node) != sfel_node_element)
		return 0;	
	if(!dsfel_isvalid(sfel_xml_node_get_value(node))){
		string = malloc(SFEL_DSFEL_MAX_RETURN_SIZE);
		memset(string, 0x0, SFEL_DSFEL_MAX_RETURN_SIZE);
		ret = dsfel_compile(doc, sfel_xml_node_get_value(node), string,SFEL_DSFEL_MAX_RETURN_SIZE - 1);
		if(ret == 0)
		{
			#ifdef LOG_DEBUG		
			printf("node name:%s, old value:%s new value:%s\n", sfel_xml_node_get_name(node), sfel_xml_node_get_value(node), string);
			#endif
			//�޸�����ֵ�����ýű�ִ�н���滻dsfel���
			if(sfel_xml_node_set_value(node, string))
			{
				ret = 0;
			}
			else
			{
				ret = -1;
			}
		}
		free(string);
	}
	return ret;
}

/**
 * sample:
 * 1. button,window,panel
 * 2. *
 * 3. *,!button,!window,!panel
 * @brief ����pattern��������Ҫƥ����ַ���
 * @pattern �û����õĴ�ƥ���ַ���
 * @level[out] ����ƥ������ -1:�쳣 0:��ƥ��; 1:ȫƥ��; 2:��ƥ��
 */
static sfpr_char_t *pattern_parse(sfpr_char_t *pattern, sfpr_int_t *level)
{
	sfpr_char_t *tmp = NULL;
	sfpr_int_t ret = -1;/*0:��ƥ��; 1:ȫƥ��; 2:��ƥ��*/

	if (!pattern)
		return NULL;
	
	if (*pattern == '*'){
		//��ƥ��
		if((tmp = strchr(pattern, '!'))){
			ret = 2; 
		}
		//ȫƥ��
		else{
			ret = 1;
		}
	}
	//��ƥ��
	else{
		tmp = pattern;
		ret = 0;
	}
	*level = ret;
	return tmp;
}

/**
 * @brief ���ָ�����ַ����Ƿ�ƥ��
 * @pattern �������ƥ��ģ��
 * @level ƥ������ 0:��ƥ��; 1:ȫƥ��; 2:��ƥ��
 * @data �û�����
 * @retval 0 ��������
 * @retval -1 ����������
 */
static sfpr_int_t pattern_match(const sfpr_char_t *pattern, sfpr_int_t level, const sfpr_char_t *data)
{
	const sfpr_int_t size = strlen(data) + 3;
	sfpr_char_t *tmp = NULL;
	switch(level){
	case 0:{
		if((tmp = strstr(pattern, data))){
			if(tmp[strlen(data)] == '\0' ||  tmp[strlen(data)] == ','){
				if(tmp != pattern){
					if(pattern[tmp-pattern-1] != ','){
						return -1;
					}else{
						return 0;
					}
				}else{
					return 0;
				}
			}else{
				return -1;
			}
		}else{
			return -1;
		}
	}
	case 1:{
		return 0;
	}
	case 2:{
		if(tmp = strstr(pattern, data)){
			if(tmp[strlen(data)] == '\0' ||	tmp[strlen(data)] == ','){
				if(pattern[tmp - pattern-1] == '!'){
					return -1;
				}else{
					return 0;
				}
			}else{
				return 0;
			}
		}else{
			return 0;
		}
		break;
	}
	default:{
		return -1;
	}
	}
}

/**************************************************************************************************
 * 4. SFEL�ļ�����
***************************************************************************************************/
//����һ��SFELʵ��
sfel_instance_t *sfel_inst_create(sfpr_char_t *sfel_file, sfpr_int_t save, sfel_portable_t *portable)
{
	sfel_instance_t *instance = malloc(sizeof(sfel_instance_t));
	if (!instance)
		return NULL;
	memset(instance,0,sizeof(sfel_instance_t));
	instance->doc = sfel_xml_parse_file(sfel_file,portable);
	if(!instance->doc)
	{
		sfel_inst_destroy(&instance);
		return NULL;
	}
	
    /** ��ѯ�ڵ�***/
	instance->root = sfel_xml_get_root(instance->doc);
	if(!instance->root)
	{
		sfel_inst_destroy(&instance);
		return NULL;
	}

	memset(instance->path, 0x0, sizeof(instance->path));
	strcpy(instance->path, sfel_file);
	instance->save = save;
	
	return instance;
}

//����һ��SFELʵ��
sfel_instance_t *sfel_inst_create_by_string(sfpr_char_t *sfel_string, sfel_portable_t *portable)
{
	sfel_instance_t *instance = malloc(sizeof(sfel_instance_t));
	if (!instance)
		return NULL;
	memset(instance,0,sizeof(sfel_instance_t));
	memset(instance->path, 0x0, sizeof(instance->path));
	instance->save = 0;
	instance->doc = sfel_xml_parse_string(sfel_string,portable);
	if(!instance->doc)
	{
		printf("sfel_inst_create_by_string() | parse string error!\n");
		sfel_inst_destroy(&instance);
		return NULL;
	}
	
    /** ��ѯ�ڵ�***/
	instance->root = sfel_xml_get_root(instance->doc);
	if(!instance->root)
	{
		printf("sfel_inst_create_by_string() | parse docment error!\n");
		sfel_inst_destroy(&instance);
		return NULL;
	}
	return instance;
}

#define SFEL_MAX_CONTEXT 12
struct sfel_context_s{
	char buffer[512];
	char *array[SFEL_MAX_CONTEXT];
	int count;
};

//ִ�нڵ�Ļص�����
static sfpr_int_t sfel_inst_exec_sub(sfel_node_t *node,struct sfel_context_s *pscontext,
				sfpr_void_t *param, sfpr_int_t depth,sfpr_char_t *pattern, 
				sfpr_int_t level, int status, sfel_exec_mode_t *exec_mode)
{
	//���������ӽڵ�
	const sfpr_char_t *node_name = NULL;
	tag_action_t action_cb = NULL;
	sfel_tag_t *tag = NULL;
	int count=0;
	
	//��ȡ�ڵ���
	node_name = sfel_xml_node_get_name(node);
	#ifdef LOG_NOTICE
	if(status == SFEL_NODE_START)
		printf("%*s %s  type:%d\n",depth*4,"===>",node_name,sfel_xml_node_get_type(node));
	else
		printf("%*s %s  type:%d\n",depth*4,"<===",node_name,sfel_xml_node_get_type(node));
	#endif
	
	if(NULL == node_name){
		return -1;
	}
	if(pattern_match(pattern, level, node_name))
		return -1;
	//��ȡָ�������ĵĻص�����
	for(count = 0;count < pscontext->count;count++){
		action_cb = sfel_action_get(node_name,(const sfpr_char_t*)pscontext->array[count]);
		//ִ�лص�
		if(action_cb){
			exec_mode->action_retval = action_cb(node,param,depth,status);
		}else{
			if(exec_mode->default_action_on){
				//���ݽڵ������ҽڵ���Ϣ
				tag = sfel_tag_get(node_name);
				if(tag && tag->default_action){
					exec_mode->action_retval = tag->default_action(node,param,depth,status);
				}
			}
		}
	}
	return 0;
}


//ִ�нڵ�Ļص�����
static sfpr_int_t sfel_inst_exec_recurse(
				sfel_node_t *node,struct sfel_context_s *pscontext,sfpr_void_t *param,
				sfpr_int_t depth,sfpr_char_t *pattern,sfpr_int_t level,sfel_exec_mode_t *exec_mode)
{
	//���������ӽڵ�
	sfel_node_t *current = NULL;
	sfel_node_t *sentinel = NULL;
	sfpr_int_t counter,loop_count = 0;
	//�����ڵ�����
	if(exec_mode->dsfel_on){
		//ֻ�з�script�ڵ������dsfel
		if(0 != strcmp(sfel_xml_node_get_name(node),"script")){
			sfel_xml_node_enum_attr(node, NULL, NULL, attr_handle_dsfel, param);
			node_handle_dsfel(node, param);
		}
	}
	sfel_inst_exec_sub(node, pscontext, param, depth, pattern, level,SFEL_NODE_START, exec_mode);
	//added by liuxq 2012-4-20
	if((exec_mode->action_error_omit == 0) && (exec_mode->action_retval < 0)){
		#ifdef LOG_ERROR
		fprintf(stderr,"%*s sfel_inst_exec() | error | node %s action failed:%d\n",
				depth*4,"<===",sfel_xml_node_get_name(node),exec_mode->action_retval);
		#endif
		return -1;
	}
	if((exec_mode->action_retval == SFEL_NODE_LOOP_BEGIN) && (exec_mode->node_depth_looped == 0)){
		exec_mode->node_depth_looped = depth;
	}

	do{
		//ʵ�ֵݹ�
		if((exec_mode->recursive == -1 || 
			exec_mode->recursive > depth) && 
			(exec_mode->action_retval != SFEL_NODE_SKIP_CHILD))
		{
			current = sfel_xml_node_get_child(node);	
			for(sentinel = sfel_xml_node_get_sibling(current); current; current = sentinel, sentinel = sfel_xml_node_get_sibling(sentinel))
			{
				sfel_inst_exec_recurse(current,pscontext,param,depth+1,pattern, level, exec_mode);
				if((exec_mode->action_error_omit == 0) && (exec_mode->action_retval < 0)){
					return -1;
				}
			}
		}
		sfel_inst_exec_sub(node, pscontext, param, depth, pattern, level,SFEL_NODE_END, exec_mode);
		//added by liuxq 2012-4-20
		if((exec_mode->action_error_omit == 0) && (exec_mode->action_retval < 0)){
			#ifdef LOG_ERROR
			fprintf(stderr,"%*s  sfel_inst_exec() | error | node %s action failed:%d\n",
					depth*4,"<===",sfel_xml_node_get_name(node),exec_mode->action_retval);
			#endif
			return -1;
		}
		loop_count++;//��ֹ������ѭ��
	}while((exec_mode->action_retval == SFEL_NODE_LOOP) &&
	 (loop_count<exec_mode->max_loop) && depth == exec_mode->node_depth_looped);
	
	if((loop_count==exec_mode->max_loop) || 
		(exec_mode->action_retval == SFEL_NODE_LOOP_END)){
		exec_mode->node_depth_looped = 0;
	}
	return 0;
}



//ִ�нڵ�Ļص�����
sfpr_int_t sfel_inst_exec(sfel_node_t *node,sfpr_char_t *context,sfpr_void_t *param,sfel_exec_mode_t *exec_mode)
{
	//���������ӽڵ�
	sfpr_char_t *sub_pattern = NULL;
	sfpr_int_t pattern_level = -1;
	sfpr_int_t depth = 0;
	sfpr_char_t pattern_buff[1024];
	struct sfel_context_s scontext;

	memset(pattern_buff, 0x0, sizeof(pattern_buff));
	strncpy(pattern_buff, exec_mode->pattern,sizeof(pattern_buff)-1);
	if(!node || !context)
		return -1;

	sfpr_str_trim(pattern_buff);
	sub_pattern = pattern_parse(pattern_buff, &pattern_level);
	if(pattern_level == -1)
		return -1;
	
	memset(&scontext,0,sizeof(struct sfel_context_s));
	strncpy(scontext.buffer,context,sizeof(scontext.buffer)-1);
	scontext.count =sfpr_str_split(scontext.buffer,scontext.array,SFEL_MAX_CONTEXT,"|\r\n");
	if(scontext.count <= 0)
		return -1;
	exec_mode->action_retval = 0;
	exec_mode->node_depth_looped = 0;
	//��ֹѭ������̫��
	exec_mode->max_loop = (exec_mode->max_loop>1024)?1024:exec_mode->max_loop;
	sfel_inst_exec_recurse(node,&scontext,param,depth,sub_pattern, pattern_level, exec_mode);
	return exec_mode->action_retval;
}

//����һ��ʵ��
sfpr_int_t sfel_inst_destroy(sfel_instance_t **sfel_inst)
{
	sfel_instance_t *instance = *sfel_inst;
	if(!instance)
		return 0;

	if(instance->save){
		sfel_xml_save(instance->doc, instance->path);
	}
	
	if(instance->doc){
		sfel_xml_destroy(instance->doc);
	}

	free(instance);
	*sfel_inst = NULL;
	return 0;
}

#ifdef __cplusplus
}
#endif

