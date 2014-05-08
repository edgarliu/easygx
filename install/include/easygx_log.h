/**
 *\file easygx_log.h
 *\brief easygxÈÕÖ¾Ä£¿é
 *\version 0.1.0
 *\date 2012
 */
#ifndef EASYGX_LOG_H
#define EASYGX_LOG_H

#include "sfpr_log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern sfpr_log_t *easygx_log;

int easygx_log_set(sfpr_log_t *log);

int easygx_log_init(char *path,int level);

#ifdef __cplusplus
}
#endif

#endif

