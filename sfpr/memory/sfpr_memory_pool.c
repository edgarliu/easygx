
#include "sfpr_memory_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\ingroup memory_pool_struct
 * @brief 内存节点
 */
typedef struct sfpr_mem_node_s sfpr_mem_node_t;

/**
 *\ ingroup memory_pool_struct
 *\ brief 内存块中的内存粒子，内存池中的最小内存单元
 */
typedef struct sfpr_atomic_s
{
	sfpr_uint32_t 	addr; 					/**<该内存粒子的地址*/
	sfpr_uint32_t 	size:24,				/**<该内存粒子的大小*/
			slot:4,							/**<该内存粒子所属内存槽级别*/
			atomic:3,						/**<该内存粒子序号*/
			effeced:1;						/**<该内存粒子是否被分配 1:被使用 0:未被使用*/
	sfpr_mem_node_t	*memnode;				/**<该内存粒子所属内存块*/
	struct sfpr_atomic_s *next_idle;		/**<指向下一个内存粒子*/
	struct sfpr_atomic_s *prep_idle;		/**<指向前一个内存粒子*/
}sfpr_atomic_t;


/**
 *\ ingroup memory_pool_struct
 *\brief 一个内存块的最大内存粒子个数
 */
#define MAX_ATOMIC		8

/**
 *\ingroup memory_pool_struct
 *\brief 内存池中的内存节点
 *\remark 每个内存块包括多个内存粒子
 */
struct sfpr_mem_node_s 
{
	sfpr_uint32_t   				size;           /**<内存节点内存大小*/
	sfpr_atomic_t						atomic[MAX_ATOMIC];/**<内存粒子集合*/
	struct sfpr_mem_node_s 	**ref;    /**< reference to self */
	struct sfpr_mem_node_s 	*next;    /**< next memnode */
};

/**
 *\ ingroup memory_pool_struct
 * @brief 已经分配的内存块
 * @remark 该结构区别与sfpr_mem_node_t，指已经分配的内存
 */
typedef struct sfpr_mem_block_s
{
	sfpr_uint32_t 	addr;/**<内存块的地址*/
	sfpr_uint32_t 	size:24,/**<内存块大小*/
									slot:4,/**<内存块所属的内存槽*/
									atomic:3,/**<内存粒子*/
									effected:1;/**<内存被使用标记 1:被使用 0:未被使用*/
	sfpr_mem_node_t	*memnode;/**<所属内存节点*/
}sfpr_mem_block_t;

struct sfpr_mem_pool_s
{
	sfpr_char_t 							name[128];/**<内存池名称*/
	sfpr_mutex_t 							mutux;/**<互斥锁*/
	sfpr_mem_node_t 	*slot[SFPR_MEM_POOL_MAX_SLOT];/**<内存槽*/
	sfpr_atomic_t			*idle_head;/**<该内存原子的地址*/
	sfpr_atomic_t			*idle_tail;/**<该内存原子的地址*/

	sfpr_mem_block_t 	*alloced; /**<已经分配的内存块集合*/
	sfpr_uint32_t 		alloced_max_num;/**<允许分配的最大内存块个数*/
	sfpr_uint32_t			alloced_count_num;/**<当前已经分配的内存块个数*/

	struct sfpr_mem_pool_s *next;/**<该内存原子的地址*/
};

struct sfpr_mem_keeper_s
{
	sfpr_mem_pool_t *header;/**<指向第一个内存池*/
	sfpr_mem_pool_t *tailer;/**<指向最后一个内存池*/
};


static sfpr_void_t debug_memnode_traverse(sfpr_mem_pool_t* pool)
{
	sfpr_atomic_t* tmp = NULL;
	int total = 0;
	printf("\n------->idle list\n");
	//遍历
	tmp = pool->idle_head;
	while(tmp)
	{
		printf("slot:%1d, node:%10p, atomic:%1d, addr:%10p, size:%6d, status:%6s\n",tmp->slot, tmp->memnode, tmp->atomic, tmp->addr, tmp->size, tmp->effeced ? "used" : "unused");
		tmp = tmp->next_idle;
		total++;
	}
	printf("<-------idle list, total num:%d\n", total);

}

static sfpr_void_t debug_allocted_traverse(sfpr_mem_pool_t* pool)
{
	int index;
	int total = 0;
	printf("\n------->alocted table\n");
	//遍历
	for(index = 0; index < SFPR_MEM_POOL_MAX_ALLOCED_NUMBER; index++)
	{
		if(pool->alloced[index].addr != 0)
		{
			printf("slot:%1d, node:%10p, atomic:%1d, addr:%10p, size:%6d, status:%6s\n",pool->alloced[index].slot, pool->alloced[index].memnode, pool->alloced[index].atomic, pool->alloced[index].addr, pool->alloced[index].size, pool->alloced[index].effected ? "used" : "unused");
			total ++;
		}
	}
	printf("<-------alocted table, total num:%d\n", total);

}


