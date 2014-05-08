#include <stdio.h>
#include <string.h>

#include "sfpr_queue.h"
#include "sfpr_string.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\file sfpr_queue.c
 *\brief SFPR���queue ģ��
 *\author bijing
 *\version 0.1.0
 *\date 2012-02-06
 */

struct sfpr_queue_node_s{
	unsigned long flag;
	void *data;                       /**< ���нڵ�洢������ */
	int level;												/**< ���ݵ����ȼ� */
	struct sfpr_queue_node_s *next;  /**<  ָ����� ����һ�����ݽڵ�*/
};

/**
 *\ingroup queue_struct
 *\brief �������ݲ����ڵ������
 */
typedef struct sfpr_queue_operator_s{
	sfpr_queue_node_t *node;    /**< ָ����е����ݽڵ�  */
	sfpr_mutex_t		lock;    /**< �ڵ����������  */
}sfpr_queue_operator_t;

struct sfpr_queue_s{
	unsigned long flag;
	int length;   /**< ������󳤶�  */
	int count;/**<   ���е�ǰ����*/
	sfpr_queue_operator_t header;   /**< ͷ���ָ��  */
	sfpr_queue_operator_t tailer;     /**<   β�ڵ�ָ��*/
	sfpr_cond_t  not_empty_cond;   /**<   ���в����ź���*/
	sfpr_queue_node_t *node;/**< ��㻺����  */
	sfpr_queue_node_t *tail_node[SFPR_QUEUE_MAX_LEVEL];   /**< ���������β���ָ��  */
	int tailer_level;
};


sfpr_queue_t  *sfpr_queue_create(void)
{ 
	sfpr_queue_t *queue;  
	
	#ifdef LOG_DEBUG
	printf("sfpr_queue_create() | debug | create a new queue\n");
	#endif
	queue = (sfpr_queue_t *)malloc(sizeof(sfpr_queue_t));   
	if (NULL== queue)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_create() | error | malloc queue failed!\n");
		#endif
		return NULL;
	}
	memset(queue,0,sizeof(sfpr_queue_t));
	queue->count = 0;
	return queue ; 
}

int sfpr_queue_destroy(sfpr_queue_t *queue)
{
	if(NULL==queue)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_destroy() | error | queue is NULL !\n");
		#endif		
		return SFPR_ERROR;
	}
	#ifdef LOG_DEBUG
	printf("sfpr_queue_destroy() | debug | \n");
	#endif

	if(queue->flag & SFPR_QUEUE_INIT_OK)
	{
		sfpr_queue_clear(queue);
		sfpr_cond_destroy(&queue->not_empty_cond);  
		sfpr_mutex_destroy(&queue->header.lock);   
		sfpr_mutex_destroy(&queue->tailer.lock);
		free(queue->node); 
	}
	free(queue);

	return SFPR_SUCCESS;
}

int sfpr_queue_init(sfpr_queue_t *queue)
{
	if(NULL==queue)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_init() | error | queue is NULL !\n");
		#endif
		return SFPR_ERROR;
	}

	#ifdef LOG_DEBUG
	printf("sfpr_queue_init() | debug | init a queue\n");
	#endif

	queue->header.node = NULL;
	queue->tailer.node  = NULL;
	
	if(queue->length <= 0){
		queue->length = SFPR_DEFAULT_QUEUE_LENGTH;
	}
	queue->length = (queue->length>SFPR_MAX_QUEUE_LENGTH)?SFPR_MAX_QUEUE_LENGTH:(queue->length);

	queue->node = (sfpr_queue_node_t*)malloc(queue->length * sizeof(sfpr_queue_node_t));
	if(!queue->node)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_init() | error | malloc queue node failed!\n");
		#endif
		return SFPR_ERROR;
	}
	memset(queue->node,0,queue->length * sizeof(sfpr_queue_node_t));

  /**< ��������������������  */
	sfpr_cond_create(&queue->not_empty_cond);
	sfpr_mutex_create(&queue->header.lock,0);
	sfpr_mutex_create(&queue->tailer.lock,0);

	queue->flag = SFPR_QUEUE_INIT_OK;

	return SFPR_SUCCESS;
}

