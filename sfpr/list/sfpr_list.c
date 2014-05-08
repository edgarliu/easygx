/**
 * @file sfpr_list.c
 * @brief sfpr库中链表的操作
 * @author dulh
 * @date 20120207
 */
#include <sfpr_list.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sfpr_list_node_s{
	void *data;					/**< 链表节点存储的数据 */
	struct sfpr_list_node_s *prep;			/**<  指向链表的前一个节点的指针*/
	struct sfpr_list_node_s *next;			/**<  指向链表的下一个节点的指针*/
};

struct sfpr_list_s{
	int double_list;				/**< 是否是双向链表  */
	sfpr_list_node_t *head;		/**< 头结点指针  */
	sfpr_list_node_t *tail;		/**< 尾节点指针 */
	int count;			/**< 链表节点的个数 */
	sfpr_mem_pool_t  *pool;	
	sfpr_mutex_t mutex;		/**< 互斥锁 */
};

sfpr_list_t *sfpr_list_new(int double_list,sfpr_mem_keeper_t *mem_keeper)
{
	sfpr_mem_pool_t *pool = NULL;
	sfpr_list_t *list = NULL;
	if(mem_keeper){
		pool = sfpr_mem_pool_new(mem_keeper,"sfpr_list");
	}
	if(pool){
		list = (sfpr_list_t *)sfpr_mem_malloc(pool,sizeof(sfpr_list_t));
	}else{
		list = (sfpr_list_t *)malloc(sizeof(sfpr_list_t));		/**< 链表结构*/
	}
	if(list == NULL){
		return NULL;
	}
	memset(list,0,sizeof(sfpr_list_t));
	list->count = 0;
	if(double_list){
		list->double_list = 1;
	}
	list->pool = pool;
	sfpr_mutex_create(&list->mutex, 0);		/**< 创建互斥锁*/
	return list;
}

static void list_clear_cb_default(void *data,void *param)
{
}

int sfpr_list_clear(sfpr_list_t *list,void(*clear_cb)(void *data,void *param),void *param)
{
	sfpr_list_node_t *p,*q;
	if(!clear_cb){
		clear_cb = list_clear_cb_default;
	}
	
 	p = list->head;  
	while(p != NULL)  
	{  
		q=p->next;  /**< 指针后移*/	
		clear_cb(p->data,param);
		if(list->pool){
			sfpr_mem_free(list->pool,p);
		}else{
			free(p);	/**< 释放当前节点*/
		}
		p = q;
		if(p){
			p->prep = NULL;
		}
		list->count--;	/**< 节点计数器减1*/
	}
	list->head = NULL;
	list->tail = NULL;
	
	return SFPR_SUCCESS;
}


int sfpr_list_destroy(sfpr_list_t *list)
{
	sfpr_mem_pool_t *pool = list->pool;
	sfpr_mutex_lock(&list->mutex);
	sfpr_list_clear(list,NULL,NULL);		/**< 清空链表*/
	sfpr_mutex_unlock(&list->mutex);
	sfpr_mutex_destroy(&list->mutex);	/**< 销毁互斥锁*/
	if(pool){
		sfpr_mem_free(pool,list);/**< 释放链表空间*/
//		sfpr_mem_pool_delete(NULL,pool);
	}else{
		free(list);	/**< 释放链表空间*/
	}
	return SFPR_SUCCESS;
}


sfpr_list_node_t *sfpr_list_append(sfpr_list_t *list, void *data)
{
	int len = 0;
	sfpr_list_node_t *node = NULL;
	
	if(list->pool){
		node = (sfpr_list_node_t *)sfpr_mem_malloc(list->pool,sizeof(sfpr_list_node_t));	/**< 申请链表节点存放数据*/		
	}else{
		node = (sfpr_list_node_t *)malloc(sizeof(sfpr_list_node_t));	/**< 申请链表节点存放数据*/
	}
	if (!node)
	{
		return NULL;
	}
	node->data = data;	
	sfpr_mutex_lock(&list->mutex);	
	if (list->head == NULL)			/**< 链表为空，插入表头*/
	{
		list->head = node;		
		list->tail = node;			
	}
	else
	{
		list->tail->next = node; 
		list->tail = list->tail->next;		
	}
	list->count++;
	list->tail->next = NULL;
	sfpr_mutex_unlock(&list->mutex);
	return node;
}

sfpr_list_node_t *sfpr_list_append_with_cb(sfpr_list_t *list,void(*append_cb)(void*data, void* param),void* param,void *data)
{
	int len = 0;
	sfpr_list_node_t *node = NULL;
	
	if(list->pool){
		node = (sfpr_list_node_t *)sfpr_mem_malloc(list->pool,sizeof(sfpr_list_node_t));	/**< 申请链表节点存放数据*/		
	}else{
		node = (sfpr_list_node_t *)malloc(sizeof(sfpr_list_node_t));	/**< 申请链表节点存放数据*/
	}
	if (!node)
	{
		fprintf(stderr,"malloc failed!");
		return NULL;
	}
	node->data = data;	
	sfpr_mutex_lock(&list->mutex);	
	if (list->head == NULL) 		/**< 链表为空，插入表头*/
	{
		list->head = node;		
		list->tail = node;			
	}
	else
	{
		list->tail->next = node; 
		list->tail = list->tail->next;		
	}
	if(append_cb)
		append_cb(data, param);
	list->count++;
	list->tail->next = NULL;
	sfpr_mutex_unlock(&list->mutex);
	return node;	
}

sfpr_list_node_t *sfpr_list_prepend(sfpr_list_t *list,void *data)
{
	int len = 0;
	sfpr_list_node_t *node = NULL;
	
	if(list->pool){
		node = (sfpr_list_node_t *)sfpr_mem_malloc(list->pool,sizeof(sfpr_list_node_t));	/**< 申请链表节点存放数据*/		
	}else{
		node = (sfpr_list_node_t *)malloc(sizeof(sfpr_list_node_t));	/**< 申请链表节点存放数据*/
	}
	if (!node)
	{
		fprintf(stderr,"malloc failed!");
		return NULL;
	}
	node->data = data;	
	sfpr_mutex_lock(&list->mutex);	
	if (list->head == NULL)			/**< 链表为空，插入表头*/
	{
		list->head = node;		
		list->tail = node;			
	}
	else
	{
		node->next = list->head;
		list->head->prep = node;
		list->head = node;		
	}
	list->count++;
	list->head->prep = NULL;
	sfpr_mutex_unlock(&list->mutex);
	return node;	
}


static int list_cmp_cb_default(void  *src, void *cmp_param)
{
	int res =0;

	if(strcmp(src , cmp_param) == 0)
		return res;
	return -1;
}

static void list_delete_cb_default(void  *data, void *delete_param)
{

}

int sfpr_list_delete(sfpr_list_t *list, int(*cmp_cb)(void  *src, void *cmp_param), void *cmp_param,
											void(*delete_cb)(void *data,void *delete_param),void *delete_param)
{
	sfpr_list_node_t *q ,*p;

	if(list->head == NULL)
	{
		return SFPR_ERROR;
	}
	sfpr_mutex_lock(&list->mutex);
	
	if(!cmp_cb){
		cmp_cb = list_cmp_cb_default;
	}
	if(!delete_cb){
		delete_cb = list_delete_cb_default;
	}
	if(cmp_cb(list->head->data, cmp_param) == 0)		/**< 要删除的数据为头结点数据*/
    {		
            p = list->head;
			list->head = list->head->next;
			
			delete_cb(p->data, delete_param);
			if(list->pool){
				sfpr_mem_free(list->pool,p);
			}else{
				free(p);
			}
			list->count--;
          	sfpr_mutex_unlock(&list->mutex);	
			return SFPR_SUCCESS;
    }
	
	p = list->head;	
	q = list->head->next;
	while(q != list->tail->next)				/**< 否则*/
	{
		if(cmp_cb(q->data, cmp_param) == 0)
        {
                q = p->next;
                p->next = q->next;
				delete_cb(p->data, delete_param);
				if(list->pool){
					sfpr_mem_free(list->pool,q);
				}else{
					free(q);
				}
                list->count--;
                break;
        }
        else
        {
                p = p->next;
                q = q->next;
        }
        if(q == NULL)
        {
                sfpr_mutex_unlock(&list->mutex);
                return SFPR_ERROR;
        }		
	}
	sfpr_mutex_unlock(&list->mutex);
	return SFPR_SUCCESS;
}


int sfpr_list_count(sfpr_list_t *list)
{
	return list->count;
}

void* sfpr_list_index(sfpr_list_t* list,int index)
{
	int _index = 0;
	sfpr_list_node_t* node = list->head;
	sfpr_mutex_lock(&list->mutex);
	while(node)
	{		
		if(_index == index){
			sfpr_mutex_unlock(&list->mutex);
			return node->data;
		}
		node = node->next;
		_index++;
	}
	sfpr_mutex_unlock(&list->mutex);
	return NULL;	
}

