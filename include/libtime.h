#ifndef TIME
#define TIME

#define FALSE 0
#define TRUE  1
#define YES 1
#define NO  0

#define ISLEAP(yr) ((!((yr) % 4) && (yr) % 100) || !((yr) % 400))
typedef struct
{
	int year, mon, jday, day;
	int hour, min, sec, msec;
	//int y,mon,jd,d,h,min;
	float f_s;
	//                    1234567890123
	char KDATE[11];    // 2015/01/01
	char KJD[9];       // 2015/001
	char KTIME[16];    // 24:12:00.120
}Time;

//Read dat from "KDATE" and "KTIME"
int rtime(Time *t);
//Read dat from "KJD" and "KTIME"
int rtimej(Time *t);
//Write string into "KDATE", "KJD" and "KTIME"
int wtime(Time *t);
//Shift "sec" from "t". The answer is t + sec
int shft(Time *t,float sec);
//Calculate jday from day
int  jd(Time *t);
//Calculate day from jay
int ijd(Time *t);
//Generate uniform dat
int uni(Time *t);
//Calculate time interval between "t1" and "t2"
float dt(Time *t1, Time *t2); //t1 - t2
//Calculate time interval between "t1" and "t2", t1 and t2 is within the same year
float dtsameyear(Time *t1, Time *t2); //t1 - t2
//Jugde whether the dat is reasonable.
//1 <= mon <= 12; 1<=jday<=36X; ...
int judge(Time *t);
#endif