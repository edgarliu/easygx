/**
 *\file easygx_private.h
 *\brief 各个图形库的私有头文件
 *\version 0.1.0
 *\date 2012
 */

#ifndef EASYGX_PRIVATE_H
#define EASYGX_PRIVATE_H

#include "easygx.h"

#include "windows.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HWND_TO_GUIWND(object) (egx_wnd_t)(object)

#define GUIWND_TO_HWND(object) (HWND)((egx_uint32_t)(object))

int egx_frame_init_();

#ifdef __cplusplus
}
#endif

#endif

