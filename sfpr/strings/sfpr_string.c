
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>

static int sfpr_isblank(int ch)
{
	if(ch == ' ' || ch == '\t')
		return 1;
	return 0;
}


//分割字符串，分隔符sep的每一个字符都是一个分隔符，例如" \r"
int sfpr_str_split(char *pdata,char *array[],int num,char *sep)
{
	char sepv[256];
	unsigned char c,*poffset = (unsigned char *)sep;
	int count = 0,sep_count = 0;
	
	if(!pdata || !array || num <1){
		return 0;
	}
	memset(sepv,0,sizeof(sepv));
	
	if(!poffset){
		poffset = (unsigned char *)" \t";//默认分隔符
	}
	while(*poffset){
		sepv[*poffset++] = 1;//分隔符标识
	}
	sepv[0] = 2;	//结束标志
	sepv['\n'] = 2;	//结束标志
	
	poffset = (unsigned char *)pdata;
	while((c = sepv[*poffset]) != 2){
		if(c != 1)
			break;
		poffset++;
	}
	array[count++] = poffset;

	while((c = sepv[*poffset]) != 2){
		if(c == 1){
			*poffset++ = 0;
			while((c = sepv[*poffset]) != 2){
				if(c != 1)
					break;
				poffset++;
			}
			if(c != 2)
				array[count++] = poffset;
			if(count == num){
				break;
			}
		}
		poffset++;
	}
	return count;
}


char *sfpr_str_ltrim(char *pdata)
{
    int l=0,p=0,k=0;
    l = strlen(pdata);
    if (l == 0) 
		return pdata;

    while (sfpr_isblank(pdata[p])) 
    {
		p++;
    }
    if(p == 0) 
		return pdata;
	
    while (pdata[k] != '\0') 
		pdata[k++] = pdata[p++];
    return pdata;
}

char *sfpr_str_rtrim(char *pdata)
{
    int l=0,p=0;
    l = strlen(pdata);
    if ( l == 0 ) 
		return;
    p = l -1;
    while (sfpr_isblank(pdata[p])) 
	{
        pdata[p--] = '\0';
        if( p < 0 ) 
			break;
    }
    return pdata;
}

char *sfpr_str_trim(char *pdata)
{
    int i=0,j=0;
    while (pdata[i] != '\0') 
	{
        if (!sfpr_isblank(pdata[i]))
        {
	        pdata[j++]= pdata[i];
        }
		i++;
    }
	pdata[j] = '\0';
	return pdata;
}

int sfpr_str_replace(char* src,char* str_src, char* str_des)
{
    char buff[4096];
    char buff2[4096];
    char *ptr=NULL;
    int i = 0;
    
    if (src != NULL)
	{
        strcpy(buff2, src);
    }
	else
	{
        return -1;
    }
    memset(buff, 0x00, sizeof(buff));
    while ((ptr = strstr( buff2, str_src)) !=0)
	{
        if(ptr-buff2 != 0) memcpy(&buff[i], buff2, ptr - buff2);
        memcpy(&buff[i + ptr - buff2], str_des, strlen(str_des));
        i += ptr - buff2 + strlen(str_des);
        strcpy(buff2, ptr + strlen(str_src));
    }
    strcat(buff,buff2);
    strcpy(src,buff);
    return 0;
}

int sfpr_str_isdigits(char* src, int len)
{
	int i=0;
	char tmp;
	for(i=0; i< len; i++)
	{
		tmp = *(src+i);
		if(!isdigit(tmp))
			return -1;
	}
    return 0;
}


#define GB2312_SUPPORT 0x02
void sfpr_str_hex_print(const char * info,const char *buf,int len)
{
	int i,len_info;
	char bufHex[100],bufChar[100];
	int byteOfGBK;	// specify that the current char is which byte of a gb2312 code
	
	printf(info);
	// append "\n" if necessary
	len_info=strlen(info);
	if( len_info>0 && info[len_info-1]!='\n' ) printf("\n");
	if( len_info==0) printf("\n");
	// print the hex 
	strcpy(bufHex,"");
	strcpy(bufChar,"");
	byteOfGBK=0;
	for(i=0;i<len;i++) {
		unsigned char c=*(buf+i),byte1_gbk;
		if(GB2312_SUPPORT) {
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
			printf("%s\t%s  \n",bufHex,bufChar);
			strcpy(bufHex,"");
			strcpy(bufChar,"");
			if(GB2312_SUPPORT){
				// copy the tail char of last line to the head of next line
				if(byteOfGBK==2) sprintf(bufChar,"%c",byte1_gbk);
			}
		}
		sprintf(bufHex,"%s%02X ",bufHex,c);
		sprintf(bufChar,"%s%c",bufChar,(iscntrl(c) ? '.':c));
	}
	printf("%-48s\t%s\n",bufHex,bufChar);
}

