
#ifndef _SERIAL_H
#define _SERIAL_H

#include "sfpr_file_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/**
 *\defgroup serial 串口
 *
 **************************************************************************************************/

/**************************************************************************************************/
/**
 *\defgroup serial_struct 数据结构
 *\ingroup serial
 **************************************************************************************************/
/**
 *\ingroup serial_struct
 *\brief 串口设备名的最大长度
 */
#define MAX_DEV_NAME_LEN	32

/**
 *\ingroup serial_struct
 *\brief 串口属性配置结构
 */
typedef struct serial_config_s{
	char dev_name[MAX_DEV_NAME_LEN];	/**< 串口设备名称*/

	unsigned long  ser_baud;  	   	/**< 串口波特率*/
		
	enum ser_data_size{			/**< 串口数据位*/
		SERIAL_DATA_SIZE_5 = 5,	/**< 5位数据位 */
		SERIAL_DATA_SIZE_6,	/**< 6位数据位 */
		SERIAL_DATA_SIZE_7,	/**< 7位数据位 */
		SERIAL_DATA_SIZE_8	/**< 8位数据位 */
	}ser_data;
	
	enum ser_parity_type{			/**<串口奇偶校验位*/
		SERIAL_PARITY_REQUEST,	
		SERIAL_PARITY_NONE,    	/**< 无校验*/
 		SERIAL_PARITY_ODD,     	/**< 奇校验*/
 		SERIAL_PARITY_EVEN,    	/**< 偶校验*/
 		SERIAL_PARITY_MARK,    	
 		SERIAL_PARITY_SPACE
 	}ser_parity;
	
 	enum ser_stop_bit{			/**<串口停止位*/
		SERIAL_STOP_BIT_1 = 1,	/**< 1位停止位 */
		SERIAL_STOP_BIT_2,	/**< 2位停止位 */
		SERIAL_STOP_BIT_3
	}ser_stop;
	
}serial_config_t;


/**************************************************************************************************/
/**
 *\defgroup serial_base 基本函数
 *\ingroup serial
 **************************************************************************************************/
/**
 *\ingroup serial_base
 *\brief 初始化串口属性配置
 *\param[in] config 属性配置结构指针
 *\return 成功返回 SFPR_SUCCESS 失败 返回相应的错误码
 */
sfpr_fd_t sfpr_serial_init(serial_config_t* config);

/**
 *\ingroup serial_base
 *\brief 关闭一个打开的串口
 *\param[in] fd 打开的串口设备描述符
 *\return 成功返回 SFPR_SUCCESS 失败 返回-1
 */
int sfpr_serial_close(sfpr_fd_t fd);

/**
 *\ingroup serial_base
 *\brief 读串口数据
 *\param[in] fd 打开的串口设备描述符
 *\param[in] ser_buff 读出的数据存放buffer指针
 *\param[in] len 要读取的字节数
 *\return 成功返回 读取到的字节数
 */
int sfpr_serial_read(sfpr_fd_t fd, char *ser_buff, int len);

/**
 *\ingroup serial_base
 *\brief 向串口写数据
 *\param[in] fd 打开的串口设备描述符
 *\param[in] ser_buff 将ser_buff中的数据写入到串口中
 *\param[in] len 要写入的字节数
 *\return 成功返回 读取到的字节数
 */

int sfpr_serial_write(sfpr_fd_t fd, char *ser_buff, int len);

/**
 *\ingroup serial_base
 *\brief 设置串口属性信息
 *\param[in] fd 打开的串口设备描述符
 *\param[in] config 属性配置结构指针
 *\return 成功返回 SFPR_SUCCESS  返回相应的错误码
 */
int sfpr_serial_set_config(sfpr_fd_t fd, serial_config_t *config);

/**
 *\ingroup serial_base
 *\brief 获取串口的属性信息
 *\param[in] fd 打开的串口设备描述符
 *\param[in] config 属性配置结构指针
 *\return 成功返回 SFPR_SUCCESS 
 */
int sfpr_serial_get_config(sfpr_fd_t fd,serial_config_t *config);

/**
 *\ingroup serial_struct
 *\brief 没有设备
 */
#define NO_DEVICE             -1
/**
 *\ingroup serial_struct
 *\brief 设备不合法
 */
#define INVALID_DEVICE        -2
/**
 *\ingroup serial_struct
 *\brief 指针为空
 */
#define POINTER_NULL          -3
/**
 *\ingroup serial_struct
 *\brief 打开串口失败
 */
#define OPEN_SERIAL_FAILED    -4
/**
 *\ingroup serial_struct
 *\brief 设置属性失败
 */
#define SET_ATTR_FAILED       -5
/**
 *\ingroup serial_struct
 *\brief 波特率非法
 */
#define INVALID_SPEED         -6
/**
 *\ingroup serial_struct
 *\brief 数据位非法
 */
#define INVALID_DATA_BITS     -7
/**
 *\ingroup serial_struct
 *\brief 停止位非法
 */
#define INVALID_STOP_BITS     -8
/**
 *\ingroup serial_struct
 *\brief 奇偶检验位非法
 */
#define INVALID_PARITY        -9	

#ifdef __cplusplus
}
#endif

#endif   /* _SERIAL_H */