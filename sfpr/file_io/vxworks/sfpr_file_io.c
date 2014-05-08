#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "ioLib.h"
#include "stat.h"
#include "dosFsLib.h"

#include "sfpr_file_io.h"
#include "sfpr_global.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
	����flags ����ʹ�õ���꣺
	O_RDONLY ��ֻ����ʽ���ļ�
	O_WRONLY ��ֻд��ʽ���ļ�
	O_RDWR �Կɶ�д��ʽ���ļ���������������ǻ����

	O_RDONLY (0)  (or READ)  - open for reading only. 
	O_WRONLY (1)  (or WRITE)  - open for writing only. 
	O_RDWR (2)  (or UPDATE)  - open for reading and writing. 
	O_CREAT (0x0200)  - create a file. 
	
S_IRWXU00700 Ȩ�ޣ�������ļ������߾��пɶ�����д����ִ�е�Ȩ�ޡ�
S_IRUSR ��S_IREAD��00400 Ȩ�ޣ�������ļ������߾��пɶ�ȡ��Ȩ�ޡ�
S_IWUSR ��S_IWRITE��00200 Ȩ�ޣ�������ļ������߾��п�д���Ȩ�ޡ�
S_IXUSR ��S_IEXEC��00100 Ȩ�ޣ�������ļ������߾��п�ִ�е�Ȩ�ޡ�
S_IRWXG 00070 Ȩ�ޣ�������ļ��û�����пɶ�����д����ִ�е�Ȩ�ޡ�
S_IRGRP 00040Ȩ�ޣ�������ļ��û�����пɶ���Ȩ�ޡ�
S_IWGRP 00020Ȩ�ޣ�������ļ��û�����п�д���Ȩ�ޡ�
S_IXGRP 00010 Ȩ�ޣ�������ļ��û�����п�ִ�е�Ȩ�ޡ�
S_IRWXO 00007 Ȩ�ޣ����������û����пɶ�����д����ִ�е�Ȩ�ޡ�
S_IROTH 00004 Ȩ�ޣ����������û����пɶ���Ȩ��
S_IWOTH 00002 Ȩ�ޣ����������û����п�д���Ȩ�ޡ�
S_IXOTH 00001 Ȩ�ޣ����������û����п�ִ�е�Ȩ�ޡ�

*/

sfpr_int_t sfpr_file_open(sfpr_file_t **newf, const char *fname,sfpr_uint32_t flag)
{
	int oflags = 0;
	sfpr_fd_t fd;


	if ((flag & SFPR_READ) && (flag & SFPR_WRITE)) 
	{
		oflags = O_RDWR;
	}
	else if (flag & SFPR_READ) 
	{
		oflags = O_RDONLY;
	}
	else if (flag & SFPR_WRITE)
	{
		oflags = O_WRONLY;
	}
	else 
	{
		return SFPR_ERROR; 
	}

	if (flag & SFPR_CREATE) 
	{
		oflags |= O_CREAT;
	}
    if ((flag & SFPR_EXCL) && !(flag & SFPR_CREATE)) 
	{
        return SFPR_ERROR;
    }   

    if (flag & SFPR_APPEND) 
	{
        oflags |= O_APPEND;
    }
    if (flag & SFPR_TRUNCATE) 
	{
        oflags |= O_TRUNC;
    }
	
	/*<�˴���Ҫע��mode������*/
	fd = open(fname,oflags,0777);	/**<mode: �ļ��ɶ�д��ִ��*/
	if(SFPR_ERROR == fd )
	{
		return SFPR_ERROR;
	}
	
	(*newf) = (sfpr_file_t *)malloc(sizeof(sfpr_file_t));
	if(NULL == (*newf))
	{
		return SFPR_ERROR;
	}
	memset((*newf), 0 , sizeof(sfpr_file_t));
	(*newf)->filedes = fd;
	(*newf)->fname = (char*)fname;
	(*newf)->flags = flag;
	
	sfpr_mutex_create(&(*newf)->lock,0);
	return SFPR_SUCCESS ;	
}

/** �ر��ļ�*/
sfpr_int_t sfpr_file_close(sfpr_file_t *file)
{
	sfpr_int_t ret = SFPR_ERROR;
	
	if (NULL == file)
	{
		return SFPR_ERROR;
	}
	
	ret = close(file->filedes);
	
	#ifdef LOG_DEBUG
	printf("sfpr_file_close    ret is :%d\n",ret);
	#endif
	
	if( ret == 0)
	{
		sfpr_mutex_destroy(&file->lock);
		free(file);
		file = NULL;
	}

	return ret;
}

/**
 *	����Ƿ��ȡ�����ļ�β������ѵ��ļ�β��
 *	����0�������������-1��
 */
sfpr_int_t sfpr_file_eof(sfpr_file_t *fptr)
{
	int res = 0;
	sfpr_uint32_t where ;
	struct stat buf;		
	if (NULL == fptr)
	{
		return SFPR_ERROR;
	}

	where = ioctl(fptr->filedes,FIOWHERE,NULL);	
	
	if(stat(fptr->fname,&buf)==-1)
	{								
		return SFPR_ERROR;
	}

	if(where == buf.st_size)						/**sb.st_sizeֻ����ͨ�ļ���Ч*/
	{
		return SFPR_SUCCESS;
	}
	return where;
}

