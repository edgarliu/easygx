
#include <stdio.h>
#include <string.h>

#include "sfel_dsfel.h"


int main(int argc,char *argv[])
{
	char *string  = malloc(SFEL_DSFEL_MAX_RETURN_SIZE);
	memset(string, 0x0, SFEL_DSFEL_MAX_RETURN_SIZE);
	
	/*${sendmessage(getelementbyid(hello))}*/
	printf("begin dsfel_handle\n");
	//dsfel_compile(NULL, "${getelementbyid(hello)}", string,SFEL_DSFEL_MAX_RETURN_SIZE - 1);
	
	//dsfel_compile(NULL,"${sendmessage(127.0.0.1, hello)}", string);
	dsfel_compile(NULL,"${sendmessage(${getelementbyid('hello')})}", string,SFEL_DSFEL_MAX_RETURN_SIZE-1);
	//dsfel_compile(NULL,"${ sendmessage ( 127.0.0.1, 8080, name=id id=p ) . name }", string,SFEL_DSFEL_MAX_RETURN_SIZE-1);
	//dsfel_compile(NULL, "${sendmessage(127.0.0.1, ${getelementbyid(hello)}, 8080)}", string,SFEL_DSFEL_MAX_RETURN_SIZE-1);
	//dsfel_compile(NULL, "${sendmessage(127.0.0.1, ${getelementbyid(id)},${getelementbyid(hello)})}", string,SFEL_DSFEL_MAX_RETURN_SIZE-1);
	//dsfel_compile(NULL, "${sendmessage(127.0.0.1, ${sendmessage(192.168.1.1, ${getelementbyid(id)}, 6868)},8080)}",string,SFEL_DSFEL_MAX_RETURN_SIZE-1);
	printf("dsfel_handle result:%s\n", string);
	free(string);
	return 0;	
}
