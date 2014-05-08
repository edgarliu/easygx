#include <stdio.h>
#include <string.h>

#include "sfpr_queue.h"
#include "sfpr_string.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\file sfpr_queue.c
 *\brief SFPR库的queue 模块
 *\author bijing
 *\version 0.1.0
 *\date 2012-02-06
 */

struct sfpr_queue_node_s{
	unsigned long flag;
	void *data;                       /**< 队列节点存储的数据 */
	int level;												/**< 数据的优先级 */
	struct sfpr_queue_node_s *next;  /**<  指向队列 的下一个数据节点*/
};

/**
 *\ingroup queue_struct
 *\brief 队列数据操作节点的描述
 */
typedef struct sfpr_queue_operator_s{
	sfpr_queue_node_t *node;    /**< 指向队列的数据节点  */
	sfpr_mutex_t		lock;    /**< 节点操作互斥锁  */
}sfpr_queue_operator_t;

struct sfpr_queue_s{
	unsigned long flag;
	int length;   /**< 队列最大长度  */
	int count;/**<   队列当前长度*/
	sfpr_queue_operator_t header;   /**< 头结点指针  */
	sfpr_queue_operator_t tailer;     /**<   尾节点指针*/
	sfpr_cond_t  not_empty_cond;   /**<   队列操作信号量*/
	sfpr_queue_node_t *node;/**< 结点缓冲区  */
	sfpr_queue_node_t *tail_node[SFPR_QUEUE_MAX_LEVEL];   /**< 各个级别的尾结点指针  */
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

  /**< 创建互斥锁和条件变量  */
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

	/**< 当队列为空时，向队尾插入数据，需要将对列头指针也指向该节点*/
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
		/**< 当队列不为空时，直接向队尾插入节点*/
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

	//level 的范围是1--8
	level = (level<0)?0:level;
	level = (level>SFPR_QUEUE_MAX_LEVEL-1)?(SFPR_QUEUE_MAX_LEVEL-1):level;
	level = (level == 0)?8:level;
	//offset 的范围是0--7
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

	/**< 当队列为空时，向队尾插入数据，需要将对列头指针也指向该节点*/
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
	/**< 队列不为空*/
	else
	{
		/**< 本级别还没有数据*/
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
 *\brief	从当前队列取出一个可用的对象，同时将取出的对象从队列中删除，
 *如果当前队列为空，则挂起直到有新的对象存入。
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
	/**<当队列为空时，等待队列数据插入后的信号量通知 */
	while (NULL == queue->header.node) 
	{
		sfpr_cond_wait(&queue->not_empty_cond,&queue->header.lock);
	}
	node =queue->header.node;

  /**<当队列只有一个数据时，需要考虑头指针和尾指针互斥操作 */	
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
	/**<当队列多于一个节点，直接从队列头去数据*/
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

/*判断当前队列的数据是否为空，true表示空，flase为非空*/
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

/*清空队列中的对象*/
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

