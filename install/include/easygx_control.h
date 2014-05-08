/**
 *\file easygx_control.h
 *\brief easygx ��װ�Ŀؼ��ṹ���弰��ؽӿ�
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
 * 1. ��������
*****************************************************************************************/
typedef enum
{
	egx_control_type_unknown = 0,  /**<���ڿؼ� */
	egx_control_type_button,  /**<�����ؼ�*/
	egx_control_type_checkbox,  /**< ��ѡ��ؼ�*/
	egx_control_type_groupbox, /**< �ؼ���*/
	egx_control_type_radiobutton,  /**< ��ѡ��ؼ�*/
	egx_control_type_edit,  /**<�༭�ؼ� */
	egx_control_type_multiedit,  /**<���б༭��ؼ� */
	egx_control_type_listbox,  /**<�б��ؼ� */
	egx_control_type_combox,  /**<��Ͽ�ؼ� */
	egx_control_type_label,  /**< ��ǩ�ؼ�*/
	egx_control_type_textarea,/**< �ı�����ؼ�*/
	egx_control_type_list,  /**<�б�ؼ� */
	egx_control_type_table,  /**<���ؼ� */
	egx_control_type_progressbar,  /**<�������ؼ� */
	egx_control_type_datetime,  /**<����ʱ��ؼ� */
	egx_control_type_ipaddr,  /**<ip�ؼ� */
	egx_control_type_image,
	egx_control_type_statusbar
}egx_control_type_e;

#define EGX_STYLE_GROUP	0x00000001

#define EASYGX_CONTROL_X_DEFAULT -1000   /**<�ؼ�Ĭ��x���� */
#define EASYGX_CONTROL_Y_DEFAULT -1000 /**<�ؼ�Ĭ��y���� */

#define EASYGX_CONTROL_WIDTH_DEFAULT 100 /**<�ؼ�Ĭ�Ͽ��*/
#define EASYGX_CONTROL_HEIGHT_DEFAULT 20 /**<�ؼ�Ĭ�ϸ߶� */

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
	egx_extend_type_hex,  /**<ʮ�����ƿؼ�*/
	egx_extend_type_maing,  /**<����ͼ�ؼ� */
	egx_extend_type_gbkedit/**<���ֿؼ� */
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

