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
	参数flags 所能使用的旗标：
	O_RDONLY 以只读方式打开文件
	O_WRONLY 以只写方式打开文件
	O_RDWR 以可读写方式打开文件。上述三种旗标是互斥的

	O_RDONLY (0)  (or READ)  - open for reading only. 
	O_WRONLY (1)  (or WRITE)  - open for writing only. 
	O_RDWR (2)  (or UPDATE)  - open for reading and writing. 
	O_CREAT (0x0200)  - create a file. 
	
S_IRWXU00700 权限，代表该文件所有者具有可读、可写及可执行的权限。
S_IRUSR 或S_IREAD，00400 权限，代表该文件所有者具有可读取的权限。
S_IWUSR 或S_IWRITE，00200 权限，代表该文件所有者具有可写入的权限。
S_IXUSR 或S_IEXEC，00100 权限，代表该文件所有者具有可执行的权限。
S_IRWXG 00070 权限，代表该文件用户组具有可读、可写及可执行的权限。
S_IRGRP 00040权限，代表该文件用户组具有可读的权限。
S_IWGRP 00020权限，代表该文件用户组具有可写入的权限。
S_IXGRP 00010 权限，代表该文件用户组具有可执行的权限。
S_IRWXO 00007 权限，代表其他用户具有可读、可写及可执行的权限。
S_IROTH 00004 权限，代表其他用户具有可读的权限
S_IWOTH 00002 权限，代表其他用户具有可写入的权限。
S_IXOTH 00001 权限，代表其他用户具有可执行的权限。

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
	
	/*<此处需要注意mode的设置*/
	fd = open(fname,oflags,0777);	/**<mode: 文件可读写可执行*/
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

/** 关闭文件*/
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
 *	侦测是否读取到了文件尾，如果已到文件尾则
 *	返回0，其他情况返回-1。
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

	if(where == buf.st_size)						/**sb.st_size只对普通文件有效*/
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

/**< 解除锁定*/
sfpr_int_t sfpr_file_unlock(sfpr_file_t *thefile)
{

	return SFPR_ERROR;
}

/**
*	fildes 为已打开的文件描述词，
*	offset 为根据参数where来移动读写位置的位移数。
*	where为下列其中一种：
*	SEEK_SET 参数offset即为新的读写位置。
*	SEEK_CUR 以目前的读写位置往后增加offset个位移量。
*	SEEK_END 将读写位置指向文件尾后再增加offset个位移量。
*	当where值为SEEK_CUR或SEEK_END 时，参数offet允许负值的出现。
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
