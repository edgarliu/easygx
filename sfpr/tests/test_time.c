#include <stdio.h>
#include "sfpr_time.h"


#ifdef __cplusplus
extern "C" {
#endif

#if defined VXWORKS
int time_main(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	sfpr_time_t timenow;
	sfpr_tm_t mytm;
	sfpr_bench_elapse_t bench;

	sfpr_time_now(&timenow);
	printf("Now Time: sec=%d, usec=%d\n", timenow.sec, timenow.usec);

	sfpr_time_localtime(&timenow,&mytm);
	printf("Now Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n",
				mytm.year,mytm.month,
				mytm.mday,mytm.hour,
				mytm.min,mytm.sec,
				mytm.msec);

	mytm.year = 1970;
	sfpr_time_mktime(&mytm,&timenow);
	printf("The Time: sec=%d, usec=%d\n", timenow.sec, timenow.usec);

	sfpr_time_localtime(&timenow,&mytm);
	printf("The Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n",
				mytm.year,mytm.month,
				mytm.mday,mytm.hour,
				mytm.min,mytm.sec,
				mytm.msec);

	sfpr_bench_start(&bench);
	sfpr_time_msleep(1000);
	sfpr_bench_stop(&bench,3475);
	sfpr_bench_print(&bench,"TestBench");

  return 0;
}

#ifdef __cplusplus
}
#endif