static sfpr_int_t pool_atomic_list_init(sfpr_mem_pool_t *pool)
{
	pool->idle_head = NULL;
	pool->idle_tail = NULL;
	return 0;
}

/**
 * @brief 判断idle链表中最大的节点(第一次)是否满足条件，是则返回
 */
static sfpr_atomic_t * pool_atomic_list_get(sfpr_mem_pool_t *pool, sfpr_int_t size)
{
	sfpr_atomic_t *tmp = NULL;
	if (!pool->idle_head)
		return NULL;

	tmp = pool->idle_head;

	
	//遍历,找到合适的位置
	while (tmp)
	{
		if(tmp->size < size)
		{
			break;
		}
		tmp = tmp->next_idle;
	}

	//如果tmp为NULL, 则tail即为可用节点
	if (tmp)
	{
		return tmp->prep_idle;
	}
	else
	{
		return pool->idle_tail;
	}
			
}

/**
 * @brief 将指定内存粒子添加到idle链表中
 */
static sfpr_int_t pool_atomic_list_insert(sfpr_mem_pool_t *pool, sfpr_atomic_t *atomic)
{
	sfpr_atomic_t *tmp = pool->idle_tail;
	
	//开始时没有节点
	if(pool->idle_head == NULL && pool->idle_tail == NULL){
		pool->idle_head = atomic;
		pool->idle_tail = atomic;
		atomic->next_idle = NULL;
		atomic->prep_idle = NULL;
		return 0;
	}
	
	//遍历,找到合适的位置
	while(tmp){
		if(tmp->size > atomic->size){
			if(tmp->next_idle)
				tmp->next_idle->prep_idle = atomic;
			atomic->next_idle = tmp->next_idle;
			atomic->prep_idle = tmp;
			tmp->next_idle = atomic;

			//如果插入位置为尾部位置;则将该节点设置为尾部
			if(tmp == pool->idle_tail){
				pool->idle_tail = atomic;
			}
			break;
		}
		tmp = tmp->prep_idle;
	}

	//如果tmp为NULL, 则插入到第一个位置
	if(!tmp){
		pool->idle_head->prep_idle = atomic;
		atomic->next_idle = pool->idle_head;
		atomic->prep_idle = NULL;
		pool->idle_head = atomic;
	}

	return 0;
}

/**
 * @brief 将第一个节点分配给用户后，从链表中删除
 */
static sfpr_int_t pool_atomic_list_delete(sfpr_mem_pool_t *pool, sfpr_atomic_t *atomic)
{
	sfpr_atomic_t * tmp = pool->idle_head;

	//由于采用双向非循环链表,则pre为NULL，则为链表头结点;如果next为NULL，则为链表尾节点
	if(!atomic->prep_idle){
		pool->idle_head = atomic->next_idle;
		if(pool->idle_head)
			pool->idle_head->prep_idle = NULL;
	}
	else{
		atomic->prep_idle->next_idle = atomic->next_idle;
	}

	if(!atomic->next_idle){
		pool->idle_tail = atomic->prep_idle;
		if(pool->idle_tail)
			pool->idle_tail->next_idle = NULL;
	}
	else{
		atomic->next_idle->prep_idle = atomic->prep_idle;
	}

	atomic->next_idle = NULL;
	atomic->prep_idle = NULL;
	return 0;
}


static sfpr_void_t memnode_destroy(sfpr_mem_node_t **memnode)
{
	if (*memnode == NULL)
		return;
	
	free(*memnode);
	*memnode = NULL;
}

/**
 * @brief 销毁内存池
 */
static sfpr_void_t pool_destroy(sfpr_mem_pool_t **_pool)
{
	sfpr_int_t i;
	sfpr_mem_pool_t* pool = *_pool;
	sfpr_mem_node_t* mem_node_tmp = NULL;
	sfpr_mem_node_t* mem_node_tmp_next = NULL;
	
	if (pool == NULL)
		return;
	
	//free mem_node
	for (i=0; i< SFPR_MEM_POOL_MAX_SLOT; i++)
	{
		for (mem_node_tmp=pool->slot[i], (mem_node_tmp==NULL)? (mem_node_tmp_next=NULL):(mem_node_tmp_next = mem_node_tmp->next);
				mem_node_tmp;
				mem_node_tmp = mem_node_tmp_next,(mem_node_tmp==NULL)? (mem_node_tmp_next=NULL):(mem_node_tmp_next = mem_node_tmp->next))
		{
			memnode_destroy(&mem_node_tmp);
		}
	}
	
	sfpr_mutex_destroy(&pool->mutux);

	free(pool->alloced);
	free(pool);
	*_pool = NULL;
}


/**
 * @brief 在指定slot上创建一个新的节点
 * @param node_size 节点大小,包括节点头
 * @param slot_size 节点可用内存大小
 */
