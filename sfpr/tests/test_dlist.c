#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sfpr_list.h"

//#define DLIST_FORWARD
void dlist_enum_cb(void *data,void *param)
{
	printf("dlist data:%s\n",data);
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
	
	sfpr_list_t *dlist = NULL;
	dlist = sfpr_list_new(1,NULL);
	if(!dlist)
	{
		fprintf(stderr,"sfpr_dlis_new() failed\n");
		return 1;
	}
	else 
	{
		printf("sfpr_dlis_new() success\n");
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
#ifdef DLIST_FORWARD
		sfpr_list_append(dlist,data);
		sfpr_list_forward_all(dlist,dlist_enum_cb,NULL);
#else
		sfpr_list_append(dlist,data);
		sfpr_list_reverse_all(dlist,dlist_enum_cb,NULL);
#endif
		memset(buffer,0,sizeof(buffer));		
	}	

	printf("dlist...\n");
#ifdef DLIST_FORWARD
	sfpr_list_all(dlist,dlist_enum_cb,NULL);
#else
    sfpr_list_reverse_all(dlist,dlist_enum_cb,NULL);
#endif
	count = sfpr_list_count(dlist);
	printf("node...%d\n",count);

	printf("delete...\n");
	sfpr_list_delete(dlist,cmp_cb,"asdf",NULL,NULL);

	printf("dlist...\n");
#ifdef DLIST_FORWARD
	sfpr_list_all(dlist,NULL,NULL);
#else
    sfpr_list_reverse_all(dlist,NULL,NULL);
#endif	
	count = sfpr_list_count(dlist);
	printf("node...%d\n",count);
	
	printf("clear...\n");
	sfpr_list_clear(dlist,NULL,NULL);
	
	printf("destory...\n");
	sfpr_list_destroy(dlist);
	return 0;
}

