#ifndef __MD5_H__
#define __MD5_H__

#ifdef _WIN32
#ifndef __PROC_C__
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
#else
#ifndef __PROC_C__
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#endif

/**************************************************************************************************/
/**
 *\defgroup md5 md5算法
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup md5_define 数据类型及宏定义
 *\ingroup md5
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup md5_struct 数据结构
 *\ingroup md5
 **************************************************************************************************/

/**
*\ingroup md5_define
*\ Size of a standard MD5 signature in bytes. This definition is for
*\ external programs only. The MD5 routines themselves reference the
*\ signature as 4 unsigned 32-bit integers.
*/
#define MD5_SIZE 16

/**
*\ingroup md5_define
*\ NOTE: the following is assumed to generate a 32-bit unsigned data
*\ type.
*/
typedef unsigned int md5_uint32;

/**
*\ingroup md5_define
*/
#define MAX_MD5_UINT32 ((md5_uint32)4294967295U)

/**
*\ingroup md5_define
*\ The MD5 algorithm works on blocks of characters of 64 bytes. This
*\ is an internal value only and is not necessary for external use.
*/
#define MD5_BLOCK_SIZE 64

/**
*\ingroup md5_struct
*\ Structure to save state of computation between the single steps.
*/
typedef struct{
	md5_uint32 md_A; 			/**< accumulater 1 */
	md5_uint32 md_B; 			/**< accumulater 2 */
	md5_uint32 md_C; 			/**< accumulater 3 */
	md5_uint32 md_D; 			/**< accumulater 4 */
	
	md5_uint32 md_total[2]; 		/**< totaling storage */
	md5_uint32 md_buf_len; 			/**< length of the storage buffer */
	char md_buffer[MD5_BLOCK_SIZE * 2]; 	/**< character storage buffer */
} md5_t;


/**************************************************************************************************/
/**
 *\defgroup md5_base 基本函数
 *\ingroup md5
 **************************************************************************************************/

/**
 *\ingroup md5_base
* md5_init
*
* DESCRIPTION:
*
* Initialize structure containing state of MD5 computation. (RFC 1321,
* 3.3: Step 3). This is for progressive MD5 calculations only. If
* you have the complete string available, md5_buffer should be used.
* md5_process should be called for each bunch of bytes and after the
* last process call, md5_finish should be called to get the
* signature.
*
* RETURNS:
*
* None.
*
* ARGUMENTS:
*
* md5_p - Pointer to md5 structure that we are initializing.
*/
void sfpr_md5_init(md5_t *md5_p);

/**
 *\ingroup md5_base
* md5_process
*
* DESCRIPTION:
*
* This function is used to progressively calculate a MD5 signature some
* number of bytes at a time. If you have the complete string
* available, md5_buffer should be used. The MD5 structure should
* have been initialized with md5_init and after the last process
* call, md5_finish should be called to get the results.
*
* RETURNS:
*
* None.
*
* ARGUMENTS:
*
* md5_p - Pointer to MD5 structure which we are progressively updating.
*
* buffer - A buffer of bytes whose MD5 signature we are calculating.
*
* buf_len - The length of the buffer.
*/
void sfpr_md5_process(md5_t *md5_p, const void *buffer, const unsigned int buf_len);

/**
 *\ingroup md5_base
* md5_finish
*
* DESCRIPTION:
*
* Finish a progressing MD5 calculation and copy the resulting MD5
* signature into the result buffer which should be 16 bytes
* (MD5_SIZE). After this call, the MD5 structure is invalid.
*
* RETURNS:
*
* None.
*
* ARGUMENTS:
*
* md5_p - Pointer to MD5 structure which we are finishing.
*
* signature - A 16 byte buffer that will contain the MD5 signature.
*/
void sfpr_md5_finish(md5_t *md5_p, void *signature);

/**
*\ingroup md5_base
* md5_buffer
*
* DESCRIPTION:
*
* This function is used to calculate a MD5 signature for a buffer of
* bytes. If you only have part of a buffer that you want to process
* then md5_init, md5_process, and md5_finish should be used.
*
* RETURNS:
*
* None.
*
* ARGUMENTS:
*
* buffer - A buffer of bytes whose MD5 signature we are calculating.
*
* buf_len - The length of the buffer.
*
* signature - A 16 byte buffer that will contain the MD5 signature.
*/
void sfpr_md5_buffer(const char *buffer, const unsigned int buf_len, void *signature);

/**
 *\ingroup md5_base
* md5_sig_to_string
*
* DESCRIPTION:
*
* Convert a MD5 signature in a 16 byte buffer into a hexadecimal string
* representation.
*
* RETURNS:
*
* None.
*
* ARGUMENTS:
*
* signature - a 16 byte buffer that contains the MD5 signature.
*
* str - a string of charactes which should be at least 33 bytes long (2
* characters per MD5 byte and 1 for the \0).
*
* str_len - the length of the string.
*/
void sfpr_md5_sig_to_string(void *signature, char *str, const unsigned int str_len);

/**
 *\ingroup md5_base
* md5_sig_from_string
*
* DESCRIPTION:
*
* Convert a MD5 signature from a hexadecimal string representation into
* a 16 byte buffer.
*
* RETURNS:
*
* None.
*
* ARGUMENTS:
*
* signature - A 16 byte buffer that will contain the MD5 signature.
*
* str - A string of charactes which _must_ be at least 32 bytes long (2
* characters per MD5 byte).
*/
void sfpr_md5_sig_from_string(void *signature, const char *str);

#ifdef _WIN32
#ifdef __cplusplus
}
#endif
#endif

#endif /* __MD5_H__ */
