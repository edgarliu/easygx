
#ifndef SFPR_LOG_H
#define SFPR_LOG_H

#include <stdio.h>
#include <string.h>

#include "sfpr_thread_mutex.h"
#include "sfpr_time.h"
#include "sfpr_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SFPR_LOG_FATAL					0  //致命信息
#define SFPR_LOG_ERROR					1  //错误信息				打印错误信息
#define SFPR_LOG_WARNING				2  //告警信息				打印告警信息
#define SFPR_LOG_NOTICE					3  //重要信息				打印正常信息，但是比较重要
#define SFPR_LOG_INFO					4  //一般信息				打印一般的信息
#define SFPR_LOG_DEBUG					5  //一般调试信息				最轻量级的调试输出，函数的出入口可用此等级
#define SFPR_LOG_DEBUG_L1				6  //轻度调试信息				比LOG_DEBUG更严格的调试信息
#define SFPR_LOG_DEBUG_L2				7  //中间级别的调试信息		比LOG_DEBUG_L1更严格的调试信息
#define SFPR_LOG_DEBUG_L3				8  //深度调试信息				比LOG_DEBUG_L2更严格的调试信息

#define SFPR_LOG_DAILY 0x01
#define SFPR_LOG_GB2312_SUPPORT 0x02
#define SFPR_LOG_OVERRIDE 0x04


#define SFPR_LOG_FILE_LEN		512
#define SFPR_LOG_CURRFILE_LEN	576
#define SFPR_LOG_LINE_LEN		8192

typedef struct sfpr_log_s sfpr_log_t;

sfpr_log_t *sfpr_log_new();

int sfpr_log_init(sfpr_log_t* log,char *log_file,int level,unsigned long flag);

int sfpr_log_quit(sfpr_log_t **log);

int sfpr_log_open(sfpr_log_t* log);

int sfpr_log_close(sfpr_log_t* log);

int sfpr_log_set_level(sfpr_log_t* log,char *level);

int sfpr_log_set_flag(sfpr_log_t* log,unsigned long flag);

int sfpr_log_del_flag(sfpr_log_t* log,unsigned long flag);


int sfpr_log(sfpr_log_t* log,int level,char *fmt,...);

int sfpr_log_info(sfpr_log_t* log,int level,char *info);

int sfpr_log_error(sfpr_log_t* log,int level,char *info);

int sfpr_log_hex(sfpr_log_t* log,int level,char *info,const char *buf,int len,int type);


#ifdef __cplusplus
}
#endif

#endif

