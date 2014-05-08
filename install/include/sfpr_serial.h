
#ifndef _SERIAL_H
#define _SERIAL_H

#include "sfpr_file_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup serial ����
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup serial_struct ���ݽṹ
 *\ingroup serial
 **************************************************************************************************/
/**
 *\ingroup serial_struct
 *\brief �����豸������󳤶�
 */
#define MAX_DEV_NAME_LEN	32

/**
 *\ingroup serial_struct
 *\brief �����������ýṹ
 */
typedef struct serial_config_s{
	char dev_name[MAX_DEV_NAME_LEN];	/**< �����豸����*/

	unsigned long  ser_baud;  	   	/**< ���ڲ�����*/
		
	enum ser_data_size{			/**< ��������λ*/
		SERIAL_DATA_SIZE_5 = 5,	/**< 5λ����λ */
		SERIAL_DATA_SIZE_6,	/**< 6λ����λ */
		SERIAL_DATA_SIZE_7,	/**< 7λ����λ */
		SERIAL_DATA_SIZE_8	/**< 8λ����λ */
	}ser_data;
	
	enum ser_parity_type{			/**<������żУ��λ*/
		SERIAL_PARITY_REQUEST,	
		SERIAL_PARITY_NONE,    	/**< ��У��*/
 		SERIAL_PARITY_ODD,     	/**< ��У��*/
 		SERIAL_PARITY_EVEN,    	/**< żУ��*/
 		SERIAL_PARITY_MARK,    	
 		SERIAL_PARITY_SPACE
 	}ser_parity;
	
 	enum ser_stop_bit{			/**<����ֹͣλ*/
		SERIAL_STOP_BIT_1 = 1,	/**< 1λֹͣλ */
		SERIAL_STOP_BIT_2,	/**< 2λֹͣλ */
		SERIAL_STOP_BIT_3
	}ser_stop;
	
}serial_config_t;


/**************************************************************************************************/
/**
 *\defgroup serial_base ��������
 *\ingroup serial
 **************************************************************************************************/
/**
 *\ingroup serial_base
 *\brief ��ʼ��������������
 *\param[in] config �������ýṹָ��
 *\return �ɹ����� SFPR_SUCCESS ʧ�� ������Ӧ�Ĵ�����
 */
sfpr_fd_t sfpr_serial_init(serial_config_t* config);

/**
 *\ingroup serial_base
 *\brief �ر�һ���򿪵Ĵ���
 *\param[in] fd �򿪵Ĵ����豸������
 *\return �ɹ����� SFPR_SUCCESS ʧ�� ����-1
 */
int sfpr_serial_close(sfpr_fd_t fd);

/**
 *\ingroup serial_base
 *\brief ����������
 *\param[in] fd �򿪵Ĵ����豸������
 *\param[in] ser_buff ���������ݴ��bufferָ��
 *\param[in] len Ҫ��ȡ���ֽ���
 *\return �ɹ����� ��ȡ�����ֽ���
 */
int sfpr_serial_read(sfpr_fd_t fd, char *ser_buff, int len);

/**
 *\ingroup serial_base
 *\brief �򴮿�д����
 *\param[in] fd �򿪵Ĵ����豸������
 *\param[in] ser_buff ��ser_buff�е�����д�뵽������
 *\param[in] len Ҫд����ֽ���
 *\return �ɹ����� ��ȡ�����ֽ���
 */

int sfpr_serial_write(sfpr_fd_t fd, char *ser_buff, int len);

/**
 *\ingroup serial_base
 *\brief ���ô���������Ϣ
 *\param[in] fd �򿪵Ĵ����豸������
 *\param[in] config �������ýṹָ��
 *\return �ɹ����� SFPR_SUCCESS  ������Ӧ�Ĵ�����
 */
int sfpr_serial_set_config(sfpr_fd_t fd, serial_config_t *config);

/**
 *\ingroup serial_base
 *\brief ��ȡ���ڵ�������Ϣ
 *\param[in] fd �򿪵Ĵ����豸������
 *\param[in] config �������ýṹָ��
 *\return �ɹ����� SFPR_SUCCESS 
 */
int sfpr_serial_get_config(sfpr_fd_t fd,serial_config_t *config);

/**
 *\ingroup serial_struct
 *\brief û���豸
 */
#define NO_DEVICE             -1
/**
 *\ingroup serial_struct
 *\brief �豸���Ϸ�
 */
#define INVALID_DEVICE        -2
/**
 *\ingroup serial_struct
 *\brief ָ��Ϊ��
 */
#define POINTER_NULL          -3
/**
 *\ingroup serial_struct
 *\brief �򿪴���ʧ��
 */
#define OPEN_SERIAL_FAILED    -4
/**
 *\ingroup serial_struct
 *\brief ��������ʧ��
 */
#define SET_ATTR_FAILED       -5
/**
 *\ingroup serial_struct
 *\brief �����ʷǷ�
 */
#define INVALID_SPEED         -6
/**
 *\ingroup serial_struct
 *\brief ����λ�Ƿ�
 */
#define INVALID_DATA_BITS     -7
/**
 *\ingroup serial_struct
 *\brief ֹͣλ�Ƿ�
 */
#define INVALID_STOP_BITS     -8
/**
 *\ingroup serial_struct
 *\brief ��ż����λ�Ƿ�
 */
#define INVALID_PARITY        -9	

#ifdef __cplusplus
}
#endif

#endif   /* _SERIAL_H */