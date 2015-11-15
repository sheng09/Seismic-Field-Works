#include "liberrmsg.h"

#ifndef __NUMRECIP__
#define __NUMRECIP__

/*--Constants for rnd_uni()--*/
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)
float rnd_uni(long *idum);

float gasdev(long *idum);

#define MIN(x,y) ( (x)>(y) ? (y) : (x) )
#define MAX(x,y) ( (x)>(y) ? (x) : (y) )

//Linear stack
float* linearStack(float **trace, int ntrace, int len, float **ak);
//NRoot stack
float* nrootStack(float **trace, int ntrace, int len, float **ak, float NRoot);


//Integral for given 'trace' from 'pre' to 'suf' using trapezoid method
float integf(float *trace, int pre, int suf, float dx);
//Find the max value of given 'trace', and its 'index'
float fdmaxf(float *trace, int len, int *index);
//Correlation for two vector
float corf(float *trace1, float *trace2, int len);
//Correlation for two vector interval [b,e]
float corf_2(float *trace1, float *trace2, int len, int b, int e);
#endif

#ifndef POW
#define POW(x) ( (x) * (x) )
#endif

