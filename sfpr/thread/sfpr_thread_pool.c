
#include <string.h>
#include <stdio.h>

#include "sfpr_thread_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\file sfpr_thread_pool.c
 *\brief SFPR����̳߳�ģ��
 *\author bijing
 *\version 0.1.0
 *\date 2012-02-14
 */

typedef struct sfpr_threadpool_worker_s{
	sfpr_thread_id thread_id;   /**< �߳�id */
	int busy;
	sfpr_threadpool_key_t key;
	sfpr_threadpool_task_t *task_queue[SFPR_THREAD_POOL_WORKER_TASK_NUM];/**< ������� */
	int task_head,task_tail,task_count;
}sfpr_threadpool_worker_t; 

/**
 *\ingroup thread_pool_struct
 *\brief �̳߳ؽṹ������
 */
struct sfpr_threadpool_s{
	char name[16]; /**< �̳߳����� */
	int stack_size; /**< �̵߳�ջ��С */
	int priority;  /**< �߳����ȼ� */
	int mode; /**< ��ǰ����ģʽ */

	sfpr_mutex_t mutex;   /**< �̳߳��� */
	sfpr_cond_t  cond;		/**< �̳߳��������� */

	sfpr_threadpool_worker_t *worker;   /**<  �����߳�*/
	int worker_min_num;   /**< �̵߳���С�� */
	int worker_max_num;   /**< �̵߳������ */
	int worker_cur_num;    /**<  ��ǰ�̸߳���*/
	int worker_busy_num;  /**<  æ���̸߳���*/

	sfpr_threadpool_task_t *task_pool;   /**< ����� */
	int task_max_num;      /**< ��������������  */
	int task_pool_sizeof;  /**< �������ռ�ڴ�Ĵ�С  */
	sfpr_threadpool_task_t *task_queue[SFPR_THREAD_POOL_MAX_TASK_NUM];/**< ������� */
	int task_head,task_tail;
	int task_count;     /**<  taskpool�Ѿ�ʹ�ø���*/

	int started;/**<  �̳߳��Ƿ��Ѿ�����*/

	sfpr_threadpool_worker_t manager_thread;   /**<  �����߳�id*/
	int all_busy_time;  /**<  �����̶߳�æ�ļ�ʱʱ��*/
	int all_idle_time;  /**<  �����̶߳����еļ�ʱʱ��*/
};

/**
 *\brief �����̳߳�	
 */
sfpr_threadpool_t *sfpr_threadpool_create(char *name,int min_num,int max_num,int stack_size,int priority)
{
	sfpr_threadpool_t *pthreadpool = NULL ;
	int i;

	#ifdef LOG_DEBUG
	fprintf(stdout, "threadpool_create() | debug | Begin ... \n");
	#endif

	/**< 1.�����̳߳ؽṹ */
	pthreadpool = (sfpr_threadpool_t*)malloc(sizeof(sfpr_threadpool_t));	
	if (NULL == pthreadpool){
		return NULL;
	}
	memset(pthreadpool, 0, sizeof(sfpr_threadpool_t));

	/**<2. ��ʼ���̳߳س�Ա���� */
	strncpy(pthreadpool->name,name,sizeof(pthreadpool->name)-1);
	pthreadpool->stack_size = stack_size;
	pthreadpool->priority = priority;
	pthreadpool->mode = sfpr_threadpool_mode_none;

	/**< 3.�̳߳�ʼ�� */
	pthreadpool->worker_min_num = min_num;
	pthreadpool->worker_cur_num = min_num;
	pthreadpool->worker_max_num = max_num;
	pthreadpool->worker = (sfpr_threadpool_worker_t*)malloc((pthreadpool->worker_max_num)*sizeof(sfpr_threadpool_worker_t));
	if (NULL== pthreadpool->worker){
		free(pthreadpool);
		return NULL;
	}
	memset(pthreadpool->worker, 0, (pthreadpool->worker_max_num)*sizeof(sfpr_threadpool_worker_t));
	pthreadpool->worker_busy_num = 0;

	/**<4. �����ʼ�� */
	pthreadpool->task_max_num = SFPR_THREAD_POOL_MAX_TASK_NUM;
	pthreadpool->task_pool_sizeof = (pthreadpool->task_max_num)*sizeof(sfpr_threadpool_task_t);
	pthreadpool->task_pool = (sfpr_threadpool_task_t*)malloc(pthreadpool->task_pool_sizeof);
	if (NULL== pthreadpool->task_pool){
		free(pthreadpool);
		free(pthreadpool->worker);
		return NULL;
	}
	memset(pthreadpool->task_pool, 0, pthreadpool->task_pool_sizeof);
	pthreadpool->task_count = 0;
	pthreadpool->task_head = 0;
	pthreadpool->task_tail = 0;
	memset(pthreadpool->task_queue, 0, sizeof(pthreadpool->task_queue));

	/**<3. �����̳߳ػ����� */
	sfpr_mutex_create(&pthreadpool->mutex, 0);	
	sfpr_cond_create(&pthreadpool->cond);
	
	return pthreadpool;
}

