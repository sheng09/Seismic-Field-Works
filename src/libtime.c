#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "libtime.h"
int rtime(Time *t)
{
	sscanf(t->KDATE,"%d-%d-%d", &(t->y), &(t->mon), &(t->d));
	sscanf(t->KTIME,"%d:%d:%f", &(t->h), &(t->min), &(t->s));
	jd(t);
	uni(t);
	return 0;
}
int rtimej(Time *t)
{
	sscanf(t->KJD  ,"%d-%d"   , &(t->y), &(t->jd));
	sscanf(t->KTIME,"%d:%d:%f", &(t->h), &(t->min), &(t->s));
	ijd(t);
	uni(t);
	return 0;
}
int wtime(Time *t)
{
	sprintf(t->KDATE,"%04d-%02d-%02d" ,t->y,t->mon,t->d);
	sprintf(t->KJD,"%04d-%03d"        ,t->y,t->jd);
	sprintf(t->KTIME,"%02d:%02d:%05.2f",t->h,t->min,t->s);
	return 0;
}

static int months[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},
                         {31,29,31,30,31,30,31,31,30,31,30,31}};
int shft(Time *t,float sec)
{
	t->s   += sec;
	/*
	int i = 1;
	if(t->y%4 != 0 || (t->y%100 == 0 && t->y%400 != 0))
		i = 0;
	t->h += x;

	// 0<= Hour <= 23
	if(t->h < 0)
	{
		t->h += 24;
		t->jd -= 1;
		if(t->d == 1)
		{
			t->d = months[i][t->mon-2];
			t->mon -= 1;
		}
		else
			t->d -= 1;
	}
	else if(t->h >= 24)
	{
		t->h -= 24;
		t->jd += 1;
		if(t->d == months[i][t->mon-1])
		{
			t->d = 1;
			t->mon += 1;
		}
		else
			t->d += 1;
	}

	// 1<= Month <= 12
	if(t->mon >12)
	{
		--t->mon;
		++t->y;
	}
	else if(t-> mon < 1)
	{
		t->mon = 12;
		--t->y;
	}*/
	return 0;
}
int  jd(Time *t)
{
	int jd = 0;
	int i;
	if(t->y%4 != 0 || (t->y%100 == 0 && t->y%400 != 0))
		for(i = 0; i < t->mon-1; ++i)
			jd += months[0][i];
	else
		for(i = 0; i < t->mon-1; ++i)
			jd += months[1][i];
	jd += t->d;
	t->jd = jd;
	return 0;
}
int ijd(Time *t)
{
	int i;
	int j = 1;
	if(t->y%4 != 0 || (t->y%100 == 0 && t->y%400 != 0))
		j = 0;

	int jd = t->jd ;
	for(i = 0; i < 12; ++i)
	{
		if(jd <= months[j][i])
		{
			t->mon = i+1;
			t->d   = jd;
			break;
		}
		else
			jd -= months[j][i];
	}
	if(jd >= 1 && i == 12)
	{
		fprintf(stderr, "Warning: \"%s\"\n", t->KJD);
		for(i = 0; i < 12; ++i)
		{
			if(jd <= months[j][i])
			{
				t->mon = i+1;
				t->d   = jd;
				break;
			}
			else
				jd -= months[j][i];
		}
		t->y += 1;
		//exit(0);
	}
	return 0;
}
int uni(Time *t)
{
	if(t->s >= 0.0)
	{
		t->min += ((int) (floorf(t->s))) / 60;
		t->s    = t->s - 60.0 * (((int) (floorf(t->s))) / 60);
	}
	else
	{
		t->min += ( ((int) (floorf(t->s))) / 60  - 1 );
		t->s    = t->s - 60.0 * (((int) (floorf(t->s))) / 60 - 1 );
	}

	if(t->min >= 0)
	{
		t->h   += t->min / 60;
		t->min  = t->min - 60 * (t->min / 60);
	}
	else
	{
		t->h   += (t->min / 60 - 1);
		t->min  = t->min - 60 * (t->min / 60 - 1);
	}

	if(t->h >= 0)
	{
		t->jd  += t->h / 24;
		t->h    = t->h - 24 * (t->h / 24);
	}
	else
	{
		t->jd  += t->h / 24 - 1;
		t->h    = t->h - 24 * (t->h / 24 - 1);
	}
	ijd(t);
	return 0;
}

float dt(Time *t1, Time *t2) //t1 - t2
{
	//int dy   = t1->y  - t2->y;
	int djd  = t1->jd - t2->jd;
	int dh   = t1->h  - t2->h;
	int dmin = t1->min  - t2->min;
	float ds = t1->s  - t2->s;

	ds += dmin * 60.0;
	ds += dh   * 3600.0;
	ds += djd  * 86400.0;
	//if(t->y%4 != 0 || (t->y%100 == 0 && t->y%400 != 0))
	//	ds +=  dy * 365.0 * 86400;
	//else
	//	ds +=  dy * 366.0 * 86400;
	return ds;
}