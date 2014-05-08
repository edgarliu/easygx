/**
 *\file easygx_sfel.h
 *\brief easygx库sfel语言处理封装函数
 *\version 3.0.0
 *\date 2014.04.03
 */
#ifndef EASYGX_SFEL_H
#define EASYGX_SFEL_H

#include "sfel.h"

#ifdef __cplusplus
extern "C" {
#endif

int easygx_sfel_file_register(const char *sfel_file);

int easygx_sfel_file_show();

int easygx_sfel_file_unregister();

int easygx_sfel_create(sfel_node_t *node);

int easygx_sfel_show(sfel_node_t *node);

int easygx_sfel_destroy(sfel_node_t *node);


#ifdef __cplusplus
}
#endif

#endif
