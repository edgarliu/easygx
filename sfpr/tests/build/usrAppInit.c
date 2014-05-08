/* usrAppInit.c - stub application initialization routine */

/* Copyright 1984-1998 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,02jun98,ms   written
*/

/*
DESCRIPTION
Initialize user application code.
*/ 

/******************************************************************************
*
* usrAppInit - initialize the users application
*/ 
#include "i2c.c"
#include "sfpr_global.h"


//#define ClientPriority 20
//#define ServerPriority 20
extern int server_main(int argc);
extern int client_main(char* strhost, int nport);
extern int timer_main(int argc,char *argv[]);
extern int crypt_main(int argc,char *argv[]);
void usrAppInit (void)
    {
#ifdef	USER_APPL_INIT
	USER_APPL_INIT;		/* for backwards compatibility */
#endif

//	hash_main();
//	memory_main();
//	queue_main();
//	server_main(80);
/*	int taskID = taskSpawn("servertask", ServerPriority, 0, 100000, (FUNCPTR)server_main, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if(taskID == ERROR)
	{
	        printf("Create Server Task error\n");
	}
	taskID = taskSpawn("clienttask", ClientPriority, 0, 100000, (FUNCPTR)client_main, "127.0.0.1", 80, 0, 0, 0, 0, 0, 0, 0, 0);
	if(taskID == ERROR)
	{
	        printf("Create Client Task error\n");
	}
*/	
    /* add application specific code here */
	//	timer_main();
        crypt_main();
    }


