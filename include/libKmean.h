#ifndef __KMEAN__
#define __KMEAN__

#define ISZERO(x)    ( ((x) == 0) + (x) )

#define K_RAY_PARA 0
#define K_DIST     1
#define K_LAT_LON  2
#define K_XYZ      3
typedef int KMEAN_TYPE;

typedef  float XYZ[3];
typedef  char  TAG[64];
typedef struct
{

	int    dimension; // Dimensions of the data. 2D corresponds to lat-lon coordinates

	TAG    *tags;     // tags for each point
	XYZ    *pts;      // data pointer
	int    *clas;     // beinting of each data
	int    npts;     // number of data
	XYZ     min,max,dmm;  // min and max value of the data

	XYZ    *cnt;      // centers pointer
	int    *counts;   // number of points beint to each centers
	float  *radius;   // max radius of each group
	float  *var;      // variance for each center
	int    nCnt;     // number of centers

	int   worst;

	float  *value;    // object values of each iteration
	int    iter_c;    // counts of iteration
	int    iter_max;  // max number of iteration
	float  threshold; // threshold for value
} Prj_Kmean;


//Calculate center points coordinates.
// This step must be after geClass()
int calCnt(Prj_Kmean *prj, long *seed);

//Determined which center points each data beints to
// This step must be after calCnt() or rndCnt()
int geClass(Prj_Kmean *prj);

//Calculate object value for current Kmean answer
float calVal(Prj_Kmean *prj);

//Calculate variance for each group, and its max radius
float calVar(Prj_Kmean *prj);
//Acquire the worst group according to its max radius
int acqWorst(Prj_Kmean *prj);

//Generate random center points coordinates given a seed value
int rndCnt(Prj_Kmean *prj, long *seed );

//Main function of the Kmean
int kMean(Prj_Kmean *prj,  long *seed );

//Calculate distance of two data, or a data and center.
//  The dimension should be given. 2 dimensions corresponds to lat-lon coordinates
float distK_mean( XYZ *p1, XYZ *p2, int dimension );

//Read dat from a file
//  This step must be after iniKmean
int rdDat(Prj_Kmean *prj, char *filename);

int wrtCnt(Prj_Kmean *prj, char *filename);

int wrtDat(Prj_Kmean *prj, char *filename);

//Initialization of Kmean.
//  In this step, number of classes, max iteration numbers, dimension of the data
//  and threshold object value will be assigned.
int iniKmean(Prj_Kmean *prj, int nCnt, int iter_max, int dimension, float threshold);

//Acquire the min and max value of the data
int minmax(Prj_Kmean *prj);

//Clear all the data array
int clear(Prj_Kmean *prj);
#endif