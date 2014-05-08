/**
 *\file easygx_private.h
 *\brief 各个图形库的私有头文件
 *\version 0.1.0
 *\date 2012
 */

#ifndef EASYGX_PRIVATE_H
#define EASYGX_PRIVATE_H

#include "ucgui/GUI.h"
#include "ucgui/WM.h"
#include "ucgui/EDIT.h"
#include "ucgui/BUTTON.h"
#include "ucgui/BUTTON_Private.h"
#include "ucgui/TEXT.h"
#include "ucgui/LISTVIEW.h"
#include "ucgui/DROPDOWN.h"
#include "ucgui/LISTBOX.h"
#include "ucgui/PROGBAR.h"
#include "ucgui/CHECKBOX.h"
#include "ucgui/RADIO.h"
#include "ucgui/MULTIEDIT.h"
#include "ucgui/HEXEDIT.h"
#include "ucgui/IPADDR.h"
#include "ucgui/RADIO_Private.h"
#include "ucgui/DRAGON.h"
#include "ucgui/IMAGE.h"
#include "ucgui/STATUSBAR.h"

#include "easygx.h"
#include "easygx_widget.h"

#define HWND_TO_GUIWND(object) (egx_wnd_t)((egx_uint32_t)(object))

#define GUIWND_TO_HWND(object) (WM_HMEM)(((egx_uint32_t)(object)) & 0x0000ffff)

int egx_frame_init_();

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif

