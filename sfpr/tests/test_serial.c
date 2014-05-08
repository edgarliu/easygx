#include <stdio.h>
#include <string.h>
#include "sfpr_serial.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined VXWORKS
int serial_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	char buffer[1024];
	sfpr_fd_t fd;
	serial_config_t config;
	char *file = NULL;
	FILE *fp = NULL;
	int retsize = 0,count = 0;

	memset(&config,0,sizeof(serial_config_t));
#if defined VXWORKS

#else
	strcpy(config.dev_name,"/dev/ttyS0");
#endif
	config.ser_baud = 9600;
	config.ser_data = SERIAL_DATA_SIZE_8;
	config.ser_parity = SERIAL_PARITY_NONE;
	config.ser_stop = SERIAL_STOP_BIT_1;	

	if(argc > 1){
		config.ser_baud = atoi(argv[1]);	
	}
	if(argc > 2){
		file = argv[2];	
	}

	printf("Begin Test Serial: %s\n",config.dev_name);
	
	fd = sfpr_serial_init(&config);
	if(fd < 0){
		printf("Init Serial: %s Failed!\n",config.dev_name);
		return 0;
	}
	memset(buffer,0,sizeof(buffer));
	if(file)
	{
		printf("Send file '%s' to Serial %s with baud %d\n",file,config.dev_name,config.ser_baud);
		fp = fopen(file,"r");
		if(!fp)
		{
			fprintf(stderr,"Open file '%s' failed\n",file);
			sfpr_serial_close(fd);
			return 1;
		}
		
		while(!feof(fp))
		{
			memset(buffer,0,sizeof(buffer));
			retsize = fread(buffer,1,1024,fp);
			count += retsize;
			sfpr_serial_write(fd,buffer,retsize);
		}		
		fclose(fp);
		printf("Send file '%s' Success, Send Size:%d\n",file,count);
		
	}
	else
	{
		while(1){
			printf("Please input:");
			fgets(buffer,sizeof(buffer),stdin);
			if(!strncmp(buffer,"quit",4)){
				break;
			}
			sfpr_serial_write(fd,buffer,strlen(buffer));
			memset(buffer,0,sizeof(buffer));		
		}	
	}
		
	sfpr_serial_close(fd);
  return 0;
}

#ifdef __cplusplus
}
#endif