int sfpr_threadpool_set_mode(sfpr_threadpool_t *pthreadpool,int mode)
{
	if(!pthreadpool){
		return -1;
	}
	//����Ѿ�������ģʽ���Ͳ����ظ������ˡ�
	if(pthreadpool->mode != sfpr_threadpool_mode_none){
		return 1;
	}
	pthreadpool->mode = mode;
	return 0;
}

struct th_thread_param{
	sfpr_threadpool_t *threadpool;
	sfpr_threadpool_worker_t *worker;
};

static RET_THREAD tp_work_thread(void *param)
{
	sfpr_thread_id curid;
	struct th_thread_param *thread_param = (struct th_thread_param*)param;
	sfpr_threadpool_t *threadpool = NULL;
	sfpr_threadpool_worker_t *worker = NULL;
	sfpr_threadpool_task_t *task = NULL ;

	if(!thread_param || !thread_param->threadpool || !thread_param->worker){
		return NULL_THREAD;
	}
	threadpool = thread_param->threadpool;
	worker = thread_param->worker;

	/**<  ��ȡ��ǰ�߳�id*/
	curid = sfpr_thread_get_id();

	while(1){
		#ifdef LOG_NOTICE
		printf("tp_work_thread() | notice | Thread '%lu' Block ...\n", curid);
		#endif
		sfpr_mutex_lock(&(threadpool->mutex));
		while(threadpool->task_head == threadpool->task_tail){
			sfpr_cond_wait(&threadpool->cond,&threadpool->mutex);
		}
		task = threadpool->task_queue[threadpool->task_head++];
		threadpool->task_head = (threadpool->task_head == SFPR_THREAD_POOL_MAX_TASK_NUM)?0:threadpool->task_head;
		threadpool->task_count--;
		#ifdef LOG_NOTICE
		printf("tp_work_thread() | notice | Thread '%lu' Get Data\n", curid);
		#endif

		if(task->flags & SFPR_THREAD_POOL_FLAG_DELETE){
			if(threadpool->worker_cur_num > threadpool->worker_min_num){
				#ifdef LOG_NOTICE
				printf("tp_work_thread() | notice | Quit Thread\n");
				#endif
				threadpool->worker_cur_num--;
				sfpr_mutex_unlock(&(threadpool->mutex));
				break;
			}
		}
		threadpool->worker_busy_num++;
		worker->busy = 1;
		sfpr_mutex_unlock(&(threadpool->mutex));

		task->task_process(task);
		while(worker->task_head != worker->task_tail){
			sfpr_mutex_lock(&(threadpool->mutex));
			task = worker->task_queue[worker->task_head++];
			worker->task_head = (worker->task_head == SFPR_THREAD_POOL_WORKER_TASK_NUM)?0:worker->task_head;
			worker->task_count--;
			sfpr_mutex_unlock(&(threadpool->mutex));
			task->task_process(task);
		}

		sfpr_mutex_lock(&(threadpool->mutex));
		threadpool->worker_busy_num--;
		task->busy = 0;
		worker->busy = 0;
		memset(&(worker->key),0,sizeof(sfpr_threadpool_key_t));
		sfpr_mutex_unlock(&(threadpool->mutex));
	}

	#ifdef LOG_NOTICE
	fprintf(stdout, "tp_work_thread() | notice | destroy worker thread '%lu'\n",curid);
	#endif
	return NULL_THREAD;
}


