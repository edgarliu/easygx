#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "sfpr_file.h"

#ifdef __cplusplus
extern "C" {
#endif

/**< 判断目录是否为空*/
static sfpr_int_t is_empty_dir(const char *pszDir)
{
    DIR *pstDirp = NULL;
    struct dirent *pstDirStru = NULL;
    sfpr_int_t ulFileNum = 0;
    sfpr_int_t ret = SFPR_SUCCESS;
    sfpr_char_t szDir[128];
    
    memset(szDir, 0, sizeof(szDir));
    
    pstDirp = opendir(pszDir);
    if (NULL == pstDirp)
    {
        printf("open dir error\n");
        return -1;
    }
    while (pstDirStru = readdir(pstDirp))
    {
        strcpy(szDir, pszDir);
        if (DT_DIR == pstDirStru->d_type)
        {
            if (strcmp(pstDirStru->d_name,".") && strcmp(pstDirStru->d_name,".."))
            {
                ret = 0;
                memset(szDir, 0, sizeof(szDir));
                sprintf(szDir, "%s/%s", pszDir, pstDirStru->d_name);
                ret = is_empty_dir(szDir);	/**< 递归查找目录是否为空*/
				if(ret != SFPR_SUCCESS)
					break;
            }
        }
        else
        {
			ret = -1;
			break;
        }
    }
    closedir(pstDirp);

    return ret;
}


/**< 判断是文件还是目录*/
static sfpr_int_t is_file_or_dir(const char *path)   
{   
   struct stat buf;   
   if(lstat(path, &buf) < 0)   
   {   
           printf("lstat error for %s\r\n", path);  
           return -2;   
   }   

   if(S_ISDIR(buf.st_mode))			/**< 是目录返回0*/
   {
   		return 0;
   }
                     
   else if(S_ISREG(buf.st_mode))	/**< 是文件返回1*/
   {
   		 return 1;
   }
  
}

/**< 获取当前用户对文件的读写权限*/
static char *mode_to_get(int  mode,char *str)
{
	char *ch;
	ch = malloc(sizeof(char)*4);
	int i = 0,j =0;	
	str[0]='-';
	if(mode & S_IRUSR)
		str[1]='r';		/*--用户的三个属性-*/
	else 
		str[1]='-';
	if(mode & S_IWUSR) 
		str[2]='w';
	else 
		str[2]='-';
	if(mode & S_IXUSR)
		str[3]='x';
	else
		str[3]='-';
	str[4] = '\0';
	for(i =0 ;i<4;i++)
	{
		if(str[i] != '-')
		{
			ch[j] = str[i];
			j++;
		}
	}
	ch[j] = '\0';
	
	return ch;
}

sfpr_int_t sfpr_file_isexited(const char *fname)
{
	int flags = 0,fd = 0;
	int res = access(fname, F_OK);
	return res;
}

/** 删除文件*/
sfpr_int_t sfpr_file_remove(const char *path)
{
	
	if(access(path,F_OK) == 0)
	{
		if(is_file_or_dir(path) == 1)
		{
			unlink(path);
			return SFPR_SUCCESS;
		}
		else
		{
			#ifdef LOG_DEBUG
			printf("delete %s: is not file\n",path);
			#endif
			return SFPR_ERROR;
		}
	}
	else
	{
		#ifdef LOG_DEBUG
		printf("delete failed : %s ,no such path!\n",path);
		#endif
		return SFPR_ERROR;
	}
}

/**设置文件的操作属性*/
sfpr_int_t sfpr_file_attrs_set(const char *fname,sfpr_uint32_t attributes,int set)
{
	int res = -1;
	int mode = 0;
	struct stat sb;		/*- 定义stat结构--*/
	char tmp[256]=".\0";	
	char *hidden = tmp;
	
	strcat(hidden,fname);

	if(1 == set)		/*设置属性*/
	{
		if(stat(fname,&sb) == -1)
		{								/**-stat函数*/
		    	return SFPR_ERROR;
		}
		mode = sb.st_mode;
		if (attributes & SFPR_FILE_ATTR_READONLY)	/**设置只读属性**/
		{  
			mode &= ~S_IWUSR ;
			mode &= ~S_IWGRP ;
			mode &= ~S_IWOTH ; 
			mode |= S_IRUSR |S_IRGRP | S_IROTH;
		}
		
		if(attributes & SFPR_FILE_ATTR_EXECUTABLE)		/**设置可执行**/
		{
			mode |= S_IXUSR ;
			mode |= S_IXGRP ;
			mode |= S_IXOTH ;  
		}
		
	    if(attributes & SFPR_FILE_ATTR_HIDDEN)		/**设置隐藏属性*/
		{
			if(rename(fname, hidden) == -1)
			{
				return SFPR_ERROR;
			}
			res = chmod(hidden, mode );
			if(res == -1)
			{
				return SFPR_ERROR;
			}
			return res;
		}

	}
	else if(0 == set)	/*取消属性*/
	{	
		if(0 == access(fname,F_OK) )//如果文件存在
		{
			if(stat(fname,&sb) == -1)
			{								/**-stat函数*/
			    	return SFPR_ERROR;
			}
			mode = sb.st_mode;
		}
		else if(0 == access(hidden,F_OK) )
		{
			if(stat(hidden,&sb) == -1)
			{					/**-stat函数*/
		        	return SFPR_ERROR;
		    }
			mode = sb.st_mode;
		}
		else
		{
		    	return SFPR_ERROR;
	
		}

		if (attributes & SFPR_FILE_ATTR_READONLY)	/**取消只读属性**/
		{  
			mode |= S_IWUSR ;
			mode |= S_IWGRP ;
			mode |= S_IWOTH ;
			mode |= S_IRUSR |S_IRGRP | S_IROTH;
		}
	
		if(attributes & SFPR_FILE_ATTR_EXECUTABLE)	/**取消可执行**/
		{
			mode &= ~S_IXUSR ;
			mode &= ~S_IXGRP ;
			mode &= ~S_IXOTH ; 
		}
		
		if(attributes & SFPR_FILE_ATTR_HIDDEN)		/**取消隐藏属性*/
		{
			if(rename(hidden,fname) == -1)
			{
				return SFPR_ERROR;
			}
		}

	}
	
	if(0 == access(fname,F_OK) )  //如果文件存在
	{
		res = chmod( fname, mode );
		if(res == -1)
		{
			return SFPR_ERROR;
		}		
	}
	else if(0 == access(hidden,F_OK) )
	{
		res = chmod( hidden, mode );
		if(res == -1)
		{
			return SFPR_ERROR;
		}	
	}
	return res;
}


sfpr_int_t sfpr_dir_make(const char *path)
{
	char current_path[1024],*pbegin = NULL,*ppos = NULL;
	if(!path){
		return -1;
	}
	memset(current_path,0,sizeof(current_path));
	pbegin = (char*)path;

	//判断是不是绝对路径
	if(*pbegin == '/'){
		strcat(current_path,"/");
		pbegin = pbegin+1;
	}
	do{
		ppos = strchr(pbegin,'/');
		if(!ppos){
			break;
		}
		strncat(current_path,pbegin,ppos-pbegin);
		pbegin = ppos+1;
		
		mkdir(current_path,0755);
		if(errno == EEXIST){//目录已经存在
		}else{//目录创建失败
			return SFPR_ERROR;
		}
		strcat(current_path,"/");
	}while(1);
	return SFPR_SUCCESS;	
/*
	if(access(path,F_OK) != 0){
		mkdir(path,0755);
		return SFPR_SUCCESS;
	}else{
		return SFPR_ERROR;
	}
	*/
}


sfpr_int_t sfpr_dir_remove(const char *path)
{
	if(access(path,F_OK) == 0)
	{
		if(is_empty_dir(path))
		{
			return SFPR_ERROR;
		}
		else
		{
			if(is_file_or_dir(path) == 0)
			{
				rmdir(path);
				return SFPR_SUCCESS;
			}
			else
			{
				return SFPR_ERROR;
			}
			
		}
	}
	else
	{
		printf("无法删除 %s: 没有那个目录或文件\n",path);
		return SFPR_ERROR;
	}
}

#ifdef __cplusplus
}
#endif
