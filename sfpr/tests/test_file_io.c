#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfpr_global.h"
#include "sfpr_thread_mutex.h"
#include "sfpr_file_io.h"
#include "sfpr_file.h"

//char FILENAME[30]="/cygdrive/f/file/test1.txt";

#if defined VXWORKS
int file_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	sfpr_int_t ret =-1;
	sfpr_file_t  *filetest = NULL;
	char filename[256],remove[256];
	char buf1[256];
	int size =256;
	char *tmpbuffer;
	int offset = 0;

	
	tmpbuffer = (char *)malloc(sizeof(char)*1024);
	memset(tmpbuffer,0,1024);
	sfpr_time_msleep(5000);
	printf("---------------------\n");
	printf("Please input file path :");
	fgets(filename,sizeof(filename),stdin);
	
	filename[strlen(filename)-1] = 0;
	atoi(filename);


	while(1)
	{
		printf("-----Please input file operate (open/write/read/.. ):");
		
		fgets(buf1,sizeof(buf1),stdin);
		if(!strncmp(buf1,"quit",4))
		{
			free(tmpbuffer);
			return -1;
		}
		buf1[strlen(buf1)-1] = 0;
		atoi(buf1);
		
		if(0== strcmp(buf1,"open"))
		{
		printf("-----begin open---%s-\n",filename);
		
			ret = sfpr_file_open(&filetest, filename, SFPR_READ|SFPR_WRITE|SFPR_CREATE)  ;
			printf("sfpr_file_open --- ret is :%d--\n",ret);


		}
		else if (0== strcmp(buf1,"write"))
		{
			memset(tmpbuffer,0,1024);
			ret = sfpr_file_open(&filetest, filename, SFPR_READ|SFPR_WRITE)  ;
			printf("sfpr_file_open --- byte is :%d--\n",ret);
			
			printf("Please input write :");
			fgets(tmpbuffer,1024,stdin);
			
			printf("\n");
			
			if(!strncmp(tmpbuffer,"quit",4))
			{
				return -1;
			}
			tmpbuffer[strlen(tmpbuffer)-1] = 0;
			atoi(tmpbuffer);
			printf("write data ::<%s>\n",tmpbuffer);
			
			ret = sfpr_file_write(filetest,tmpbuffer,strlen(tmpbuffer));
			printf("sfpr_file_write --- byte is :%d--\n",ret);

		}
		
		else if (0== strcmp(buf1,"close"))
		{
			sfpr_file_close(filetest);
		}
		
		else if (0== strcmp(buf1,"read"))
		{
			memset(tmpbuffer,0,1024);
			ret = sfpr_file_open(&filetest, filename, SFPR_READ)  ;
			printf("sfpr_file_open --- byte is :%d--\n",ret);
			
			ret = sfpr_file_read(filetest,tmpbuffer ,1024);
			printf("sfpr_file_read --- byte is :%d--\n",ret);

			printf("--sfpr_file_read-read file :<<%s>>\n",tmpbuffer);

		}
		else if (0== strcmp(buf1,"getc"))
		{
			memset(tmpbuffer,0,1024);
			
			sfpr_file_getc(tmpbuffer, filetest);

			printf("--sfpr_file_getc-read file :<<%s>>\n",tmpbuffer);

		}

		else if (0== strcmp(buf1,"puts"))
		{
			memset(tmpbuffer,0,1024);
		
			printf("Please input  a character string(<1024):");
			fgets(tmpbuffer,1024,stdin);
			
			printf("\n");
			
			if(!strncmp(tmpbuffer,"quit",4))
			{
				return -1;
			}
			tmpbuffer[strlen(tmpbuffer)-1] = 0;
			atoi(tmpbuffer);
			printf("data ::<%s>\n",tmpbuffer);
			
			sfpr_file_puts(tmpbuffer,filetest);

		}
		
		else if (0== strcmp(buf1,"gets"))
		{
			memset(tmpbuffer,0,1024);
			sfpr_file_gets(tmpbuffer, 10,filetest);
			printf("--sfpr_file_gets-read file data is  :<<%s>>\n",tmpbuffer);

		}
		else if (0== strcmp(buf1,"seek"))
		{
			ret = sfpr_file_seek(filetest,SFPR_END,offset);
			if(-1 == ret)
			{
				printf("seek error!\n");
			}
			else
			{
				printf("seek ok!\n");
			}
		
		}		
		else if (0== strcmp(buf1,"eof"))
		{
			ret = sfpr_file_eof(filetest);
			if(SFPR_SUCCESS== ret)
			{
				printf("IS EOF !!\n");
			}
			else
			{
				printf("sfpr_file_eof   return ret == %d\n",ret);
			}
		
		}
		else if (0== strcmp(buf1,"remove"))
		{
			printf("Please input remove  file path :");
			fgets(remove,sizeof(remove),stdin);

			remove[strlen(remove)-1] = 0;
			atoi(remove);


			ret = sfpr_file_remove(remove);
			if(SFPR_SUCCESS== ret)
			{
				printf("sfpr_file_remove is ok !!\n");
			}
			else
			{
				printf("sfpr_file_remove   return ret == %d\n",ret);
			}
		
		}
		else if (0== strcmp(buf1,"flush"))
		{
			ret = sfpr_file_flush(filetest);
			if(SFPR_SUCCESS== ret)
			{
				printf("sfpr_file_flush is ok !!\n");
			}
			else
			{
				printf("sfpr_file_flush   return ret == %d\n",ret);
			}
		
		}
		else if (0== strcmp(buf1,"mkdir"))
		{
			ret = sfpr_dir_make(filename);
			if(SFPR_SUCCESS== ret)
			{
				printf("sfpr_dir_make is ok !!\n");
			}
			else
			{
				printf("sfpr_dir_make   return ret == %d\n",ret);
			}
		
		}
		else if (0== strcmp(buf1,"rmdir"))
		{
			ret = sfpr_dir_remove(filename);
			if(SFPR_SUCCESS== ret)
			{
				printf("sfpr_file_remove is ok !!\n");
			}
			else
			{
				printf("sfpr_file_remove   return ret == %d\n",ret);
			}
		
		}
		else if (0== strcmp(buf1,"attr"))
		{
			printf("Please input //set attr file path :");
			fgets(remove,sizeof(remove),stdin);

			remove[strlen(remove)-1] = 0;
			atoi(remove);

			ret = sfpr_file_attrs_set(remove,SFPR_FILE_ATTR_READONLY,1);
			if(SFPR_SUCCESS== ret)
			{
				printf("sfpr_file_attrs_set is ok !!\n");
			}
			else
			{
				printf("sfpr_file_attrs_set   return ret == %d\n",ret);
			}
		
		}
		else if (0== strcmp(buf1,"attr11"))
		{
			printf("Please input //undo set attr file path :");
			fgets(remove,sizeof(remove),stdin);

			remove[strlen(remove)-1] = 0;
			atoi(remove);

			ret = sfpr_file_attrs_set(remove,SFPR_FILE_ATTR_READONLY,0);
			if(SFPR_SUCCESS== ret)
			{
				printf("sfpr_file_attrs_set is ok !!\n");
			}
			else
			{
				printf("sfpr_file_attrs_set   return ret == %d\n",ret);
			}
		
		}

		
		else
		{
		
			printf("\n");
		}
		
	
	}
	sfpr_file_close(filetest);

	return SFPR_SUCCESS;


}

