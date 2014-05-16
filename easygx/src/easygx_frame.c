
#include <stdlib.h>
#include <string.h>

#include "sfpr_stack.h"

#include "easygx.h"
#include "easygx_core.h"
#include "easygx_frame.h"
#include "easygx_message.h"
#include "easygx_portable.h"
#include "easygx_log.h"
#include "easygx_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
 * 1. 公共部分
*****************************************************************************************/
int egx_frame_set_title(char *frame_id,char *title)
{
	egx_frame_t *frame = (egx_frame_t*)egx_widget_find(frame_id);
	if(NULL == frame || NULL == title){
		return -1;
	}
	return egx_frame_set_title_(frame->object,title);	
}

/*****************************************************************************************
 * 2. Window
*****************************************************************************************/
int egx_window_create(char *id,egx_window_type_e type,char *name,egx_uint32_t style,int x,int y,int width,int height)
{
	egx_window_t *_window=NULL;
	egx_rect_t rect;
	
	if(!id)
		return -1;
	_window = (egx_window_t *)malloc(sizeof(struct egx_window_s));
	if(!_window)
		return -2;
	memset(_window, 0x0, sizeof(struct egx_window_s));
	_window->id = egx_string_malloc(id);
	if(!_window->id){
		free(_window);
		return -3;
	}
	_window->widget_type = egx_widget_frame;
	_window->frame_type = egx_frame_window;
	_window->window_type = type;
	if(name){
		_window->name = egx_string_malloc(name);
	}
	_window->rect.x = x;
	_window->rect.y = y;
	_window->rect.width = width;
	_window->rect.height = height;
	_window->object = egx_window_create_(name,style,x,y,width,height);
	egx_widget_get_rect((egx_widget_t*)_window,&rect);
	sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_window_create() |  id:%s fd:%x  width:%d  height:%d\n",
		_window->id,_window->object,rect.width,rect.height);
	egx_widget_set_struct_(_window->object,(egx_widget_t*)_window);
	egx_widget_register((egx_widget_t*)_window);
	return 0;
}

int egx_window_show(char *id)
{
	easygx_t *easygx = easygx_instance_get();
	egx_window_t *window = (egx_window_t*)egx_widget_find(id);
	egx_window_t *top_window = NULL;
	egx_rect_t rect;
	int size,flag;

	if(!easygx || !window){
		return -1;
	}
	if(window->widget_type != egx_widget_frame || window->frame_type != egx_frame_window){
		return -2;
	}

	//如果窗口已经在栈顶了，就不用再入栈了。
	top_window = sfpr_stack_get_top_pointer(easygx->showed_frame,&size,&flag);
	if(top_window == window){
		return 0;
	}
	//隐藏栈顶窗口
	if(top_window){
		egx_widget_get_rect((egx_widget_t*)top_window,&rect);
		egx_window_hide_(top_window->object);
		egx_widget_set_rect((egx_widget_t*)window,rect.x,rect.y,rect.width,rect.height);
	}

	sfpr_stack_push(easygx->showed_frame,window,sizeof(egx_frame_t*),0);
	sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_window_show() | push: %s  stack count:%d\n",
			window->id,sfpr_stack_get_count(easygx->showed_frame));
	//如果是视图模式
	if(window->have_view){
		if(window->vs_type ==  egx_vs_type_stack){//栈模式
			if(0 == window->stack_top){
				window->stack_view[window->stack_top++] = window->head;				
			}
			egx_frame_set_title_(window->object,window->stack_view[window->stack_top-1]->name);
			egx_view_show_(window->stack_view[window->stack_top-1]->object);
		}else{//链表模式
			if(NULL == window->current_view){
				window->current_view = window->head;
			}
			egx_frame_set_title_(window->object,window->current_view->name);
			egx_view_show_(window->current_view->object);
		}
	}
	return egx_window_show_(window->object);
}

int egx_window_hide(char *id)
{
	easygx_t *easygx = easygx_instance_get();
	egx_window_t *window = NULL;
	egx_rect_t rect;
	int size;
	int flag;
	window = (egx_window_t*)egx_widget_find(id);
	if(!easygx || !window){
		return -1;
	}
	if(window->widget_type != egx_widget_frame || window->frame_type != egx_frame_window){
		return -2;
	}
	egx_widget_get_rect((egx_widget_t*)window,&rect);
	egx_window_hide_(window->object);
	sfpr_stack_pop_pointer(easygx->showed_frame,&size,&flag);
	sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_window_hide() | pop: %s\n",window->id);
	if(sfpr_stack_get_count(easygx->showed_frame) > 0){
		egx_window_t *top_window = sfpr_stack_get_top_pointer(easygx->showed_frame,&size,&flag);
		egx_widget_set_rect((egx_widget_t*)top_window,rect.x,rect.y,rect.width,rect.height);
		egx_window_show_(top_window->object);
	}else{
		easygx_set_quit();
	}
	return 0;
}

