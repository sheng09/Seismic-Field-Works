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
int shft(Time *t,float sec);
int  jd(Time *t);
int ijd(Time *t);
int uni(Time *t);
float dt(Time *t1, Time *t2); //t1 - t2
#endif