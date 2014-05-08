/**
 *\file sfpr_file_io.h
 *\brief SFPR库的文件I/O模块\n
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
 *\defgroup file_io 文件I/O模块
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup file_io_struct 基本数据结构
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
 *\breif 以读的方式打开文件
 */
#define SFPR_READ       0x00001

/**
 *\ingroup file_io_struct
 *\breif 以写的方式打开文件
 */
#define SFPR_WRITE      0x00002

/**
 *\ingroup file_io_struct
 *\breif 如果文件不存在则创建
 */
#define SFPR_CREATE     0x00004

/**
 *\ingroup file_io_struct
 *\breif 以追加的方式打开
 */
#define SFPR_APPEND     0x00008

/**
 *\ingroup file_io_struct
 *\breif 打开一个文件以截断的方式
 */
#define SFPR_TRUNCATE   0x00010

/**
 *\ingroup file_io_struct
 *\breif 打开一个文件，以二进制方式
 */
#define SFPR_BINARY     0x00020

/**
 *\ingroup file_io_struct
 *\breif和SFPR_CREATE一起使用， 如果定义SFPR_CREATE 并且文件存在，文件将打开失败
 */
#define SFPR_EXCL       0x00040

/**
 *\ingroup file_io_struct
 *\breif 打开一个文件以缓冲I/O方式 
 */
#define SFPR_BUFFERED   0x00080

/**
 *\ingroup file_io_struct
 *\breif 关闭之后删除该文件
 */
#define SFPR_DELONCLOSE 0x00100

#ifdef WIN32
/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek的标志 
 *\ 定位文件的位置 
 */
#define SFPR_SET 	0x00001

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek的标志 
 *\ 设置文件的读写位置为当前位置
 */
#define SFPR_CUR 	0x00002

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek的标志 
 *\ 设置文件位置为文件末尾
 */
#define SFPR_END 	0x00004

#else

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek的标志 
 *\ 定位文件的位置 
 */
#define SFPR_SET SEEK_SET

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek的标志 
 *\ 设置文件的读写位置为当前位置
 */
#define SFPR_CUR SEEK_CUR

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_seek的标志 
 *\ 设置文件位置为文件末尾
 */
#define SFPR_END SEEK_END

#endif

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_lock的标志 
 *\建立共享锁定
 */
#define SFPR_LOCK_SH	0x00001

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_lock的标志 
 *\ 建立互斥锁定
 */
#define SFPR_LOCK_EX	0X00002

/**
 *\ingroup file_io_struct
 *\breif sfpr_file_lock的标志 
 *\ 无法建立锁定时，此操作可不被阻断，马上返回进程,
 */
#define SFPR_LOCK_NB	0X00004
/**
 *\ingroup file_io_struct
 *\breif 封装统一的数据类型 
 */

#if defined VXWORKS

typedef int sfpr_fd_t; // Vxworks 上不知道是不是这样

#elif defined WIN32

typedef HANDLE sfpr_fd_t;

#else

typedef int sfpr_fd_t;

#endif

/**
 *\ingroup file_io_struct
 *\brief 描述一个文件的结构
 */
typedef struct sfpr_file_s{
	char *fname;					/**< 文件名*/
	sfpr_fd_t filedes;				/**< 文件描述符*/
	sfpr_uint32_t flags;			/**< 文件读写状态*/
	sfpr_mutex_t lock;				/**< 读写文件锁*/
}sfpr_file_t;


/**************************************************************************************************/
/**
 *\defgroup file_io_base 基本函数
 *\ingroup file_io
 **************************************************************************************************/

/**
 *\ingroup file_io_base
 *\brief 打开一个文件
 *\param[in] **newf	保存文件信息的结构
 *\param[in] *fname 	要打开的文件名
 *\param[in] flag		打开文件的方式
 *\return  打开状态SFPR_SUCCESS 成功，SFPR_ERROR 失败
 */
sfpr_int_t sfpr_file_open(sfpr_file_t **newf, const char *fname,sfpr_uint32_t flag);

/**
 *\ingroup file_io_base
 *\brief 关闭一个文件
 *\param[in]  file 	保存文件信息的结构
 *\return  关闭状态0 成功，-1 失败
 */
sfpr_int_t sfpr_file_close(sfpr_file_t *file);


/**************************************************************************************************/
/**
 *\defgroup file_io_ability 读写函数
 *\ingroup file_io
 **************************************************************************************************/

/**
 *\ingroup file_io_ability
 *\brief 从指定的文件中读取内容
 *\param[in] *thefile	保存文件信息的结构指针
 *\param[in] buf 	将读出的内容放在buf中
 *\param[in] nbytes	读取的字节数
 *\return 成功返回读取到的字节数，失败返回SFPR_ERROR 
 */
sfpr_int_t sfpr_file_read(sfpr_file_t *thefile, void *buf,sfpr_int_t nbytes);