static int sfpr_threadpool_thread_add(sfpr_threadpool_t *threadpool)
{
	int i;
	sfpr_threadpool_worker_t *new_thread = NULL;
	struct th_thread_param *param;

	if(threadpool->worker_cur_num  >= threadpool->worker_max_num){
		return SFPR_ERROR;
	}

	for(i=0;i<threadpool->worker_max_num;i++){
		if(!threadpool->worker[i].thread_id){
			new_thread = &threadpool->worker[i];
			break;
		}
	}
	if(!new_thread){
		return SFPR_ERROR;		
	}
	threadpool->worker_cur_num++;

	/**< �������߳�*/
	param = (struct th_thread_param*)malloc(sizeof(struct th_thread_param));
	if(!param){
		return SFPR_ERROR;			
	}
	param->threadpool = threadpool;
	param->worker = new_thread;
	new_thread->thread_id = sfpr_thread_create_ex(tp_work_thread,param,
					threadpool->name?threadpool->name:SFPR_THREAD_POOL_NAME_DEF,
					threadpool->stack_size,threadpool->priority);
	if(SFPR_ERROR == new_thread->thread_id){
		printf("threadpool_thread_add(): add work thread failed\n");
		return SFPR_ERROR;
	}

	#ifdef LOG_NOTICE
	printf("tp_manager_thread() | notice | add worker thread '%lu'\n", new_thread->thread_id);
	#endif
	
	return SFPR_SUCCESS;
}


static void task_delete_thread(sfpr_threadpool_task_t *this)
{
}

static RET_THREAD tp_manager_thread(void *param)
{
	sfpr_threadpool_t *threadpool = (sfpr_threadpool_t*)param;

	#ifdef LOG_DEBUG
	printf("tp_manager_thread() | debug | Thread '%lu' Begin...\n",sfpr_thread_get_id());
	#endif

	while(1){
		sfpr_time_msleep(SFPR_THREAD_POOL_MANAGER_INTERVAL);
		#ifdef LOG_NOTICE
		printf("tp_manager_thread() | notice | Task Number:%d  Busy Thread Number:%d  Total Thread Number:%d\n",
						threadpool->task_count,threadpool->worker_busy_num,threadpool->worker_cur_num);
		#endif

		//��ǰ����������æ���߳������������û������������
		if(threadpool->task_count == threadpool->worker_busy_num){
			threadpool->all_busy_time = 0;
			//�����̶߳�����
			if(threadpool->worker_busy_num == 0){
				if((threadpool->all_idle_time >= SFPR_THREAD_POOL_DEALY_TIME) && 
					(threadpool->worker_cur_num > threadpool->worker_min_num)){
					//ע�����ܴ��ⲿɱ���̣߳��п��ܳ���������
					//ֻ�����̴߳��ڲ��˳���
					sfpr_threadpool_task_t task;
					task.task_process = task_delete_thread;
					task.flags = SFPR_THREAD_POOL_FLAG_DELETE;
					sfpr_threadpool_submit(threadpool,&task);
				}
				threadpool->all_idle_time++;			
			}
		}
		//��ǰ����������æ���߳��������������������������ǰ�߳��Ѿ���������
		else if(threadpool->task_count > threadpool->worker_busy_num){
			threadpool->all_idle_time = 0;
			//�ۻ���һ���̶ȲŴ������߳�
			if(threadpool->all_busy_time >= SFPR_THREAD_POOL_DEALY_TIME){
				sfpr_threadpool_thread_add(threadpool);
			}
			threadpool->all_busy_time++;
		}
	}
	return NULL_THREAD;
}

