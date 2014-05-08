
#include <stdarg.h>
#include <stdio.h>

#include "sfpr_thread_mutex.h"
#include "sfpr_time.h"

#include "sfpr_log.h"
#ifdef VXWORKS
#include  <taskLib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sfpr_log_s{
	char file[SFPR_LOG_FILE_LEN];
	char suffix[8];
	int level;
	unsigned long flag;

	char curr_file[SFPR_LOG_CURRFILE_LEN];
	char curr_date[64];
	int curr_time;
	int last_time;

	char *line;

	FILE *fp;
	sfpr_mutex_t mutex;
	
	int save_days;
	int max_file_size;
	int max_dir_size;
};

static char *sfpr_log_level[] = {
	"fatal",
	"error",
	"warn",
	"notice",
	"info",
	"debug",
	"debug1",
	"debug2",
	"debug3",
	NULL
};

sfpr_log_t *sfpr_log_new()
{
	sfpr_log_t *log = malloc(sizeof(sfpr_log_t));
	if(log){
		memset(log,0,sizeof(sfpr_log_t));
	}
	return log;
}

int sfpr_log_init(sfpr_log_t* log,char *log_file,int level,unsigned long flag)
{
	char *ppos = NULL;
	if(!log || !log_file){
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_log_init() | error | Invalid param\n");
		#endif
		return -1;
	}
	memset(log,0,sizeof(sfpr_log_t));
	log->fp = NULL;

	ppos = strrchr(log_file,'.');
	if(ppos){
		strncpy(log->file,log_file,ppos - log_file);
		strcpy(log->suffix,ppos);
	}else{
		strncpy(log->file,log_file,sizeof(log->file));			
		strcpy(log->suffix,".log");
	}

	log->level=level;
	log->level = (log->level < SFPR_LOG_FATAL)?SFPR_LOG_FATAL:log->level;
	log->level = (log->level > SFPR_LOG_DEBUG_L3)?SFPR_LOG_DEBUG_L3:log->level;

	log->flag = flag;

	log->save_days = 30;
	log->max_file_size = 0;
	log->max_dir_size = 0;

	log->line = (char *)malloc(SFPR_LOG_LINE_LEN);
	if(NULL == log->line){
		return -1;
	}
	sfpr_mutex_create(&(log->mutex),0);

	return 0;
}

int sfpr_log_quit(sfpr_log_t **log)
{
	sfpr_log_t *_log = *log;
	if(!log || !_log){
		return -1;
	}
	sfpr_mutex_destroy(&(_log->mutex));
	memset(_log,0,sizeof(sfpr_log_t));
	free(_log);
	*log = NULL;
	return 0;
}

int sfpr_log_set_level(sfpr_log_t* log,char *level)
{
	int i=0;
	if(!log){
		return -1;
	}
	while(sfpr_log_level[i]){
		if(0 == strcmp(sfpr_log_level[i],level)){
			break;
		}
		i++;
	}
	if(sfpr_log_level[i]){
		log->level=i;
		return 0;
	}
	return -1;
}

int sfpr_log_set_flag(sfpr_log_t* log,unsigned long flag)
{
	if(!log){
		return -1;
	}
	log->flag |= flag;
}

int sfpr_log_del_flag(sfpr_log_t* log,unsigned long flag)
{
	if(!log){
		return -1;
	}
	log->flag &= ~flag;
}


static int current_time_get(char *currtime)
{
	sfpr_time_t now;
	sfpr_tm_t tm;
	
	sfpr_time_now(&now);
	sfpr_time_localtime(&now,&tm);

	sprintf(currtime,"%04d%02d%02d%02d%02d%02d%03d",
		tm.year,tm.month,tm.mday,
		tm.hour,tm.min,tm.sec,tm.msec);
	return tm.mday;
}

static int sfpr_log_rm_old_file(sfpr_log_t* log)
{
	
	return 0;
}

static int sfpr_log_update_flag(sfpr_log_t* log,char *info)
{
	char currentTime[32];
	int len;
	sfpr_thread_id threadid=0;
	char *threadname = NULL;

	if(!log){
		return -1;
	}
	current_time_get(currentTime);
	threadid=sfpr_thread_get_id();
	#ifdef VXWORKS
	threadname = taskName(threadid);
	#endif

	if(NULL == threadname){
		fprintf(log->fp,"%s|%x|%s|%s",currentTime,threadid,sfpr_log_level[SFPR_LOG_NOTICE],info);
	}else{
		fprintf(log->fp,"%s|%-9s|%x|%-6s|%s",currentTime,threadname,threadid,sfpr_log_level[SFPR_LOG_NOTICE],info);
	}

	// append "\n" if necessary
	len=strlen(info);
	if( len>0 && info[len-1]!='\n' ) fprintf(log->fp,"\n");
	if( len==0) fprintf(log->fp,"\n");

	fflush(log->fp);
	return 0;
}

