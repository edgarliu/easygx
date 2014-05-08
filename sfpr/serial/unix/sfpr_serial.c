
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <termios.h>

#include "sfpr_serial.h"

//The baud of serial port
struct tcspeed{
	unsigned long baud;
	speed_t code;
};

static struct tcspeed tcspeeds[] = {
        {50, B50},
        {75, B75},
        {110, B110},
        {134, B134},
        {150, B150},
        {200, B200},
        {300, B300},
        {600, B600},
        {1200, B1200},
        {1800, B1800},
        {2400, B2400},
        {4800, B4800},
        {9600, B9600},
        {19200, B19200},
        {38400, B38400},
        {57600, B57600},
        {115200, B115200},
        {230400, B230400},
        {460800, B460800},
        {921600, B921600}
//        {0, B0},
//        {0, 0}
};

sfpr_fd_t sfpr_serial_init(serial_config_t* config)
{
	int ret;
	char szName[32];
	sfpr_fd_t fd;
	
	if(!strlen(config->dev_name))
	{
		fprintf(stderr,"No Device!!\n");
		return NO_DEVICE;
	}
	memset(szName,0,sizeof(szName));
	strncpy(szName,config->dev_name,sizeof(szName)-1);
	if(strcmp(szName,"/dev/ttyS"))
	{
		fprintf(stderr,"Invalid Device Name!\n");
		return INVALID_DEVICE;
	}

	fd = open( config->dev_name, O_RDWR | O_NOCTTY | O_NDELAY);
	if (-1 == fd)
	{ 
		fprintf(stderr,"Can't Open Serial Port");
		return OPEN_SERIAL_FAILED;
	}
	fcntl(fd,F_SETFL,0);

	ret = sfpr_serial_set_config(fd,config);
	return fd;
}

int sfpr_serial_close(sfpr_fd_t fd)
{
	close(fd);
	return 0;
}

int sfpr_serial_read(sfpr_fd_t fd, char *ser_buff, int len)
{
	int n;
	if(ser_buff == NULL)
	{
		fprintf(stderr,"Pointer is Null!\n");
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
		fprintf(stderr,"Pointer is Null!\n");
		return POINTER_NULL;
	}
	n = write(fd,ser_buff,len);
	if(n < 0)
		fprintf(stderr,"write %s failed!\n",ser_buff);
	return n;
}

static int set_speed(int fd, int speed)
{
	int   i,flag = 0 ;
	int   status;
	struct termios   Opt;
  
	tcgetattr(fd, &Opt);
	for ( i= 0;  i < sizeof(tcspeeds) / sizeof(struct tcspeed);  i++)
	{
		if  (speed == tcspeeds[i].baud)
		{
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt,tcspeeds[i].code);
			cfsetospeed(&Opt,tcspeeds[i].code);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if  (status != 0)
			{
				perror("tcsetattr speed");
				return SET_ATTR_FAILED;
			}
			flag = 1;
//			tcflush(fd,TCIOFLUSH);
			break;
		}
	}
	if(flag == 0)
	{
		fprintf(stderr,"Invalid Speed!\n");
		return INVALID_SPEED;
	}
	return SFPR_SUCCESS;

}

static int set_other(int fd, serial_config_t* config)
{
	struct termios options;

	tcgetattr(fd,&options);

	options.c_cflag &= ~CSIZE;
	switch (config->ser_data) /*��������λ��*/
	{
	case SERIAL_DATA_SIZE_5:
		options.c_cflag |= CS5;
		break;
	case SERIAL_DATA_SIZE_6:
		options.c_cflag |= CS6;
		break;
	case SERIAL_DATA_SIZE_7:
		options.c_cflag |= CS7;
		break;
	case SERIAL_DATA_SIZE_8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr,"Unsupported data size\n");
		return INVALID_DATA_BITS;
	}

	switch (config->ser_parity)
  	{
//	case SERIAL_PARITY_REQUEST:
//		break;
	case SERIAL_PARITY_NONE:
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case SERIAL_PARITY_ODD:
		options.c_cflag |= PARENB;
		options.c_cflag |= PARODD;
		options.c_iflag |= INPCK;
		break;
	case SERIAL_PARITY_EVEN:
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
//	case SERIAL_PARITY_MARK:
//		break;
	case SERIAL_PARITY_SPACE:
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return INVALID_PARITY;
	}
	  /* ����ֹͣλ*/   
	switch (config->ser_stop)
	{
	case SERIAL_STOP_BIT_1:
		options.c_cflag &= ~CSTOPB;
		break;
	case SERIAL_STOP_BIT_2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported stop bits\n");
		return INVALID_STOP_BITS;
	}

