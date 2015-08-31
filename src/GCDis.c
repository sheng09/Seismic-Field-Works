// Wangsheng 2015-05-10
// According to http://www.eas.slu.edu/People/RBHerrmann/Courses/EASA462/ASSIGNMENTS/Ass07/Ass07.pdf
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "liberrmsg.h"
#include "libsph.h"

static char HMSG[]=
{"\
Description: Calculate great circle distance given 2 points.\n\
Usage: %-6s -Elon/lat -Slon/lat [-H]\n\
\n\
(-E) Coordination(Deg) of the Eq.\n\
(-S) Coordination(Deg) of the Sta.\n\
[-H] Display this message.\n\
"};

int main(int argc, char const *argv[])
{
	Point pE,pS;
	float az,bz;
	int i;
	int kp1=NO,kp2=NO;
	for(i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'E':
					sscanf(argv[i],"-E%f/%f",&(pE.lon), &(pE.lat));
					kp1 = YES;
					break;
				case 'S':
					sscanf(argv[i],"-S%f/%f",&(pS.lon), &(pS.lat));
					kp2 = YES;
					break;
				case 'H':
					fprintf(stderr, HMSG, argv[0]);
					exit(0);
				default:
					break;
			}
		}

	}
	if(kp1 != YES || kp2 != YES)
	{
		perrmsg("",ERR_MORE_ARGS);
		fprintf(stderr, HMSG, argv[0]);
		exit(1);
	}
	if(pE.lat < -90.0f || pE.lat > 90.0f || pS.lat < -90.0f || pS.lat > 90.0f ||
	   pE.lon < -180.0f|| pE.lon > 180.0f|| pS.lon < -180.0f|| pS.lon > 180.0f)
	{
		perrmsg("Err: Longitude is out of (-180,180) or latitude is out of (-90,90)\n   ", ERR_BAD_ARGS);
		exit(1);
	}
	CordGeo2XYZ(&pE);
	CordGeo2XYZ(&pS);

	az = Az(pE,pS);
	bz = Az(pS,pE);

	printf("GCD: %8.4f AZ: %8.4f BAZ: %8.4f\n", Dis(pE,pS), az, bz);
	return 0;
}