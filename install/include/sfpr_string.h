
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
 *\defgroup string �ַ���
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup string_base ��������
 *\ingroup string
 **************************************************************************************************/

/**
 *\ingroup string_base
 *\param[in] *pdata �ַ���ָ��
 *\param[in] *array ���ָ�����ַ��������ַ�����
 *\param[in] num    ���ָ����
 *\param[in] *sep   �ָ���
 *\ �ָ��ַ������ָ���sep��ÿһ���ַ�����һ���ָ���������" \r"
 */
int sfpr_str_split(char *pdata,char *array[],int num,char *sep);

/**
 *\ingroup string_base
 *\brief ȥ���ַ������հ�
 *\param[in] *pdata �ַ���ָ��
 */
char *sfpr_str_ltrim(char *pdata);
/**
 *\ingroup string_base
 *\brief ȥ���ַ����Ҳ�հ�
 *\param[in] *pdata �ַ���ָ��
 */
char *sfpr_str_rtrim(char *pdata);
/**
 *\ingroup string_base
 *\brief ȥ���ַ������пհ�
 *\param[in] *pdata �ַ���ָ��
 */
char *sfpr_str_trim(char *pdata);

/**
 *\ingroup string_base
 * @brief ����ָ���ַ����滻Դ�ַ����е�ָ�����ַ���
 * @param src Դ�ַ���
 * @param str_src ���滻���ַ���
 * @param str_des Ŀ���ַ���
 * @retval 0 �ɹ�
 * @retval -1 ʧ��
 */
int sfpr_str_replace(char* src,char* str_src, char* str_des);

/**
 *\ingroup string_base
 * @brief ���ָ���ַ��Ƿ�Ϊ�����ַ���
 * @param src ������ַ�
 * @param len �ַ�������
 * @retval 0 ƥ��
 * @retval -1 ��ƥ��
 */
int sfpr_str_isdigits(char* src, int len);

/**
 *\ingroup string_base
 * @brief ����ʮ����������ַ���
 * @param info ̧ͷ��Ϣ
 * @param buf �ַ���
 * @param len �ַ�������
 */
void sfpr_str_hex_print(const char * info,const char *buf,int len);


/**
 *\ingroup string_base
 * @brief ������תΪ�����Ƹ�ʽ���ַ���
 * @param digi ���� 
 * @param format ת���ĸ�ʽ
 * @param output ���������
 * @param size �������������
 */
char *sfpr_str_hextobin(unsigned long digi,char *format,char *output,int size);

/**
 *\ingroup string_base
 * @brief �Ѷ����Ƹ�ʽ���ַ���תΪ����
 * @param binstr �����Ƹ�ʽ���ַ���
 */
unsigned long sfpr_str_bintohex(char *binstr);

/**
 *\ingroup string_base
 * @brief �����ַ���
 * @param data �ַ���
 * @param c1 ��Ҫ���˵��ַ�
 * @param c2 Ŀ���ַ�
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
