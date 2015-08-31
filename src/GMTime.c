#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libtime.h"
#include "liberrmsg.h"
static char HMSG[]=
{"\
Description: Transform date format and shift the hour.\n\
Usage: %-6s (-Dyyyy/mm/dd | -Jyyyy/ddd)\n\
              [-Thh:mm:ss.sss]\n\
              [-Sss.sss]\n\
              [-H]\n\
\n\
(-D|-J) Date\n\
[-T]    Time\n\
[-S]    Shift the time, This option should be used with the -T and -D|-J option.\n\
[-H]    Display this message.\n\
"};

int main(int argc, char const *argv[])
{
	Time t;
	int i;
	int ktd=NO, kjd=NO, ktt=NO, ks=NO;
	float sshift;
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
					sscanf(argv[i]+2,"%f",&sshift);
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
		perrmsg("",ERR_MORE_ARGS);
		fprintf(stderr, HMSG, argv[0]);
		exit(1);
	}
	if(ktt != YES)
		strcpy(t.KTIME,"00:00:00.000");
	//Read Date and Time
	if(ktd == YES)
		rtime(&t);
	else if(kjd == YES)
		rtimej(&t);
	if(!judge(&t))
	{
		exit(1);
	}
	//Shift the hour when -T and -S are used
	if(ks == YES)
	{
		if(ktt == YES)
			shft(&t,sshift);
		else
		{
			perrmsg("Err: -T is needed for -S\n   ", ERR_MORE_ARGS);
			exit(1);
		}
	}

	//Write Date and Time into "string"
	uni(&t);
	wtime(&t);

	//Output Date or Time corresponding to options
	//if(ktd == YES)
	printf("%s ",t.KJD);
	//if(kjd == YES)
	printf("%s ",t.KDATE );
	//if(ks == YES && ktt == YES)
	if(ktt == YES)
		printf("%s ",t.KTIME );
	printf("\n");
	return 0;
}