
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sfpr_memory_pool.h"
#include "sfpr_string.h"

#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"


//#define TEST_BASIC_FUNC
#define TEST_PRESSURE


static int test_basic_func(int argc,char *argv[])
{
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	char buffer[1024];
	char *array[10];
	int count,i;
	
	sfpr_mem_keeper_t *keeper = sfpr_mem_pool_create();
	if(!keeper){
		fprintf(stderr,"error: sfpr_mem_pool_init failed\n");
		return 1;
	}

	memset(buffer,0,sizeof(buffer));

	while(1){
		printf("Please input command:");
		fgets(buffer,sizeof(buffer),stdin);
		if(!strncmp(buffer,"quit",4) || !strncmp(buffer,"q",1)){
			sfpr_mem_pool_destroy(&keeper);
			break;
		}
		buffer[strlen(buffer)-1] = 0;
		
		count = sfpr_str_split(buffer,array,10," \t\r\n");
//		for(i=0;i<count;i++){
//			printf("=====: %s\n",array[i]);
//		}
		if(!strcmp(array[0],"new") && count >= 2){
			sfpr_mem_pool_new(keeper,array[1]);
			sfpr_mem_pool_statall(keeper);
		}else if(!strcmp(array[0],"malloc") && count >= 3){
			sfpr_mem_pool_t *pool = sfpr_mem_pool_get(keeper,array[1]);
			if(!pool){
				fprintf(stderr,"error: cannt get pool '%s'\n",array[1]);
				continue;
			}else{
				sfpr_mem_malloc(pool,atoi(array[2]));
				printf("**********disp pool statuc.\r\n");
				sfpr_mem_pool_statall(keeper);
			}
		}else if(!strcmp(array[0],"free") && count >= 3){
			sfpr_mem_pool_t *pool = sfpr_mem_pool_get(keeper,array[1]);
			if(!pool){
				fprintf(stderr,"error: cannt free pool '%s'\n",array[1]);
				continue;
			}else{
				//sfpr_mem_free(pool,(void*)(atol(array[2])));
				printf("**********begin free\r\n");
				sfpr_mem_free(pool,(void*)(atol(array[2])));
				printf("**********disp pool statuc.\r\n");
				sfpr_mem_pool_statall(keeper);
			}
		}else if(!strcmp(array[0],"destroy_by_addr") && count >= 2){
				sfpr_mem_pool_delete(keeper,(sfpr_mem_pool_t*)(atol(array[1])));
				sfpr_mem_pool_statall(keeper);

		}
		else if(!strcmp(array[0],"destroy_by_name") && count >= 2){
				sfpr_mem_pool_delete_byname(keeper,array[1]);
				sfpr_mem_pool_statall(keeper);
		}else{
			fprintf(stderr,"error: unknown or wrong command '%s'\n",array[0]);
			memset(buffer,0,sizeof(buffer));
			//_CrtDumpMemoryLeaks();
			continue;		
		}

		memset(buffer,0,sizeof(buffer));	
	};
}

/**
 * param1: 分配多少内存为一次循环
 * param2: 最大分配内存大小
 */
static int test_pressure(int argc,char *argv[])
{
	int debug_num;
	int i;
	int base_value;
	int max_value;
	sfpr_uint32_t allocted[1024]; 
	sfpr_mem_pool_t *pool = NULL;
	sfpr_mem_keeper_t *keeper = NULL;

	if(argc < 3)
	{
		printf("please set malloc max and num!\n");
		return 1;
	}

	base_value = 256;
	max_value = atoi(argv[1]);
	debug_num = atoi(argv[2]);
	srand((unsigned)time(NULL));
	
	keeper = sfpr_mem_pool_create();
	if(!keeper)
	{
		fprintf(stderr,"error: sfpr_mem_pool_init failed\n");
		return 1;
	}

	pool = sfpr_mem_pool_new(keeper, "test");
	sfpr_mem_pool_statall(keeper);

	while(1)
	{
		for(i = 0; i< debug_num; i++)
		{
			int size = rand()%max_value;
			void *tmp = sfpr_mem_malloc(pool, size);

			printf("\n""\033[1;32m""****************************************************""\033[m""\nmalloc addr:%p, size:%d\n", tmp, size);
			sfpr_mem_pool_statall(keeper);

			allocted[i] = (sfpr_uint32_t)tmp;
			sfpr_time_msleep(1000);

		}

		for(i = 0; i< debug_num; i++)
		{
			printf("\n""\033[1;32m""****************************************************""\033[m""\nfree addr:%p\n", (void*)(allocted[i]));
			sfpr_mem_free(pool,(void*)(allocted[i]));
			sfpr_mem_pool_statall(keeper);
			sfpr_time_msleep(1000);
		}
		sfpr_time_msleep(2000);
	}
	sfpr_mem_pool_destroy(&keeper);
	return 0;
}

#if defined VXWORKS
int test_memory(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{

#ifdef 	TEST_BASIC_FUNC
	test_basic_func(argc, argv);
#endif

#ifdef TEST_PRESSURE
	test_pressure(argc, argv);
#endif
}

