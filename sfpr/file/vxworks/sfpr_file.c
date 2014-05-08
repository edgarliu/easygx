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

#include "sfpr_file.h"
#include "sfpr_global.h"


#ifdef __cplusplus
extern "C" {
#endif

sfpr_int_t sfpr_file_isexited(const char *fname)
{
	struct stat buf;   
	if(stat((char*)fname, &buf) < 0)   
	{	
		return  SFPR_ERROR ;	
	}
	return 0;
}

/**< 判断是文件还是目录*/
static sfpr_int_t is_file_or_dir(const char *path)   
{   
   struct stat buf;   
   if(stat((char*)path, &buf) < 0)   
   {   
	return  SFPR_ERROR ;   
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

/** 删除文件*/
sfpr_int_t sfpr_file_remove(const char *path)
{
	int ret =-1;

	if (NULL ==path)
	{
		#ifdef LOG_DEBUG
		printf("sfpr_file_remove path is NULL !!\n");
		#endif
		return SFPR_ERROR;
	}
	if(is_file_or_dir(path) == 1)
	{
		printf("sfpr_file_remove path is =%s\n",path);
		ret = remove(path);
		return ret;
	}
	return SFPR_ERROR;
}

static int sfpr_attrib(const char * fileName,const char * attr)
{
    int  set = SFPR_TRUE ;
    int  status ;
    sfpr_uint8_t bit = 0 ;
    struct stat fileStat;
    int fd ;

    if (attr == NULL)
	{
	return SFPR_ERROR;
	}

    fd = open (fileName, O_RDONLY, 0);

    if (fd == SFPR_ERROR)
	{
	perror(fileName);
	return SFPR_ERROR ;
	}

    if (fstat (fd, &fileStat) == SFPR_ERROR)          /* get file status    */
	{
	printf("Can't get stat on %s\n", fileName );
	return SFPR_ERROR;
	}

    for ( ; *attr != '\0' ; attr ++)
	{
		switch( *attr)
	    {
		    case '+' :
			set = SFPR_TRUE ;
			break ;
		    case '-' :
			set = SFPR_FALSE ;
			break ;
		    case 'A' : case 'a' :
			bit = DOS_ATTR_ARCHIVE ;
			break ;
		    case 'S' : case 's' :
			bit = DOS_ATTR_SYSTEM ;
			break ;
		    case 'H' : case 'h' :
			bit = DOS_ATTR_HIDDEN ;
			break ;
		    case 'R' : case 'r' :
			bit = DOS_ATTR_RDONLY ;
			break ;
		    default:
			printErr("Illegal attribute flag \"%c\", ignored\n", *attr );
	    } /* end of switch */
		if (set)
		{
		    fileStat.st_attrib |= bit ;
		}
		else
		{
		    fileStat.st_attrib &= ~bit ;
		}
	}

	status = ioctl (fd, FIOATTRIBSET, fileStat.st_attrib);
	if (-1== status)
	{
	    printErr ("\nError loading file: errno = %d.\n", errno);
	}

    close(fd);

    return status ;
}

sfpr_int_t sfpr_file_attrs_set(const char *fname,sfpr_uint32_t attributes,int set)
{
	int ret  = -1;
	char attr[3] ;
	
	if (NULL == fname)
	{
		return SFPR_ERROR;
	}
	if (1==set )
	{
	         attr[0]='+';
	}
	else
	{
	         attr[0]='-';
	}
	switch(attributes)
	{
		case SFPR_FILE_ATTR_READONLY:
                 attr[1]='r';
                 attr[2]='\0';
		break;
		case SFPR_FILE_ATTR_HIDDEN:
                 attr[1]='h';
                 attr[2]='\0';
		break;

		default:
		return SFPR_ERROR;
	}
	if (1 == is_file_or_dir(fname))
	{
		ret = sfpr_attrib( fname, attr);	//dosFs only
	}
	
	return ret;

}


sfpr_int_t sfpr_dir_make(const char *path)
{
	char current_path[1024],*pbegin = NULL,*ppos = NULL;
	int depth = 0;
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
		
		if(depth++ > 0){
			mkdir(current_path);
		}
		strcat(current_path,"/");
	}while(1);
	return SFPR_SUCCESS;
}



sfpr_int_t sfpr_dir_remove(const char *path)
{
	if(NULL ==path)
	{
		return SFPR_ERROR;
	}
	return rmdir(path);

}

#ifdef __cplusplus
}
#endif
