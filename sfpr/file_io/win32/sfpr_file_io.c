
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
	sfpr_file_t *file = NULL;
	DWORD dwDesiredAccess = 0;
	DWORD dwCreationDisposition = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	OFSTRUCT	tempofn;
	int bexist = 1;

	hFile = (HANDLE)OpenFile(fname, &tempofn, OF_EXIST);
	if(hFile == INVALID_HANDLE_VALUE)
		bexist = 0;
	hFile = INVALID_HANDLE_VALUE;
	
	if ((flag & SFPR_READ) && (flag & SFPR_WRITE)) 
		dwDesiredAccess = GENERIC_READ | GENERIC_WRITE; 
	else if (flag & SFPR_READ) 
		dwDesiredAccess = GENERIC_READ;
	else if (flag & SFPR_WRITE)
		dwDesiredAccess = GENERIC_WRITE;
	else
		return SFPR_ERROR;


    if ( (flag & SFPR_TRUNCATE) && bexist) 
        dwCreationDisposition |= TRUNCATE_EXISTING;
	else if(bexist)
		dwCreationDisposition = dwCreationDisposition | OPEN_EXISTING;
	else if(flag & SFPR_CREATE)
		dwCreationDisposition = dwCreationDisposition | CREATE_NEW;

	hFile = CreateFile( fname, dwDesiredAccess, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL );
	if(hFile == INVALID_HANDLE_VALUE){
		return SFPR_ERROR;
	}
	file = (sfpr_file_t *)malloc(sizeof(sfpr_file_t));
	if(!file){
		return SFPR_ERROR;
	}
	
	memset(file, 0 , sizeof(sfpr_file_t));
	file->fname = (char*)malloc(strlen(fname)+8);
	memset(file->fname,0,strlen(fname)+8);
	strcpy(file->fname,fname);
	file->flags = flag;
	file->filedes = hFile;
	sfpr_mutex_create(&(file->lock),0);
	*newf = file;
	return SFPR_SUCCESS;
}


/** �ر��ļ�*/
sfpr_int_t sfpr_file_close(sfpr_file_t *file)
{
	sfpr_int_t res = 0;
	
	res = CloseHandle((HANDLE)file->filedes);
	if(!res)
		res = -1;
	
	if( res == 0){
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
	int size = 0;
	size = GetFileSize((HANDLE)fptr->filedes, NULL);
   	if(size == -1)
	{
		return SFPR_ERROR;
    }

   	res =sfpr_file_seek(fptr,SFPR_CUR,offset);
	if(res == size)
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
	int res = 0;
	int read_bytes = 0;
	if(nbytes <= 0)
	{
		return SFPR_ERROR;
	}
	res = ReadFile((HANDLE)thefile->filedes, buf, nbytes, (LPDWORD)&read_bytes, NULL);
	if(!res)
		return SFPR_ERROR;
	return read_bytes;
}


sfpr_int_t sfpr_file_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes)
{
	int res = 0;
	int write_bytes = 0;
	sfpr_mutex_lock(&thefile->lock);
	res = WriteFile((HANDLE)thefile->filedes, buf, nbytes, (LPDWORD)&write_bytes, NULL);
	if(!res)
		return SFPR_ERROR;
	FlushFileBuffers((HANDLE)thefile->filedes);
	sfpr_mutex_unlock(&thefile->lock);
	return write_bytes;
}

sfpr_int_t sfpr_file_safe_write(sfpr_file_t *thefile, const void *buf,sfpr_int_t nbytes,int whence,int offset)
{
	int res = 0;
	int write_bytes = 0;
	sfpr_mutex_lock(&thefile->lock);
	sfpr_file_seek(thefile,whence,offset);
	res = WriteFile((HANDLE)thefile->filedes, buf, nbytes, (LPDWORD)&write_bytes, NULL);
	if(!res){
		sfpr_mutex_unlock(&thefile->lock);
		return SFPR_ERROR;
	}
	FlushFileBuffers((HANDLE)thefile->filedes);
	sfpr_mutex_unlock(&thefile->lock);
	return write_bytes;	
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
	int res = -1;
	res = FlushFileBuffers((HANDLE)thefile->filedes);
	if(!res)
		res = -1;
	return res;
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
	OVERLAPPED ov;
	int flags = 0;
	int size = 0;
	
	memset(&ov, 0, sizeof(OVERLAPPED));
	size = GetFileSize((HANDLE)thefile->filedes,NULL);
	if(size == -1)
		return -1;
	if(type & SFPR_LOCK_EX)
		flags |= LOCKFILE_EXCLUSIVE_LOCK;
	if(type & SFPR_LOCK_NB)
		flags |= LOCKFILE_FAIL_IMMEDIATELY;
	res = LockFileEx((HANDLE)thefile->filedes, flags , 0, size, 0, &ov);
	if(!res)
		res = -1;
	return res;
}

/**< ��������ɹ�����0��ʧ�ܷ���-1*/
sfpr_int_t sfpr_file_unlock(sfpr_file_t *thefile)
{
	int res = -1;
	OVERLAPPED ov;
	int size = 0;
	
	memset(&ov, 0, sizeof(OVERLAPPED));
	size = GetFileSize((HANDLE)thefile->filedes,NULL);
	if(size == -1)
		return -1;
	res = UnlockFileEx((HANDLE)thefile->filedes, 0, size, 0, &ov);
	if(!res)
		res = -1;
	return res;
}

/**
*	fildes Ϊ�Ѵ򿪵��ļ������ʣ�
*	offset Ϊ���ݲ���where���ƶ���дλ�õ�λ������
*	whereΪ��������һ�֣�
*	SEEK_SET ����offset��Ϊ�µĶ�дλ�á�
*	SEEK_CUR ��Ŀǰ�Ķ�дλ����������offset��λ������
*	SEEK_END ����дλ��ָ���ļ�β��������offset��λ������
*	��whereֵΪSEEK_CUR��SEEK_END ʱ������offet����ֵ�ĳ��֡�
*	�ɹ����ص�ǰλ�õ��ļ���ʼ�����ֽ�����ʧ�ܷ���-1
*/
sfpr_int_t sfpr_file_seek(sfpr_file_t *thefile,int whence,int offset)
{
	int res  = -1;
	DWORD movemode;
	if( whence == SFPR_SET){
		if(offset < 0){
			return res;
		}
		movemode = FILE_BEGIN;
	}
	else if(whence == SFPR_CUR)
		movemode = FILE_CURRENT;
	else if(whence == SFPR_END)
		movemode = FILE_END;
	res = SetFilePointer((HANDLE)thefile->filedes,offset , NULL, movemode);
	return res;
}

sfpr_int_t sfpr_file_get_size(sfpr_file_t *thefile)
{
	int size = 0;
	if(!thefile)
		return -1;
	size = GetFileSize((HANDLE)thefile->filedes,NULL);
	if(size == -1)
		return -1;
	return size;   
}

#ifdef __cplusplus
}
#endif