static sfpr_mem_node_t *memnode_new(sfpr_int_t node_size, sfpr_int_t slot_size)
{
	sfpr_mem_node_t *memnode = NULL;	
	
	memnode = (sfpr_mem_node_t*)malloc(node_size);
	if (!memnode)
	{
		return NULL;
	}
	memset(memnode,0,node_size);
	memnode->size = slot_size;
	memnode->atomic[0].addr = (sfpr_uint32_t)memnode + sizeof(sfpr_mem_node_t);
	memnode->atomic[0].size = slot_size;
	memnode->atomic[0].atomic = 0;
	memnode->atomic[0].memnode = memnode;
	memnode->atomic[0].effeced = 0;
	memnode->ref = &memnode;
	memnode->next = NULL;
	
	return memnode;
}

/**
 * @brief 确定 addr 在 pool->alloced 数组中的位置，用hash算法
 * @param flg 0-set; 1-get
 */
static sfpr_int_t get_atomic_head(sfpr_mem_pool_t *pool, sfpr_void_t *addr, sfpr_int_t flg)
{
	sfpr_int_t index = 0;
	sfpr_int_t basic = SFPR_MEM_POOL_MAX_ALLOCED_NUMBER;

	while (1)
	{
		if (basic <= 0)
			break;
		
		index = ((sfpr_uint32_t)addr)%basic;
		if (0 == flg)
		{
			if ((pool->alloced[index].addr == (sfpr_uint32_t)addr) 
					|| 0 == pool->alloced[index].addr)
			{

				return index;
			}
		}
		else
		{
			if (pool->alloced[index].addr == (sfpr_uint32_t)addr)
			{

				return index;
			}
		}	
			
		basic--;
	}

	return -1;
}

/**
 * @brief 设置 addr 在 pool->alloced 数组中的位置，用hash算法
 * @return -1:false; >=0:alloced_count_num
 */
static sfpr_int_t alloced_set(sfpr_mem_pool_t *pool, sfpr_mem_block_t block)
{
	sfpr_int_t index = -1;
	
	if (pool->alloced_count_num > pool->alloced_max_num)
		return -1;

	index = get_atomic_head(pool, (sfpr_void_t*)block.addr, 0);
	if (-1 == index)
		return -1;
	pool->alloced[index] = block;
	pool->alloced_count_num++;
#ifdef LOG_DEBUG	
	printf("allocted num:%d\n",pool->alloced_count_num);
#endif	
	return pool->alloced_count_num;
}

/**
 * @brief 设置 addr 在 pool->alloced 数组中的位置，用hash算法
 * @param addr 用户内存地址
 * @param block 用来获取内存block信息
 */
static sfpr_int_t alloced_get(sfpr_mem_pool_t *pool, sfpr_void_t *addr, sfpr_mem_block_t **block)
{
	sfpr_int_t index = -1;
	
	index = get_atomic_head(pool, addr, 1);
	if (-1 == index)
		return -1;

	*block = &pool->alloced[index];
#ifdef LOG_DEBUG	
	printf("index:%d,addr:%d,atomic:%d,effected:%d,memnode:%d,size:%d,slot:%d\n",index,
		(*block)->addr,
		(*block)->atomic,
		(*block)->effected,
		(*block)->memnode,
		(*block)->size,
		(*block)->slot);
#endif

	return 0;
}

/**
 * @brief 删除 addr 在 pool->alloced 数组中的位置，用hash算法
 * @return -1:false; >=0:alloced_count_num
 */
static sfpr_int_t alloced_del(sfpr_mem_pool_t *pool, sfpr_void_t *addr, sfpr_mem_block_t *block)
{
	sfpr_int_t index = -1;
	
	if (pool->alloced_count_num <= 0)
		return -1;

	index = get_atomic_head(pool,addr, 1);
	if (-1 == index)
		return -1;

	*block = pool->alloced[index];
#ifdef LOG_DEBUG	
	printf("del allocted:%d,addr:%d,atomic:%d,effected:%d,memnode:%d,size:%d,slot:%d\n",index,
		block->addr,
		block->atomic,
		block->effected,
		block->memnode,
		block->size,
		block->slot);
#endif
	memset(&pool->alloced[index], 0x0, sizeof(sfpr_mem_block_t));
	pool->alloced_count_num--;
#ifdef LOG_DEBUG	
	printf("allocted num:%d\n",pool->alloced_count_num);
#endif

	return pool->alloced_count_num;
}