//	options.c_cc[VTIME] = 150; // 15 seconds
//	options.c_cc[VMIN] = 0;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST;

	tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("tcsetattr other");
		return SET_ATTR_FAILED;
	}
	return SFPR_SUCCESS;
}

int sfpr_serial_set_config(sfpr_fd_t fd, serial_config_t *config)
{
	int ret;
	ret = set_speed(fd,config->ser_baud);
	if(ret != SFPR_SUCCESS)
		return ret;
	ret = set_other(fd,config);
	return ret;
}


/******************************************
 *ע��:
 *�ṹ struct termios�ĳ�Աc_cflag����������λ, У��λ, ֹͣλ
 *����Ϣ. c_cflag��һ��unsigned int ���͵�����,�����ø���λ������
 *������Ϣ,����������ʾ:
 *
 * λ:         14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 * (4800, 8N1)  0  0  0  1  1  0  0  1  0  1  0  1  1  0  0
 *����:  0---3, 12 λ  ���沨������Ϣ,
 *	 4---5  λ    ��������λ��Ϣ
			 00 5λ����λ 
			 01  6λ   
			 10  7 λ
			 11 8 λ
 *	 6  λ        ����ֹͣλ��Ϣ
			0  1λֹͣλ
			1  2 λֹͣλ
 *       7   λ       ��
 *       8---9  λ    ����У��λ��Ϣ
			00 ��У��
			11 ��У��
			01 żУ��

 *������Щ��Ϣ���ǾͿ��Դ�c_cflag�����з�����������Ҫ����Ϣ,����
 *ʵ�����º���get_serial_config()��ʾ
 ****************************************/
int sfpr_serial_get_config(sfpr_fd_t fd,serial_config_t *config)
{
	struct termios options;
	int i;
	unsigned int flag;
	
	tcgetattr(fd,&options);
	
	flag = options.c_cflag&__MAX_BAUD;
	for ( i= 0;  i < sizeof(tcspeeds) / sizeof(struct tcspeed);  i++)
	{
		if  (flag == tcspeeds[i].code)
		{
			config->ser_baud = tcspeeds[i].baud;
			break;
		}
	}
	
	flag = options.c_cflag&CSIZE;
	flag >>=4;
	switch (flag)
	{
	case 0:
		config->ser_data = SERIAL_DATA_SIZE_5;
		break;
	case 1:
		config->ser_data = SERIAL_DATA_SIZE_6;
		break;
	case 2:
		config->ser_data = SERIAL_DATA_SIZE_7;
		break;
	case 3:
		config->ser_data = SERIAL_DATA_SIZE_8;
		break;
	}	

	flag = options.c_cflag&CSTOPB;
	flag >>= 6;
	if(flag == 0)
		config->ser_stop = SERIAL_STOP_BIT_1;
	else
		config->ser_stop = SERIAL_STOP_BIT_2;
		
	flag = options.c_cflag&(PARENB | PARODD);
	flag >>= 8;
	switch (flag)
	{
	case 0:
		config->ser_parity = SERIAL_PARITY_NONE;
		break;
	case 1:
		config->ser_parity = SERIAL_PARITY_EVEN;
		break;
	case 3:
		config->ser_parity = SERIAL_PARITY_ODD;
		break;
	}
	return SFPR_SUCCESS;
}

