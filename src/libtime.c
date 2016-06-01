#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libtime.h"
#include "liberrmsg.h"

int rtime(Time *t)
{
	sscanf(t->KDATE,"%d/%d/%d", &(t->year), &(t->mon), &(t->day));
	sscanf(t->KTIME,"%d:%d:%f", &(t->hour), &(t->min), &(t->f_s));
	t->sec  = (int) ( floorf(t->f_s) );
	t->msec = (int) ((t->f_s - t->sec) * 1000.0f);
	jd(t);
	uni(t);
	return 0;
}
int rtimej(Time *t)
{
	sscanf(t->KJD  ,"%d/%d"   , &(t->year), &(t->jday));
	sscanf(t->KTIME,"%d:%d:%f", &(t->hour), &(t->min), &(t->f_s));
	t->sec  = floorf(t->f_s);
	t->msec = (int) ((t->f_s - t->sec) * 1000.0f);
	ijd(t);
	uni(t);
	return 0;
}
int wtime(Time *t)
{
	sprintf(t->KDATE,"%04d/%02d/%02d"  ,t->year, t->mon,t->day);
	sprintf(t->KJD,  "%04d/%03d"       ,t->year, t->jday);
	//sprintf(t->KTIME,"%02d:%02d:%05.3f",t->h, t->min, t->s);
	sprintf(t->KTIME,"%02d:%02d:%03d.%03d",t->hour, t->min, t->sec, t->msec);
	return 0;
}

static long long int months[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},
                          {31,29,31,30,31,30,31,31,30,31,30,31}};
static long long int pre[2][13]= {{0,31,59,90,120,151,181,212,243,273,304,334,365},
                        {0,31,60,91,121,152,182,213,244,274,305,335,366}};

int shft(Time *t,float dfsec)
{
	t->f_s   += dfsec;
	uni(t);
	return 0;
}
int  jd(Time *t)
{
	//if(ISLEAP(t->year))
	//	t->jday = pre[1][t->mon -1] + t->day;
	//else
	//	t->jday = pre[0][t->mon -1] + t->day;

	//Revised by wangsheng 2015/09/01
	t->jday = pre[ISLEAP(t->year)][t->mon -1] + t->day;
	return 0;
}
int ijd(Time *t)
{
	int i, year_type;
	//if(ISLEAP(t->year))
	//	year_type = 1;
	//else
	//	year_type = 0;

	//Revised by wangsheng 2015/09/01
	year_type = ISLEAP(t->year);
	for(i = 0; i < 12; ++i)
	{
		if( (t->jday >  pre[year_type][i]) &&
		    (t->jday <= pre[year_type][i+1]) )
		{
			t->mon = i + 1;
			t->day = t->jday - pre[year_type][i];
		}
	}
	return 0;
}
int uni(Time *t)
{
	int  i;
	if(t->f_s >= 0.0)
	{
		t->min += ((int) (floorf(t->f_s))) / 60;
		t->f_s  = t->f_s - 60.0f * (((int) (floorf(t->f_s))) / 60);
		t->sec  = floorf(t->f_s);
		t->msec = (int) ((t->f_s - t->sec) * 1000.0f);
	}
	else
	{
		t->min += ( ((int) (floorf(t->f_s))) / 60  - 1 );
		t->f_s  = t->f_s - 60.0f * (((int) (floorf(t->f_s))) / 60 - 1 );
		t->sec  = floorf(t->f_s);
		t->msec = (int) ((t->f_s - t->sec) * 1000.0f);
	}

	if(t->min >= 0)
	{
		t->hour+= t->min / 60;
		t->min  = t->min - 60 * (t->min / 60);
	}
	else
	{
		t->hour+= (t->min / 60 - 1);
		t->min  = t->min - 60 * (t->min / 60 - 1);
	}

	if(t->hour >= 0)
	{
		t->jday   += t->hour / 24;
		t->hour    = t->hour - 24 * (t->hour / 24);
	}
	else
	{
		t->jday   += t->hour / 24 - 1;
		t->hour    = t->hour - 24 * (t->hour / 24 - 1);
	}
	//Add and revised by wangsheng 2015/09/01
	//ijd(t);
	if(t->jday > pre[ISLEAP(t->year)][12])
	{
		for( i = t->year; t->jday > pre[ISLEAP(i)][12]; ++i)
		{
			t->jday -= pre[ISLEAP(i)][12];
			t->year += 1;
		}
	}
	else if(t->jday <= 0)
	{
		for( i = t->year; t->jday <= 0; ++i)
		{
			t->jday += pre[ISLEAP(i-1)][12];
			t->year -= 1;
		}
	}
	ijd(t);
	//End of Add and revision 2015/09/01
	return 0;
}
float dtsameyear(Time *t1, Time *t2, long long int *dSecs, int *dMsec ) //t1 - t2
{
	int djd  = t1->jday - t2->jday;
	int dh   = t1->hour - t2->hour;
	int dmin = t1->min  - t2->min;
	float ds = t1->f_s  - t2->f_s;
	int dsecd = (int) (floorf(ds)) ;
	float dt;

	//Calculate seconds count"dSecs" and msecond counts"dMsec"
	*dSecs = 0;
	*dSecs += djd * 86400;
	*dSecs += dh  * 3600;
	*dSecs += dmin * 60;

	dt = *dSecs + ds;

	*dSecs += dsecd;
	*dMsec = (int) rint((ds - dsecd) * 1000);

	//printf("%lld %f %f\n", *dSecs, ds, (ds - dsecd)*1000 );
	//dt += dmin * 60.0f;
	//dt += dh   * 3600.0f;
	//dt += djd  * 86400.0f;

	//if(t->year%4 != 0 || (t->year%100 == 0 && t->year%400 != 0))
	//	dt +=  dyear * 365.0 * 86400;
	//else
	//	dt +=  dyear * 366.0 * 86400;
	return dt;
}

