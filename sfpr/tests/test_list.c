#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sfpr_list.h"


void list_enum_cb(void *data,void *param)
{
	printf("list data:%s\n",data);
}


int cmp_cb(void  *src, void *param)
{
	int res =0;

	if(strcmp(src , param) == 0)
		return res;
	return -1;
	
}


int main(void)
{
	int i,count = 0;
	char buffer[256];
	char *data =NULL;
	
	sfpr_list_t *list = NULL;
	list = sfpr_list_new(0,NULL);
	if(!list)
	{
		fprintf(stderr,"sfpr_lis_new() failed\n");
		return 1;
	}
	else 
	{
		printf("sfpr_lis_new() success\n");
	}

	while(1){
		printf("Please input data:");
		fgets(buffer,sizeof(buffer),stdin);
		if(strncmp(buffer,"quit",4)==0){
			break;
		}
		buffer[strlen(buffer)-1] = 0;
		data = (char*)malloc(strlen(buffer)+1);
		memcpy(data,buffer,strlen(buffer));
		
		sfpr_list_append(list,data);
		
		sfpr_list_all(list,list_enum_cb,NULL);
		memset(buffer,0,sizeof(buffer));		
	}	

	printf("list...\n");
	sfpr_list_all(list,list_enum_cb,NULL);
	count = sfpr_list_count(list);
	printf("node...%d\n",count);

	printf("delete...\n");
	sfpr_list_delete(list,cmp_cb,"asdf",NULL,NULL);

	printf("list...\n");
	sfpr_list_all(list,NULL,NULL);
	count = sfpr_list_count(list);
	printf("node...%d\n",count);
	
	printf("clear...\n");
	sfpr_list_clear(list,NULL,NULL);
	
	printf("destory...\n");
	sfpr_list_destroy(list);
	

	return 0;
	
}