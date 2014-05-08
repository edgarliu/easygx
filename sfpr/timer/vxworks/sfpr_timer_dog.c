#include"sfpr_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

sfpr_timer_t  sfpr_timer_create(void (*function)(unsigned long),sfpr_timer_para_t *para,sfpr_timer_info_t *timer_info)
{
	sfpr_timer_t Timer;
	
	Timer = wdCreate();
	
	timer_info->function = function;
	timer_info->para  = para;
	timer_info->timerid = Timer;
		
	return Timer;
}

int  sfpr_timer_destory(sfpr_timer_t Timerid )
{
	int ret   ;

	ret = wdDelete (Timerid);
	return ret;
}

int  sfpr_timer_start(sfpr_timer_t Timerid ,int sec,sfpr_timer_info_t *timer_info)
{
	int ret   ;

	ret = wdStart (Timerid, sysClkRateGet()*sec, timer_info->function,timer_info->para->para_value.para_int);
	return ret;
}

int  sfpr_timer_cancel(sfpr_timer_t Timerid )
{
	int ret   ;

	ret = wdCancel (Timerid);
	return ret;
}



#ifdef __cplusplus
}
#endif

