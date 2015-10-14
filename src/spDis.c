// Wangsheng 2015-05-10
// According to http://www.eas.slu.edu/People/RBHerrmann/Courses/EASA462/ASSIGNMENTS/Ass07/Ass07.pdf
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "liberrmsg.h"
#include "libsph.h"

static char HMSG[]=
{"\
Description: Calculate the distance from a point to a line on the sphere.\n\
Usage: %-6s -Plon/lat -Llon1/lat1/lon2/lat2 [-H]\n\
\n\
(-P) Coordination(Deg) of the point.\n\
(-L) Coordination(Deg) of the two end of the spherical line.\n\
[-H] Display this message.\n\
"};

int main(int argc, char const *argv[])
{
	Point p, pL1, pL2, pL3;
	//float az,bz;
	float dis;
	int i;
	int kp=NO,kpL=NO;
	for(i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'P':
					sscanf(argv[i],"-P%f/%f",&(p.lon), &(p.lat));
					kp = YES;
					break;
				case 'L':
					sscanf(argv[i],"-L%f/%f/%f/%f",&(pL1.lon), &(pL1.lat), &(pL2.lon), &(pL2.lat) );
					kpL = YES;
					break;
				case 'H':
					fprintf(stderr, HMSG, argv[0]);
					exit(0);
				default:
					break;
			}
		}

	}
	if(kp != YES || kpL != YES)
	{
		perrmsg("",ERR_MORE_ARGS);
		fprintf(stderr, HMSG, argv[0]);
		exit(1);
	}
	if( p.lat   < -90.0f || p.lat   > 90.0f || p.lon   < -180.0f|| p.lon   > 180.0f ||
		pL1.lat < -90.0f || pL1.lat > 90.0f || pL1.lon < -180.0f|| pL1.lon > 180.0f ||
		pL2.lat < -90.0f || pL2.lat > 90.0f || pL2.lon < -180.0f|| pL2.lon > 180.0f   )
	{
		perrmsg("Err: Longitude is out of (-180,180) or latitude is out of (-90,90)\n   ", ERR_BAD_ARGS);
		exit(1);
	}
	CordGeo2XYZ(&p);
	CordGeo2XYZ(&pL1);
	CordGeo2XYZ(&pL2);

	dirCircle(&pL1, &pL2, &pL3);
	dis = 90.0f - Dis(pL3, p);
	if(dis < 0.0f)
		dis = -dis;
	printf("Distance from Point to Line is: %f\n", dis);
	//printf("GCD: %8.4f AZ: %8.4f BAZ: %8.4f\n", Dis(pE,pS), az, bz);
	return 0;
}