static int sfpr_log_update_daily(sfpr_log_t* log)
{
	if(!log){
		return -1;
	}	
	if((log->curr_time == log->last_time) || (0 == (log->flag & SFPR_LOG_DAILY))){
		return 0;
	}
	//日期发生改变.
	log->last_time = log->curr_time;
	sfpr_log_update_flag(log,"The log will update");
	sfpr_log_close(log);
	sfpr_log_rm_old_file(log);
	sfpr_log_open(log);
	return 0;
}

int sfpr_log_open(sfpr_log_t* log)
{
	if(!log){
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_log_init() | error | Invalid param\n");
		#endif
		return -1;
	}
	if(0 == strcmp(log->file,"stdout")){
		log->fp = stdout;
		log->flag &= ~SFPR_LOG_DAILY;
		return 0;
	}
	else if(0 == strcmp(log->file,"stderr")){
		log->fp = stderr;
		log->flag &= ~SFPR_LOG_DAILY;
		return 0;
	}
	log->curr_time = current_time_get(log->curr_date);
	log->curr_date[8]='\0';
	log->last_time = log->curr_time;

	if(log->flag&SFPR_LOG_DAILY){
		sprintf(log->curr_file,"%s%s%s",log->file,log->curr_date,log->suffix);
	} else {
		sprintf(log->curr_file,"%s%s",log->file,log->suffix);
	}
	if(strlen(log->curr_file)<=0) {
		log->fp=NULL;
		memset(log->curr_date,0,sizeof(log->curr_date));
		return -1;
	}

	sfpr_dir_make(log->file);

	if(log->flag&SFPR_LOG_OVERRIDE)
		log->fp=fopen(log->curr_file,"w");
	else
		log->fp=fopen(log->curr_file,"a");

	if(NULL == log->fp) {
		memset(log->curr_date,0,sizeof(log->curr_date));
		return -1;
	}
	return 0;
}

int sfpr_log_close(sfpr_log_t* log)
{
	if(!log){
		#ifdef LOG_ERROR
		fprintf(stderr,"sfpr_log_init() | error | Invalid param\n");
		#endif
		return -1;
	}
	if(!log->fp || log->fp==stdout)
		return -1;
	if(log->fp!=stdout){
		fclose(log->fp);
		fflush(log->fp);
	}
	log->fp=NULL;
	return 0;
}

int sfpr_log(sfpr_log_t* log,int level,char *fmt,...)
{
	char currentTime[32];
	int len;
	sfpr_thread_id threadid=0;
	char *threadname = NULL;
	va_list argp;

	if(!log){
		return -1;
	}

	if((level > log->level) || !fmt || (strlen(fmt)==0))
		return 0;

	if(!log->fp || NULL == log->line){
		return -1;
	}

	sfpr_mutex_lock(&(log->mutex));

	log->curr_time = current_time_get(currentTime);
	sfpr_log_update_daily(log);

	threadid=sfpr_thread_get_id();
	#ifdef VXWORKS
	threadname = taskName(threadid);
	#endif

 	memset(log->line,0,SFPR_LOG_LINE_LEN);
	va_start(argp,fmt);
	vsprintf(log->line,fmt,argp);
	va_end(argp);	

	if(NULL == threadname){
		fprintf(log->fp,"%s|%x|%s|%s",currentTime,threadid,sfpr_log_level[level],log->line);
	}else{
		fprintf(log->fp,"%s|%-9s|%x|%-6s|%s",currentTime,threadname,threadid,sfpr_log_level[level],log->line);
	}

	// append "\n" if necessary
	len=strlen(log->line);
	if( len>0 && log->line[len-1]!='\n' ) fprintf(log->fp,"\n");
	if( len==0) fprintf(log->fp,"\n");
	
	fflush(log->fp);
	sfpr_mutex_unlock(&(log->mutex));
	return 0;
}