int egx_window_destroy(char *id)
{
	egx_window_t *window = NULL;
	window = (egx_window_t*)egx_widget_find(id);
	if(!window){
		return -1;
	}
	if(window->widget_type != egx_widget_frame || window->frame_type != egx_frame_window){
		return -2;
	}
	egx_window_destroy_(window->object);
	free(window);
	return 0;
}

int egx_window_set_stack(char *id)
{
	egx_window_t *window = NULL;
	window = (egx_window_t*)egx_widget_find(id);
	if(!window){
		return -1;
	}
	sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_window_set_stack() | window:%s\n",window->id);
	window->vs_type = egx_vs_type_stack;
	return 0;
}

/*****************************************************************************************
 * 3. Dialog
*****************************************************************************************/
int egx_dialog_create(char *id,egx_dialog_type_e type,char *name,egx_uint32_t style,int x,int y,int width,int height)
{
	egx_dialog_t *_dialog=NULL;
	egx_rect_t rect;

	if(!id)
		return -1;
	_dialog = (egx_dialog_t *)malloc(sizeof(struct egx_dialog_s));
	if(!_dialog)
		return -2;
	memset(_dialog, 0x0, sizeof(struct egx_dialog_s));
	_dialog->id = egx_string_malloc(id);
	if(!_dialog->id){
		free(_dialog);
		return -3;
	}
	_dialog->widget_type = egx_widget_frame;
	_dialog->frame_type = egx_frame_dialog;
	_dialog->dialog_type = type;
	if(name){
		_dialog->name = egx_string_malloc(name);
	}
	_dialog->rect.x = x;
	_dialog->rect.y = y;
	_dialog->rect.width = width;
	_dialog->rect.height = height;
	_dialog->object = egx_dialog_create_(name,style,x,y,width,height);
	egx_widget_get_rect((egx_widget_t*)_dialog,&rect);
	sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_dialog_create() |  id:%s fd:%x  width:%d  height:%d\n",
		_dialog->id,_dialog->object,rect.width,rect.height);
	egx_widget_set_struct_(_dialog->object,(egx_widget_t*)_dialog);
	egx_widget_register((egx_widget_t*)_dialog);
	return 0;
}

int egx_dialog_show(char *id)
{
	easygx_t *easygx = easygx_instance_get();
	egx_dialog_t *dialog = NULL;
	egx_window_t *top_window = NULL;
	int size,flag;

	dialog = (egx_dialog_t*)egx_widget_find(id);
	if(!dialog){
		return -1;
	}
	top_window = sfpr_stack_get_top_pointer(easygx->showed_frame,&size,&flag);
	if(top_window){
		egx_widget_disabled((egx_widget_t*)top_window);
	}
	return egx_dialog_show_(dialog->object);
}

int egx_dialog_hide(char *id)
{
	easygx_t *easygx = easygx_instance_get();
	egx_dialog_t *dialog = NULL;
	egx_window_t *top_window = NULL;
	int size,flag;

	dialog = (egx_dialog_t*)egx_widget_find(id);
	if(!dialog){
		return -1;
	}
	top_window = sfpr_stack_get_top_pointer(easygx->showed_frame,&size,&flag);
	if(top_window){
		egx_widget_enabled((egx_widget_t*)top_window);
	}
	return egx_dialog_hide_(dialog->object);
}

int egx_dialog_destroy(char *id)
{
	egx_dialog_t *dialog = NULL;
	dialog = (egx_dialog_t*)egx_widget_find(id);
	if(!dialog){
		return -1;
	}
	egx_dialog_destroy_(dialog->object);
	free(dialog);
	return 0;
}