static sfpr_void_t* memnode_alloc_atomic(sfpr_mem_node_t *memnode,sfpr_int_t size, sfpr_mem_pool_t *pool, sfpr_int_t slot)
{
	sfpr_int_t iret = 0;
	sfpr_int_t i;
	sfpr_mem_block_t block;
	sfpr_int_t tmpSize = 0;
	sfpr_atomic_t *atomic = NULL;	

	if (!memnode || size<=0 )
	{
		return NULL;
	}
	if (memnode->size < size)
	{
		return NULL;
	}

	atomic = memnode->atomic;
	for (i=0; i<MAX_ATOMIC - 1; i++)
	{
#ifdef LOG_DEBUG		
		printf("memnode_alloc_atomic() atomic[%d].size=%d effeced=%d\n",i,atomic[i].size,atomic[i].effeced);
#endif
		if (!atomic[i].effeced && atomic[i].size >= size)
		{
			tmpSize = atomic[i].size;
			atomic[i].effeced = 1;
			atomic[i].size = size;
			atomic[i].slot = slot;
			atomic[i].atomic = i;
			atomic[i].memnode = memnode;

#ifdef LOG_DEBUG
			printf("atomic[%d].size:%d, remain size:%d\n",i,atomic[i].size, tmpSize - size);
#endif
			//如果依然有可用内存块;且没有达到MAX_ATOMIC;则创建新的atomic元素
			if(tmpSize - size != 0)
			{
				atomic[i+1].addr = atomic[i].addr + atomic[i].size;
				atomic[i+1].size = tmpSize - size;
				atomic[i+1].slot = slot;
				atomic[i+1].atomic = i+1;
				atomic[i+1].memnode = memnode;
#ifdef LOG_DEBUG				
				printf("memnode_alloc_atomic(): index:%d, addr:0X%08X  size:%d   Base Addr:0X%08X\n",i, atomic[i].addr,size,(sfpr_uint32_t)memnode);	
#endif
				//将该可用内存块存入到idle链表中
				pool_atomic_list_insert(pool, &atomic[i+1]);
			}
#ifdef LOG_DEBUG
			debug_memnode_traverse(pool);
#endif
			//将分配好的内存在alloced中注册
			memset(&block, 0x0, sizeof(block));
			block.addr = atomic[i].addr;
			block.atomic = i;
			block.effected = 1;
			block.memnode = memnode;
			block.size = atomic[i].size;
			block.slot = slot;
			iret = alloced_set(pool,block);
			if (iret)
				return (void*)atomic[i].addr;
			else
				return NULL;
		}
	}
	//最后一个内存块;如果该内存块满足条件，即使内存块size大于实际需求，内存块大小仍为size
#ifdef LOG_DEBUG
	printf("memnode_alloc_atomic() atomic[%d].size=%d effeced=%d\n",i,atomic[i].size,atomic[i].effeced);
#endif
	if (!atomic[i].effeced && atomic[i].size >= size)
	{
		atomic[i].effeced = 1;
		//atomic[i].size = size;
#ifdef LOG_DEBUG			
		printf("memnode_alloc_atomic(): index:%d, addr:0X%08X  size:%d	 Base Addr:0X%08X\n",i, atomic[i].addr,size,(sfpr_uint32_t)memnode);	
#endif	
		//在alloced中注册
		memset(&block, 0x0, sizeof(block));
		block.addr = atomic[i].addr;
		block.atomic = i;
		block.effected = 1;
		block.memnode = memnode;
		block.size = atomic[i].size;
		block.slot = slot;
		iret = alloced_set(pool,block);
		if (iret)
			return (void*)atomic[i].addr;
		else
			return NULL;
	}
	
	return NULL;
}

/**
 * @brief: 直接从指定的atomic开始分配内存
 */
