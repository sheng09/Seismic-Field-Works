// Wangsheng 2015-05-10
// According to http://www.eas.slu.edu/People/RBHerrmann/Courses/EASA462/ASSIGNMENTS/Ass07/Ass07.pdf
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define  YES 1
#define  NO  0
#define  PI (atan(1.0) * 4.0)
#define  POW(x) ((x)*(x))
#ifndef POINT
#define POINT
typedef struct
{
	double x;
	double y;
	double z;
	double lon;
	double lat;
	//double r;
	double f;
	double t;
}Point;
int Cord(Point *p);
double Dis(Point pE, Point pS);
double Az(Point p1, Point p2); // p1-->p2
#endif

static char HMSG[]=
{"\
Description: Calculate great circle distance given 2 points.\n\
Usage: %-6s -Elon/lat -Tlon/lat [-H]\n\
(-E) Coordination(Deg) of the Eq.\n\
(-S) Coordination(Deg) of the Sta.\n\
[-H] Display this message.\n\
"};

int main(int argc, char const *argv[])
{
	Point pE,pS;
	double az,bz;
	int i;
	int kp1=NO,kp2=NO;
	for(i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'E':
					sscanf(argv[i],"-E%lf/%lf",&(pE.lon), &(pE.lat));
					kp1 = YES;
					break;
				case 'S':
					sscanf(argv[i],"-S%lf/%lf",&(pS.lon), &(pS.lat));
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
		fprintf(stderr, HMSG, argv[0]);
		exit(0);
	}
	if(pE.lat < -90.0 || pE.lat > 90.0 || pS.lat < -90.0 || pS.lat > 90.0 ||
	   pE.lon < -180.0|| pE.lon > 180.0|| pS.lon < -180.0|| pS.lon > 180.0)
	{
		fprintf(stderr, "Longitude is out of (-180,180) or latitude is out of (-90,90)!\n");
		exit(0);
	}
	Cord(&pE);
	Cord(&pS);

	az = Az(pE,pS);
	bz = Az(pS,pE);

	printf("GCD: %7.3lf AZ: %7.3lf BAZ: %7.3lf\n", Dis(pE,pS), az, bz);
	return 0;
}

int Cord(Point *p)
{
	p->t = (90.0 - p->lat) *PI /180.0;
	p->f = p->lon *PI/180.0;
	p->x = sin(p->t) * cos(p->f);
	p->y = sin(p->t) * sin(p->f);
	p->z = cos(p->t);
	return 0;
}

double Dis(Point pE, Point pS)
{
	double c = pE.x*pS.x + pE.y*pS.y + pE.z*pS.z;
	double sby2 = 0.5*sqrt(POW(pE.x-pS.x) + POW(pE.y-pS.y) + POW(pE.z-pS.z));
	double cby2 = 0.5*sqrt(POW(pE.x+pS.x) + POW(pE.y+pS.y) + POW(pE.z+pS.z));
	double s = sby2 * cby2 *2.0;
	return atan2(s,c)*180.0/PI;
}

double Az(Point p1, Point p2) // p1-->p2
{
	double dis = Dis(p1, p2) * PI/180.0;
	double saz = sin(p2.t) * sin(p1.t) * sin(p2.f-p1.f);
	double caz = cos(p2.t) - cos(dis) * cos(p1.t);
	double az  = atan2(saz,caz)*180.0/PI;

	if( az < 0.0 )
		az += 360.0; // 0.0 <= az <= 360

	if( fabs(az - 0.0) < 0.000001 || fabs(az - 360.0) < 0.000001)
		az = 0.0; // Make sure 0.0 is always 0.0, not 360.0
	return az;
}