int sfpr_threadpool_start(sfpr_threadpool_t *pthreadpool)
{
	char name[20];
	struct th_thread_param *param;
	int i;

	#ifdef LOG_NOTICE
	fprintf(stdout, "sfpr_threadpool_start() | notice | begin ...\n");
	#endif
	
	if (NULL == pthreadpool){
		#ifdef LOG_ERROR
		printf("threadpool_start:pthreadpool is null ! \n");
		#endif
		return SFPR_ERROR;
	}
	if(pthreadpool->started){
		return SFPR_ERROR;		
	}
	pthreadpool->started = 1;

	/**<3.���������̺߳ͳ�ʼ��thread_info�ṹ*/
	for(i=0;i<pthreadpool->worker_min_num;i++){
		param = (struct th_thread_param*)malloc(sizeof(struct th_thread_param));
		if(!param){
			pthreadpool->started = 0;
			return SFPR_ERROR;
		}
		param->threadpool = pthreadpool;
		param->worker = &(pthreadpool->worker[i]);
		pthreadpool->worker[i].thread_id = sfpr_thread_create_ex(tp_work_thread,param,
				pthreadpool->name?pthreadpool->name:SFPR_THREAD_POOL_NAME_DEF,
				pthreadpool->stack_size,pthreadpool->priority);		
		if(SFPR_ERROR == pthreadpool->worker[i].thread_id){
			#ifdef LOG_ERROR
			printf("sfpr_threadpool_start() | error | creat worker thread failed\n");
			#endif
			pthreadpool->started = 0;
			return SFPR_ERROR;
		}		
	}

	/**<4. ���������߳� */
	sprintf(name,"%sM",pthreadpool->name?pthreadpool->name:SFPR_THREAD_POOL_NAME_DEF);
	pthreadpool->manager_thread.thread_id = sfpr_thread_create_ex(tp_manager_thread, pthreadpool,
			name,pthreadpool->stack_size,pthreadpool->priority);
	if(SFPR_ERROR == pthreadpool->manager_thread.thread_id){
		#ifdef LOG_ERROR
		printf("sfpr_threadpool_start() | error | creat manager thread failed\n");
		#endif
		return SFPR_ERROR;
	}
	return SFPR_SUCCESS;
}


int sfpr_threadpool_close(sfpr_threadpool_t **ppthreadpool)
{
	int i;
	sfpr_threadpool_t *pthreadpool = NULL;
	sfpr_threadpool_task_t task;

	if (NULL == ppthreadpool){
		return SFPR_ERROR;
	}
	pthreadpool = *ppthreadpool;
	if (NULL == pthreadpool){
		return SFPR_ERROR;
	}

	/**< �رչ����߳�*/
	for(i=0;i<pthreadpool->worker_max_num;i++){
		task.task_process = task_delete_thread;
		task.argc = 0;
		task.flags = SFPR_THREAD_POOL_FLAG_DELETE;
		sfpr_threadpool_submit(pthreadpool,&task);
	}

  /**< �رչ����߳� �������̳߳���*/
	sfpr_thread_destory(pthreadpool->manager_thread.thread_id);

	sfpr_mutex_destroy(&pthreadpool->mutex);
	sfpr_cond_destroy(&pthreadpool->cond);
	#ifdef LOG_NOTICE
	printf("sfpr_threadpool_close() | notice | kill manager thread %d\n",
				pthreadpool->manager_thread.thread_id);
	#endif

	#ifdef LOG_NOTICE
	printf("sfpr_threadpool_close() | notice | Destroy Queue Success\n");
	#endif

	/**< �ͷ��̹߳���ṹ���ڴ�ռ�*/
	free(pthreadpool->worker);
	pthreadpool->worker = NULL;

	/**< �ͷ�����ؽṹ*/
	free(pthreadpool->task_pool);
	pthreadpool->task_pool = NULL;

	/**< �ͷ��̳߳�*/
	free(pthreadpool);
	*ppthreadpool = NULL;
	return SFPR_SUCCESS;	
}


static sfpr_threadpool_task_t *sfpr_threadpool_get_idle_task(sfpr_threadpool_t *pthreadpool)
{
	int i;
	sfpr_threadpool_task_t *task = NULL;

	for(i = 0;i <pthreadpool->task_max_num; i++){
		if(0 == pthreadpool->task_pool[i].busy){
			#ifdef LOG_DEBUG
			fprintf(stdout, "sfpr_threadpool_get_idle_task()  taskpool_id==%d\n",i);
			#endif
			return &(pthreadpool->task_pool[i]);
		}
	}
	return NULL;
}

