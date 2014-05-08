
#include <stdio.h>

#include "sfpr_string.h"

#include "sfel.h"
#include "sfel_dsfel.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 处理sfel配置文件中属性值中的dsfel脚本
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
			//修改属性值，利用脚本执行结果替换dsfel语句
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
			//修改属性值，利用脚本执行结果替换dsfel语句
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
 * @brief 解析pattern，返回需要匹配的字符串
 * @pattern 用户设置的待匹配字符串
 * @level[out] 返回匹配类型 -1:异常 0:正匹配; 1:全匹配; 2:负匹配
 */
static sfpr_char_t *pattern_parse(sfpr_char_t *pattern, sfpr_int_t *level)
{
	sfpr_char_t *tmp = NULL;
	sfpr_int_t ret = -1;/*0:正匹配; 1:全匹配; 2:负匹配*/

	if (!pattern)
		return NULL;
	
	if (*pattern == '*'){
		//负匹配
		if((tmp = strchr(pattern, '!'))){
			ret = 2; 
		}
		//全匹配
		else{
			ret = 1;
		}
	}
	//正匹配
	else{
		tmp = pattern;
		ret = 0;
	}
	*level = ret;
	return tmp;
}

/**
 * @brief 检测指定的字符串是否匹配
 * @pattern 解析后的匹配模板
 * @level 匹配类型 0:正匹配; 1:全匹配; 2:负匹配
 * @data 用户数据
 * @retval 0 符合条件
 * @retval -1 不符合条件
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
 * 4. SFEL文件管理
***************************************************************************************************/
//创建一个SFEL实例
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
	
    /** 查询节点***/
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

//创建一个SFEL实例
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
	
    /** 查询节点***/
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

//执行节点的回调函数
static sfpr_int_t sfel_inst_exec_sub(sfel_node_t *node,struct sfel_context_s *pscontext,
				sfpr_void_t *param, sfpr_int_t depth,sfpr_char_t *pattern, 
				sfpr_int_t level, int status, sfel_exec_mode_t *exec_mode)
{
	//遍历所有子节点
	const sfpr_char_t *node_name = NULL;
	tag_action_t action_cb = NULL;
	sfel_tag_t *tag = NULL;
	int count=0;
	
	//获取节点名
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
	//获取指定上下文的回调函数
	for(count = 0;count < pscontext->count;count++){
		action_cb = sfel_action_get(node_name,(const sfpr_char_t*)pscontext->array[count]);
		//执行回调
		if(action_cb){
			exec_mode->action_retval = action_cb(node,param,depth,status);
		}else{
			if(exec_mode->default_action_on){
				//根据节点名查找节点信息
				tag = sfel_tag_get(node_name);
				if(tag && tag->default_action){
					exec_mode->action_retval = tag->default_action(node,param,depth,status);
				}
			}
		}
	}
	return 0;
}


//执行节点的回调函数
static sfpr_int_t sfel_inst_exec_recurse(
				sfel_node_t *node,struct sfel_context_s *pscontext,sfpr_void_t *param,
				sfpr_int_t depth,sfpr_char_t *pattern,sfpr_int_t level,sfel_exec_mode_t *exec_mode)
{
	//遍历所有子节点
	sfel_node_t *current = NULL;
	sfel_node_t *sentinel = NULL;
	sfpr_int_t counter,loop_count = 0;
	//解析节点属性
	if(exec_mode->dsfel_on){
		//只有非script节点才能有dsfel
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
		//实现递归
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
		loop_count++;//防止进入死循环
	}while((exec_mode->action_retval == SFEL_NODE_LOOP) &&
	 (loop_count<exec_mode->max_loop) && depth == exec_mode->node_depth_looped);
	
	if((loop_count==exec_mode->max_loop) || 
		(exec_mode->action_retval == SFEL_NODE_LOOP_END)){
		exec_mode->node_depth_looped = 0;
	}
	return 0;
}



//执行节点的回调函数
sfpr_int_t sfel_inst_exec(sfel_node_t *node,sfpr_char_t *context,sfpr_void_t *param,sfel_exec_mode_t *exec_mode)
{
	//遍历所有子节点
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
	//防止循环次数太多
	exec_mode->max_loop = (exec_mode->max_loop>1024)?1024:exec_mode->max_loop;
	sfel_inst_exec_recurse(node,&scontext,param,depth,sub_pattern, pattern_level, exec_mode);
	return exec_mode->action_retval;
}

//销毁一个实例
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

