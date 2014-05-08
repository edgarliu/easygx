#include <stdio.h>
#include "sfpr_stack.h"


#ifdef __cplusplus
extern "C" {
#endif

#if defined VXWORKS
int time_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	int ret;
	sfpr_stack_t *stack;
	sfpr_stack_node_t *node;
	//void *result;
	int result;
	int *tmp;

#if 0	
	char *one = "one testing.";
	printf("one :%p\n", one);
	char *two = "two testing.";
	char *three = "three testing.";
#endif

#if 0	
	char *one = malloc(128);
	strcpy(one, "one testing.");
	char *two = malloc(128);
	strcpy(two, "two testing.");
	char *three = malloc(128);
	strcpy(three, "three testing.");
#endif

#if 1	
	int one = 1;
	int two = 2;
	int three = 3;
#endif

    //创建栈
	ret = sfpr_stack_create(&stack, 4096,sfpr_stack_mode_data);
	if(ret < 0)
	{
		return -1;
	}

#if 1
	//整型
	sfpr_stack_push(stack, &one, sizeof(int), 0);
	sfpr_stack_push(stack, &two, sizeof(int), 0);
	sfpr_stack_push(stack, &three, sizeof(int), 0);

	//printf("result 3:%d\n", result);
#endif	
    //销毁栈
	sfpr_stack_destroy(&stack);
	return 0;
}

#ifdef __cplusplus
}
#endif
