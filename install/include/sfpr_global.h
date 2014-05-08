
#ifndef SFPR_GLOBAL_H
#define SFPR_GLOBAL_H
/**
 * @file sfpr_global.h
 * @brief SFPR库的全局定义
 * @author 刘晓强
 * @version 0.1.0
 * @date 2012-02-9
 */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup global 全局性定义
 *
 **************************************************************************************************/


/**************************************************************************************************/
/**
 *\defgroup global_retval 函数返回值
 *\ingroup global
 *
 **************************************************************************************************/

/**
 *\ingroup global_retval
 *\brief sfpr库的返回值，表示成功
 */
#define SFPR_SUCCESS 0

/**
 *\ingroup global_retval
 *\brief sfpr库的返回值，表示失败
 */
#define SFPR_ERROR -1

/**
 *\ingroup global_retval
 *\brief sfpr库的失败返回值: 超时
 */
#define SFPR_TIMEOUT -2

/**
 *\ingroup global_retval
 *\brief sfpr库的返回值: 资源正被占用
 */
#define SFPR_BUSY -3

/**
 *\ingroup global_retval
 *\brief sfpr库的返回值: 文件描述符被关闭
 */
#define SFPR_CLOSED -4

/**
 *\ingroup global_retval
 *\brief sfpr库的返回值: 内存分配失败
 */
#define SFPR_NOMEM -5

/**
 *\ingroup global_retval
 *\brief sfpr库的返回值: 满的
 */
#define SFPR_FULL -6

/**
 *\ingroup global_retval
 *\brief sfpr库的返回值: false
 */
#define SFPR_FALSE   0

/**
 *\ingroup global_retval
 *\brief sfpr库的返回值: true
 */
#define SFPR_TRUE   1


/**************************************************************************************************/
/**
 *\defgroup global_datatype 数据类型
 *\ingroup global
 *
 **************************************************************************************************/

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 整形
 */
typedef int sfpr_int_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 长整形
 */
typedef long sfpr_long_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 短整形
 */
typedef short sfpr_short_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 32位无符号类型
 */
typedef unsigned long sfpr_uint32_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 32位符号类型
 */
typedef long int sfpr_int32_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 16位无符号类型
 */
typedef unsigned short sfpr_uint16_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 16位符号类型
 */
typedef short int sfpr_int16_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 8位无符号类型
 */
typedef unsigned char sfpr_uint8_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 8位符号类型
 */
typedef char sfpr_int8_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 浮点数
 */
typedef float sfpr_float_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 布尔值
 */
typedef short sfpr_bool_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 字符值
 */
typedef char sfpr_char_t;

/**
 *\ingroup global_datatype
 *\brief sfpr库自定义数据类型: 空值
 */
typedef void sfpr_void_t;


/**************************************************************************************************/
/**
 *\defgroup global_function 全局函数
 *\ingroup global
 *
 **************************************************************************************************/

/**
 *\ingroup global_function
 *\brief sfpr库版本
 *\return sfpr库版本
 */
char *sfpr_version();

#ifdef __cplusplus
}
#endif

#endif
