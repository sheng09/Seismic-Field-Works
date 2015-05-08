//what ??
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define  YES 1
#define  NO  0
#define  PI (atan(1.0) * 4.0)
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
double Dis(Point p1, Point p2);
int main(int argc, char const *argv[])
{
	Point p1,p2;
	int i;
	int kp1=NO,kp2=NO;
	for(i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'E':
					sscanf(argv[i],"-E%lf/%lf",&(p1.lon), &(p1.lat));
					kp1 = YES;
					break;
				case 'T':
					sscanf(argv[i],"-T%lf/%lf",&(p2.lon), &(p2.lat));
					kp2 = YES;
					break;
				default:
					break;
			}
		}

	}
	if(kp1 != YES || kp2 != YES)
	{
		fprintf(stderr, "%s -Elon/lat -Tlon/lat\n",argv[0]);
		exit(0);
	}
	if(p1.lat < -90.0 || p1.lat > 90.0 || p2.lat < -90.0 || p2.lat > 90.0 ||
	   p1.lon < -180.0|| p1.lon > 180.0|| p2.lon < -180.0|| p2.lon > 180.0)
	{
		fprintf(stderr, "Longitude is out of (-180,180) or latitude is out of (-90,90)!\n");
		exit(0);
	}
	Cord(&p1);
	Cord(&p2);
	printf("%7.3f\n", Dis(p1,p2));
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

double Dis(Point p1, Point p2)
{
	return 180.0/PI*
			acos(p1.x*p2.x +
	        	 p1.y*p2.y +
	             p1.z*p2.z   );
}