char *sfpr_str_hextobin(unsigned long digi,char *format,char *output,int size)
{
    char buffer[100];
    int mod = 0,count = 0;

    char *current = buffer + sizeof(buffer)-1;

    memset(buffer,0,sizeof(buffer));

    while(current >= buffer && digi != 0)
    {
        mod = digi%2;
        digi /= 2;
        if(count && !(count%4)){
                *(--current) = ' ';
        }
        count++;
        *(--current) = mod + '0';
    }
    while(count%atoi(format))
    {
        if(count && !(count%4)){
                *(--current) = ' ';
        }
        *(--current) = '0';
        count++;
    }
    count = buffer + sizeof(buffer)-1 - current;
    memset(output,0,size);
    if(count >= size){
            return NULL;
    }
    memcpy(output,current,count);
    return output;
}

unsigned long sfpr_str_bintohex(char *binstr)
{
	unsigned long digi = 0;
	char *curr = binstr;

	while(*curr != '\0'){
		if(*curr == ' '){
			curr++;
			continue;
		}
		if(*curr != '0' && *curr != '1'){
			break;
		}
		digi *= 2;
		digi += (*curr - '0');
		curr++;
	}
	
	return digi;
}


int sfpr_str_filter(char *data,char c1,char c2)
{
	char *pos = NULL;
	if(!data){
		return -1;
	}
	pos = data;
	while(*pos != '\0'){
		if(*pos == c1){
			*pos = c2;
		}
		pos++;
	}
	return 0;
}

char* sfpr_str_reverse(char* s)  
{
	/* h指向s的头部 */  
	char* h = s;      
	char* t = s;  
	char ch;  
  
	/* t指向s的尾部 */  
	while(*t++){};  
	t--;    /* 与t++抵消 */  
	t--;    /* 回跳过结束符'\0' */  
  
	/* 当h和t未重合时，交换它们所指向的字符 */  
	while(h < t)  
	{  
		ch = *h;  
		*h++ = *t;    /* h向尾部移动 */  
		*t-- = ch;    /* t向头部移动 */  
	}  
	return(s);  
}  
    
unsigned long int sfpr_number_pow(unsigned char a1,unsigned char a2)
{
	unsigned char i;
	unsigned long int value=1;
	for(i=0;i<a2;i++)
		value*=a1;
	return value;
}

#ifdef WIN32
char *strtok_r(char *s, const char *delim, char **save_ptr) {  
    char *token = NULL;  
    if (s == NULL) s = *save_ptr;  
    if (s == NULL)   
        return NULL;  
    /* Scan leading delimiters.  */  
    s += strspn(s, delim);  
    if (*s == '\0')   
        return NULL;  
  
    /* Find the end of the token.  */  
    token = s;  
    s = strpbrk(token, delim);  
    if (s == NULL)  {
        /* This token finishes the string.  */  
        *save_ptr = NULL;//strchr(token, '/0');  
    }else {  
        /* Terminate the token and make *SAVE_PTR point past it.  */  
        *s = '\0';  
        *save_ptr = s + 1;  
    }  
  
    return token;  
} 

/*
 __strtok_r_1c (char *__s, char __sep, char **__nextp)
{
  char *__result;
  if (__s == NULL)
    __s = *__nextp;
  while (*__s == __sep)
    ++__s;
  __result = NULL;
  if (*__s != '\0')
    {
      __result = __s++;
      while (*__s != '\0')
	if (*__s++ == __sep)
	  {
	    __s[-1] = '\0';
	    break;
	  }
      *__nextp = __s;
    }
  return __result;
}
*/
#endif

#ifdef __cplusplus
}
#endif
