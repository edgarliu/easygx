/**
 *\file easygx_widget.h
 *\brief easygx封装的控件结构定义及相关接口
 *\version 0.1.0
 *\date 2012
 */
#ifndef EASYGX_WIDGET_H
#define EASYGX_WIDGET_H

#include <string.h>

#include "sfel_xml.h"

#include "easygx.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	egx_widget_unknown = 0,  /**<未知 */
	egx_widget_frame,/**<帧控件 */
	egx_widget_control,/**<控制控件 */
	egx_widget_extend/**<扩展控件 */
}egx_widget_type_e;

/**
 *\brief 控件的位置、大小信息
 */
typedef struct{
	int x;/**<x坐标*/
	int y; /**<y坐标*/
	int width; /**<宽度*/
	int height; /**<高度*/
}egx_rect_t;

typedef struct{
	int x;/**<x坐标*/
	int y; /**<y坐标*/
}egx_position_t;

typedef struct{
	int width; /**<宽度*/
	int height; /**<高度*/	
}egx_size_t;

/**
 *\brief 部件对象定义，部件的公共部分(属性)
 *char *name;  标签名称
 *char *desc;  标签描述，一般用来显示在界面上，可以多语言显示
 *char *value;  标签值
 *char *datatype;  标签值的数据类型
 *char *userdata;  用户数据
 *char *data;   节点的内容
 *char *showinws  是否把当前标签的desc属性值显示到窗口的状态栏
 *int bit  用于控制字设置，记录控制字的位
 *int disable 1-不允许操作; 0-允许操作
 *int col 对应于焦点管理的列
 */
#define EGX_WIDGET \
	egx_widget_type_e widget_type;  /**<控件类型*/\
	char            *id;/**<id标识*/\
	char            *name;/**< 名称*/\
	egx_wnd_t    object; /**<对象句柄*/\
	egx_rect_t    rect;/**<几何属性*/\
	sfel_node_t  *node/**< 对应于配置文件的标签*/\

struct egx_widget_s
{
	EGX_WIDGET; /**<控件对象定义*/
};


int egx_widget_set_parent(egx_widget_t *widget,egx_widget_t *parent);

egx_widget_t *egx_widget_get_parent(egx_widget_t *widget);

int egx_widget_update(egx_widget_t *widget,int erase);

int egx_widget_update_rect(egx_widget_t *widget, int x, int y, int width, int height,int erase);

int egx_widget_show(egx_widget_t *widget);

int egx_widget_hide(egx_widget_t *widget);

int egx_widget_enabled(egx_widget_t *widget);

int egx_widget_disabled(egx_widget_t *widget);

int egx_widget_is_enabled(egx_widget_t *widget);

int egx_widget_set_focus(egx_widget_t *widget);

int egx_widget_set_pos(egx_widget_t *widget,int x,int y);

int egx_widget_set_size(egx_widget_t *widget,int width,int height);

int egx_widget_set_rect(egx_widget_t *widget,int x,int y,int width,int height);

int egx_widget_get_rect(egx_widget_t *widget,egx_rect_t *rect);

int egx_widget_get_client_rect(egx_widget_t *widget,egx_rect_t *rect);


#ifdef __cplusplus
}
#endif

#endif