static sfpr_void_t* atomic_alloc_atomic(sfpr_atomic_t *atomic_src, sfpr_int_t size, sfpr_mem_pool_t *pool)
{

	sfpr_int_t iret = 0;
	sfpr_int_t i;
	sfpr_mem_block_t block;
	
	sfpr_mem_block_t* block_tmp = NULL;
	sfpr_mem_node_t *memnode = NULL;
	sfpr_atomic_t *atomic = NULL;	
	
	sfpr_int_t slot;	
	sfpr_int_t atomic_index;
	sfpr_int_t tmpSize = 0;

	if (size<=0 )
	{
		return NULL;
	}
#ifdef LOG_DEBUG
	printf("src->node:%p, src->slot:%d, src->atomic:%d\n",atomic_src->memnode,atomic_src->slot,atomic_src->atomic);
#endif
	memnode = atomic_src->memnode;
	slot = atomic_src->slot;
	atomic_index = atomic_src->atomic;
	atomic = memnode->atomic;
#ifdef LOG_DEBUG
	printf("atomic_alloc_atomic() atomic[%d].size=%d effeced=%d\n",atomic_index,atomic_src->size,atomic_src->effeced);
#endif
	tmpSize = atomic_src->size;
	atomic_src->effeced = 1;

	//判断当前node是否全被分配完成
	if ((0 == atomic[MAX_ATOMIC-1].addr) && (tmpSize - size != 0))
	{
		//移位
		for (i=MAX_ATOMIC-1; i> atomic_index; i--)
		{
			if (atomic[i].addr == 0)
				continue;

			//更新allocted中的信息
			if (memnode->atomic[i].effeced)
			{
				//查找
				block_tmp = NULL;
				iret = alloced_get(pool,(void*)memnode->atomic[i].addr, &block_tmp);
				if (iret != -1)
				{
					//移动atomic位置
					memnode->atomic[i+1] = memnode->atomic[i];
					memnode->atomic[i+1].atomic = i+1;
					memset(&memnode->atomic[i], 0x0, sizeof(sfpr_atomic_t));
					
					//更新内容
					block_tmp->atomic = i+1;
				}
			}
			//更新idle中的信息
			else
			{
				//移除
				pool_atomic_list_delete(pool, &memnode->atomic[i]);
			
				//移动
				memnode->atomic[i+1] = memnode->atomic[i];
				memnode->atomic[i+1].atomic = i+1;
				memset(&memnode->atomic[i], 0x0, sizeof(sfpr_atomic_t));
			
				//添加
				pool_atomic_list_insert(pool, &memnode->atomic[i+1]);
			}
		}

		//分裂新的atomic
		atomic_src->size = size;
		atomic[atomic_index+1].addr = atomic_src->addr + atomic_src->size;
		atomic[atomic_index+1].size = tmpSize - size;
		atomic[atomic_index+1].slot = slot;
		atomic[atomic_index+1].atomic = atomic_index +1;
		atomic[atomic_index+1].memnode = memnode;
#ifdef LOG_DEBUG		
		printf("remain atomic[%d].size:%d\n",atomic_index+1, atomic[atomic_index+1].size);
#endif
		//添加到idle中
		pool_atomic_list_insert(pool, &atomic[atomic_index+1]);
	}
#ifdef LOG_DEBUG
	debug_memnode_traverse(pool);
#endif
	//在alloced中注册
	memset(&block, 0x0, sizeof(block));
	block.addr = atomic_src->addr;
	block.atomic = atomic_index;
	block.effected = 1;
	block.memnode = memnode;
	block.size = atomic_src->size;
	block.slot = slot;
	iret = alloced_set(pool,block);
	if (iret)
		return (void*)atomic_src->addr;
	else
		return NULL;
	
	return NULL;
}


sfpr_mem_keeper_t *sfpr_mem_pool_create()
{
	sfpr_mem_keeper_t *keeper = NULL;
	
	keeper = (sfpr_mem_keeper_t*)malloc(sizeof(sfpr_mem_keeper_t));
	if (!keeper)
	{
		return NULL;
	}
	memset(keeper, 0, sizeof(sfpr_mem_keeper_t));
	
	return keeper;
}

sfpr_mem_pool_t *sfpr_mem_pool_new(sfpr_mem_keeper_t *keeper, const sfpr_char_t *name)
{
	sfpr_mem_pool_t *pool = NULL;
	sfpr_mem_block_t *alloced = NULL;	
	
	pool = (sfpr_mem_pool_t*)malloc(sizeof(sfpr_mem_pool_t));
	if (!pool)
	{
		return NULL;
	}
	
	memset(pool,0,sizeof(sfpr_mem_pool_t));
	strncpy(pool->name,name,sizeof(pool->name)-1);
	
	pool->alloced_max_num = SFPR_MEM_POOL_MAX_ALLOCED_NUMBER;
	alloced = (sfpr_mem_block_t*)malloc(sizeof(sfpr_mem_block_t) * pool->alloced_max_num);
	if (!alloced)
	{
		return NULL;
	}
	memset(alloced, 0x0, sizeof(sfpr_mem_block_t) * pool->alloced_max_num);
	//init lock
	sfpr_mutex_create(&pool->mutux, 0);
	
	//2011-12-31 add by nahaitao
	memset(alloced,0x0,sizeof(sfpr_mem_block_t) * pool->alloced_max_num);
	pool->alloced = alloced;

	//2011-1-4 add by nahaitao
	pool_atomic_list_init(pool);
	
	//把池子放入链表中
	if (!keeper->header)
	{
		keeper->header = pool;
		keeper->tailer = pool;
	}
	else
	{
		keeper->tailer->next = pool;
		keeper->tailer = pool;
	}
	
	return pool;
}

sfpr_int_t sfpr_mem_pool_delete(sfpr_mem_keeper_t *keeper, sfpr_mem_pool_t *pool)
{
	sfpr_int_t flg = 0;
	sfpr_mem_pool_t* tmp = NULL;
	sfpr_mem_pool_t* tmp_pre = NULL;

	if (!keeper || !keeper->header)
		return SFPR_SUCCESS;

    if (pool == keeper->header)
	{
		keeper->header = pool->next;
		if(pool == keeper->tailer)
			keeper->tailer = pool->next;
		flg = 1;
    }
	else
	{
	    tmp_pre = keeper->header;
		tmp = keeper->header->next;
		while (tmp)
		{
			if (tmp == pool)
			{
				tmp_pre->next= tmp->next;
				if (pool == keeper->tailer)
				{
					keeper->tailer = tmp_pre;
				}
				flg = 1;
				break;
			}
			tmp_pre = tmp;
			tmp = tmp->next;
		}
    }

	if(flg)
	{
		pool_destroy(&pool);
		return SFPR_SUCCESS;
	}
	else
	{
		return SFPR_ERROR;
	}
}


