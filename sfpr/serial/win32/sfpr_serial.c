
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "sfpr_serial.h"

#ifdef __cplusplus
extern "C" {
#endif

sfpr_fd_t sfpr_serial_init(serial_config_t* config)
{
	int ret;
	char szName[32];
	sfpr_fd_t fd;

	if(!strlen(config->dev_name)){
		fprintf(stderr,"No Device!!\n");
		return (sfpr_fd_t)NO_DEVICE;
	}
	memset(szName,0,sizeof(szName));
	strncpy(szName,config->dev_name,sizeof(szName)-1);
	if(strcmp(szName,"COM"))
	{
		fprintf(stderr,"Invalid Device Name!\n");
		return (sfpr_fd_t)INVALID_DEVICE;
	}

	fd = CreateFile(config->dev_name,GENERIC_READ | GENERIC_WRITE,0,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
	if (INVALID_HANDLE_VALUE == fd){ 
		fprintf(stderr,"Can't Open Serial Port");
		return (sfpr_fd_t)OPEN_SERIAL_FAILED;
	}
	ret = sfpr_serial_set_config(fd,config);
	return fd;
}

int sfpr_serial_close(sfpr_fd_t fd)
{
	CloseHandle(fd);
	return 0;
}

int sfpr_serial_read(sfpr_fd_t fd, char *ser_buff, int len)
{
	unsigned long n = 0;
	if(ser_buff == NULL){
		fprintf(stderr,"Pointer is Null!\n");
		return POINTER_NULL;
	}
	memset(ser_buff,0,len);
	ReadFile(fd,ser_buff,len,&n,NULL);
	return (int)n;
}

int sfpr_serial_write(sfpr_fd_t fd, char *ser_buff, int len)
{
	unsigned long n = 0;
	if(ser_buff == NULL){
		fprintf(stderr,"Pointer is Null!\n");
		return POINTER_NULL;
	}
	WriteFile(fd,ser_buff,len,&n,NULL);
	return (int)n;
}

int sfpr_serial_set_config(sfpr_fd_t fd, serial_config_t *config)
{
	int ret = 0;
	DCB dcb;
	if(GetCommState(fd,&dcb)){
		return -1;
	}
	dcb.BaudRate = config->ser_baud;
	dcb.ByteSize = config->ser_data;
	dcb.Parity = config->ser_parity;
	dcb.StopBits = config->ser_stop;
	ret = SetCommState(fd,&dcb);
	return ret;
}

int sfpr_serial_get_config(sfpr_fd_t fd,serial_config_t *config)
{
	DCB dcb;
	if(GetCommState(fd,&dcb)){
		return -1;
	}
	config->ser_baud = dcb.BaudRate;
	config->ser_data = dcb.ByteSize;
	config->ser_parity = dcb.Parity;
	config->ser_stop = dcb.StopBits;
	return 0;
}

#ifdef __cplusplus
}
#endif
