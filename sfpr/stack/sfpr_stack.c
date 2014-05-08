/** 
 *@brief 动态顺序栈，即栈的动态顺序存储实现  
 */ 
#include "sfpr_stack.h"  
#ifdef __cplusplus
extern "C" {
#endif

struct sfpr_stack_s 
{  
    char *bottom;             /**<栈内存空间起始地址*/
    char *top;             /**< 栈顶, 指向当前可用地址*/
    int stacksize;          /**<栈提供的内存最大值*/
    int mode;            /**<0:使用栈内存; 1:用户分配内存*/
    int count;           /**<个数*/
};  

int sfpr_stack_create(sfpr_stack_t **stack, int size, int mode)
{  
	sfpr_stack_t *_stack = NULL;

	if(mode != sfpr_stack_mode_data && mode != sfpr_stack_mode_pointer){
		return -1;
	}

	_stack = malloc(sizeof(sfpr_stack_t));
	if(!_stack){
		return SFPR_ERROR;
	}
	memset(_stack,0,sizeof(sfpr_stack_t));
	
	if(size <= 0){
		size = 4096;
	}

    _stack->bottom = malloc(size); 
	if(!_stack->bottom){
		sfpr_stack_destroy(&_stack);
		return SFPR_ERROR;
	}
	memset(_stack->bottom,0,size);

    _stack->top = _stack->bottom;
	_stack->mode = mode;

    _stack->stacksize = size;  
	_stack->count = 0;
	*stack = _stack;
    return SFPR_SUCCESS;  
}  

void sfpr_stack_destroy(sfpr_stack_t **stack)
{  
	sfpr_stack_t *_stack = *stack;
    free(_stack->bottom);  
	free(_stack);
    *stack = NULL;  
}  

int sfpr_stack_is_empty(sfpr_stack_t *stack)
{
    return (stack->top - stack->bottom) == stack->stacksize;  
}  
  
int sfpr_stack_get_free_size(sfpr_stack_t *stack)
{  
    return stack->stacksize - (stack->top - stack->bottom);  
}  
   
int sfpr_stack_get_count(sfpr_stack_t *stack)
{
	return stack->count;
}

int sfpr_stack_push(sfpr_stack_t *stack, void *data, int size, int flag)
{
	sfpr_stack_node_t *top_node = NULL;
	
    if(stack == NULL)  
        return SFPR_ERROR;  
   
	if(stack->mode == sfpr_stack_mode_data){
	    memcpy(stack->top, data, size);
		stack->top += size;
		top_node = (sfpr_stack_node_t*)stack->top;
		top_node->size = size;
		top_node->flag = flag;
		top_node->data = stack->top-size;
	}else{
		top_node = (sfpr_stack_node_t*)stack->top;
		top_node->size = 0;
		top_node->flag = flag;
		top_node->data = data;
	}
	stack->top += SFPR_STACK_NODE_SIZE;
	stack->count ++;
    return SFPR_SUCCESS; 
}

void *sfpr_stack_pop_pointer(sfpr_stack_t *stack, int *size, int *flag)
{  
    sfpr_stack_node_t *top_node;
	if(stack->mode == sfpr_stack_mode_data)
		return NULL;
	if(stack->count <= 0)
		return NULL;
    top_node =  (sfpr_stack_node_t*)(stack->top - SFPR_STACK_NODE_SIZE);
	*size = top_node->size;
	*flag = top_node->flag;
	stack->top -= SFPR_STACK_NODE_SIZE;
	stack->count --;
    return top_node->data;
} 

void *sfpr_stack_get_top_pointer(sfpr_stack_t *stack, int *size, int *flag)
{
    sfpr_stack_node_t *top_node;
	if(stack->mode == sfpr_stack_mode_data)
		return NULL;
	if(stack->count <= 0)
		return NULL;
    top_node =  (sfpr_stack_node_t*)(stack->top - SFPR_STACK_NODE_SIZE);
	*size = top_node->size;
	*flag = top_node->flag;
    return top_node->data;	
}

void *sfpr_stack_get_bottom_pointer(sfpr_stack_t *stack, int *size, int *flag)
{
    sfpr_stack_node_t *top_node;
	if(stack->mode == sfpr_stack_mode_data)
		return NULL;
	if(stack->count <= 0)
		return NULL;
    top_node =  (sfpr_stack_node_t*)(stack->bottom);
	*size = top_node->size;
	*flag = top_node->flag;
    return top_node->data;	
}

int sfpr_stack_pop_data(sfpr_stack_t *stack, void *data, int *size, int *flag)
{
    sfpr_stack_node_t *top_node;
	if(stack->mode == sfpr_stack_mode_pointer){
		return SFPR_ERROR;
	}
	if(stack->count <= 0)
		return -1;
    top_node =  (sfpr_stack_node_t*)(stack->top - SFPR_STACK_NODE_SIZE);
	memcpy(data, top_node->data, top_node->size);
	*size = top_node->size;
	*flag = top_node->flag;
	stack->top -= SFPR_STACK_NODE_SIZE;
	stack->top -= top_node->size;
	stack->count--;
	return SFPR_SUCCESS;
}

int sfpr_stack_clear(sfpr_stack_t *stack, void(*delete_cb)(sfpr_stack_node_t *node))
{
	int index;
    sfpr_stack_node_t *top_node;
	if(stack->count <= 0)
		return -1;

	while(stack->count){
	    top_node =  (sfpr_stack_node_t*)(stack->top - SFPR_STACK_NODE_SIZE);
		if(delete_cb){
			delete_cb(top_node);
		}
		stack->top -= SFPR_STACK_NODE_SIZE;
		if(!stack->mode){
			stack->top -= top_node->size;
		}
		stack->count --;
	}
	return SFPR_SUCCESS;
}

#ifdef __cplusplus
}
#endif
