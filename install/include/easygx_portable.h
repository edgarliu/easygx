
#ifndef EASYGX_PORTABLE_H
#define EASYGX_PORTABLE_H

#include "sfpr_stack.h"

#include "easygx.h"
#include "easygx_widget.h"
#include "easygx_control.h"
#include "easygx_message.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
 * 1. 基本函数
 ***********************************************************************************/
int easygx_init_(easygx_init_conf_t *init_conf);

int easygx_run_();

int easygx_quit_();

void easygx_set_quit_();

/************************************************************************************
 * 2. Widget函数
 ***********************************************************************************/
int egx_widget_set_struct_(egx_wnd_t hwnd,egx_widget_t *widget);

egx_widget_t *egx_widget_get_struct_(egx_wnd_t hwnd);

int egx_widget_set_title_(egx_wnd_t hwnd,char *title);

int egx_widget_set_parent_(egx_wnd_t hwnd,egx_wnd_t hparent);

egx_wnd_t egx_widget_get_parent_(egx_wnd_t hwnd);

int egx_widget_set_focus_(egx_wnd_t hwnd);

int egx_widget_update_(egx_wnd_t hwnd,int erase);

int egx_widget_update_rect_(egx_wnd_t hwnd, int x, int y, int width, int height,int erase);

int egx_widget_show_(egx_wnd_t hwnd);

int egx_widget_hide_(egx_wnd_t hwnd);

int egx_widget_enabled_(egx_wnd_t hwnd);

int egx_widget_disabled_(egx_wnd_t hwnd);

int egx_widget_is_enabled_(egx_wnd_t hwnd);

int egx_widget_get_rect_(egx_wnd_t hwnd,egx_rect_t *rect);

int egx_widget_set_rect(egx_widget_t *widget,int x,int y,int width,int height);

int egx_widget_set_pos_(egx_wnd_t hwnd,int x,int y);

int egx_widget_set_size_(egx_wnd_t hwnd,int width,int height);

/************************************************************************************
 * 3. Frame函数
 ***********************************************************************************/



/************************************************************************************
 * 4. Message函数
 ***********************************************************************************/
int egx_message_reg_notification_(egx_frame_t *frame,egx_control_t *control,egx_notification_e event);

unsigned int egx_message_reg_timer_(egx_wnd_t hwnd,unsigned int timer_id,int interval);


/************************************************************************************
 * 5. Window函数
 ***********************************************************************************/
egx_wnd_t egx_window_create_(char *name,egx_uint32_t style,int x,int y,int width,int height);

int egx_window_show_(egx_wnd_t hwnd);

int egx_window_hide_(egx_wnd_t hwnd);

int egx_window_close_(egx_wnd_t hwnd);

int egx_window_destroy_(egx_wnd_t hwnd);


egx_wnd_t egx_view_create_(egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent);

int egx_view_show_(egx_wnd_t hwnd);

int egx_view_hide_(egx_wnd_t hwnd);

/************************************************************************************
 * 6. Dialog函数
 ***********************************************************************************/


/************************************************************************************
 * 7. View函数
 ***********************************************************************************/


/************************************************************************************
 * 8. Control函数
 ***********************************************************************************/
//2. Label
egx_wnd_t egx_label_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent);

int egx_label_set_text_(egx_wnd_t hwnd,char *text);

//3. Button
int egx_button_enable_(egx_wnd_t hwnd,int enable);

int egx_button_checked_(egx_wnd_t hwnd);

egx_wnd_t egx_pushbutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent);

egx_wnd_t egx_checkbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent);

egx_wnd_t egx_groupbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent);

egx_wnd_t egx_radiobutton_begin_(egx_wnd_t parent);

egx_wnd_t egx_radiobutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent,egx_wnd_t groupbox);

//4. Edit
egx_wnd_t egx_edit_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent);

int egx_edit_get_text_(egx_wnd_t hwnd,char *text,int length);

int egx_edit_set_text_(egx_wnd_t hwnd,char *text);


/************************************************************************************
 * 9. 绘图函数
 ***********************************************************************************/
egx_painter_t egx_paint_begin_(egx_wnd_t hwnd);

int egx_paint_end_(egx_wnd_t hwnd,egx_painter_t painter);

int egx_draw_line_(egx_painter_t painter,int x0,int y0,int x1,int y1);

int egx_draw_ellipse_(egx_painter_t painter,int x0,int y0,int width,int height);



#ifdef __cplusplus
}
#endif

#endif