sfpr_int_t sfpr_file_read(sfpr_file_t *thefile, void *buf,sfpr_int_t nbytes)
{
	int byte = SFPR_ERROR ;
	
	if (NULL == thefile)
	{
		return SFPR_ERROR;
	}

	byte = read(thefile->filedes,buf,(size_t)nbytes);
	
	return byte;
}


sfpr_int_t sfpr_file_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes)
{
	int byte = SFPR_ERROR;
	if (NULL == thefile){
		return SFPR_ERROR;
	}
	sfpr_mutex_lock(&thefile->lock);
	byte = write(thefile->filedes,(char *)buf,(size_t)nbytes);
	sfpr_mutex_unlock(&thefile->lock);
	return byte;
}

sfpr_int_t sfpr_file_safe_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes,int whence,int offset)
{
	int byte = SFPR_ERROR;
	if (NULL == thefile){
		return SFPR_ERROR;
	}
	sfpr_mutex_lock(&thefile->lock);
	sfpr_file_seek(thefile,whence,offset);
	byte = write(thefile->filedes,(char *)buf,(size_t)nbytes);
	sfpr_mutex_unlock(&thefile->lock);
	return byte;	
}

sfpr_int_t sfpr_file_putc(char ch, sfpr_file_t *thefile)
{
	sfpr_int_t  nbytes = 1;
	if (NULL == thefile)
	{
		return SFPR_ERROR;
	}

	return sfpr_file_write(thefile, &ch, nbytes);

}	

sfpr_int_t sfpr_file_getc(char *ch, sfpr_file_t *thefile)
{
	sfpr_int_t  nbytes = 1;

	if (NULL == thefile)
	{
		return SFPR_ERROR;
	}
	return sfpr_file_read(thefile, ch, nbytes);
	
}

sfpr_int_t sfpr_file_gets(char *str, int len,sfpr_file_t *thefile)
{
	int count = 0;
	char ch;
	while(count < len){
		if(sfpr_file_read(thefile,&ch,1) < 0){
			if(count == 0)
				return SFPR_ERROR;
			else
				return count;
		}
		*(str+count++) = ch;
		if(ch == '\n')
			return count;	
	}
	return count;
}


sfpr_int_t sfpr_file_puts(const char *str, sfpr_file_t *thefile)
{
	sfpr_int_t status;
	sfpr_int_t  nbytes = 0;
	sfpr_int_t  total_written = 0;
	
	if (NULL == thefile)
	{
		return SFPR_ERROR;
	}
	
	nbytes = strlen(str);

	do 
	{
		status = sfpr_file_write(thefile, str, nbytes);

		str = (char *)str + status;
		nbytes -= status;
		total_written += status;
	
	} while ((nbytes > 0)&&( SFPR_ERROR !=status));
	
	#ifdef LOG_DEBUG
	printf("sfpr_file_puts  write total is :%d\n",total_written);
	#endif
	
	if (( SFPR_ERROR ==status)&&(total_written != strlen(str)))
	{
		return SFPR_ERROR;
	}
	return SFPR_SUCCESS;
}


sfpr_int_t sfpr_file_flush(sfpr_file_t *thefile)
{
	int res = -1;
	
	sfpr_mutex_lock(&thefile->lock);	
	res = ioctl (thefile->filedes, FIOFLUSH, 0 );	
	sfpr_mutex_unlock(&thefile->lock);
	
	return res;
}

sfpr_int_t sfpr_file_lock(sfpr_file_t *thefile, int type)
{

	return SFPR_ERROR;
}

/**< �������*/
sfpr_int_t sfpr_file_unlock(sfpr_file_t *thefile)
{

	return SFPR_ERROR;
}

/**
*	fildes Ϊ�Ѵ򿪵��ļ������ʣ�
*	offset Ϊ���ݲ���where���ƶ���дλ�õ�λ������
*	whereΪ��������һ�֣�
*	SEEK_SET ����offset��Ϊ�µĶ�дλ�á�
*	SEEK_CUR ��Ŀǰ�Ķ�дλ����������offset��λ������
*	SEEK_END ����дλ��ָ���ļ�β��������offset��λ������
*	��whereֵΪSEEK_CUR��SEEK_END ʱ������offet����ֵ�ĳ��֡�
*/
sfpr_int_t sfpr_file_seek(sfpr_file_t *thefile,int whence,int offset)
{
	if (NULL == thefile){
		return SFPR_ERROR;
	}
	return lseek(thefile->filedes,offset,whence);
}

sfpr_int_t sfpr_file_get_size(sfpr_file_t *thefile)
{
	int size = 0;
	if(!thefile)
		return -1;
	
	size = lseek(thefile->filedes,0,SEEK_END);
	lseek(thefile->filedes,0,SEEK_SET);
	return size;   
}

#ifdef __cplusplus
}
#endif
