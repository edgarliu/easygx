
#ifdef __cplusplus
extern "C" {
#endif

#ifndef JIAEN_PORTABLE_RUNTIME_STRING_H
#define JIAEN_PORTABLE_RUNTIME_STRING_H

#include <ctype.h>
#ifdef WIN32
#define snprintf _snprintf
#define strcasecmp(str1,str2) stricmp(str1,str2)
#define strncasecmp(str1,str2,len) strnicmp(str1,str2,len)
#else
#define stricmp(str1,str2) strcasecmp(str1,str2)
#define strnicmp(str1,str2,len) strncasecmp(str1,str2,len)
#endif

/**************************************************************************************************/
/**
 *\defgroup string 字符串
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup string_base 基本函数
 *\ingroup string
 **************************************************************************************************/

/**
 *\ingroup string_base
 *\param[in] *pdata 字符串指针
 *\param[in] *array 将分割完的字符串放入字符数组
 *\param[in] num    最多分割次数
 *\param[in] *sep   分隔符
 *\ 分割字符串，分隔符sep的每一个字符都是一个分隔符，例如" \r"
 */
int sfpr_str_split(char *pdata,char *array[],int num,char *sep);

/**
 *\ingroup string_base
 *\brief 去掉字符串左侧空白
 *\param[in] *pdata 字符串指针
 */
char *sfpr_str_ltrim(char *pdata);
/**
 *\ingroup string_base
 *\brief 去掉字符串右侧空白
 *\param[in] *pdata 字符串指针
 */
char *sfpr_str_rtrim(char *pdata);
/**
 *\ingroup string_base
 *\brief 去掉字符串所有空白
 *\param[in] *pdata 字符串指针
 */
char *sfpr_str_trim(char *pdata);

/**
 *\ingroup string_base
 * @brief 利用指定字符串替换源字符串中的指定子字符串
 * @param src 源字符串
 * @param str_src 待替换子字符串
 * @param str_des 目标字符串
 * @retval 0 成功
 * @retval -1 失败
 */
int sfpr_str_replace(char* src,char* str_src, char* str_des);

/**
 *\ingroup string_base
 * @brief 检测指定字符是否为数字字符串
 * @param src 待检测字符
 * @param len 字符串长度
 * @retval 0 匹配
 * @retval -1 不匹配
 */
int sfpr_str_isdigits(char* src, int len);

/**
 *\ingroup string_base
 * @brief 按照十六进制输出字符串
 * @param info 抬头信息
 * @param buf 字符串
 * @param len 字符串长度
 */
void sfpr_str_hex_print(const char * info,const char *buf,int len);


/**
 *\ingroup string_base
 * @brief 把数字转为二进制格式的字符串
 * @param digi 数字 
 * @param format 转化的格式
 * @param output 输出缓冲区
 * @param size 输出缓冲区长度
 */
char *sfpr_str_hextobin(unsigned long digi,char *format,char *output,int size);

/**
 *\ingroup string_base
 * @brief 把二进制格式的字符串转为数字
 * @param binstr 二进制格式的字符串
 */
unsigned long sfpr_str_bintohex(char *binstr);

/**
 *\ingroup string_base
 * @brief 过滤字符串
 * @param data 字符串
 * @param c1 需要过滤的字符
 * @param c2 目的字符
 */
int sfpr_str_filter(char *data,char c1,char c2);


unsigned long int sfpr_number_pow(unsigned char a1,unsigned char a2);

#ifdef WIN32
char *strtok_r(char *s, const char *delim, char **save_ptr); 
#endif

#endif

#ifdef __cplusplus
}
#endif
