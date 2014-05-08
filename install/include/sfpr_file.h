/**
 *\file sfpr_file.h
 *\brief SFPR库的文件模块\n
 *\version 0.1.0
 *\date 2012-02-24
 */


#ifndef SFPR_FILE_H
#define SFPR_FILE_H


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup file 文件模块
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup file_struct 基本数据结构
 *\ingroup file
 **************************************************************************************************/

#if defined VXWORKS
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#elif defined WIN32
#include <dirent.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>
#endif

#include "sfpr_global.h"
#include "sfpr_thread_mutex.h"

/**
 *\ingroup file_struct
 *\breif sfpr_file_attrs_set 的标志
 *\  文件为只读属性
 */
#define SFPR_FILE_ATTR_READONLY   0x01

/**
 *\ingroup file_struct
 *\breif sfpr_file_attrs_set 的标志
 *\ 文件可执行
 */
#define SFPR_FILE_ATTR_EXECUTABLE 0x02

/**
 *\ingroup file_struct
 *\breif  sfpr_file_attrs_set 的标志
 *\ 文件为隐藏文件
 */
#define SFPR_FILE_ATTR_HIDDEN     0x04


/**************************************************************************************************/
/**
 *\defgroup file_base 基本函数
 *\ingroup file
 **************************************************************************************************/

/**
 *\ingroup file_base
 *\brief 删除一个文件
 *\param[in] path	 要删除的文件路径
 *\return  删除状态SFPR_SUCCESS 成功，SFPR_ERROR 失败
 */
sfpr_int_t sfpr_file_remove(const char *path);

/**************************************************************************************************/
/**
 *\defgroup  file_status 状态函数
 *\ingroup file
 **************************************************************************************************/

/**
 *\ingroup file_status
 *\brief	设置文件的操作属性
 *\param[in] fname  要设置的文件
 *\param[in] attributes  设置的属性
 *\param[in] set 1 设置该属性，0 取消该属性
 *\ SFPR_FILE_ATTR_READONLY 只读SFPR_FILE_ATTR_EXECUTABLE (可执行)SFPR_FILE_ATTR_HIDDEN(是否隐藏)
  *\return  SFPR_SUCCESS 成功，SFPR_ERROR 失败
*/
sfpr_int_t sfpr_file_attrs_set(const char *fname,sfpr_uint32_t attributes,int set);

sfpr_int_t sfpr_file_isexited(const char *fname);

/**************************************************************************************************/
/**
 *\defgroup  file_dir 目录函数
 *\ingroup file
 **************************************************************************************************/

/**
 *\ingroup file_dir
 *\brief 创建一个目录
 *\param[in] path		要创建的目录名
 *\return  创建状态SFPR_SUCCESS 成功，SFPR_ERROR 失败
 */
sfpr_int_t sfpr_dir_make(const char *path);

/**
 *\ingroup file_dir
 *\brief 删除一个目录
 *\param[in] path	 要删除的目录名
 *\return  删除状态SFPR_SUCCESS 成功，SFPR_ERROR 失败
 */
sfpr_int_t sfpr_dir_remove(const char *path);

#define sfpr_dir_open  opendir
#define sfpr_dir_read  readdir
#define sfpr_dir_close  closedir


#ifdef __cplusplus
}
#endif

#endif