sfpr_list_node_t *sfpr_list_get_head(sfpr_list_t *list)
{
	if (!list){
        return NULL;
    }
	if (list->count <= 0){
		return NULL;
	}
	return list->head;	
}

sfpr_list_node_t *sfpr_list_get_tail(sfpr_list_t *list)
{
	if (!list){
        return NULL;
    }
	if (list->count <= 0){
		return NULL;
	}
	return list->tail;		
}

sfpr_list_node_t *sfpr_list_get_next(sfpr_list_node_t *node)
{
	if (!node){
        return NULL;
    }
	return node->next;
}

sfpr_list_node_t *sfpr_list_get_prep(sfpr_list_node_t *node)
{
	if (!node){
        return NULL;
    }
	return node->prep;	
}

void* sfpr_list_node_get_data(sfpr_list_node_t *node)
{
	if (!node){
        return NULL;
    }
	return node->data;	
}

void* sfpr_list_find(sfpr_list_t* list,int(*cmp_cb)(void* data,void* param),void* param)
{
	sfpr_list_node_t* node = list->head;
	sfpr_mutex_lock(&list->mutex);
	while(node)
	{
		if(cmp_cb(node->data,param) == 0){
			sfpr_mutex_unlock(&list->mutex);
			return node->data;
		}
		node = node->next;
	}
	sfpr_mutex_unlock(&list->mutex);
	return NULL;
}

int sfpr_list_find_index(sfpr_list_t* list,int index, int(*handle_cb)(void *data,int index,void *param),void *param)
{
	int _index = 0;
	int ret = 0;
	sfpr_list_node_t* node = list->head;
	
	sfpr_mutex_lock(&list->mutex);
	while(node)
	{
		if(_index == index)
		{
			break;
		}
		node = node->next;
		_index++;
	}
	if(NULL == node){
		sfpr_mutex_unlock(&list->mutex);
		return -1;
	}
	if(handle_cb)
		ret = handle_cb(node->data,_index,param);	
	sfpr_mutex_unlock(&list->mutex);
	return ret;
}

int sfpr_list_enum_count(sfpr_list_t* list,int startIndex,int count,void(*handle_cb)(void *data,int index,void *param),void *param)
{
	int _count = 0;
	int _index = 0;
	sfpr_list_node_t* node = list->head;
	sfpr_mutex_lock(&list->mutex);
	while(node)
	{
		if(_index == startIndex)
		{
			break;
		}
		node = node->next;
		_index++;
	}
	if(NULL == node){
		sfpr_mutex_unlock(&list->mutex);
		return;
	}
	while(node && (_count < count))
	{
		handle_cb(node->data,_count++,param);	
		node = node->next;
	}
	sfpr_mutex_unlock(&list->mutex);
	return _count;
}

static void list_all_cb_default(void *data,void *param)
{
	printf("data:%s\n", data);
}

int sfpr_list_all(sfpr_list_t *list, void(*handle_cb)(void *data,void *param),void *param)
{
	sfpr_list_node_t *q =NULL;
	
	q = list->head;
	if(q == NULL)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_list_all() | error | sfpr_list is empty\n");
		#endif
		return SFPR_ERROR;
	}
	sfpr_mutex_lock(&list->mutex);
	if(!handle_cb)
	{
		handle_cb = list_all_cb_default;
	}
	
	while(q != NULL)
	{
		handle_cb(q->data,param);
		q = q->next;
	}
	
	sfpr_mutex_unlock(&list->mutex);

	return SFPR_SUCCESS;
}

int sfpr_list_reverse_all(sfpr_list_t *list, void(*handle_cb)(void *data,void *param),void *param)
{
	sfpr_list_node_t *q =NULL;
	
	q = list->tail;
	if(q == NULL)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_dlist_all() | error | sfpr_dlist is empty\n");
		#endif
		return SFPR_ERROR;
	}
	sfpr_mutex_lock(&list->mutex);
	if(!handle_cb)
	{
		handle_cb = list_all_cb_default;
	}
	
	while(q != NULL)
	{
		handle_cb(q->data,param);
		q = q->prep;
	}
	
	sfpr_mutex_unlock(&list->mutex);

	return SFPR_SUCCESS;
}

#ifdef __cplusplus
}
#endif
