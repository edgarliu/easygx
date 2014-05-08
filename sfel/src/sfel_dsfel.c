
#include "sfel_dsfel.h"

#ifdef __cplusplus
extern "C" {
#endif
#define INNER_DEBUG 1
#define DSFEL_BUFF_SIZE 128
#define DSFEL_SCRIPT_SIZE 1024

static sfpr_int_t getelementbyid(sfel_document_t *doc, sfpr_void_t *argv[], dsfel_handle_result_t *dsfel_result)
{
	sfel_node_t *node = NULL;

	sfpr_int_t ret = -1;
	#ifdef LOG_DEBUG	
	fprintf(stdout, "cb::getelementbyid id:%s\n", argv[0]);
	#endif	

	#if INNER_DEBUG
	dsfel_result->type = dsfel_result_string;
	//printf("string size:%d\n", dsfel_result->size);
	//memset(dsfel_result->string, 0x0, dsfel_result->size);
	strcpy(dsfel_result->string, argv[0]);
    //dsfel_result->type = dsfel_result_int;
	//dsfel_result->ret = 999;

	#else
	node = sfel_xml_node_find(doc, 0, NULL, "id", argv[0]);
	//ret = find_node_by_id(instance->root, argv[0], &node);
	//if(node)
	{
		#ifdef LOG_DEBUG		
		fprintf(stdout, "getelementbyid | debug | node:%p, name:%s, value:%s\n", node, sfel_xml_node_get_name(node), sfel_xml_node_get_value(node));
		#endif		
		dsfel_result->type = dsfel_result_pointer;
		dsfel_result->pointer = node;
	}

	#endif
	return ret;
}

static sfpr_int_t sendmessage(sfel_document_t *doc, sfpr_void_t *argv[],dsfel_handle_result_t *dsfel_result)
{
	#ifdef LOG_DEBUG	
	fprintf(stdout, "cb::sendmessage addr:%s\n", argv[0]);
	#endif
	dsfel_result->type = dsfel_result_string;
	strcpy(dsfel_result->string, argv[0]);
	return 0;
}

/**
 * @brief ����ű���Ϣ
 */
typedef struct
{
	sfpr_char_t name[DSFEL_BUFF_SIZE]; /**<����/������*/
	sfpr_void_t *param[SFEL_DSFEL_MAX_ARGV];/**<����*/
	sfpr_char_t attribute[DSFEL_BUFF_SIZE];/**<����*/
}dsfel_script_t;

static dsfel_t dsfel[64] = {
	{"getelementbyid",dsfel_function,getelementbyid,},
	{"sendmessage",dsfel_function,sendmessage,},
	{NULL,}
};

int dsfel_func_register(char *name, dsfel_cb_t cb)
{
	int i=0;
	while(dsfel[i].name != NULL)
	{
		if(strcmp(dsfel[i].name, name) == 0)
		{
			return -1;
		}
		i++;	
	}
	dsfel[i].name = name;
	dsfel[i].type = dsfel_function;
	dsfel[i].body.cb = cb;
	return 0;
}

/**
 * @brief ����ָ��key��ȡע��Ľű��������߱���
 */
static dsfel_t *dsfel_get(const sfpr_char_t *key)
{
	sfpr_int_t i=0;
	do
	{
		sfpr_int_t tmp = strcmp(dsfel[i].name, key);
		if(!tmp)
		{
			return &dsfel[i];
		}
		i++;
	}while(dsfel[i].name && strcmp(dsfel[i].name, ""));
	return NULL;
}

typedef enum
{
	dsfel_stack_data_func, /**<����ָ��*/
	dsfel_stack_data_pointer, /**<ָ�룬ʹ�ú���Ҫ�û��ͷ���Դ*/
	dsfel_stack_data_string /**<�ַ������û�ʹ��ǰ��ʹ�ú�Ҫ���䡢�ͷ��ڴ�*/
}dsfel_stack_type_t;

sfpr_int_t dsfel_compile(sfel_document_t *doc, const sfpr_char_t *script, sfpr_char_t *output,int outsz)
{
	sfpr_int_t ret = -1;
	sfpr_int_t i=0, j=0, k=0, n=0, m;
	sfpr_char_t symbol = 0;
	dsfel_t *inner = NULL;
	sfpr_stack_t *stack = NULL;
	sfpr_void_t *node = NULL;/*��ջ��ȡ�����û�����*/
	sfpr_int_t flag;/*ջ���û����ݵı��*/
	sfpr_int_t size;/*ջ���û�����Ϊstringʱ����С*/
	sfel_node_t *xml_node = NULL;
	sfpr_char_t *begin;//dsfel����������ʼ��ַ
	sfpr_char_t *left=NULL;//dsfel��������ʼ��ַ
	sfpr_char_t *right;//dsfel���в����Ľ�����ַ
	sfpr_char_t *end;//dsfel������������ַ���������������
	sfpr_char_t *param;//�������������У�����������Ч����
	sfpr_int_t multiFlg = 0;//1-��ǰΪdsfel���; 0-δ��ʼ�������һ��dsfel����
	const sfpr_int_t script_len = strlen(script)+3;
	sfpr_char_t buff[DSFEL_SCRIPT_SIZE];
	sfpr_void_t *argv[SFEL_DSFEL_MAX_ARGV];
	sfpr_void_t *argv_reverse[SFEL_DSFEL_MAX_ARGV];/*������ջ��ȡ����ʵ�ֵ���*/
	sfel_attr_t *attr = NULL;
	dsfel_handle_result_t dsfel_result;
	sfpr_char_t *result = NULL;/*�������ؽ�����ߴ�ջ��ȡ�����û�����*/
	sfpr_void_t *argv_malloc[SFEL_DSFEL_MAX_ARGV];/*��̬����Ĳ���*/
	memset(&argv_malloc, 0x0, sizeof(argv_malloc));
	memset(&dsfel_result, 0x0, sizeof(dsfel_result));
	dsfel_result.string = malloc(SFEL_DSFEL_MAX_RETURN_SIZE);
	dsfel_result.size = SFEL_DSFEL_MAX_RETURN_SIZE;
	memset(buff, 0x0, sizeof(buff));
	strcpy(buff, script);
	
    //��֤�Ƿ���Ч�ű�
	if(dsfel_isvalid(buff))
	{
		strcat(output, script);
		return -1;
	}

	sfpr_str_ltrim(buff);
	sfpr_str_rtrim(buff);

    //����ջ
	ret = sfpr_stack_create(&stack, 1024, sfpr_stack_mode_pointer);
	if(ret < 0){
		strncpy(output, script,outsz);
		return -1;
	}

    /*${getelementbyid(hello)}*/
	for(i=0; i< script_len; i++){
		switch(buff[i])
		{
			case '$':
			{
				multiFlg =1;
				break;
			}
			case '{':
			{
				if(multiFlg == 0)
					strncat(output, buff+i, 1);
				else
				{
					begin = buff+i+1;//��ȡ�ӿ����ƿ�ʼ��ַ
				}
				break;
			}
			case '(':
			{
				if(multiFlg == 0)
					strncat(output, buff+i, 1);
				else
				{
					*(buff+i) = '\0';
					sfpr_str_trim(begin);
					inner = dsfel_get(begin);
					if(!inner)
						goto END;
					//��ջ
					ret = sfpr_stack_push(stack, inner, 0, dsfel_stack_data_func);
					if(ret < 0)
					{
						#ifdef LOG_ERROR
						fprintf(stderr, "dsfel_handle | debug | sfpr_stack_push failed\n");
						#endif
						goto END;
					}
				}
				break;
			}
			case '\''://���Ϊ������
			{
				if(multiFlg == 0)
					strncat(output, buff+i, 1);
				else
				{
					if(!left)//������ʼ��ַ
					{
						left = buff+i;
						//��ѯ������������ջ
						*left = '\0';
						left += 1;
					}
					else//����������ַ
					{
						right = buff+i;
						*right = '\0';
						//������ջ
						if(!strchr(left, '$'))
						{
							sfpr_str_ltrim(left);
							sfpr_str_rtrim(left);
							//��ջ
							if(strlen(left))
							{
								ret = sfpr_stack_push(stack, left, 0, dsfel_stack_data_pointer);
								if(ret < 0)
								{
									#ifdef LOG_ERROR
									fprintf(stderr, "dsfel_handle | debug | sfpr_stack_push failed\n");
									#endif
									goto END;
								}
							}
						}
						left = NULL;
					}
				}
				break;
			}
			case ',':
			{
				if(multiFlg == 0)
					strncat(output, buff+i, 1);
				break;
			}
			case ')':
			{
				if(multiFlg == 0)
					strncat(output, buff+i, 1);
				else
				{
					param = buff+i;
					*param = '\0';
					param++;
					//ִ�к�������(����)
					memset(argv, 0x0, sizeof(argv));
					memset(argv_reverse, 0x0, sizeof(argv_reverse));
					j = 0;
					k = 0;
					size = 0;
					flag = -1;
					result = sfpr_stack_pop_pointer(stack, &size, &flag);
					while(flag != dsfel_stack_data_func)
					{
						if(flag == dsfel_stack_data_pointer)
						{
							argv[j++] = result;
						}
						else if(flag == dsfel_stack_data_string)
						{
							argv[j++] = result;
							argv_malloc[n++] = result;
						}
						size = 0;
						flag = -1;
						result = sfpr_stack_pop_pointer(stack, &size, &flag);
					}

					m = j;
					for(k=0; k< j; k++)
					{
						argv_reverse[k] = argv[--m];
					}
	                inner =(dsfel_t*)result;
					ret = 0;
					if(inner->body.cb)
					{
						inner->body.cb(doc, argv_reverse, &dsfel_result);

						//ִ�н����ջ
						if(dsfel_result.type == dsfel_result_int)
						{
							result = malloc(128);
							sprintf(result, "%d", dsfel_result.ret);
							ret = sfpr_stack_push(stack, result, 0, dsfel_stack_data_string);
						}
						else if(dsfel_result.type == dsfel_result_pointer)
						{
							ret = sfpr_stack_push(stack, dsfel_result.pointer, 0, dsfel_stack_data_pointer);
						}
						else if(dsfel_result.type == dsfel_result_string)
						{
							result = malloc(strlen(dsfel_result.string)+1);
							strcpy(result, dsfel_result.string);
							ret = sfpr_stack_push(stack, result, 0, dsfel_stack_data_string);
						}
					}
					
					//������Դ
					for(k=0; k<n; k++)
					{
						free(argv_malloc[k]);
					}
					
					if(ret < 0)
					{
						#ifdef LOG_ERROR
						fprintf(stderr, "dsfel_handle | debug | sfpr_stack_push failed\n");
						#endif
						goto END;
					}
				}
				break;
			}
			case '}':
			{
				if(multiFlg == 0)
					strncat(output, buff+i, 1);
				else
				{
					//�ж��Ƿ�������
					end = buff+i;
					*end = '\0';
					left = end+1;
					if(param = strchr(param, '.'))//��ȡ����
					{
						param++;
						sfpr_str_ltrim(param);
						sfpr_str_rtrim(param);
						//��ջ
						size = 0;
						flag = -1;
						result = sfpr_stack_pop_pointer(stack, &size, &flag);
						xml_node= (sfel_node_t*)result;
						//��ȡ����
						attr = sfel_xml_attr_get(xml_node, param);
						if(attr)
						{
							//���ջ��ֻ��һ������Ϊִ�н��
							if(sfpr_stack_get_count(stack) > 1)
							{
								ret = sfpr_stack_push(stack, sfel_xml_attr_get_avalue(attr), 0, dsfel_stack_data_pointer);
								if(ret < 0)
								{
									#ifdef LOG_ERROR
									fprintf(stderr, "dsfel_handle | debug | sfpr_stack_push failed\n");
									#endif
									goto END;
								}
							}
							else
							{
								//����
								strcat(output, sfel_xml_attr_get_avalue(attr));
								multiFlg = 0;
								//goto END;
							}
						}
						else
						{
							ret = -1;
							#ifdef LOG_ERROR
							fprintf(stderr, "dsfel_handle | debug | sfel_xml_attr_get_by_name failed\n");
							#endif
						}
					}
					else
					{
						//���ջ��ֻ��һ������Ϊִ�н��
						if(1 == sfpr_stack_get_count(stack))
						{
							size = 0;
							flag = -1;
							result = sfpr_stack_pop_pointer(stack, &size, &flag);
							strcat(output, result);
							if(flag == dsfel_stack_data_string)
							{
								free(result);
							}
							multiFlg = 0;
							begin = NULL;//dsfel����������ʼ��ַ
							left=NULL;//dsfel��������ʼ��ַ
							right = NULL;//dsfel���в����Ľ�����ַ
							end = NULL;//dsfel������������ַ���������������
							param = NULL;//�������������У�����������Ч����
						}
					}
				}
				break;
			}
			case '\0':
			{
				goto END;
			}
			default:
			{
				if(multiFlg == 0)
					strncat(output, buff+i, 1);
				break;
				
			}
		}
	}

	END:
    //����ջ
	sfpr_stack_destroy(&stack);
	free(dsfel_result.string);
	return ret;
}

sfpr_int_t dsfel_isvalid(const sfpr_char_t *script)
{
    //��֤�Ƿ���Ч�ű�
	if(!strchr(script, '$') || !strchr(script, '{') ||!strchr(script, '}') || !strchr(script, '(') ||!strchr(script, ')'))
	{
		return -1;
	}
	return 0;
}

#ifdef __cplusplus
}
#endif
