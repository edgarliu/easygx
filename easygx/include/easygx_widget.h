/**
 *\file easygx_widget.h
 *\brief easygx��װ�Ŀؼ��ṹ���弰��ؽӿ�
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
	egx_widget_unknown = 0,  /**<δ֪ */
	egx_widget_frame,/**<֡�ؼ� */
	egx_widget_control,/**<���ƿؼ� */
	egx_widget_extend/**<��չ�ؼ� */
}egx_widget_type_e;

/**
 *\brief �ؼ���λ�á���С��Ϣ
 */
typedef struct{
	int x;/**<x����*/
	int y; /**<y����*/
	int width; /**<���*/
	int height; /**<�߶�*/
}egx_rect_t;

typedef struct{
	int x;/**<x����*/
	int y; /**<y����*/
}egx_position_t;

typedef struct{
	int width; /**<���*/
	int height; /**<�߶�*/	
}egx_size_t;

/**
 *\brief ���������壬�����Ĺ�������(����)
 *char *name;  ��ǩ����
 *char *desc;  ��ǩ������һ��������ʾ�ڽ����ϣ����Զ�������ʾ
 *char *value;  ��ǩֵ
 *char *datatype;  ��ǩֵ����������
 *char *userdata;  �û�����
 *char *data;   �ڵ������
 *char *showinws  �Ƿ�ѵ�ǰ��ǩ��desc����ֵ��ʾ�����ڵ�״̬��
 *int bit  ���ڿ��������ã���¼�����ֵ�λ
 *int disable 1-���������; 0-�������
 *int col ��Ӧ�ڽ���������
 */
#define EGX_WIDGET \
	egx_widget_type_e widget_type;  /**<�ؼ�����*/\
	char            *id;/**<id��ʶ*/\
	char            *name;/**< ����*/\
	egx_wnd_t    object; /**<������*/\
	egx_rect_t    rect;/**<��������*/\
	sfel_node_t  *node/**< ��Ӧ�������ļ��ı�ǩ*/\

struct egx_widget_s
{
	EGX_WIDGET; /**<�ؼ�������*/
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

