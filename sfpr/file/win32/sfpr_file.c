#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfpr_file.h"
#include <windows.h>
#include <shellapi.h>

#ifdef __cplusplus
extern "C" {
#endif

/**< 判断目录是否为空*/
static sfpr_int_t is_empty_dir(const char *pszDir)
{
    	char path[128];
	WIN32_FIND_DATA wfd;
	HANDLE hFindFile;
	memset(path, 0, 128);
	sprintf(path, "%s\\*.*", pszDir);

	hFindFile = FindFirstFile(path, &wfd);
	if (hFindFile != INVALID_HANDLE_VALUE) {
		do
		{
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp(wfd.cFileName,".") && strcmp(wfd.cFileName,"..")) {

					memset(path, 0, 128);
					sprintf(path, "%s\\%s", pszDir, wfd.cFileName);
					if(-1 == is_empty_dir(path))
						return -1;
				}
			}
			else {
				return -1;
			}

		}while(FindNextFile(hFindFile, &wfd));
	}
	return 0;
}


/**< 判断是文件还是目录*/
static sfpr_int_t is_file_or_dir(const char *path)   
{   
    	DWORD attr;
	attr = GetFileAttributes(path);
	if(-1 == attr)
		return -2;
	else if(attr & FILE_ATTRIBUTE_DIRECTORY)
		return 0;
	else
		return 1;
}

sfpr_int_t sfpr_file_isexited(const char *fname)
{
	DWORD attr;
	attr = GetFileAttributes(fname);
	if(attr == -1) {
		if((2 == GetLastError()) || (3 == GetLastError()))
			return -1;
	}
	return 0;
}

/** 删除文件*/
sfpr_int_t sfpr_file_remove(const char *path)
{
	if(is_file_or_dir(path) == 1)
	{
		if(DeleteFile(path))
			return 0;
		else
			return -1;
	}
	else
		return -1;
}

/**设置文件的操作属性*/
sfpr_int_t sfpr_file_attrs_set(const char *fname,sfpr_uint32_t attributes,int set)
{
	DWORD mode = 0;
	mode = GetFileAttributes(fname);
	if(mode == -1)
		return -1;
	if(set){
		if(attributes == SFPR_FILE_ATTR_READONLY)
			mode |= FILE_ATTRIBUTE_READONLY;
		if(attributes == SFPR_FILE_ATTR_HIDDEN)
			mode |= FILE_ATTRIBUTE_HIDDEN;
	}
	else {
		if(attributes == SFPR_FILE_ATTR_READONLY)
			mode &= ~FILE_ATTRIBUTE_READONLY;
		if(attributes == SFPR_FILE_ATTR_HIDDEN)
			mode &= ~FILE_ATTRIBUTE_HIDDEN;
	}
	if(SetFileAttributes(fname, mode))
		return 0;
	else
		return -1;
}


sfpr_int_t sfpr_dir_make(const char *path)
{
	char current_path[1024],*pbegin = NULL,*ppos = NULL;
	if(!path)
		return -1;

	memset(current_path,0,sizeof(current_path));
	pbegin = (char*)path;

	if( (*pbegin >= 'a' && *pbegin <= 'z') || (*pbegin >= 'A' && *pbegin <= 'Z') ) {
		strncpy(current_path, pbegin, 3);
		pbegin = pbegin+3;
	}

	do{
		ppos = strchr(pbegin,'/');
		if(!ppos)
			break;
		strncat(current_path,pbegin,ppos-pbegin);
		pbegin = ppos+1;
		if(!CreateDirectory(current_path, NULL))
			if(183 != GetLastError())
				return -1;	
		strcat(current_path,"/");
	}while(1);
	return SFPR_SUCCESS;	
/*
	if(*pbegin != '\0') {
		strcat(current_path, pbegin);
		if(!CreateDirectory(current_path, NULL))
			if(183 != GetLastError())
				return -1;	
	}
	return 0;
*/
}


sfpr_int_t sfpr_dir_remove(const char *path)
{
	if(is_file_or_dir(path) == 0)
	{
		if(RemoveDirectory(path))
			return 0;
		else
			return -1;
	}
	else
		return -1;
}

#ifdef __cplusplus
}
#endif
