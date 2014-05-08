
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfpr_file_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
	���ļ�������flags ����ʹ�õ���꣺
	SFPR_READ ��ֻ����ʽ���ļ�
	SFPR_WRITE ��ֻд��ʽ���ļ�
	SFPR_WRITE|SFPR_READ �Կɶ�д��ʽ���ļ���
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
	
	fd = open(fname,flags,0666);	/**< ���ļ�*/
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


/** �ر��ļ�*/
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
 *	����Ƿ��ȡ�����ļ�β������ѵ��ļ�β��
 *	����0�������������-1��
 */
sfpr_int_t sfpr_file_eof(sfpr_file_t *fptr)
{
	int res = 0;
	int offset = 0;
	struct stat sb;		/*- ����stat�ṹ--*/

    if(stat(fptr->fname,&sb)==-1)
	{								/**-stat����*/
        	return SFPR_ERROR;
    }

   	res =sfpr_file_seek(fptr,SFPR_CUR,offset);		/**�õ��ļ���ǰ�Ķ�дλ�ú��ļ��Ĵ�С�Ƚ�*/
	if(res == sb.st_size)						/**sb.st_sizeֻ����ͨ�ļ���Ч*/
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
		byte = read(thefile->filedes,buf,nbytes);			/**< д�ļ�*/
		if(byte <= 0)
		{
			return SFPR_ERROR;
		}
		if(nbytes >0)		/**< ����ʣ����ֽ���*/
		{
			buf += byte;
			nbytes -= byte;
		}
		read_bytes += byte;
	}while(nbytes > 0);	/**< ���������ֽ���������ʱ������*/
	
	return read_bytes;
}

sfpr_int_t sfpr_file_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes)
{
	int byte = -1;
	int write_byte =0;
	
	sfpr_mutex_lock(&thefile->lock);
	while(nbytes > 0)	/**< ��д������ݻ���ʱ������*/
	{
		byte = write(thefile->filedes,buf,nbytes);			/**< д�ļ�*/
		if(byte <0){
			sfpr_mutex_lock(&thefile->lock);
			return SFPR_ERROR;
		}
		if(nbytes >0){		/**< ����ʣ����ֽ���*/
			buf += byte;
			nbytes -= byte;
		}
		write_byte += byte;
	}
	fsync(thefile->filedes);	/**< �ļ�ͬ��д�ش��� */
	sfpr_mutex_unlock(&thefile->lock);
	return write_byte;
}

sfpr_int_t sfpr_file_safe_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes,int whence,int offset)
{
	int byte = -1;
	int write_byte =0;
	
	sfpr_mutex_lock(&thefile->lock);
	sfpr_file_seek(thefile,whence,offset);
	while(nbytes > 0)	/**< ��д������ݻ���ʱ������*/
	{
		byte = write(thefile->filedes,buf,nbytes);			/**< д�ļ�*/
		if(byte <0){
			sfpr_mutex_lock(&thefile->lock);
			return SFPR_ERROR;
		}
		if(nbytes >0){		/**< ����ʣ����ֽ���*/
			buf += byte;
			nbytes -= byte;
		}
		write_byte += byte;
	}
	fsync(thefile->filedes);	/**< �ļ�ͬ��д�ش��� */
	sfpr_mutex_unlock(&thefile->lock);
	return write_byte;	
}

/**
 *	��ָ�����ļ���д��һ���ַ���
*/
sfpr_int_t sfpr_file_putc(char ch, sfpr_file_t *thefile)
{
	int res =0;
	int nbytes = 1;

	res = sfpr_file_write(thefile,&ch,nbytes);

	return res;

}	

/**
 *	 ��ָ�����ļ��ж�ȡһ���ַ���
 */
sfpr_int_t sfpr_file_getc(char *ch, sfpr_file_t *thefile)
{
	int res =0;
	int nbytes = 1;
	
	res = sfpr_file_read(thefile, ch,nbytes);
	
	return res;
}

/**
 *	 ��ָ�����ļ��ڶ����ַ����浽����str��ָ���ڴ�ռ䣬
 *	 ֱ�����ֻ����ַ��������ļ�β�����Ѷ���size-1���ַ�Ϊֹ��
 * 	�������NULL��Ϊ�ַ���������
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
 *	������str,��ָ���ַ���д�뵽����ָ�����ļ��ڡ�
 *	����ֵ ���ɹ��򷵻�д�����ַ�����������EOF���ʾ�д�������
 */
sfpr_int_t sfpr_file_puts(const char *str, sfpr_file_t *thefile)
{
	int byte = -1;
	int len = strlen(str);
	byte = sfpr_file_write(thefile,str,len);
	return byte;
}

/**
 *	 ǿ�Ƚ��������ڵ�����д��ָ�����ļ��С�
 *	 ����ֵ �ɹ�����0��ʧ�ܷ���EOF������������errno�С�
 */
sfpr_int_t sfpr_file_flush(sfpr_file_t *thefile)
{
	return fsync(thefile->filedes);
}

/**< �����ļ�,����type�������:*/
/*	LOCK_SH ������������
 *    LOCK_EX ������������
 *	LOCK_UN ����ļ�����״̬
 *	LOCK_NB �޷���������ʱ���˲����ɲ�����ϣ����Ϸ��ؽ���,
 *	ͨ����LOCK_SH ��LOCK_EX ��OR��|����ϡ���һ�ļ��޷�ͬʱ�������������ͻ�������,
 *	�ɹ�����0��ʧ�ܷ���-1
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

/**< ��������ɹ�����0��ʧ�ܷ���-1*/
sfpr_int_t sfpr_file_unlock(sfpr_file_t *thefile)
{
	int res = -1;
	
	res = flock(thefile->filedes, LOCK_UN); 

	return res;
}

/**
*	fildes Ϊ�Ѵ򿪵��ļ������ʣ�
*	offset Ϊ���ݲ���whence���ƶ���дλ�õ�λ������
*	whenceΪ��������һ�֣�
*	SEEK_SET ����offset��Ϊ�µĶ�дλ�á�
*	SEEK_CUR ��Ŀǰ�Ķ�дλ����������offset��λ������
*	SEEK_END ����дλ��ָ���ļ�β��������offset��λ������
*	��whenceֵΪSEEK_CUR��SEEK_END ʱ������offet����ֵ�ĳ��֡�
*	�ɹ����ص�ǰλ�õ��ļ���ʼ�����ֽ�����ʧ�ܷ���-1
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
