#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libtime.h"
#include "liberrmsg.h"
static char HMSG[]=
{"\
Description: Calculate dt for {D T - F Y}, return value is in seconds.\n\
Usage: %-6s (-Dyyyy/mm/dd | -Jyyyy/ddd)\n\
            (-Thh:mm:ss.sss)\n\
            (-Fyyyy/mm/dd | -Kyyyy/ddd)\n\
            (-Yhh:mm:ss.sss)\n\
            [-H]\n\
\n\
(-D|-J) Date1\n\
(-T)    Time1\n\
(-F|-K) Date1\n\
(-Y)    Time1\n\
[-H]    Display this message.\n\
\n\
NOTE: Return value is t1 - t2. \n\
"};

int main(int argc, char const *argv[])
{
	Time t1,t2;
	int i;
	int kt1d = NO, kt1jd = NO, kt1t = NO;
	int kt2d = NO, kt2jd = NO, kt2t = NO;
	float Dt;
	memset(t1.KDATE,0,16);
	memset(t1.KTIME,0,16);
	memset(t1.KJD  ,0,16);
	memset(t2.KDATE,0,16);
	memset(t2.KTIME,0,16);
	memset(t2.KJD  ,0,16);
	for(i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'D':
					memcpy(t1.KDATE,argv[i]+2,15);
					kt1d = YES;
					break;
				case 'J':
					memcpy(t1.KJD,argv[i]+2,15);
					kt1jd = YES;
					break;
				case 'T':
					memcpy(t1.KTIME,argv[i]+2,15);
					kt1t = YES;
					break;
				///
				case 'F':
					memcpy(t2.KDATE,argv[i]+2,15);
					kt2d = YES;
					break;
				case 'K':
					memcpy(t2.KJD,argv[i]+2,15);
					kt2jd = YES;
					break;
				case 'Y':
					memcpy(t2.KTIME,argv[i]+2,15);
					kt2t = YES;
					break;
				///
				case 'H':
					fprintf(stderr, HMSG, argv[0]);
					exit(0);
				default:
					break;
			}
		}
	}
	if( ( (kt1d + kt1jd) != YES ) || kt1t != YES ||
	    ( (kt2d + kt2jd) != YES ) || kt2t != YES    )
	{
		perrmsg("",ERR_MORE_ARGS);
		fprintf(stderr, HMSG, argv[0]);
		exit(0);
	}

	//Read Date and Time
	if(kt1d == YES)
		rtime(&t1);
	else if(kt1jd == YES)
		rtimej(&t1);

	if(kt2d == YES)
		rtime(&t2);
	else if(kt2jd == YES)
		rtimej(&t2);
	if(!judge(&t1) || !judge(&t2))
	{
		exit(1);
	}
	Dt = dt(&t1,&t2);
	if(Dt < 0)
		printf("%f -1\n", Dt );
	else
		printf("%f 1\n", Dt );
	return 0;
}
