#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YES 1
#define NO  0
typedef struct
{
	int y,mon,jd,d,h,min;
	float s;
	char KDATE[16];
	char KJD[16];
	char KTIME[16];
}Time;
int rtime(Time *t);
int wtime(Time *t);
int chh(Time *t,int x);
int jd(Time t);
int main(int argc, char const *argv[])
{
	Time t;
	int i;
	int ktd=NO,ktt=NO,ks=NO;
	int hshift;
	memset(t.KDATE,0,16);
	memset(t.KTIME,0,16);
	memset(t.KJD,0,16);
	for(i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'D':
					memcpy(t.KDATE,argv[i]+2,15);
					ktd = YES;
					break;
				case 'T':
					memcpy(t.KTIME,argv[i]+2,15);
					ktt = YES;
					break;
				case 'S':
					sscanf(argv[i]+2,"%d",&hshift);
					ks = YES;
					break;
				default:
					break;
			}
		}
	}
	if(ktd != YES || ktt != YES)
	{
		fprintf(stderr, "%s -Dyyyy-mm-dd -Thh:mm:ss.ss [-Shourshift]\n",argv[0]);
		exit(0);
	}
	rtime(&t);
	if(ks == YES)
		chh(&t,hshift);
	wtime(&t);
	printf("%s %s %s\n",t.KDATE,t.KJD, t.KTIME);
	return 0;
}

int rtime(Time *t)
{
	sscanf(t->KDATE,"%d-%d-%d", &(t->y), &(t->mon), &(t->d));
	sscanf(t->KTIME,"%d:%d:%f", &(t->h), &(t->min), &(t->s));
	t->jd = jd(*t);
	return 0;
}
int wtime(Time *t)
{
	sprintf(t->KDATE,"%04d-%02d-%02d" ,t->y,t->mon,t->d);
	sprintf(t->KJD,"%04d-%03d"        ,t->y,t->jd);
	sprintf(t->KTIME,"%02d:%02d:%05.2f",t->h,t->min,t->s);

}

static months[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},
                      {31,29,31,30,31,30,31,31,30,31,30,31}};
int chh(Time *t,int x)
{
	int i = 1;
	if(t->y%4 != 0 || (t->y%100 == 0 && t->y%400 != 0))
		i = 0;
	t->h += x;
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
		{
			t->d -= 1;
		}
	}
	if(t->h >= 24)
	{
		t->h -= 24;
		t->jd += 1;
		if(t->d == months[i][t->mon-1])
		{
			t->d = 1;
			t->mon += 1;
		}
		else
		{
			t->d += 1;
		}
	}
	return 0;
}
int jd(Time t)
{
	int jd = 0;
	int i;
	if(t.y%4 != 0 || (t.y%100 == 0 && t.y%400 != 0))
		for(i = 0; i < t.mon-1; ++i)
			jd += months[0][i];
	else
		for(i = 0; i < t.mon-1; ++i)
			jd += months[1][i];
	jd += t.d;
	return jd;
}
//2015-04-30 	00:09:52.3