/**
 *\ingroup file_io_ability
 *\brief 向指定的文件中写入内容
 *\param[in] *thefile	保存文件信息的结构指针
 *\param[in] buf 	将buf中的内容写入到文件中
 *\param[in] nbytes	写入的字节数
 *\return 成功返回写入的字节数，失败返回SFPR_ERROR 
 */
sfpr_int_t sfpr_file_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes);

/**
 *\ingroup file_io_ability
 *\brief 向指定的文件中写入内容
 *\param[in] *thefile	保存文件信息的结构指针
 *\param[in] buf 	将buf中的内容写入到文件中
 *\param[in] nbytes	写入的字节数
 *\param[in] whence 写到何处
 *\param[in] offset	偏移值
 *\return 成功返回写入的字节数，失败返回SFPR_ERROR 
 */
sfpr_int_t sfpr_file_safe_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes,int whence,int offset);

/**
 *\ingroup file_io_ability
 *\brief	向指定的文件中写入一个字符。
 *\param[in] ch	要写入的字符
 *\param[in] *thefile 	保存文件信息的结构指针
  *\return  返回1 成功，否则失败
*/
sfpr_int_t sfpr_file_putc(char ch, sfpr_file_t *thefile);

/**
 *\ingroup file_io_ability
 *\brief	从指定的文件中读出一个字符。
 *\param[in] *ch	得到的字符保存在ch中
 *\param[in] *thefile 	保存文件信息的结构指针
  *\return  返回1 成功，否则失败
*/
sfpr_int_t sfpr_file_getc(char *ch, sfpr_file_t *thefile);

/**
 *\ingroup file_io_ability
 *\brief	从指定的文件内读入字符并存到参数str所指的内存空间，
 *\	 直到出现换行字符、读到文件尾或是已读了size-1个字符为止，
 *\param[in] str	将得到的字符串保存在str中
 *\param[in] len   要读的字符长度
 *\param[in] *thefile 	保存文件信息的结构指针
   *\return  返回读取字符的个数
*/
sfpr_int_t sfpr_file_gets(char *str, int len,sfpr_file_t *thefile);

/**
 *\ingroup file_io_ability
 *\brief      将参数str,所指的字符串写入到参数指定的文件内。
 *\param[in] str	要写入的字符串
 *\param[in] *thefile 	保存文件信息的结构指针
  *\return   若成功则返回写出的字符个数，返回EOF则表示有错误发生。
*/
sfpr_int_t sfpr_file_puts(const char *str, sfpr_file_t *thefile);


/**************************************************************************************************/
/**
 *\defgroup  file_io_status 状态函数
 *\ingroup file_io
 **************************************************************************************************/

/**
 *\ingroup file_io_status
 *\brief	强迫将缓冲区内的数据写回指定的文件中。
 *\param[in] *thefile	保存文件信息的结构指针
 *\return   成功返回0，失败返回EOF，
*/
sfpr_int_t sfpr_file_flush(sfpr_file_t *thefile);

/**
 *\ingroup file_io_status
 *\brief	锁定一个文件
 *\param[in] *thefile	保存文件信息的结构指针
 *\param[in] type 情况如下(1)LOCK_SH 建立共享锁定(2)LOCK_EX 建立互斥锁定
 *\	(3)LOCK_NB 无法建立锁定时，此操作可不被阻断，马上返回进程,通常与LOCK_SH 
 *\	或LOCK_EX 做OR（|）组合。单一文件无法同时建立共享锁定和互斥锁定,
 *\return   成功返回0，失败返回-1
 */
sfpr_int_t sfpr_file_lock(sfpr_file_t *thefile, int type);

/**
 *\ingroup file_io_status
 *\brief	解除锁定的文件
 *\param[in] *thefile	保存文件信息的结构指针
  *\return   成功返回0，失败返回-1
*/
sfpr_int_t sfpr_file_unlock(sfpr_file_t *thefile);

/**
 *\ingroup file_io_status
 *\brief	设置文件的位置，定位一个文件
 *\param[in] *thefile	保存文件信息的结构指针
 *	SFPR_SET 参数offset即为新的读写位置。
 *	SFPR_CUR 以目前的读写位置往后增加offset个位移量。
 *	SFPR_END 将读写位置指向文件尾后再增加offset个位移量。
 *	当whence值为SFPR_CUR或SFPR_END 时，参数offet允许负值的出现
 *\return 成功返回当前位置到文件开始处的字节数，失败返回-1
*/
sfpr_int_t sfpr_file_seek(sfpr_file_t *thefile,int whence,int offset);

/**
 *\ingroup file_io_status
 *\brief	获取文件大小
 *\return 文件大小
*/
sfpr_int_t sfpr_file_get_size(sfpr_file_t *thefile);

/**
 *\ingroup file_io_status
 *\brief	判断是否到达文件末尾
 *\param[in] *fptr 保存文件信息的结构指针
 *\return  SFPR_SUCCESS 成功是文件末尾，SFPR_ERROR 不是文件末尾
*/
sfpr_int_t sfpr_file_eof(sfpr_file_t *fptr);

#ifdef __cplusplus
}
#endif

#endif
