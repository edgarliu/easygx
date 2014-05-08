
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfpr_file_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
	打开文件，参数flags 所能使用的旗标：
	SFPR_READ 以只读方式打开文件
	SFPR_WRITE 以只写方式打开文件
	SFPR_WRITE|SFPR_READ 以可读写方式打开文件。
*/

sfpr_int_t sfpr_file_open(sfpr_file_t **newf, const char *fname,sfpr_uint32_t flag)
{
	int flags = 0,fd = 0;
	if ((flag & SFPR_READ) && (flag & SFPR_WRITE)) 
	{    
		flags = O_RDWR;    
	}   
	else if (flag & SFPR_READ) 
	{  
		flags = O_RDONLY;   
	}   
	else if (flag & SFPR_WRITE) 
	{        
		flags = O_WRONLY;    
	}    
	else 
	{        
		return SFPR_ERROR;    
	}

	if (flag & SFPR_CREATE) 
	{
        flags |= O_CREAT;
        if (flag & SFPR_EXCL) 
		{
            flags |= O_EXCL;
        }
    }
    if (flag & SFPR_APPEND) 
	{
        flags |= O_APPEND;
    }
    if (flag & SFPR_TRUNCATE) 
	{
        flags |= O_TRUNC;
    }
	
	#ifdef O_BINARY    
	if (flag & SFPR_BINARY) 
	{        
		flags |= O_BINARY;    
	}
	#endif
	
	fd = open(fname,flags,0666);	/**< 打开文件*/
	if(fd < 0)
	{
		return SFPR_ERROR;
	}
	(*newf) = (sfpr_file_t *)malloc(sizeof(sfpr_file_t));
	if(!(*newf))
	{
		return SFPR_ERROR;
	}
	memset((*newf), 0 , sizeof(sfpr_file_t));
	
	(*newf)->fname = (char*)malloc(strlen(fname)+8);
	memset((*newf)->fname,0,strlen(fname)+8);
	strcpy((*newf)->fname,fname);
	(*newf)->flags = flag;
	(*newf)->filedes = fd;
	sfpr_mutex_create(&(*newf)->lock,0);
	
	return SFPR_SUCCESS;
}


/** 关闭文件*/
sfpr_int_t sfpr_file_close(sfpr_file_t *file)
{
	sfpr_int_t res = -1;
	
	res = close(file->filedes);
	if( res == 0)
	{
		sfpr_mutex_destroy(&file->lock);
		file->filedes = 0;
		file->fname = NULL;
		file->flags = 0;
		free(file->fname);
		free(file);
		file = NULL;
	}

	return res;
}

/**
 *	侦测是否读取到了文件尾，如果已到文件尾则
 *	返回0，其他情况返回-1。
 */
sfpr_int_t sfpr_file_eof(sfpr_file_t *fptr)
{
	int res = 0;
	int offset = 0;
	struct stat sb;		/*- 定义stat结构--*/

    if(stat(fptr->fname,&sb)==-1)
	{								/**-stat函数*/
        	return SFPR_ERROR;
    }

   	res =sfpr_file_seek(fptr,SFPR_CUR,offset);		/**得到文件当前的读写位置和文件的大小比较*/
	if(res == sb.st_size)						/**sb.st_size只对普通文件有效*/
	{
		return SFPR_SUCCESS;
	}
	else
	{
		return SFPR_ERROR;
	}
}

sfpr_int_t sfpr_file_read(sfpr_file_t *thefile, void *buf,sfpr_int_t nbytes)
{
	int byte = -1;
	int read_bytes = 0;
	if(nbytes <= 0)
	{
		return SFPR_ERROR;
	}
	do{
		byte = read(thefile->filedes,buf,nbytes);			/**< 写文件*/
		if(byte <= 0)
		{
			return SFPR_ERROR;
		}
		if(nbytes >0)		/**< 计算剩余的字节数*/
		{
			buf += byte;
			nbytes -= byte;
		}
		read_bytes += byte;
	}while(nbytes > 0);	/**< 当读到的字节数大于零时，继续*/
	
	return read_bytes;
}

sfpr_int_t sfpr_file_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes)
{
	int byte = -1;
	int write_byte =0;
	
	sfpr_mutex_lock(&thefile->lock);
	while(nbytes > 0)	/**< 当写入的内容还有时，继续*/
	{
		byte = write(thefile->filedes,buf,nbytes);			/**< 写文件*/
		if(byte <0){
			sfpr_mutex_lock(&thefile->lock);
			return SFPR_ERROR;
		}
		if(nbytes >0){		/**< 计算剩余的字节数*/
			buf += byte;
			nbytes -= byte;
		}
		write_byte += byte;
	}
	fsync(thefile->filedes);	/**< 文件同步写回磁盘 */
	sfpr_mutex_unlock(&thefile->lock);
	return write_byte;
}