int sfpr_log_info(sfpr_log_t* log,int level,char *info)
{
	char currentTime[32];
	int len;
	sfpr_thread_id threadid=0;
	char *threadname = NULL;

	if(!log){
		return -1;
	}

	if(level > log->level)
		return 0;

	if(!log->fp){
		return -1;
		log->fp=stdout;
	}
	
	sfpr_mutex_lock(&(log->mutex));

	log->curr_time = current_time_get(currentTime);
	sfpr_log_update_daily(log);

	threadid=sfpr_thread_get_id();
	#ifdef VXWORKS
	threadname = taskName(threadid);
	#endif

	if(NULL == threadname){
		fprintf(log->fp,"%s|%x|%s|%s",currentTime,threadid,sfpr_log_level[level],info);
	}else{
		fprintf(log->fp,"%s|%-9s|%x|%-6s|%s",currentTime,threadname,threadid,sfpr_log_level[level],info);
	}

	// append "\n" if necessary
	len=strlen(info);
	if( len>0 && info[len-1]!='\n' ) fprintf(log->fp,"\n");
	if( len==0) fprintf(log->fp,"\n");
	
	fflush(log->fp);
	sfpr_mutex_unlock(&(log->mutex));
	return 0;
}

int sfpr_log_error(sfpr_log_t* log,int level,char *info)
{
	if(!log){
		return -1;
	}
	
	return 0;
}

int sfpr_log_hex(sfpr_log_t* log,int level,char *info,const char *buf,int len,int type)
{
	int i,len_info;
	int byteOfGBK;	// specify that the current char is which byte of a gb2312 code
	char bufHex[100],bufChar[100];
	char currentTime[32];
	sfpr_thread_id threadid=0;
	char *threadname = NULL;

	if(!log){
		return -1;
	}
	
	if((level > log->level) || !buf || (len<0))
		return 0;

	if(!log->fp){
		return -1;
		log->fp=stdout;
	}

	if(!info)
		info="";

	sfpr_mutex_lock(&(log->mutex));

	log->curr_time = current_time_get(currentTime);
	sfpr_log_update_daily(log);

	threadid=sfpr_thread_get_id();
	#ifdef VXWORKS
	threadname = taskName(threadid);
	#endif

	if(NULL == threadname){
		fprintf(log->fp,"%s|%x|%s|%s",currentTime,threadid,sfpr_log_level[level],info);
	}else{
		fprintf(log->fp,"%s|%-9s|%x|%-6s|%s",currentTime,threadname,threadid,sfpr_log_level[level],info);
	}
	
	// append "\n" if necessary
	len_info=strlen(info);
	if( len_info>0 && info[len_info-1]!='\n' ) fprintf(log->fp,"\n");
	if( len_info==0) fprintf(log->fp,"\n");

	memset(bufHex,0,sizeof(bufHex));
	memset(bufChar,0,sizeof(bufChar));	
	byteOfGBK=0;

	if(0 == type){
		for(i=0;i<len;i++) {
			unsigned char c=*(buf+i),byte1_gbk;
			if(log->flag & SFPR_LOG_GB2312_SUPPORT) {
				// which part of a gb2312 charactor that the current byte is 
				switch(byteOfGBK) {
				case 0:
				case 2:
					if(c>=0xa1 && c<=0xf7) byteOfGBK=1;
						else byteOfGBK=0;
					byte1_gbk=c;
					break;
				case 1:
					if(c>=0xa1 && c<=0xfe) byteOfGBK=2;
						else byteOfGBK=0;
					break;
				}
			}
			if((i%16==0) && i>0) {
				// print current line and prepair the next one
				fprintf(log->fp,"%s\t%s  \n",bufHex,bufChar);
				memset(bufHex,0,sizeof(bufHex));
				memset(bufChar,0,sizeof(bufChar));	
				if(log->flag & SFPR_LOG_GB2312_SUPPORT) {
					// copy the tail char of last line to the head of next line
					if(byteOfGBK==2) sprintf(bufChar,"%c",byte1_gbk);
				}
			}
			sprintf(bufHex,"%s%02X ",bufHex,c);
			sprintf(bufChar,"%s%c",bufChar,(iscntrl(c) ? '.':c));
		}
		fprintf(log->fp,"%-48s\t%s  \n",bufHex,bufChar);
	}else{
		for(i=0;i<len;i++) {
			unsigned char c=*(buf+i);
			if((i%16==0) && i>0) {
				fprintf(log->fp,"%s",bufHex);
				memset(bufHex,0,sizeof(bufHex));
			}
			memset(bufChar,0,sizeof(bufChar));			
			sprintf(bufChar,"%02X ",c);
			strcat(bufHex,bufChar);
		}
		fprintf(log->fp,"%s\n",bufHex);
	}
	fflush(log->fp);
	sfpr_mutex_unlock(&(log->mutex));
	return 0;
}

#ifdef __cplusplus
}
#endif