float dt(Time *t1, Time *t2, long long int *dSecs, int *dMsec ) //t1 - t2
{
	float ds = 0.0f;
	int i;
	float pres1, pres2;
	Time t1_0, t2_0;
	long long int dSecs_1, dSecs_2;
	int dMsec_1, dMsec_2;
	if(t1->year == t2->year)
		return dtsameyear(t1, t2, dSecs, dMsec);
	t1_0.year = t1->year;  t2_0.year = t2->year;
	t1_0.mon  = 1;         t2_0.mon  = 1;
	t1_0.day  = 1;         t2_0.day  = 1;
	t1_0.jday = 1;         t2_0.jday = 1;
	t1_0.hour = 0;         t2_0.hour = 0;
	t1_0.min  = 0;         t2_0.min  = 0;
	t1_0.sec  = 0;         t2_0.sec  = 0;
	t1_0.msec = 0;         t2_0.msec = 0;
	t1_0.f_s  = 0.0;       t2_0.f_s  = 0.0;

	pres1 = dtsameyear(t1, &t1_0, &dSecs_1, &dMsec_1);
	pres2 = dtsameyear(t2, &t2_0, &dSecs_2, &dMsec_2);
	if(t1->year > t2->year)
	{
		*dSecs = pre[ ISLEAP(t2->year) ][12] * 86400    - dSecs_2 + dSecs_1;
		ds     = pre[ ISLEAP(t2->year) ][12] * 86400.0f - pres2   + pres1  ;
		for(i = t2->year + 1; i < t1->year; ++i)
		{
			ds     += pre[ISLEAP(i)][12] * 86400.0f;
			*dSecs += pre[ISLEAP(i)][12] * 86400   ;
		}

		// dMsec must between 0~1000
		*dMsec = dMsec_1 - dMsec_2;
		if(*dMsec < 0)
		{
			*dSecs -= 1;
			*dMsec += 1000;
		}
	}
	else
	{
		*dSecs = pre[ ISLEAP(t1->year) ][12] * 86400    - dSecs_1 + dSecs_2;
		ds     = pre[ ISLEAP(t1->year)][12] * 86400.0f  - pres1   + pres2;
		for(i = t1->year + 1; i < t2->year; ++i)
		{
			ds     += pre[ISLEAP(i)][12] * 86400.0f;
			*dSecs += pre[ISLEAP(i)][12] * 86400   ;
		}
		ds = -ds;

		// dMsec must between 0~1000
		*dMsec = dMsec_1 - dMsec_2;
		if(*dMsec < 0)
		{
			*dSecs -= 1;
			*dMsec += 1000;
		}
		*dSecs = -(*dSecs);
		*dMsec = -(*dMsec);
	}
	return ds;
}
int judge(Time *t)
{
	int year_type;
	//if(ISLEAP(t->year))
	//	year_type = 1;
	//else
	//	year_type = 0;
	year_type = ISLEAP(t->year);
	//Revised by wangsheng 2015/09/01
	if( t->mon < 1  || t->mon > 12 ||
            t->day < 1  || t->day > months[year_type][t->mon - 1] ||
            t->jday < 1 || t->jday > pre[year_type][12]
		)
	{
		perrmsg( "Err: Input time is not valid\n   ", ERR_BAD_ARGS);
		return FALSE;
	}
	return TRUE;
}