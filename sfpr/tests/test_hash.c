
#include <stdio.h>

#include "sfpr_hash.h"

void hash_enum_cb(sfpr_hash_node_t *node,void *param)
{
	printf("hash_enum_cb(): slot:%-4d hash:%-13d key:%s\n",
				sfpr_hash_node_get_slot(node),
				sfpr_hash_node_get_hashvalue(node),
				sfpr_hash_node_get_key(node));
}
#if defined VXWORKS
int hash_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	char buffer[1024],*key=NULL;
	
	sfpr_hash_t *hash = NULL;
	hash = sfpr_hash_new();
	if(!hash){
		fprintf(stderr,"sfpr_hash_new() failed\n");
		return 1;
	}
	printf("sfpr_hash_new() success\n");

	if(sfpr_hash_init(hash,"testHash",10,NULL,NULL,NULL)<0)
	{
		fprintf(stderr,"sfpr_hash_new() failed\n");
		return 1;		
	}
	printf("sfpr_hash_init() success\n");
	
	while(1){
		printf("Please input key:");
		fgets(buffer,sizeof(buffer),stdin);
		if(!strncmp(buffer,"quit",4)){
			break;
		}
		buffer[strlen(buffer)-1] = 0;
		key = (char*)malloc(strlen(buffer)+8);
		memcpy(key,buffer,strlen(buffer));
		
		sfpr_hash_insert(hash,key,strlen(key),key,0,0);

		printf("hash count: %d\n",sfpr_hash_count(hash));

		sfpr_hash_enum(hash,hash_enum_cb,NULL);

		memset(buffer,0,sizeof(buffer));		
	}	
	
	sfpr_hash_destroy(&hash);
	printf("sfpr_hash_destroy() success\n");	
	return 0;
}
