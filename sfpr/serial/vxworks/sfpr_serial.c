#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Vxworks.h"
#include "sioLib.h"
#include <ioLib.h>

#include "sfpr_serial.h"

static unsigned long speed_rate[20] = {921600, 460800, 230400, 115200, 57600, 38400, 19200, 9600,4800, 1800,
					 2400, 1200, 600, 300, 200, 150, 134, 110, 75, 50 };


sfpr_fd_t sfpr_serial_init(serial_config_t* config)
{
	int ret;
	char szName[32];
	sfpr_fd_t fd;
	if(!strlen(config->dev_name))
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"No Device!!\n");
		#endif
		return NO_DEVICE;
	}
	memset(szName,0,sizeof(szName));
	strncpy(szName,config->dev_name,9);

//	fd = open( config->dev_name, O_RDWR | O_NOCTTY | O_NDELAY , 0);
	fd = open( config->dev_name, O_RDWR, 0);
	if (-1 == fd)
	{ 
		#ifdef LOG_ERROR
		fprintf(stderr,"Can't Open Serial Port");
		#endif
		return OPEN_SERIAL_FAILED;
	}

	sfpr_serial_set_config(fd,config);
	return fd;
}

int sfpr_serial_close(sfpr_fd_t fd)
{
	int res;
	res = close(fd);
	if(res <0)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"close failed!\n");
		#endif
	}
	return res;
}

int sfpr_serial_read(sfpr_fd_t fd, char *ser_buff, int len)
{
	int n;
	if(ser_buff == NULL)
	{
		#ifdef LOG_DEBUG
		fprintf(stderr,"Pointer is Null!\n");
		#endif
		return POINTER_NULL;
	}
	memset(ser_buff,0,len);

	n = read(fd,ser_buff,len);

	return n;
}

int sfpr_serial_write(sfpr_fd_t fd, char *ser_buff, int len)
{
	int n;

	if(ser_buff == NULL)
	{
		#ifdef LOG_DEBUG
		fprintf(stderr,"Pointer is Null!\n");
		#endif
		return POINTER_NULL;
	}

	n = write(fd,ser_buff,len);
	if(n < 0)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_serial_write() | write failed! data:%s\n",ser_buff);
		#endif
	}

	return n;
}

static int set_speed(int fd, int speed)
{
	int res,i;
	for(i =0;i<20;i++)		/*设置波特率*/
	{
		if(speed_rate[i] == speed)
		{
			res = ioctl(fd, FIOBAUDRATE, speed);
			if(res != SFPR_SUCCESS)
				return res;
			break;
		}
	}

	if(i == 20)
	{
		#ifdef LOG_ERROR
		fprintf(stderr,"Invalid Speed!\n");
		#endif
		return INVALID_SPEED;
	}
	return 	SFPR_SUCCESS;
}

int sfpr_serial_set_config(sfpr_fd_t fd, serial_config_t *config)
{
	int ret,flag;

	ret = set_speed(fd, config->ser_baud);	/*设置波特率*/
	if(ret != SFPR_SUCCESS)
	{
		return ret;
	}
	
//	ioctl(fd,FIOFLUSH,0);         /*清空输入输出缓冲*/

//	flag = ioctl(fd, FIOGETOPTIONS ,0);		/*返回当前设备选项的内容*/

/*设置数据位数*/
/*
	switch (config->ser_data)	 
	{
		case SERIAL_DATA_SIZE_5:
			flag |= ioctl(fd, SIO_HW_OPTS_SET, CS5);
			break;
		case SERIAL_DATA_SIZE_6:
			flag |= ioctl(fd, SIO_HW_OPTS_SET, CS6);
			break;
		case SERIAL_DATA_SIZE_7:
			flag |= ioctl(fd, SIO_HW_OPTS_SET, CS7);
			break;
		case SERIAL_DATA_SIZE_8:
			flag |= ioctl(fd, SIO_HW_OPTS_SET, CS8);
			break;
		default:
			#ifdef LOG_ERROR
			fprintf(stderr,"Unsupported data size\n");
			#endif
			return INVALID_DATA_BITS;
	}
	*/
	/*设置校验位*/
	/*
	switch (config->ser_parity)	
  	{
		case SERIAL_PARITY_REQUEST:
			break;
		case SERIAL_PARITY_NONE:
			flag &= ioctl(fd, SIO_HW_OPTS_SET, ~PARENB);
			break;    	
 		case SERIAL_PARITY_ODD: 
			flag |= ioctl(fd, SIO_HW_OPTS_SET, PARENB);
			flag |= ioctl(fd, SIO_HW_OPTS_SET, PARODD); 
			break;   	
 		case SERIAL_PARITY_EVEN:
			flag |= ioctl(fd, SIO_HW_OPTS_SET, PARENB);
			flag &= ioctl(fd, SIO_HW_OPTS_SET, ~PARODD);  
			break;   	
		case SERIAL_PARITY_MARK: 
			break;  	
 		case SERIAL_PARITY_SPACE:
			flag &= ioctl(fd, SIO_HW_OPTS_SET, ~PARENB);
			flag &= ioctl(fd, SIO_HW_OPTS_SET, ~STOPB);
			break;
		default:
			#ifdef LOG_ERROR
			fprintf(stderr,"Unsupported parity\n");
			#endif
			return INVALID_PARITY;
	}
		  */
	
	/* 设置停止位*/ 
	/*
	switch (config->ser_stop)	
	{
		case SERIAL_STOP_BIT_1:
			flag &= ioctl(fd, SIO_HW_OPTS_SET, ~STOPB);
			break;
		case SERIAL_STOP_BIT_2:
			flag |= ioctl(fd, SIO_HW_OPTS_SET, STOPB);
			break;
		case SERIAL_STOP_BIT_3:
			break;
		default:
			#ifdef LOG_ERROR
			fprintf(stderr,"Unsupported stop bits\n");
			#endif
			return INVALID_STOP_BITS;
	}
	*/

	return SFPR_SUCCESS;
}

int sfpr_serial_get_config(sfpr_fd_t fd,serial_config_t *config)
{
	unsigned int flag;

	ioctl(fd, FIOGETOPTIONS,0);
	
	config->ser_baud = ioctl(fd, SIO_BAUD_GET,0);
	
	flag = ioctl(fd, SIO_HW_OPTS_GET,0);

	if(flag & CS5)
	{
		config->ser_data = SERIAL_DATA_SIZE_5;
	}		
	else if(flag & CS6)
	{
		config->ser_data = SERIAL_DATA_SIZE_6;
	}
	else if(flag & CS7)
	{
		config->ser_data = SERIAL_DATA_SIZE_7;
	}
	else
	{
		config->ser_data = SERIAL_DATA_SIZE_8;
	}

	if(flag & STOPB)
	{
		config->ser_stop = SERIAL_STOP_BIT_2;	
	}
	else
	{
		config->ser_stop = SERIAL_STOP_BIT_1;
	}
		
		
	if(flag & ~PARENB)
	{
		config->ser_parity = SERIAL_PARITY_NONE;
	}		
	else if((flag & PARENB) && (flag & PARODD))
	{
		config->ser_parity = SERIAL_PARITY_ODD;
	}
	else
	{
		config->ser_parity = SERIAL_PARITY_EVEN;
	}

	return SFPR_SUCCESS;
}
