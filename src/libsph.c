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
	return Dis(ps, ps);
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