sfpr_int_t sfpr_file_safe_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes,int whence,int offset)
{
	int byte = -1;
	int write_byte =0;
	
	sfpr_mutex_lock(&thefile->lock);
	sfpr_file_seek(thefile,whence,offset);
	while(nbytes > 0)	/**< 当写入的内容还有时，继续*/
	{
		byte = write(thefile->filedes,buf,nbytes);			/**< 写文件*/
		if(byte <0){
			sfpr_mutex_lock(&thefile->lock);
			return SFPR_ERROR;
		}
		if(nbytes >0){		/**< 计算剩余的字节数*/
			buf += byte;
			nbytes -= byte;
		}
		write_byte += byte;
	}
	fsync(thefile->filedes);	/**< 文件同步写回磁盘 */
	sfpr_mutex_unlock(&thefile->lock);
	return write_byte;	
}

/**
 *	向指定的文件中写入一个字符。
*/
sfpr_int_t sfpr_file_putc(char ch, sfpr_file_t *thefile)
{
	int res =0;
	int nbytes = 1;

	res = sfpr_file_write(thefile,&ch,nbytes);

	return res;

}	

/**
 *	 从指定的文件中读取一个字符。
 */
sfpr_int_t sfpr_file_getc(char *ch, sfpr_file_t *thefile)
{
	int res =0;
	int nbytes = 1;
	
	res = sfpr_file_read(thefile, ch,nbytes);
	
	return res;
}

/**
 *	 从指定的文件内读入字符并存到参数str所指的内存空间，
 *	 直到出现换行字符、读到文件尾或是已读了size-1个字符为止，
 * 	最后会加上NULL作为字符串结束。
 */
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

/**
 *	将参数str,所指的字符串写入到参数指定的文件内。
 *	返回值 若成功则返回写出的字符个数，返回EOF则表示有错误发生。
 */
sfpr_int_t sfpr_file_puts(const char *str, sfpr_file_t *thefile)
{
	int byte = -1;
	int len = strlen(str);
	byte = sfpr_file_write(thefile,str,len);
	return byte;
}

/**
 *	 强迫将缓冲区内的数据写回指定的文件中。
 *	 返回值 成功返回0，失败返回EOF，错误代码存于errno中。
 */
sfpr_int_t sfpr_file_flush(sfpr_file_t *thefile)
{
	return fsync(thefile->filedes);
}

/**< 锁定文件,参数type情况如下:*/
/*	LOCK_SH 建立共享锁定
 *    LOCK_EX 建立互斥锁定
 *	LOCK_UN 解除文件锁定状态
 *	LOCK_NB 无法建立锁定时，此操作可不被阻断，马上返回进程,
 *	通常与LOCK_SH 或LOCK_EX 做OR（|）组合。单一文件无法同时建立共享锁定和互斥锁定,
 *	成功返回0，失败返回-1
 */
sfpr_int_t sfpr_file_lock(sfpr_file_t *thefile, int type)
{
	int res = -1;
	int flags = 0;
	if (type & SFPR_LOCK_SH) 
	{    
		flags |= LOCK_SH;
	}   
	else if (type & SFPR_LOCK_EX) 
	{  
		flags |= LOCK_EX;
	}
	else
	{
		return SFPR_ERROR;
	}
	
	if(type & SFPR_LOCK_NB)
	{	
		flags |= LOCK_NB;
	}
	res = flock(thefile->filedes, flags);

	return res;
}

/**< 解除锁定成功返回0，失败返回-1*/
sfpr_int_t sfpr_file_unlock(sfpr_file_t *thefile)
{
	int res = -1;
	
	res = flock(thefile->filedes, LOCK_UN); 

	return res;
}

/**
*	fildes 为已打开的文件描述词，
*	offset 为根据参数whence来移动读写位置的位移数。
*	whence为下列其中一种：
*	SEEK_SET 参数offset即为新的读写位置。
*	SEEK_CUR 以目前的读写位置往后增加offset个位移量。
*	SEEK_END 将读写位置指向文件尾后再增加offset个位移量。
*	当whence值为SEEK_CUR或SEEK_END 时，参数offet允许负值的出现。
*	成功返回当前位置到文件开始处的字节数，失败返回-1
*/
sfpr_int_t sfpr_file_seek(sfpr_file_t *thefile,int whence,int offset)
{
	if (NULL == thefile){
		return SFPR_ERROR;
	}
	return  lseek(thefile->filedes,offset ,whence);
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
