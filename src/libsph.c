#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libsph.h"

void   CordGeo2XYZ(Point *p)
{
	p->t = (90.0f - p->lat) *PI /180.0f;
	p->f = p->lon *PI/180.0f;
	p->x = sinf(p->t) * cosf(p->f);
	p->y = sinf(p->t) * sinf(p->f);
	p->z = cosf(p->t);
	return;
}
void   CordXYZ2Geo(Point *p)
{
	float lxy = sqrtf( POW(p->x) + POW(p->y) );
	p->lon =  atan2f(p->y, p->x) / PI * 180.0f;
	p->lat =  atan2f(p->z, lxy)  / PI * 180.0f;
	return;
}
float Dis(Point pE, Point pS)
{
	float c = pE.x*pS.x + pE.y*pS.y + pE.z*pS.z;
	float sby2 = 0.5f*sqrtf(POW(pE.x-pS.x) + POW(pE.y-pS.y) + POW(pE.z-pS.z));
	float cby2 = 0.5f*sqrtf(POW(pE.x+pS.x) + POW(pE.y+pS.y) + POW(pE.z+pS.z));
	float s = sby2 * cby2 *2.0f;
	return atan2f(s,c)*180.0f/PI;
}

//DisLaLo(evdat[j].evla, evdat[j].evlo, sacdat[0].stla, sacdat[0].stlo);
float DisLaLo(float evla, float evlo, float stla, float stlo)
{
	Point pe,ps;
	pe.lat = evla; pe.lon = evlo;
	ps.lat = stla; ps.lon = stlo;
	CordGeo2XYZ(&pe); CordGeo2XYZ(&ps);
	//printf("%f %f %f %f: ", pe.lat, pe.lon, ps.lat, ps.lon);
	return Dis(pe, ps);
}

float Az(Point p1, Point p2) // p1-->p2
{
	float dis = Dis(p1, p2) * PI/180.0f;
	float saz = sinf(p2.t) * sinf(p1.t) * sinf(p2.f-p1.f);
	float caz = cosf(p2.t) - cosf(dis) * cosf(p1.t);
	float az  = atan2f(saz,caz)*180.0f/PI;

	if( az < 0.0f )
		az += 360.0f; // 0.0 <= az <= 360

	if( fabs(az - 0.0f) < 0.000001f || fabs(az - 360.0f) < 0.000001f)
		az = 0.0f; // Make sure 0.0 is always 0.0, not 360.0
	return az;
}

float AzLaLo(float evla, float evlo, float stla, float stlo)
{
	Point pe,ps;
	pe.lat = evla; pe.lon = evlo;
	ps.lat = stla; ps.lon = stlo;
	CordGeo2XYZ(&pe); CordGeo2XYZ(&ps);
	return Az(pe,ps);
}

//cross pruduct of n1 and n2. In xyz coordinates
void vecCross(vector3d *n1, vector3d *n2, vector3d *ak)
{
	float len;
	ak->vx = n1->vy * n2->vz - n1->vz * n2->vy;
	ak->vy = n1->vz * n2->vx - n1->vx * n2->vz;
	ak->vz = n1->vx * n2->vy - n1->vy * n2->vx;
	len = sqrtf( POW(ak->vx ) + POW(ak->vy) + POW(ak->vz) ) ;
	ak->vx /= len;
	ak->vy /= len;
	ak->vz /= len;
	return ;
}

//direction of the great circle cross p1 and p2.
void dirCircle(Point *p1, Point *p2, Point *ak)
{
	vector3d v1, v2, v;
	v1.vx = p1->x;  v2.vx = p2->x;
	v1.vy = p1->y;  v2.vy = p2->y;
	v1.vz = p1->z;  v2.vz = p2->z;
	vecCross(&v1, &v2, &v);
	ak->x = v.vx;
	ak->y = v.vy;
	ak->z = v.vz;
	CordXYZ2Geo(ak);
	return ;
}
//direction of the great circle cross p1 and p2.
void dirCircleLaLo(float lo1, float la1, float lo2, float la2, float *lo, float *la )
{
	Point p1, p2, p;
	p1.lat = la1; p1.lon = lo1;
	p2.lat = la2; p2.lon = lo2;
	CordGeo2XYZ(&p1);
	CordGeo2XYZ(&p2);
	dirCircle(&p1, &p2, &p);
	*lo = p.lon;
	*la = p.lat;
	return ;
}