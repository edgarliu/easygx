
#include "sfpr_time.h"
#include "sfpr_timecal.h"

static int months[] = {0,31,29,31,30,31,30,31,31,30,31,30,31};	

int sfpr_time_get_days_of_year(int year)
{
	int not_run_nian = 1;
	not_run_nian = (year%4||!(year%100))&&year%400;
	if(!not_run_nian)
		return 366;
	else
		return 365;
}

int sfpr_time_get_days_of_february(int year)
{
	int not_run_nian = 1;
	not_run_nian = (year%4||!(year%100))&&year%400;
	if(!not_run_nian)
		return 29;
	else
		return 28;
}


int sfpr_time_get_elapse_seconds_in_day(int hour,int minite,int second)
{
	int seconds = 0,count = 0;

	while(count < hour){
		seconds += 3600;
		count++;
	}
	count = 0;
	while(count < minite){
		seconds += 60;
		count++;
	}
	seconds += second;
	return seconds;
}

int sfpr_time_get_elapse_days_in_year(int year,int month,int day)
{
	int nDays = 0,nCount = 1;
	while(nCount < month){
		if(nCount == 2){
			nDays += sfpr_time_get_days_of_february(year);
		}else{
			nDays += months[nCount];
		}
		nCount++;
	}
	nDays += day;
	return nDays;	
}


int sfpr_time_days_calculate(int year1,int month1,int day1,
								  int year2,int month2,int day2)
{
	int days1 = 0 , days2 = 0 , count;
	
	if(year1 <= year2){
		count = year1;
		while(count < year2){
			days2 += sfpr_time_get_days_of_year(count);
			count++;
		}
		days2 += sfpr_time_get_elapse_days_in_year(year2,month2,day2);
		days1 = sfpr_time_get_elapse_days_in_year(year1,month1,day1);
	}else{
		count = year2;
		while(count < year1){
			days1 += sfpr_time_get_days_of_year(count);
			count++;
		}
		days1 += sfpr_time_get_elapse_days_in_year(year1,month1,day1); 
		days2 = sfpr_time_get_elapse_days_in_year(year2,month2,day2);
	}
	return days2 - days1;
}

long int sfpr_time_time2second(int year,int month,int day,int hour,int minite,int second,int start_year)
{
	unsigned int days = 0,seconds = 0;
	int nCount = start_year;
	
	while(nCount < year){
		days += sfpr_time_get_days_of_year(nCount);
		nCount++;
	}
	
	days += sfpr_time_get_elapse_days_in_year(year,month,day);
	seconds = sfpr_time_get_elapse_seconds_in_day(hour,minite,second);
	
	seconds += days*24*60*60;

	return seconds;	

}

int sfpr_time_second2time(int seconds,int* year,int* month,
		int* day,int* hour,int* minite,int* second,int start_year)
{
	int ys[] = {366*24*60*60,365*24*60*60};
	int count = start_year;
	int not_run_nian = 1,days;;

	while(1){
		not_run_nian = (count%4||!(count%100))&&count%400;
		if(not_run_nian)
			not_run_nian = 1;
		if(seconds > ys[not_run_nian])
		{
			seconds -= ys[not_run_nian];
			count++;
		}
		else
			break;
	}
	*year = count+ start_year;

	days = seconds/(24*60*60);
	count = 1;
	not_run_nian = (*year%4||!(*year%100))&&*year%400;
	
	while(1){
		days -= months[count];
		count++;
		if(count == 2 && !not_run_nian && days < 29)
			break;
		else if(count == 2 && not_run_nian && days < 28)
			break;
		else if(days < months[count])
			break;	
	}
	*month = count;
	*day = days;

	days = seconds%(24*60*60);
	*hour = days/3600;
	days = days%3600;
	*minite = days/60;
	*second = days%60;
	return 0;
}

int sfpr_time_get_sum(sfpr_tm_t *time1,sfpr_tm_t* time2)
{
	int seconds1,seconds2;
	int year;	

	if(time1->year > time2->year)
	{
		seconds1 = sfpr_time_time2second(time1->year,time1->month,time1->mday,
			time1->hour,time1->min,time1->sec,time2->year);			
		seconds2 = sfpr_time_time2second(time2->year,time2->month,time2->mday,
			time2->hour,time2->min,time2->sec,time2->year);
		seconds1 += seconds2;
		
		sfpr_time_second2time(seconds1,&time1->year,&time1->month,&time1->mday,
			&time1->hour,&time1->min,&time1->sec,time2->year);
	}
	else
	{
		seconds1 = sfpr_time_time2second(time1->year,time1->month,time1->mday,
			time1->hour,time1->min,time1->sec,time1->year);			
		seconds2 = sfpr_time_time2second(time2->year,time2->month,time2->mday,
			time2->hour,time2->min,time2->sec,time1->year);
		seconds1 += seconds2;

		sfpr_time_second2time(seconds1,&time1->year,&time1->month,&time1->mday,
			&time1->hour,&time1->min,&time1->sec,time1->year);			
	}
	return 0;
}

int sfpr_time_count_time(sfpr_tm_t *time1,int offset,int mode)
{
	int seconds;
	seconds = sfpr_time_time2second(time1->year,time1->month,time1->mday,
			time1->hour,time1->min,time1->sec,time1->year);	
	if(mode == 1)
	{
		seconds += offset;
	}
	else if(mode == 2)
	{
		seconds -= offset;
	}
	sfpr_time_second2time(seconds,&time1->year,&time1->month,&time1->mday,
		&time1->hour,&time1->min,&time1->sec,time1->year);			
	
	return 0;
}

