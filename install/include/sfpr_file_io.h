/**
 *\file sfpr_file_io.h
 *\brief SFPR����ļ�I/Oģ��\n
 *\version 0.1.0
 *\date 2012-02-24
 */

#ifndef SFPR_FILE_IO_H
#define SFPR_FILE_IO_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup file_io �ļ�I/Oģ��
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup file_io_struct �������ݽṹ
 *\ingroup file_io
 **************************************************************************************************/

#if defined VXWORKS
//#include <file.h>

#elif defined WIN32

#else
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <errno.h>
#endif

#include "sfpr_global.h"
#include "sfpr_thread_mutex.h"

/**
 *\ingroup file_io_struct
 *\breif �Զ��ķ�ʽ���ļ�
 */
#define SFPR_READ       0x00001

/**
 *\ingroup file_io_struct
 *\breif ��д�ķ�ʽ���ļ�
 */
#define SFPR_WRITE      0x00002

/**
 *\ingroup file_io_struct
 *\breif ����ļ��������򴴽�
 */
#define SFPR_CREATE     0x00004

/**
 *\ingroup file_io_struct
 *\breif ��׷�ӵķ�ʽ��
 */
#define SFPR_APPEND     0x00008

/**
 *\ingroup file_io_struct
 *\breif ��һ���ļ��Խضϵķ�ʽ
 */
#define SFPR_TRUNCATE   0x00010

/**
 *\ingroup file_io_struct
 *\breif ��һ���ļ����Զ����Ʒ�ʽ
 */
#define SFPR_BINARY     0x00020

/**
 *\ingroup file_io_struct
 *\breif��SFPR_CREATEһ��ʹ�ã� �������SFPR_CREATE �����ļ����ڣ��ļ�����ʧ��
 */
#define SFPR_EXCL       0x00040

/**
 *\ingroup file_io_struct
 *\breif ��һ���ļ��Ի���I/O��ʽ 
 */
#define SFPR_BUFFERED   0x00080

/**
 *\ingroup file_io_struct
 *\breif �ر�֮��ɾ�����ļ�
 */
#define SFPR_DELONCLOSE 0x00100

#ifdef WIN32
/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek�ı�־ 
 *\ ��λ�ļ���λ�� 
 */
#define SFPR_SET 	0x00001

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek�ı�־ 
 *\ �����ļ��Ķ�дλ��Ϊ��ǰλ��
 */
#define SFPR_CUR 	0x00002

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek�ı�־ 
 *\ �����ļ�λ��Ϊ�ļ�ĩβ
 */
#define SFPR_END 	0x00004

#else

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek�ı�־ 
 *\ ��λ�ļ���λ�� 
 */
#define SFPR_SET SEEK_SET

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek�ı�־ 
 *\ �����ļ��Ķ�дλ��Ϊ��ǰλ��
 */
#define SFPR_CUR SEEK_CUR

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek�ı�־ 
 *\ �����ļ�λ��Ϊ�ļ�ĩβ
 */
#define SFPR_END SEEK_END

#endif

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_lock�ı�־ 
 *\������������
 */
#define SFPR_LOCK_SH	0x00001

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_lock�ı�־ 
 *\ ������������
 */
#define SFPR_LOCK_EX	0X00002

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_lock�ı�־ 
 *\ �޷���������ʱ���˲����ɲ�����ϣ����Ϸ��ؽ���,
 */
#define SFPR_LOCK_NB	0X00004
/**
 *\ingroup file_io_struct
 *\breif ��װͳһ���������� 
 */

#if defined VXWORKS

typedef int sfpr_fd_t; // Vxworks �ϲ�֪���ǲ�������

#elif defined WIN32

typedef HANDLE sfpr_fd_t;

#else

typedef int sfpr_fd_t;

#endif

/**
 *\ingroup file_io_struct
 *\brief ����һ���ļ��Ľṹ
 */