/*****************************************************************************************
 * 4. View
*****************************************************************************************/
int egx_view_create(char *view_id,egx_view_type_e type,char *name,char *parent_id)
{
	egx_view_t *_view=NULL;
	egx_window_t *parent = (egx_window_t*)egx_widget_find(parent_id);
	egx_rect_t rect;
	
	if(!view_id || !parent)
		return -1;
	_view = (egx_view_t *)malloc(sizeof(struct egx_view_s));
	if(!_view)
		return -2;
	memset(_view, 0x0, sizeof(struct egx_view_s));
	_view->id = egx_string_malloc(view_id);
	if(!_view->id){
		free(_view);
		return -3;
	}
	_view->widget_type = egx_widget_frame;
	_view->frame_type = egx_frame_view;
	_view->view_type = type;
	if(name){
		_view->name = egx_string_malloc(name);
	}
	_view->cur_window = parent;

	egx_widget_get_client_rect((egx_widget_t*)parent,&rect);
	_view->rect.x = 0;
	_view->rect.y = 0;
	_view->rect.width = rect.width;
	_view->rect.height = rect.height;
	
	_view->object = egx_view_create_(0,rect.x,rect.y,rect.width,rect.height,(parent)?(parent->object):0);
	egx_view_hide_(_view->object);
	egx_widget_set_struct_(_view->object,(egx_widget_t*)_view);
	egx_widget_register((egx_widget_t*)_view);

	if(NULL == parent->head){
		parent->head = _view;
		parent->tail = _view;
		_view->prep = parent->tail;
		_view->next = parent->head;
	}else{
		_view->prep = parent->tail;
		_view->next = parent->head;
		parent->tail->next = _view;
		parent->head->prep = _view;
		parent->tail = _view;
	}
	parent->have_view = 1;
	
	egx_widget_get_rect((egx_widget_t*)_view,&rect);
	sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_view_create() |  id:%s fd:%x  width:%d  height:%d\n",
		_view->id,_view->object,rect.width,rect.height);
	return 0;
}

int egx_view_show(char *view_id)
{
	egx_view_t *view = (egx_view_t*)egx_widget_find(view_id);
	egx_window_t *window = NULL;
	if(!view){
		return -1;
	}
	window = view->cur_window;
	if(!window){
		return -2;
	}
	if(window->vs_type == egx_vs_type_stack){//栈模式
		int hided = 0;
		if(window->stack_top >= EGX_WINDOW_MAX_STACK){
			return -3;
		}
		if(window->stack_top>0){
			egx_view_t *top_view = window->stack_view[window->stack_top-1];
			if(top_view == view){
				return 0;
			}
			hided = 1;
			sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_view_show() | hide: %s  show:%s",top_view->id,view->id);
			egx_view_hide_(top_view->object);			
		}
		//入栈
		window->stack_view[window->stack_top++] = view;
		if(hided == 0)
			sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_view_show() | show: %s  stack  count: %d",view->id,window->stack_top);
	}else{
		int hided = 0;
		if(window->current_view){
			hided = 1;
			sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_view_show() | hide: %s show:%s",
				window->current_view->id,view->id);
			egx_view_hide_(window->current_view->object);
		}
		window->current_view = view;
		if(hided == 0)
			sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_view_show() | show: %s",view->id);
	}
	egx_widget_set_focus_(view->object);
	egx_view_show_(view->object);
	egx_frame_set_title_(window->object,view->name);
	egx_window_show(window->id);
	return 0;
}

int egx_view_hide(char *view_id,egx_view_action_e action)
{
	egx_view_t *view = (egx_view_t*)egx_widget_find(view_id);
	egx_window_t *window = NULL;
	egx_view_t *new_view = NULL;
	if(!view){
		return -1;
	}
	window = view->cur_window;
	if(!window){
		return -2;
	}
	if(window->vs_type == egx_vs_type_stack){//栈模式
		if(action == egx_view_action_pop){//从栈顶弹出
			if(window->stack_top <= 1){
				sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_view_hide() | hide window: %s",window->id);
				egx_window_hide(window->id);
				//window->current = NULL;
				return 0;
			}else{
				//如果是栈顶视图，则弹出栈顶视图，然后再显示当前栈顶视图，
				//如果不是栈顶视图，则隐藏当前视图，然后显示栈顶视图
				egx_view_t *top_view = window->stack_view[window->stack_top-1];
				if(top_view == view){
					window->stack_top--;
					new_view = window->stack_view[window->stack_top-1];					
				}else{
					new_view = top_view;
				}
			}
		}else{
			return -3;
		}
	}else{
		if(action == egx_view_action_next){
			new_view = view->next;
		}else if(action == egx_view_action_prep){
			new_view = view->prep;
		}else{
			return -4;
		}
		window->current_view = new_view;
	}
	sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_view_hide() | hide:%s  show:%s\n",view->id,new_view->id);
	egx_view_hide_(view->object);
	egx_frame_set_title_(window->object,new_view->name);
	egx_view_show_(new_view->object);
	return 0;
}

int egx_view_pop(char *window_id)
{
	egx_window_t *window = (egx_window_t*)egx_widget_find(window_id);
	if(!window){
		return -1;
	}
//	if(window->)
	return 0;
}

int egx_view_destroy(char *view_id)
{

	return 0;
}


#ifdef __cplusplus
}
#endif