static sfpr_threadpool_worker_t *sfpr_threadpool_get_matched_worker(
				sfpr_threadpool_t *pthreadpool,sfpr_threadpool_key_t *key)
{
	sfpr_threadpool_worker_t *worker = NULL;
	sfpr_threadpool_key_t empty;
	memset(&empty,0,sizeof(sfpr_threadpool_key_t));
	if(NULL == pthreadpool || 0 == memcmp(key,&empty,sizeof(sfpr_threadpool_key_t))){
		return NULL;
	}
	if(pthreadpool->mode == sfpr_threadpool_mode_none){
		return NULL;
	}else if(pthreadpool->mode == sfpr_threadpool_mode_socket){
		int i;
		for(i = 0;i <pthreadpool->worker_max_num; i++){
			if(pthreadpool->worker[i].busy && 
			0 == memcmp(&(pthreadpool->worker[i].key.sock),&(key->sock),sizeof(key->sock))){
				return &(pthreadpool->worker[i]);
			}
		}
		return NULL;
	}else{
		return NULL;
	}	
}

int sfpr_threadpool_submit(sfpr_threadpool_t *pthreadpool,sfpr_threadpool_task_t *task)
{
	int i,find = 0;
	sfpr_threadpool_task_t *task_idle = NULL;
	sfpr_threadpool_worker_t *worker = NULL;

	if(!pthreadpool || !task || !task->task_process){
		return -1;
	}

	#ifdef LOG_DEBUG
	fprintf(stdout, "sfpr_threadpool_submit() | notice | begin ...\n");
	#endif

	sfpr_mutex_lock(&pthreadpool->mutex);
	task_idle = sfpr_threadpool_get_idle_task(pthreadpool);
	if(!task_idle){
		#ifdef LOG_ERROR
		fprintf(stdout, "sfpr_threadpool_submit() | error | task pool is full\n");
		#endif
		sfpr_mutex_unlock(&(pthreadpool->mutex));
		return -1;
	}
	task_idle->busy = 1;
	task_idle->task_process = task->task_process;
	task_idle->flags = task->flags;
	task_idle->argc = task->argc;
	if(task_idle->argc >= SFPR_THREAD_POOL_TASK_MAX_ARGC || task_idle->argc < 0)
		task_idle->argc = 0;
	for(i=0;i<task_idle->argc;i++){
		task_idle->argv[i] = task->argv[i];
	}
	memcpy(&(task_idle->key),&(task->key),sizeof(sfpr_threadpool_key_t));

	#ifdef LOG_NOTICE
	fprintf(stdout, "threadpool_submit() | notice | sfpr_queue_put() | %s\n",task->argv[0]?task->argv[0]:"");
	#endif

	if(pthreadpool->mode != sfpr_threadpool_mode_none && (task->magic == SFPR_THREAD_POOL_KEY_MAGIC)){
		worker = sfpr_threadpool_get_matched_worker(pthreadpool,&(task->key));
		if(worker){
			find = 1;
			worker->task_queue[worker->task_tail++] = task_idle;
			worker->task_tail = (worker->task_tail == SFPR_THREAD_POOL_WORKER_TASK_NUM)?0:worker->task_tail;
			worker->task_count++;
			sfpr_mutex_unlock(&(pthreadpool->mutex));
			return worker->task_count;
		}
	}
	if(0 == find){
		pthreadpool->task_queue[pthreadpool->task_tail++] = task_idle;
		pthreadpool->task_tail = (pthreadpool->task_tail == SFPR_THREAD_POOL_MAX_TASK_NUM)?0:pthreadpool->task_tail;
		pthreadpool->task_count++;
		sfpr_cond_signal_all(&pthreadpool->cond);
		sfpr_mutex_unlock(&(pthreadpool->mutex));
		return pthreadpool->task_count;		
	}
}

char* sfpr_threadpool_get_name(sfpr_threadpool_t *threadpool)
{
	return threadpool->name;
}

#ifdef __cplusplus
}
#endif	