typedef struct sfpr_file_s{
	char *fname;					/**< �ļ���*/
	sfpr_fd_t filedes;				/**< �ļ�������*/
	sfpr_uint32_t flags;			/**< �ļ���д״̬*/
	sfpr_mutex_t lock;				/**< ��д�ļ���*/
}sfpr_file_t;


/**************************************************************************************************/
/**
 *\defgroup file_io_base ��������
 *\ingroup file_io
 **************************************************************************************************/

/**
 *\ingroup file_io_base
 *\brief ��һ���ļ�
 *\param[in] **newf	�����ļ���Ϣ�Ľṹ
 *\param[in] *fname 	Ҫ�򿪵��ļ���
 *\param[in] flag		���ļ��ķ�ʽ
 *\return  ��״̬SFPR_SUCCESS �ɹ���SFPR_ERROR ʧ��
 */
sfpr_int_t sfpr_file_open(sfpr_file_t **newf, const char *fname,sfpr_uint32_t flag);

/**
 *\ingroup file_io_base
 *\brief �ر�һ���ļ�
 *\param[in]  file 	�����ļ���Ϣ�Ľṹ
 *\return  �ر�״̬0 �ɹ���-1 ʧ��
 */
sfpr_int_t sfpr_file_close(sfpr_file_t *file);


/**************************************************************************************************/
/**
 *\defgroup file_io_ability ��д����
 *\ingroup file_io
 **************************************************************************************************/

/**
 *\ingroup file_io_ability
 *\brief ��ָ�����ļ��ж�ȡ����
 *\param[in] *thefile	�����ļ���Ϣ�Ľṹָ��
 *\param[in] buf 	�����������ݷ���buf��
 *\param[in] nbytes	��ȡ���ֽ���
 *\return �ɹ����ض�ȡ�����ֽ�����ʧ�ܷ���SFPR_ERROR 
 */
sfpr_int_t sfpr_file_read(sfpr_file_t *thefile, void *buf,sfpr_int_t nbytes);

/**
 *\ingroup file_io_ability
 *\brief ��ָ�����ļ���д������
 *\param[in] *thefile	�����ļ���Ϣ�Ľṹָ��
 *\param[in] buf 	��buf�е�����д�뵽�ļ���
 *\param[in] nbytes	д����ֽ���
 *\return �ɹ�����д����ֽ�����ʧ�ܷ���SFPR_ERROR 
 */
sfpr_int_t sfpr_file_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes);

/**
 *\ingroup file_io_ability
 *\brief ��ָ�����ļ���д������
 *\param[in] *thefile	�����ļ���Ϣ�Ľṹָ��
 *\param[in] buf 	��buf�е�����д�뵽�ļ���
 *\param[in] nbytes	д����ֽ���
 *\param[in] whence д���δ�
 *\param[in] offset	ƫ��ֵ
 *\return �ɹ�����д����ֽ�����ʧ�ܷ���SFPR_ERROR 
 */
sfpr_int_t sfpr_file_safe_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes,int whence,int offset);

/**
 *\ingroup file_io_ability
 *\brief	��ָ�����ļ���д��һ���ַ���
 *\param[in] ch	Ҫд����ַ�
 *\param[in] *thefile 	�����ļ���Ϣ�Ľṹָ��
  *\return  ����1 �ɹ�������ʧ��
*/
sfpr_int_t sfpr_file_putc(char ch, sfpr_file_t *thefile);

/**
 *\ingroup file_io_ability
 *\brief	��ָ�����ļ��ж���һ���ַ���
 *\param[in] *ch	�õ����ַ�������ch��
 *\param[in] *thefile 	�����ļ���Ϣ�Ľṹָ��
  *\return  ����1 �ɹ�������ʧ��
*/
sfpr_int_t sfpr_file_getc(char *ch, sfpr_file_t *thefile);

