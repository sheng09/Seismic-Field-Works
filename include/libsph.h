// Wangsheng 2015-05-10
// According to http://www.eas.slu.edu/People/RBHerrmann/Courses/EASA462/ASSIGNMENTS/Ass07/Ass07.pdf
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef  __SPHPOINT__
#define  __SPHPOINT__

#define  YES 1
#define  NO  0
#define  PI (atanf(1.0f) * 4.0f)
#define  POW(x) ((x)*(x))
typedef struct
{
	float x;
	float y;
	float z;
	float lon;
	float lat;
	//float r;
	float f;
	float t;
}Point;

//Transfrom Geo coordinates to XYZ
void   CordGeo2XYZ(Point *p);
//Transfrom XYZ coordinates to Geo
void   CordXYZ2Geo(Point *p);
//Calculate great circle distance. The return value is 'Degree'
float Dis(Point pE, Point pS);
//Calculate great circle distance given lat and lon of two points.
float DisLaLo(float evla, float evlo, float stla, float stlo);
//Calculate AZ of p1-->p2. The return value is 'Degree'
float Az(Point p1, Point p2); // p1-->p2
//Calculate AZ of p1-->p2.
float AzLaLo(float evla, float evlo, float stla, float stlo);
#endif