int sfpr_queue_put(sfpr_queue_t *queue,void *data)
{
	sfpr_queue_node_t  *node ;
	int i;

	if (NULL == queue)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_put() | error | queue is NULL !\n");
		#endif
		return SFPR_ERROR;
	}
	#ifdef LOG_DEBUG
	sfpr_str_hex_print("sfpr_queue_put() | put data",data,16);
	#endif

	sfpr_mutex_lock(&queue->tailer.lock);
	for(i=0;i<queue->length;i++)
	{
		if(!(queue->node[i].flag & SFPR_QUEUE_NODE_USED))
		{
			node = &(queue->node[i]);
			break;
		}
	}
	if(i == queue->length)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_put() | error | queue is overflow!\n");
		#endif
		sfpr_mutex_unlock(&queue->tailer.lock);
		return SFPR_ERROR;
	}
	memset(node,0,sizeof(sfpr_queue_node_t));
	node->data = data;
	node->flag = SFPR_QUEUE_NODE_USED;
	node->next =NULL;

	/**< ������Ϊ��ʱ�����β�������ݣ���Ҫ������ͷָ��Ҳָ��ýڵ�*/
	if (NULL == queue->tailer.node)
	{
		queue->tailer.node = node;
		sfpr_mutex_unlock(&queue->tailer.lock);
		
		sfpr_mutex_lock(&queue->header.lock);
		queue->header.node = node;
		queue->count++;
		sfpr_cond_signal_all(&queue->not_empty_cond);
		sfpr_mutex_unlock(&queue->header.lock);
	}
	else
	{
		/**< �����в�Ϊ��ʱ��ֱ�����β����ڵ�*/
		queue->tailer.node->next = node;
		queue->tailer.node = node;
		queue->count++;
		sfpr_mutex_unlock(&queue->tailer.lock);
	}
	return SFPR_SUCCESS ;
}

static sfpr_queue_node_t *get_prep_tail(sfpr_queue_t *queue,int offset)
{
	int i = offset - 1;
	while(i>=0){
		if(queue->tail_node[i]){
			return queue->tail_node[i];
		}
		i--;
	}
	return NULL;
}

int sfpr_queue_put_with_level(sfpr_queue_t *queue,void *data,int level)
{
	sfpr_queue_node_t  *node = NULL,*prep = NULL;
	int i,offset = 0;

	if (NULL == queue){
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_put() | error | queue is NULL !\n");
		#endif
		return SFPR_ERROR;
	}

	//level �ķ�Χ��1--8
	level = (level<0)?0:level;
	level = (level>SFPR_QUEUE_MAX_LEVEL-1)?(SFPR_QUEUE_MAX_LEVEL-1):level;
	level = (level == 0)?8:level;
	//offset �ķ�Χ��0--7
	offset = level - 1;

	#ifdef LOG_NOTICE
	fprintf(stderr,"sfpr_queue_put() | node level:%d\n",level);
	#endif

	sfpr_mutex_lock(&queue->tailer.lock);
	for(i=0;i<queue->length;i++){
		if(!(queue->node[i].flag & SFPR_QUEUE_NODE_USED)){
			node = &(queue->node[i]);
			break;
		}
	}
	if(i == queue->length){
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_put() | error | queue is overflow!\n");
		#endif
		sfpr_mutex_unlock(&queue->tailer.lock);
		return SFPR_ERROR;
	}
	memset(node,0,sizeof(sfpr_queue_node_t));
	node->data = data;
	node->flag = SFPR_QUEUE_NODE_USED;
	node->next = NULL;

	/**< ������Ϊ��ʱ�����β�������ݣ���Ҫ������ͷָ��Ҳָ��ýڵ�*/
	if (NULL == queue->tailer.node)
	{
		queue->tailer.node = node;
		queue->tail_node[offset] = node;
		queue->tailer_level = offset;
		sfpr_mutex_unlock(&queue->tailer.lock);

		sfpr_mutex_lock(&queue->header.lock);
		queue->header.node = node;
		queue->count++;
		sfpr_cond_signal_all(&queue->not_empty_cond);
		sfpr_mutex_unlock(&queue->header.lock);
	}
	/**< ���в�Ϊ��*/
	else
	{
		/**< ������û������*/
		if(!queue->tail_node[offset])
		{
			queue->tail_node[offset] = node;
			prep = get_prep_tail(queue,offset);
			if(!prep){
				sfpr_mutex_lock(&queue->header.lock);
				node->next = queue->header.node;
				queue->header.node = queue->tail_node[offset];
				sfpr_mutex_unlock(&queue->header.lock);
			}else{
				node->next = prep->next;
				prep->next = queue->tail_node[offset];
			}
		}
		else
		{
			node->next = queue->tail_node[offset]->next;
			queue->tail_node[offset]->next = node;
			queue->tail_node[offset] = node;
		}

		if(offset >= queue->tailer_level)
		{
			queue->tailer.node->next = node;
			queue->tailer.node = node;
			queue->tailer_level = offset;
		}
		queue->count++;
		sfpr_mutex_unlock(&queue->tailer.lock);
	}
	return SFPR_SUCCESS;
}