/**
 *\ingroup file_io_ability
 *\brief	��ָ�����ļ��ڶ����ַ����浽����str��ָ���ڴ�ռ䣬
 *\	 ֱ�����ֻ����ַ��������ļ�β�����Ѷ���size-1���ַ�Ϊֹ��
 *\param[in] str	���õ����ַ���������str��
 *\param[in] len   Ҫ�����ַ�����
 *\param[in] *thefile 	�����ļ���Ϣ�Ľṹָ��
   *\return  ���ض�ȡ�ַ��ĸ���
*/
sfpr_int_t sfpr_file_gets(char *str, int len,sfpr_file_t *thefile);

/**
 *\ingroup file_io_ability
 *\brief      ������str,��ָ���ַ���д�뵽����ָ�����ļ��ڡ�
 *\param[in] str	Ҫд����ַ���
 *\param[in] *thefile 	�����ļ���Ϣ�Ľṹָ��
  *\return   ���ɹ��򷵻�д�����ַ�����������EOF���ʾ�д�������
*/
sfpr_int_t sfpr_file_puts(const char *str, sfpr_file_t *thefile);


/**************************************************************************************************/
/**
 *\defgroup  file_io_status ״̬����
 *\ingroup file_io
 **************************************************************************************************/

/**
 *\ingroup file_io_status
 *\brief	ǿ�Ƚ��������ڵ�����д��ָ�����ļ��С�
 *\param[in] *thefile	�����ļ���Ϣ�Ľṹָ��
 *\return   �ɹ�����0��ʧ�ܷ���EOF��
*/
sfpr_int_t sfpr_file_flush(sfpr_file_t *thefile);

/**
 *\ingroup file_io_status
 *\brief	����һ���ļ�
 *\param[in] *thefile	�����ļ���Ϣ�Ľṹָ��
 *\param[in] type �������(1)LOCK_SH ������������(2)LOCK_EX ������������
 *\	(3)LOCK_NB �޷���������ʱ���˲����ɲ�����ϣ����Ϸ��ؽ���,ͨ����LOCK_SH 
 *\	��LOCK_EX ��OR��|����ϡ���һ�ļ��޷�ͬʱ�������������ͻ�������,
 *\return   �ɹ�����0��ʧ�ܷ���-1
 */
sfpr_int_t sfpr_file_lock(sfpr_file_t *thefile, int type);

/**
 *\ingroup file_io_status
 *\brief	����������ļ�
 *\param[in] *thefile	�����ļ���Ϣ�Ľṹָ��
  *\return   �ɹ�����0��ʧ�ܷ���-1
*/
sfpr_int_t sfpr_file_unlock(sfpr_file_t *thefile);

/**
 *\ingroup file_io_status
 *\brief	�����ļ���λ�ã���λһ���ļ�
 *\param[in] *thefile	�����ļ���Ϣ�Ľṹָ��
 *	SFPR_SET ����offset��Ϊ�µĶ�дλ�á�
 *	SFPR_CUR ��Ŀǰ�Ķ�дλ����������offset��λ������
 *	SFPR_END ����дλ��ָ���ļ�β��������offset��λ������
 *	��whenceֵΪSFPR_CUR��SFPR_END ʱ������offet����ֵ�ĳ���
 *\return �ɹ����ص�ǰλ�õ��ļ���ʼ�����ֽ�����ʧ�ܷ���-1
*/
sfpr_int_t sfpr_file_seek(sfpr_file_t *thefile,int whence,int offset);

/**
 *\ingroup file_io_status
 *\brief	��ȡ�ļ���С
 *\return �ļ���С
*/
sfpr_int_t sfpr_file_get_size(sfpr_file_t *thefile);

/**
 *\ingroup file_io_status
 *\brief	�ж��Ƿ񵽴��ļ�ĩβ
 *\param[in] *fptr �����ļ���Ϣ�Ľṹָ��
 *\return  SFPR_SUCCESS �ɹ����ļ�ĩβ��SFPR_ERROR �����ļ�ĩβ
*/
sfpr_int_t sfpr_file_eof(sfpr_file_t *fptr);

#ifdef __cplusplus
}
#endif

#endif