sfpr_int_t sfpr_mem_pool_delete_byname(sfpr_mem_keeper_t *keeper, const sfpr_char_t *name)
{
	sfpr_int_t flg = 0;
	sfpr_mem_pool_t* tmp = NULL;
	sfpr_mem_pool_t* tmp_pre = NULL;
	sfpr_mem_pool_t* pool = NULL;
	
	if(!keeper || !keeper->header)
		return 0;

    if(!strcmp(keeper->header->name, name))
	{
		pool = keeper->header;
		keeper->header = pool->next;
		if(pool == keeper->tailer)
			keeper->tailer = pool->next;
		flg = 1;
    }
	else
	{
	    tmp_pre = keeper->header;
		tmp = keeper->header->next;
		while (tmp)
		{
			if (!strcmp(tmp->name,name))
			{
				pool = tmp;
				tmp_pre->next= tmp->next;
				if (pool == keeper->tailer)
				{
					keeper->tailer = tmp_pre;
				}
				flg = 1;
				break;
			}
			tmp_pre = tmp;
			tmp = tmp->next;
		}
    }

	if (flg)
	{
		pool_destroy(&pool);
		return 0;
	}
	else
	{
		return -1;
	}
}

sfpr_int_t sfpr_mem_pool_destroy(sfpr_mem_keeper_t **_keeper)
{
	//free pool
	sfpr_int_t i;
	sfpr_mem_pool_t* mem_pool_tmp = NULL;
	sfpr_mem_pool_t* mem_pool_tmp_next = NULL;
	sfpr_mem_keeper_t *keeper = *_keeper;

	if (keeper == NULL)
		return SFPR_SUCCESS;
	
	//free mem_node
	for (mem_pool_tmp=keeper->header, (mem_pool_tmp==NULL)?(mem_pool_tmp_next=NULL):(mem_pool_tmp_next = mem_pool_tmp->next);
			mem_pool_tmp;
			mem_pool_tmp = mem_pool_tmp_next,(mem_pool_tmp==NULL)?(mem_pool_tmp_next=NULL):(mem_pool_tmp_next = mem_pool_tmp->next))
	{
		pool_destroy(&mem_pool_tmp);
	}

	
	free(keeper);
	*_keeper = NULL;
	
	return SFPR_SUCCESS;
}

sfpr_void_t* sfpr_mem_malloc(sfpr_mem_pool_t *pool,sfpr_int_t size)
{
	sfpr_int_t slot,node_size,slot_size;
	sfpr_mem_node_t *memnode = NULL;
	sfpr_mem_node_t *memnode_tmp = NULL;
	sfpr_atomic_t* atomic = NULL;
	sfpr_void_t *alloced_addr = NULL;
	
	if (!pool || size <= 0)
	{
		return NULL;
	}

    //lock
    sfpr_mutex_lock(&pool->mutux);
	//查找空闲的atomic链表，是否有满足条件的atomic
	if ((atomic = pool_atomic_list_get(pool, size)))
	{
	
		pool_atomic_list_delete(pool, atomic);
		
		//直接从idle atomic中分配内存
		alloced_addr = atomic_alloc_atomic(atomic, size, pool);

		//将该atomic从idle链表中移除
		if(!alloced_addr)
		{
			pool_atomic_list_insert(pool, atomic);
		}
		sfpr_mutex_unlock(&pool->mutux);
		memset(alloced_addr,0,size);
		return alloced_addr;
	}
	else
	{
		//从对应的槽中分配内存
		//确定在哪一个slot中
		node_size = SFPR_MEM_POOL_SLOT_BASE_SIZE;
		for (slot=1;slot<SFPR_MEM_POOL_MAX_SLOT;slot++)
		{
			node_size = (slot==1)?node_size:(node_size*2);
			slot_size = node_size - sizeof(sfpr_mem_node_t);
			if(size <= slot_size)
			{
				break;
			}
		}
		if (slot >= SFPR_MEM_POOL_MAX_SLOT)
		{
			slot = 0;
			slot_size = size;
			node_size = slot_size + sizeof(sfpr_mem_node_t) + 8;
		}

				
		//创建节点
		memnode = memnode_new(node_size,slot_size);
		if (!memnode)
		{
			sfpr_mutex_unlock(&pool->mutux);
#ifdef LOG_ERROR			
			fprintf(stderr,"sfpr_mem_malloc() | error | malloc slot[%d] failed\n",slot);
#endif
			return NULL;
		}

		if(!pool->slot[slot])
		{
			pool->slot[slot] = memnode;
		}
		else
		{
			memnode_tmp = pool->slot[slot];
			while(memnode_tmp)
			{
				if(NULL == memnode_tmp->next)
				{
					memnode_tmp->next = memnode;
					break;
				}
				memnode_tmp = memnode_tmp->next;
			}
		}
		
		//分配内存块
		alloced_addr = memnode_alloc_atomic(memnode,size,pool,slot);
		sfpr_mutex_unlock(&pool->mutux);
		memset(alloced_addr,0,size);
		return alloced_addr;
	}
	sfpr_mutex_unlock(&pool->mutux);
	return NULL;
}