/**
 *\brief	�ӵ�ǰ����ȡ��һ�����õĶ���ͬʱ��ȡ���Ķ���Ӷ�����ɾ����
 *�����ǰ����Ϊ�գ������ֱ�����µĶ�����롣
 */
void *sfpr_queue_get(sfpr_queue_t *queue)
{
	void *data = NULL;
	sfpr_queue_node_t *node = NULL;
	
	if(NULL== queue)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_get() | error | queue is NULL !\n");
		#endif
		return NULL;
	}

	sfpr_mutex_lock(&queue->header.lock);
	/**<������Ϊ��ʱ���ȴ��������ݲ������ź���֪ͨ */
	while (NULL == queue->header.node) 
	{
		sfpr_cond_wait(&queue->not_empty_cond,&queue->header.lock);
	}
	node =queue->header.node;

  /**<������ֻ��һ������ʱ����Ҫ����ͷָ���βָ�뻥����� */	
	if (node == queue->tailer.node)
	{
		queue->header.node = queue->header.node->next;
		sfpr_mutex_lock(&queue->tailer.lock);
		if(node == queue->tailer.node)
		{
			#ifdef LOG_DEBUG
			fprintf(stderr,"sfpr_queue_get() | debug | GETGET (tmpnode == queue->tailer.node) \n");
      #endif
			queue->tailer.node = NULL;
		}
		sfpr_mutex_unlock(&queue->tailer.lock);
	} 
	/**<�����ж���һ���ڵ㣬ֱ�ӴӶ���ͷȥ����*/
	else 
	{
		queue->header.node = queue->header.node->next;
	}
	
	data = node->data;
	node->flag = 0;
	queue->count--;
	sfpr_mutex_unlock(&queue->header.lock);
	return data;
}

int sfpr_queue_size(sfpr_queue_t *queue)
{
	if (NULL != queue)
	{
		return queue->count;
	}
	return -1;
}

/*�жϵ�ǰ���е������Ƿ�Ϊ�գ�true��ʾ�գ�flaseΪ�ǿ�*/
int sfpr_queue_is_empty(sfpr_queue_t *queue)
{
	int isEmpty;

	if (NULL != queue->tailer.node)
		return SFPR_FALSE;
	
	sfpr_mutex_lock(&queue->header.lock);
	sfpr_mutex_lock(&queue->tailer.lock);
	
	isEmpty = (NULL ==queue->tailer.node);
	
	sfpr_mutex_unlock(&queue->tailer.lock);
	sfpr_mutex_unlock(&queue->header.lock);
	#ifdef LOG_DEBUG
	fprintf(stderr,"sfpr_queue_is_empty() | debug | isEmpty =%d \n",isEmpty);
	#endif		

	return isEmpty;
 }

/*��ն����еĶ���*/
int sfpr_queue_clear(sfpr_queue_t *queue)
{
	sfpr_queue_node_t *node = NULL;
	if(NULL== queue)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_get() | error | queue is NULL !\n");
		#endif
		return -1;
	}
        
	sfpr_mutex_lock(&queue->header.lock);
	sfpr_mutex_lock(&queue->tailer.lock);
	while (NULL != queue->header.node) 
	{
		node = queue->header.node;
		queue->header.node = node->next;
		memset(node,0,sizeof(sfpr_queue_node_t));
	}
	queue->tailer.node= NULL;
	queue->count = 0;
	sfpr_mutex_unlock(&queue->tailer.lock);
	sfpr_mutex_unlock(&queue->header.lock);
	return SFPR_SUCCESS;
}

int sfpr_queue_enum(sfpr_queue_t *queue)
{
	char buffer[256];
	sfpr_queue_node_t *node = NULL;
	if(NULL== queue)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_queue_get() | error | queue is NULL !\n");
		#endif
		return -1;
	}
	sfpr_mutex_lock(&queue->header.lock);
	sfpr_mutex_lock(&queue->tailer.lock);
	node = queue->header.node;
	while (NULL != node) 
	{
		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,"sfpr_queue_enum() | node level:%d",node->level);
		sfpr_str_hex_print(buffer,node->data,16);
		node = node->next;
		sfpr_time_msleep(1000);
	}
	sfpr_mutex_unlock(&queue->tailer.lock);
	sfpr_mutex_unlock(&queue->header.lock);
	return SFPR_SUCCESS;
}

#ifdef __cplusplus
}
#endif

