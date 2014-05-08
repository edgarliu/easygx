/**
 *\file easygx_control.h
 *\brief easygx 封装的控件结构定义及相关接口
 *\version 0.1.0
 *\date 2012
 */
#ifndef EASYGX_CONTROL_H
#define EASYGX_CONTROL_H

#include <string.h>

#include "easygx.h"
#include "easygx_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
 * 1. 公共部分
*****************************************************************************************/
typedef enum
{
	egx_control_type_unknown = 0,  /**<窗口控件 */
	egx_control_type_button,  /**<按键控件*/
	egx_control_type_checkbox,  /**< 复选框控件*/
	egx_control_type_groupbox, /**< 控件组*/
	egx_control_type_radiobutton,  /**< 单选框控件*/
	egx_control_type_edit,  /**<编辑控件 */
	egx_control_type_multiedit,  /**<多行编辑框控件 */
	egx_control_type_listbox,  /**<列表框控件 */
	egx_control_type_combox,  /**<组合框控件 */
	egx_control_type_label,  /**< 标签控件*/
	egx_control_type_textarea,/**< 文本区域控件*/
	egx_control_type_list,  /**<列表控件 */
	egx_control_type_table,  /**<表格控件 */
	egx_control_type_progressbar,  /**<进度栏控件 */
	egx_control_type_datetime,  /**<日期时间控件 */
	egx_control_type_ipaddr,  /**<ip控件 */
	egx_control_type_image,
	egx_control_type_statusbar
}egx_control_type_e;

#define EGX_STYLE_GROUP	0x00000001

#define EASYGX_CONTROL_X_DEFAULT -1000   /**<控件默认x坐标 */
#define EASYGX_CONTROL_Y_DEFAULT -1000 /**<控件默认y坐标 */

#define EASYGX_CONTROL_WIDTH_DEFAULT 100 /**<控件默认宽度*/
#define EASYGX_CONTROL_HEIGHT_DEFAULT 20 /**<控件默认高度 */

#define EGX_CONTROL  \
	EGX_WIDGET;\
	egx_control_type_e control_type; \
	int res_id; \
	egx_frame_t *cur_frame

struct egx_control_s
{
	EGX_CONTROL;
};

typedef enum
{
	egx_extend_type_hex,  /**<十六进制控件*/
	egx_extend_type_maing,  /**<接线图控件 */
	egx_extend_type_gbkedit/**<汉字控件 */
}egx_extend_type_e;



/*****************************************************************************************
 * 2. Label
*****************************************************************************************/
typedef struct
{
	EGX_CONTROL;
}egx_label_t;

int egx_label_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id);

int egx_label_set_text(char *id,char *data);

/*****************************************************************************************
 * 3. Button
*****************************************************************************************/
int egx_button_enable(char *id,int enable);

int egx_button_checked(char *id);

typedef struct
{
	EGX_CONTROL;
}egx_button_t;

int egx_pushbutton_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id);

typedef struct
{
	EGX_CONTROL;
}egx_checkbox_t;

int egx_checkbox_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id);

typedef struct
{
	EGX_CONTROL;
}egx_groupbox_t;

int egx_groupbox_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id);

typedef struct
{
	EGX_CONTROL;
}egx_radiobutton_t;

int egx_radiobutton_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id);

/*****************************************************************************************
 * 4. Edit
*****************************************************************************************/
typedef struct
{
	EGX_CONTROL;
}egx_edit_t;

int egx_edit_create(char *id,int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,char *parent_id);

int egx_edit_get_text(char *id,char *data,int length);

int egx_edit_set_text(char *id,char *data);

#ifdef __cplusplus
}
#endif

#endif

