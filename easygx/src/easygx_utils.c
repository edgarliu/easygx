
#include <stdlib.h>
#include <string.h>

#include "easygx.h"

#ifdef __cplusplus
extern "C" {
#endif

char *egx_string_malloc(char *data)
{
	int len = 0;
	char *object = NULL;
	if(!data)
		return NULL;
	len = strlen(data) + 1;
	if(len){
		object = malloc(len);
		if(!object)
			return NULL;
	}
	if(object){
		memset(object, 0x0, len);
		strcpy(object, data);
	}
	return object;
}

#ifdef __cplusplus
}
#endif
