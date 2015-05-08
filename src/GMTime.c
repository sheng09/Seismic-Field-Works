#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YES 1
#define NO  0

#ifndef TIME
#define TIME 
typedef struct
{
	int y,mon,jd,d,h,min;
	float s;
	char KDATE[16];
	char KJD[16];
	char KTIME[16];
}Time;
int rtime(Time *t);
int rtimej(Time *t);
int wtime(Time *t);
int chh(Time *t,int x);
int  jd(Time *t);
int ijd(Time *t);
#endif

static char HMSG[]=
{"\
Description: Transform date format and shift the hour.\n\
Usage: %-6s (-Dyyyy-mm-dd | -Jyyyy-ddd)\n\
              [-Thh:mm:ss.ss -Shourshift]\n\
              [-H]\n\
(-D|-J) Date\n\
[-T]    Time\n\
[-S]    Shift the hour, This  option should be used with the -T and -D|-J option.\n\
[-H]    Display this message.\n\
"};

int main(int argc, char const *argv[])
{
	Time t;
	int i;
	int ktd=NO, kjd=NO, ktt=NO, ks=NO;
	int hshift;
	memset(t.KDATE,0,16);
	memset(t.KTIME,0,16);
	memset(t.KJD  ,0,16);
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
				case 'J':
					memcpy(t.KJD,argv[i]+2,15);
					kjd = YES;
					break;
				case 'T':
					memcpy(t.KTIME,argv[i]+2,15);
					ktt = YES;
					break;
				case 'S':
					sscanf(argv[i]+2,"%d",&hshift);
					ks = YES;
					break;
				case 'H':
					fprintf(stderr, HMSG, argv[0]);
					exit(0);
				default:
					break;
			}
		}
	}
	if(ktd != YES && kjd != YES)
	{
		fprintf(stderr, HMSG, argv[0]);
		exit(0);
	}

	//Read Date and Time
	if(ktd == YES)
		rtime(&t);
	else if(kjd == YES)
		rtimej(&t);

	//Shift the hour when -T and -S are used
	if(ks == YES)
	{
		if(ktt == YES)
			chh(&t,hshift);
		else
		{
			fprintf(stderr, "Err: -T is needed fo -S\n");
			exit(0);
		}
	}

	//Write Date and Time into "string"
	wtime(&t);

	//Output Date or Time corresponding to options
	if(ktd == YES)
		printf("%s ",t.KJD);
	if(kjd == YES)
		printf("%s ",t.KDATE );
	if(ks == YES && ktt == YES)
		printf("%s ",t.KTIME );
	printf("\n");
	return 0;
}

int rtime(Time *t)
{
	sscanf(t->KDATE,"%d-%d-%d", &(t->y), &(t->mon), &(t->d));
	sscanf(t->KTIME,"%d:%d:%f", &(t->h), &(t->min), &(t->s));
	jd(t);
	return 0;
}
int rtimej(Time *t)
{
	sscanf(t->KJD  ,"%d-%d"   , &(t->y), &(t->jd));
	sscanf(t->KTIME,"%d:%d:%f", &(t->h), &(t->min), &(t->s));
	ijd(t);
	return 0;
}
int wtime(Time *t)
{
	sprintf(t->KDATE,"%04d-%02d-%02d" ,t->y,t->mon,t->d);
	sprintf(t->KJD,"%04d-%03d"        ,t->y,t->jd);
	sprintf(t->KTIME,"%02d:%02d:%05.2f",t->h,t->min,t->s);
	return 0;
}

static months[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},
                      {31,29,31,30,31,30,31,31,30,31,30,31}};
int chh(Time *t,int x)
{
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
	}
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
		fprintf(stderr, "Err: \"%s\"\n", t->KJD);
		exit(0);
	}
	return 0;
}