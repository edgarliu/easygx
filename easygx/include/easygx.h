/**
 *\file easygx.h
 *\brief easygx��װ�ӿڵķ�װ������
 *\version 3.0.0
 *\date 2014.04.03
 */
#ifndef EASYGX_H
#define EASYGX_H

#include "sfpr_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EASYGX_ID_USER 0x800

#define EASYGX_ID_CONTROL (EASYGX_ID_USER + 0x400)

/**
 *\brief easygx Ӧ�ýṹ����
 */
typedef struct easygx_s easygx_t;

typedef struct egx_widget_s egx_widget_t;

typedef struct egx_frame_s egx_frame_t;

typedef struct egx_window_s egx_window_t;

typedef struct egx_dialog_s egx_dialog_t;

typedef struct egx_view_s egx_view_t;

typedef struct egx_control_s egx_control_t;

typedef unsigned long egx_uint32_t;

typedef egx_uint32_t egx_wnd_t;

typedef egx_uint32_t egx_painter_t;

typedef struct
{
	char font[32];
	char error[64];
}easygx_init_conf_t;


/**
 *\brief ��������ʼ��easygx_t���ݽṹ
 *\param[in] res_path easygx������ļ������Ŀ¼����Ϊ����Ϊ��ǰĿ¼��
 *\return 0 �ɹ��� -1 ʧ��
 */
int easygx_init(easygx_init_conf_t *init_conf);

int easygx_run();

int easygx_quit();


easygx_t *easygx_instance_get();

const char*easygx_name_get();

void easygx_set_quit();

int easygx_is_quit();

int easygx_application_set(void *application);

void* easygx_application_get();


int egx_widget_register(egx_widget_t *widget);

egx_widget_t *egx_widget_find(const char *id);

void egx_widget_enum();


#ifdef __cplusplus
}
#endif

#endif
