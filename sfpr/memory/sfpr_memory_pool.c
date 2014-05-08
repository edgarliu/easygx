
#include "sfpr_memory_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\ingroup memory_pool_struct
 * @brief �ڴ�ڵ�
 */
typedef struct sfpr_mem_node_s sfpr_mem_node_t;

/**
 *\ ingroup memory_pool_struct
 *\ brief �ڴ���е��ڴ����ӣ��ڴ���е���С�ڴ浥Ԫ
 */
typedef struct sfpr_atomic_s
{
	sfpr_uint32_t 	addr; 					/**<���ڴ����ӵĵ�ַ*/
	sfpr_uint32_t 	size:24,				/**<���ڴ����ӵĴ�С*/
			slot:4,							/**<���ڴ����������ڴ�ۼ���*/
			atomic:3,						/**<���ڴ��������*/
			effeced:1;						/**<���ڴ������Ƿ񱻷��� 1:��ʹ�� 0:δ��ʹ��*/
	sfpr_mem_node_t	*memnode;				/**<���ڴ����������ڴ��*/
	struct sfpr_atomic_s *next_idle;		/**<ָ����һ���ڴ�����*/
	struct sfpr_atomic_s *prep_idle;		/**<ָ��ǰһ���ڴ�����*/
}sfpr_atomic_t;


/**
 *\ ingroup memory_pool_struct
 *\brief һ���ڴ�������ڴ����Ӹ���
 */
#define MAX_ATOMIC		8

/**
 *\ingroup memory_pool_struct
 *\brief �ڴ���е��ڴ�ڵ�
 *\remark ÿ���ڴ���������ڴ�����
 */
struct sfpr_mem_node_s 
{
	sfpr_uint32_t   				size;           /**<�ڴ�ڵ��ڴ��С*/
	sfpr_atomic_t						atomic[MAX_ATOMIC];/**<�ڴ����Ӽ���*/
	struct sfpr_mem_node_s 	**ref;    /**< reference to self */
	struct sfpr_mem_node_s 	*next;    /**< next memnode */
};

/**
 *\ ingroup memory_pool_struct
 * @brief �Ѿ�������ڴ��
 * @remark �ýṹ������sfpr_mem_node_t��ָ�Ѿ�������ڴ�
 */
typedef struct sfpr_mem_block_s
{
	sfpr_uint32_t 	addr;/**<�ڴ��ĵ�ַ*/
	sfpr_uint32_t 	size:24,/**<�ڴ���С*/
									slot:4,/**<�ڴ���������ڴ��*/
									atomic:3,/**<�ڴ�����*/
									effected:1;/**<�ڴ汻ʹ�ñ�� 1:��ʹ�� 0:δ��ʹ��*/
	sfpr_mem_node_t	*memnode;/**<�����ڴ�ڵ�*/
}sfpr_mem_block_t;

struct sfpr_mem_pool_s
{
	sfpr_char_t 							name[128];/**<�ڴ������*/
	sfpr_mutex_t 							mutux;/**<������*/
	sfpr_mem_node_t 	*slot[SFPR_MEM_POOL_MAX_SLOT];/**<�ڴ��*/
	sfpr_atomic_t			*idle_head;/**<���ڴ�ԭ�ӵĵ�ַ*/
	sfpr_atomic_t			*idle_tail;/**<���ڴ�ԭ�ӵĵ�ַ*/

	sfpr_mem_block_t 	*alloced; /**<�Ѿ�������ڴ�鼯��*/
	sfpr_uint32_t 		alloced_max_num;/**<������������ڴ�����*/
	sfpr_uint32_t			alloced_count_num;/**<��ǰ�Ѿ�������ڴ�����*/

	struct sfpr_mem_pool_s *next;/**<���ڴ�ԭ�ӵĵ�ַ*/
};

struct sfpr_mem_keeper_s
{
	sfpr_mem_pool_t *header;/**<ָ���һ���ڴ��*/
	sfpr_mem_pool_t *tailer;/**<ָ�����һ���ڴ��*/
};


