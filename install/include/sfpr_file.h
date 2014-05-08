/**
 *\file sfpr_file.h
 *\brief SFPR����ļ�ģ��\n
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
 *\defgroup file �ļ�ģ��
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup file_struct �������ݽṹ
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
 *\breif sfpr_file_attrs_set �ı�־
 *\  �ļ�Ϊֻ������
 */
#define SFPR_FILE_ATTR_READONLY   0x01

/**
 *\ingroup file_struct
 *\breif sfpr_file_attrs_set �ı�־
 *\ �ļ���ִ��
 */
#define SFPR_FILE_ATTR_EXECUTABLE 0x02

/**
 *\ingroup file_struct
 *\breif  sfpr_file_attrs_set �ı�־
 *\ �ļ�Ϊ�����ļ�
 */
#define SFPR_FILE_ATTR_HIDDEN     0x04


/**************************************************************************************************/
/**
 *\defgroup file_base ��������
 *\ingroup file
 **************************************************************************************************/

/**
 *\ingroup file_base
 *\brief ɾ��һ���ļ�
 *\param[in] path	 Ҫɾ�����ļ�·��
 *\return  ɾ��״̬SFPR_SUCCESS �ɹ���SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_file_remove(const char *path);

/**************************************************************************************************/
/**
 *\defgroup  file_status ״̬����
 *\ingroup file
 **************************************************************************************************/

/**
 *\ingroup file_status
 *\brief	�����ļ��Ĳ�������
 *\param[in] fname  Ҫ���õ��ļ�
 *\param[in] attributes  ���õ�����
 *\param[in] set 1 ���ø����ԣ�0 ȡ��������
 *\ SFPR_FILE_ATTR_READONLY ֻ��SFPR_FILE_ATTR_EXECUTABLE (��ִ��)SFPR_FILE_ATTR_HIDDEN(�Ƿ�����)
  *\return  SFPR_SUCCESS �ɹ���SFPR_ERROR ʧ��
*/
sfpr_int_t sfpr_file_attrs_set(const char *fname,sfpr_uint32_t attributes,int set);

sfpr_int_t sfpr_file_isexited(const char *fname);

/**************************************************************************************************/
/**
 *\defgroup  file_dir Ŀ¼����
 *\ingroup file
 **************************************************************************************************/

/**
 *\ingroup file_dir
 *\brief ����һ��Ŀ¼
 *\param[in] path		Ҫ������Ŀ¼��
 *\return  ����״̬SFPR_SUCCESS �ɹ���SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_dir_make(const char *path);

/**
 *\ingroup file_dir
 *\brief ɾ��һ��Ŀ¼
 *\param[in] path	 Ҫɾ����Ŀ¼��
 *\return  ɾ��״̬SFPR_SUCCESS �ɹ���SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_dir_remove(const char *path);

#define sfpr_dir_open  opendir
#define sfpr_dir_read  readdir
#define sfpr_dir_close  closedir


#ifdef __cplusplus
}
#endif

#endif