sfpr_void_t* sfpr_mem_realloc(sfpr_mem_pool_t *pool, sfpr_void_t *addr, sfpr_int_t old_size, sfpr_int_t new_size)
{
	sfpr_int_t ret = SFPR_SUCCESS;

	//重新分配一个内存单元
	sfpr_char_t * tmp = sfpr_mem_malloc(pool, new_size);
	if(tmp == NULL)
		return NULL;
	
	//copy
	memcpy(tmp, addr, old_size);
	
	//释放原有内存单元
	sfpr_mem_free(pool ,addr);
	return tmp;
}

sfpr_int_t sfpr_mem_free(sfpr_mem_pool_t *pool,sfpr_void_t *addr)
{
    sfpr_int_t iret = SFPR_SUCCESS;
	sfpr_int_t i=0,j=0,k=0;
	sfpr_int_t atomic_index = -1;
	sfpr_int_t atomic_index_tmp = -1;
	sfpr_mem_block_t block;	
	sfpr_mem_block_t* block_tmp = NULL;
    sfpr_mem_node_t *memnode = NULL;
    sfpr_mem_node_t *memnode_tmp = NULL;
    sfpr_mem_node_t *memnode_pre = NULL;

	memset(&block, 0x0, sizeof(block));
    //lock
    sfpr_mutex_lock(&pool->mutux);
	
	iret = alloced_del(pool,addr, &block);
	if (iret != -1)
	{
		memnode = block.memnode;
		memnode->atomic[block.atomic].effeced = 0;
		atomic_index = block.atomic;
		//如果当前mem_node大于设定值;则直接free给系统
		if(memnode->size > SFPR_MEM_POOL_MEM_NODE_MAX_SIZE)
		{
			if(pool->slot[block.slot] == memnode)
			{
				pool->slot[block.slot] = memnode->next;
			}
			else
			{
				memnode_pre = pool->slot[block.slot];
				memnode_tmp = pool->slot[block.slot]->next;
				while(memnode_tmp)
				{
					if(memnode_tmp == memnode)
					{
						memnode_pre->next = memnode->next;
						break;
					}
					memnode_tmp = memnode_tmp->next;
				}
			}
			memnode_destroy(&memnode);
		}
		else
		{
			//向上检测相邻的内存块是否挂载在idle链表中,是则合并，并置空空闲出来的atomic
			for (i=block.atomic-1; i>= 0; i--)
			{
				if (memnode->atomic[i].effeced == 1)
					break;

				//将待合并内存块从idle链表中移除
				pool_atomic_list_delete(pool, &memnode->atomic[i]);

				//合并
				memnode->atomic[i].size += memnode->atomic[i+1].size;

				//置空空闲的atomic
				memset(&memnode->atomic[i+1], 0x0, sizeof(sfpr_atomic_t));
				
				atomic_index = i;
			}

			//向下检测相邻的内存块是否挂载在idle链表中，是则合并，并置空空闲出来的atomic
			for (j=block.atomic+1; j < MAX_ATOMIC; j++)
			{
				if (memnode->atomic[j].addr == 0)
					break;
				if (memnode->atomic[j].effeced == 1)
					break;

				//将待合并内存块从idle链表中移除
				pool_atomic_list_delete(pool, &memnode->atomic[j]);

				//合并
				memnode->atomic[atomic_index].size += memnode->atomic[j].size;

				//置空空闲的atomic
				memset(&memnode->atomic[j], 0x0, sizeof(sfpr_atomic_t));
			}

			//将当前释放的内存挂载到idle中
			pool_atomic_list_insert(pool, &memnode->atomic[atomic_index]);
			
			//如果载合并过程中发生atomic中间真空，则更新atomic数组下边节点的信息及其对应的idle和allocted链表中的信息
			if (j - atomic_index > 1)
			{
				for (k = j, i=1; k< MAX_ATOMIC; k++,i++)
				{
					if (memnode->atomic[k].addr == 0)
						break;
					
					//更新allocted中的信息
					if (memnode->atomic[k].effeced)
					{
						//查找
						block_tmp = NULL;
						iret = alloced_get(pool,(void*)memnode->atomic[k].addr, &block_tmp);
						if(iret != -1)
						{
							//移动atomic位置
							memnode->atomic[atomic_index+i] = memnode->atomic[k];
							memnode->atomic[atomic_index+i].atomic = atomic_index+i;
							memset(&memnode->atomic[k], 0x0, sizeof(sfpr_atomic_t));
							
							//更新内容
							block_tmp->atomic = atomic_index+i;
						}
					}
					//更新idle中的信息
					else
					{
						//移除
						pool_atomic_list_delete(pool, &memnode->atomic[k]);

						//移动
						memnode->atomic[atomic_index+i] = memnode->atomic[k];
						memnode->atomic[atomic_index+i].atomic = atomic_index+i;
						memset(&memnode->atomic[k], 0x0, sizeof(sfpr_atomic_t));

						//添加
						pool_atomic_list_insert(pool, &memnode->atomic[atomic_index+i]);
					}
				}
			}
		}
	}
#ifdef LOG_DEBUG
	debug_memnode_traverse(pool);
#endif	
    sfpr_mutex_unlock(&pool->mutux);
	return iret;
}


