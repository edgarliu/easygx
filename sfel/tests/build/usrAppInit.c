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

#include "i2c.c"
/******************************************************************************
*
* usrAppInit - initialize the users application
*/ 
extern int inst_main(int argc, char *argv[]);
void usrAppInit (void)
    {
#ifdef	USER_APPL_INIT
	USER_APPL_INIT;		/* for backwards compatibility */
#endif

	//test main

    /* add application specific code here */
//	printf("this is sfel test.\n");
		inst_main();
    }


