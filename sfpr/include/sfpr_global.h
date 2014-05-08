
#ifndef SFPR_GLOBAL_H
#define SFPR_GLOBAL_H
/**
 * @file sfpr_global.h
 * @brief SFPR���ȫ�ֶ���
 * @author ����ǿ
 * @version 0.1.0
 * @date 2012-02-9
 */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup global ȫ���Զ���
 *
 **************************************************************************************************/


/**************************************************************************************************/
/**
 *\defgroup global_retval ��������ֵ
 *\ingroup global
 *
 **************************************************************************************************/

/**
 *\ingroup global_retval
 *\brief sfpr��ķ���ֵ����ʾ�ɹ�
 */
#define SFPR_SUCCESS 0

/**
 *\ingroup global_retval
 *\brief sfpr��ķ���ֵ����ʾʧ��
 */
#define SFPR_ERROR -1

/**
 *\ingroup global_retval
 *\brief sfpr���ʧ�ܷ���ֵ: ��ʱ
 */
#define SFPR_TIMEOUT -2

/**
 *\ingroup global_retval
 *\brief sfpr��ķ���ֵ: ��Դ����ռ��
 */
#define SFPR_BUSY -3

/**
 *\ingroup global_retval
 *\brief sfpr��ķ���ֵ: �ļ����������ر�
 */
#define SFPR_CLOSED -4

/**
 *\ingroup global_retval
 *\brief sfpr��ķ���ֵ: �ڴ����ʧ��
 */
#define SFPR_NOMEM -5

/**
 *\ingroup global_retval
 *\brief sfpr��ķ���ֵ: ����
 */
#define SFPR_FULL -6

/**
 *\ingroup global_retval
 *\brief sfpr��ķ���ֵ: false
 */
#define SFPR_FALSE   0

/**
 *\ingroup global_retval
 *\brief sfpr��ķ���ֵ: true
 */
#define SFPR_TRUE   1


/**************************************************************************************************/
/**
 *\defgroup global_datatype ��������
 *\ingroup global
 *
 **************************************************************************************************/

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: ����
 */
typedef int sfpr_int_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: ������
 */
typedef long sfpr_long_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: ������
 */
typedef short sfpr_short_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: 32λ�޷�������
 */
typedef unsigned long sfpr_uint32_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: 32λ��������
 */
typedef long int sfpr_int32_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: 16λ�޷�������
 */
typedef unsigned short sfpr_uint16_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: 16λ��������
 */
typedef short int sfpr_int16_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: 8λ�޷�������
 */
typedef unsigned char sfpr_uint8_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: 8λ��������
 */
typedef char sfpr_int8_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: ������
 */
typedef float sfpr_float_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: ����ֵ
 */
typedef short sfpr_bool_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: �ַ�ֵ
 */
typedef char sfpr_char_t;

/**
 *\ingroup global_datatype
 *\brief sfpr���Զ�����������: ��ֵ
 */
typedef void sfpr_void_t;


/**************************************************************************************************/
/**
 *\defgroup global_function ȫ�ֺ���
 *\ingroup global
 *
 **************************************************************************************************/

/**
 *\ingroup global_function
 *\brief sfpr��汾
 *\return sfpr��汾
 */
char *sfpr_version();

#ifdef __cplusplus
}
#endif

#endif