static sfpr_void_t debug_memnode_traverse(sfpr_mem_pool_t* pool)
{
	sfpr_atomic_t* tmp = NULL;
	int total = 0;
	printf("\n------->idle list\n");
	//����
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
	//����
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
 * @brief �ж�idle���������Ľڵ�(��һ��)�Ƿ��������������򷵻�
 */
static sfpr_atomic_t * pool_atomic_list_get(sfpr_mem_pool_t *pool, sfpr_int_t size)
{
	sfpr_atomic_t *tmp = NULL;
	if (!pool->idle_head)
		return NULL;

	tmp = pool->idle_head;

	
	//����,�ҵ����ʵ�λ��
	while (tmp)
	{
		if(tmp->size < size)
		{
			break;
		}
		tmp = tmp->next_idle;
	}

	//���tmpΪNULL, ��tail��Ϊ���ýڵ�
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
 * @brief ��ָ���ڴ�������ӵ�idle������
 */
static sfpr_int_t pool_atomic_list_insert(sfpr_mem_pool_t *pool, sfpr_atomic_t *atomic)
{
	sfpr_atomic_t *tmp = pool->idle_tail;
	
	//��ʼʱû�нڵ�
	if(pool->idle_head == NULL && pool->idle_tail == NULL){
		pool->idle_head = atomic;
		pool->idle_tail = atomic;
		atomic->next_idle = NULL;
		atomic->prep_idle = NULL;
		return 0;
	}
	
	//����,�ҵ����ʵ�λ��
	while(tmp){
		if(tmp->size > atomic->size){
			if(tmp->next_idle)
				tmp->next_idle->prep_idle = atomic;
			atomic->next_idle = tmp->next_idle;
			atomic->prep_idle = tmp;
			tmp->next_idle = atomic;

			//�������λ��Ϊβ��λ��;�򽫸ýڵ�����Ϊβ��
			if(tmp == pool->idle_tail){
				pool->idle_tail = atomic;
			}
			break;
		}
		tmp = tmp->prep_idle;
	}

	//���tmpΪNULL, ����뵽��һ��λ��
	if(!tmp){
		pool->idle_head->prep_idle = atomic;
		atomic->next_idle = pool->idle_head;
		atomic->prep_idle = NULL;
		pool->idle_head = atomic;
	}

	return 0;
}

/**
 * @brief ����һ���ڵ������û��󣬴�������ɾ��
 */
static sfpr_int_t pool_atomic_list_delete(sfpr_mem_pool_t *pool, sfpr_atomic_t *atomic)
{
	sfpr_atomic_t * tmp = pool->idle_head;

	//���ڲ���˫���ѭ������,��preΪNULL����Ϊ����ͷ���;���nextΪNULL����Ϊ����β�ڵ�
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
 * @brief �����ڴ��
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
 * @brief ��ָ��slot�ϴ���һ���µĽڵ�
 * @param node_size �ڵ��С,�����ڵ�ͷ
 * @param slot_size �ڵ�����ڴ��С
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
 * @brief ȷ�� addr �� pool->alloced �����е�λ�ã���hash�㷨
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
 * @brief ���� addr �� pool->alloced �����е�λ�ã���hash�㷨
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
 * @brief ���� addr �� pool->alloced �����е�λ�ã���hash�㷨
 * @param addr �û��ڴ��ַ
 * @param block ������ȡ�ڴ�block��Ϣ
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
 * @brief ɾ�� addr �� pool->alloced �����е�λ�ã���hash�㷨
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
			//�����Ȼ�п����ڴ��;��û�дﵽMAX_ATOMIC;�򴴽��µ�atomicԪ��
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
				//���ÿ����ڴ����뵽idle������
				pool_atomic_list_insert(pool, &atomic[i+1]);
			}
#ifdef LOG_DEBUG
			debug_memnode_traverse(pool);
#endif
			//������õ��ڴ���alloced��ע��
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
	//���һ���ڴ��;������ڴ��������������ʹ�ڴ��size����ʵ�������ڴ���С��Ϊsize
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
		//��alloced��ע��
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
 * @brief: ֱ�Ӵ�ָ����atomic��ʼ�����ڴ�
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

	//�жϵ�ǰnode�Ƿ�ȫ���������
	if ((0 == atomic[MAX_ATOMIC-1].addr) && (tmpSize - size != 0))
	{
		//��λ
		for (i=MAX_ATOMIC-1; i> atomic_index; i--)
		{
			if (atomic[i].addr == 0)
				continue;

			//����allocted�е���Ϣ
			if (memnode->atomic[i].effeced)
			{
				//����
				block_tmp = NULL;
				iret = alloced_get(pool,(void*)memnode->atomic[i].addr, &block_tmp);
				if (iret != -1)
				{
					//�ƶ�atomicλ��
					memnode->atomic[i+1] = memnode->atomic[i];
					memnode->atomic[i+1].atomic = i+1;
					memset(&memnode->atomic[i], 0x0, sizeof(sfpr_atomic_t));
					
					//��������
					block_tmp->atomic = i+1;
				}
			}
			//����idle�е���Ϣ
			else
			{
				//�Ƴ�
				pool_atomic_list_delete(pool, &memnode->atomic[i]);
			
				//�ƶ�
				memnode->atomic[i+1] = memnode->atomic[i];
				memnode->atomic[i+1].atomic = i+1;
				memset(&memnode->atomic[i], 0x0, sizeof(sfpr_atomic_t));
			
				//���
				pool_atomic_list_insert(pool, &memnode->atomic[i+1]);
			}
		}

		//�����µ�atomic
		atomic_src->size = size;
		atomic[atomic_index+1].addr = atomic_src->addr + atomic_src->size;
		atomic[atomic_index+1].size = tmpSize - size;
		atomic[atomic_index+1].slot = slot;
		atomic[atomic_index+1].atomic = atomic_index +1;
		atomic[atomic_index+1].memnode = memnode;
#ifdef LOG_DEBUG		
		printf("remain atomic[%d].size:%d\n",atomic_index+1, atomic[atomic_index+1].size);
#endif
		//��ӵ�idle��
		pool_atomic_list_insert(pool, &atomic[atomic_index+1]);
	}
#ifdef LOG_DEBUG
	debug_memnode_traverse(pool);
#endif
	//��alloced��ע��
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
	
	//�ѳ��ӷ���������
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
	//���ҿ��е�atomic�����Ƿ�������������atomic
	if ((atomic = pool_atomic_list_get(pool, size)))
	{
	
		pool_atomic_list_delete(pool, atomic);
		
		//ֱ�Ӵ�idle atomic�з����ڴ�
		alloced_addr = atomic_alloc_atomic(atomic, size, pool);

		//����atomic��idle�������Ƴ�
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
		//�Ӷ�Ӧ�Ĳ��з����ڴ�
		//ȷ������һ��slot��
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

				
		//�����ڵ�
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
		
		//�����ڴ��
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

	//���·���һ���ڴ浥Ԫ
	sfpr_char_t * tmp = sfpr_mem_malloc(pool, new_size);
	if(tmp == NULL)
		return NULL;
	
	//copy
	memcpy(tmp, addr, old_size);
	
	//�ͷ�ԭ���ڴ浥Ԫ
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
		//�����ǰmem_node�����趨ֵ;��ֱ��free��ϵͳ
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
			//���ϼ�����ڵ��ڴ���Ƿ������idle������,����ϲ������ÿտ��г�����atomic
			for (i=block.atomic-1; i>= 0; i--)
			{
				if (memnode->atomic[i].effeced == 1)
					break;

				//�����ϲ��ڴ���idle�������Ƴ�
				pool_atomic_list_delete(pool, &memnode->atomic[i]);

				//�ϲ�
				memnode->atomic[i].size += memnode->atomic[i+1].size;

				//�ÿտ��е�atomic
				memset(&memnode->atomic[i+1], 0x0, sizeof(sfpr_atomic_t));
				
				atomic_index = i;
			}

			//���¼�����ڵ��ڴ���Ƿ������idle�����У�����ϲ������ÿտ��г�����atomic
			for (j=block.atomic+1; j < MAX_ATOMIC; j++)
			{
				if (memnode->atomic[j].addr == 0)
					break;
				if (memnode->atomic[j].effeced == 1)
					break;

				//�����ϲ��ڴ���idle�������Ƴ�
				pool_atomic_list_delete(pool, &memnode->atomic[j]);

				//�ϲ�
				memnode->atomic[atomic_index].size += memnode->atomic[j].size;

				//�ÿտ��е�atomic
				memset(&memnode->atomic[j], 0x0, sizeof(sfpr_atomic_t));
			}

			//����ǰ�ͷŵ��ڴ���ص�idle��
			pool_atomic_list_insert(pool, &memnode->atomic[atomic_index]);
			
			//����غϲ������з���atomic�м���գ������atomic�����±߽ڵ����Ϣ�����Ӧ��idle��allocted�����е���Ϣ
			if (j - atomic_index > 1)
			{
				for (k = j, i=1; k< MAX_ATOMIC; k++,i++)
				{
					if (memnode->atomic[k].addr == 0)
						break;
					
					//����allocted�е���Ϣ
					if (memnode->atomic[k].effeced)
					{
						//����
						block_tmp = NULL;
						iret = alloced_get(pool,(void*)memnode->atomic[k].addr, &block_tmp);
						if(iret != -1)
						{
							//�ƶ�atomicλ��
							memnode->atomic[atomic_index+i] = memnode->atomic[k];
							memnode->atomic[atomic_index+i].atomic = atomic_index+i;
							memset(&memnode->atomic[k], 0x0, sizeof(sfpr_atomic_t));
							
							//��������
							block_tmp->atomic = atomic_index+i;
						}
					}
					//����idle�е���Ϣ
					else
					{
						//�Ƴ�
						pool_atomic_list_delete(pool, &memnode->atomic[k]);

						//�ƶ�
						memnode->atomic[atomic_index+i] = memnode->atomic[k];
						memnode->atomic[atomic_index+i].atomic = atomic_index+i;
						memset(&memnode->atomic[k], 0x0, sizeof(sfpr_atomic_t));

						//���
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

