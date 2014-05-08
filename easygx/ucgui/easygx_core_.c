
#include <stdlib.h>
#include <string.h>

#include "easygx_private.h"
#include "easygx.h"
#include "easygx_frame.h"

#ifdef __cplusplus
extern "C" {
#endif

extern GUI_FONT GUI_FontHZ16;
extern GUI_FONT GUI_FontHZ12;

GUI_FONT *easygx_font = &GUI_FontHZ12;

int easygx_init_(easygx_init_conf_t *init_conf)
{
	GUI_Init();
	WM_SetDesktopColor(GUI_WHITE);//GUI_GRAY);
	WM_SetCreateFlags(WM_CF_MEMDEV);
//	if(0 == strcmp(font_name,"HZ12")){
		easygx_font = &GUI_FontHZ12;
//	}
	GUI_SetFont(easygx_font); 
	FRAMEWIN_SetDefaultFont(easygx_font);
	MENU_SetDefaultFont(easygx_font);
	BUTTON_SetDefaultFont(easygx_font);
	TEXT_SetDefaultFont(easygx_font);
	EDIT_SetDefaultFont(easygx_font);
	STATUSBAR_SetDefaultFont(easygx_font);
	RADIO_SetDefaultFont(easygx_font);
	LISTVIEW_SetDefaultFont(easygx_font);
	HEADER_SetDefaultFont(easygx_font);
	CHECKBOX_SetDefaultFont(easygx_font);
	LISTBOX_SetDefaultFont(easygx_font);
	DROPDOWN_SetDefaultFont(easygx_font);
	MULTIEDIT_SetDefaultFont(easygx_font);
	
	egx_frame_init_();
	
	return 0;
}

int easygx_run_()
{
	while(!easygx_is_quit()){
		GUI_Delay(100);
	}
	return 0;
}

int easygx_quit_()
{
	GUI_Clear();
	return 0;
}

void easygx_set_quit_()
{
}

#ifdef __cplusplus
}
#endif