sfpr_mem_pool_t *sfpr_mem_pool_get(sfpr_mem_keeper_t *keeper, const sfpr_char_t *name)
{
	sfpr_mem_pool_t *pool = keeper->header;
	
	while (pool)
	{
		if (!strcmp(pool->name,name))
			return pool;
		pool = pool->next;
	}
	
	return NULL;
}

#if 0
sfpr_int_t sfpr_mem_pool_statall(sfpr_mem_keeper_t *keeper)
{
	sfpr_int_t i,j;
	sfpr_int_t index = 0;
	sfpr_mem_node_t* tmp = NULL;
	sfpr_mem_pool_t *pool = keeper->header;

	printf("\n----->begin Stat Memory Pool:\n");
	while (pool)
	{
		//lock
		sfpr_mutex_lock(&pool->mutux);
		
		//printf("Memory Pool:  Name:%s\n",pool->name);
		for (i=0; i<SFPR_MEM_POOL_MAX_SLOT; i++)
		{
			printf("\nslot[%d]:\n", i);
			tmp = pool->slot[i];
			index = 0;
			while (tmp)
			{
				for (j=0; j< MAX_ATOMIC; j++)
				{
					printf("slot %d:\natomic[%d] addr:%u, size:%d, index:%d, use:%d\n",i, j,tmp->atomic[j].addr,tmp->atomic[j].size,tmp->atomic[j].atomic, tmp->atomic[j].effeced);
				}
				tmp = tmp->next;
				index++;
			}
		}
		//lock
		sfpr_mutex_unlock(&pool->mutux);
		
		pool = pool->next;
	}
	printf("<-----end Stat Memory Pool\n");
	return 0;
}
#endif

sfpr_int_t sfpr_mem_pool_statall(sfpr_mem_keeper_t *keeper)
{
	sfpr_int_t i,j;
	sfpr_int_t index = 0;
	sfpr_mem_node_t* tmp = NULL;
	sfpr_mem_pool_t *pool = keeper->header;

	printf("----->begin Stat Memory Pool:\n");
	while (pool)
	{
		//lock
		//sfpr_mutex_lock(&pool->mutux);
		//printf("Memory Pool:  Name:%s\n",pool->name);
		for (i=0; i<SFPR_MEM_POOL_MAX_SLOT; i++)
		{
			printf("\nslot[%d]:\n", i);
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("node:%-38p", tmp);
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("addr:%10p, size:%6d, status:%6s", tmp->atomic[0].addr, tmp->atomic[0].size, (tmp->atomic[0].effeced ? "used" : "unused"));
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
			
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("addr:%10p, size:%6d, status:%6s", tmp->atomic[1].addr, tmp->atomic[1].size, (tmp->atomic[1].effeced ? "used" : "unused"));
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("addr:%10p, size:%6d, status:%6s", tmp->atomic[2].addr, tmp->atomic[2].size, (tmp->atomic[2].effeced ? "used" : "unused"));
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("addr:%10p, size:%6d, status:%6s", tmp->atomic[3].addr, tmp->atomic[3].size, (tmp->atomic[3].effeced ? "used" : "unused"));
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("addr:%10p, size:%6d, status:%6s", tmp->atomic[4].addr, tmp->atomic[4].size, (tmp->atomic[4].effeced ? "used" : "unused"));
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("addr:%10p, size:%6d, status:%6s", tmp->atomic[5].addr, tmp->atomic[5].size, (tmp->atomic[5].effeced ? "used" : "unused"));
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("addr:%10p, size:%6d, status:%6s", tmp->atomic[6].addr, tmp->atomic[6].size, (tmp->atomic[6].effeced ? "used" : "unused"));
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
			tmp = pool->slot[i];
			if(tmp)
			{
				while (tmp)
				{
					printf("addr:%10p, size:%6d, status:%6s", tmp->atomic[7].addr, tmp->atomic[7].size, (tmp->atomic[7].effeced ? "used" : "unused"));
					printf("\t");
					tmp = tmp->next;
				}
				printf("\n");
			}
		}
		//lock
		//sfpr_mutex_unlock(&pool->mutux);

		//idle list
		debug_memnode_traverse(pool);

		//alocted table
		debug_allocted_traverse(pool);
		pool = pool->next;
	}
	printf("<-----end Stat Memory Pool\n");
	return 0;
}


#ifdef __cplusplus
}
#endif

