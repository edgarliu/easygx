
#include <stdlib.h>
#include <string.h>

#include "easygx.h"
#include "easygx_core.h"
#include "easygx_widget.h"
#include "easygx_portable.h"

#include "easygx_private.h"

#ifdef __cplusplus
extern "C" {
#endif

int easygx_init_(easygx_init_conf_t *init_conf)
{
	egx_frame_init_();
	return 0;
}

int easygx_run_()
{
	easygx_t *easygx = easygx_instance_get();
	MSG msg;
	BOOL bRet;
	while(1){
		bRet = GetMessage(&msg, NULL, 0, 0);
		if(bRet == -1){
			
		}
		else if(bRet){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			if(sfpr_stack_get_count(easygx->showed_frame) == 0){
				break;
			}
		}
	}
	return 0;
}

int easygx_quit_()
{
	return 0;
}

void easygx_set_quit_()
{
	PostQuitMessage(0);	
}

#ifdef __cplusplus
}
#endif
