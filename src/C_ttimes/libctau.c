#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libctau.h"

static float tt[MAX];   // traveltime
static float dtdd[MAX]; // ray parameter
static float dtdh[MAX]; // dt/dh
static float dddp[MAX]; // dt/dp
static float mn[MAX];   // traveltime minutes
static float ts[MAX];   // traveltime seconds
static float toa[MAX];  // takeoff-angle
static char  phFound[MAX][8];
static char  str_phFound[MAX][9];
static int   nphFound;

    
/*
ctau() is used to calculate traveltime, rayparameter... given phase name, depth and delta.

This function is a C-interface to ftau(...) which is build in fortran in 'libftau.f'.
The file 'libftau.f' is revised from ttimes.f, which is from ttimes-Package.

You could view example 'test_ctau.c' for the usage.

If your prefre fortran, ftau(...) could be your option.

Wangsheng IGG-CAS
wangsheng.cas@gmail.com

2015/8/27
   write libftau.f and ctau()

 */

void ctau(char *strphase,        // Input phase name
          float h, float delta,  // depth in km, and delta in DEG
          float **_tt,           // traveltime    (s)
          float **_dtdd,         // ray parameter (s/deg)
          float **_dtdh,         // dt/dh
          float **_dddp,         // dt/dp
          float **_mn,           // traveltime minutes (min)
          float **_ts,           // traveltime seconds (s)
          float **_toa,          // takeoff-angle      (?DEG?)
          int   *_nph,           // number of phase found
          char  ((**_phnm)[9]) ) // phase name, ended by '\0'
{
    int i,j;
    char phase[8]={' ',' ',' ',' ',' ',' ',' ',' '};

    //Copy phase name from string format to backspace format
    for(i = 0; strphase[i] != 0; ++i)
        phase[i] = strphase[i];
    memset((char*) phFound, 0, MAX * 8);
    //printf("%f %f\n", h, delta );

    if(h < 1.0f)
        h = 1.0f;
    //if(h == 0.0) 
    //   h = 1;
    memset( (char *)tt,    0, 4*MAX );
    memset( (char *)dtdd,  0, 4*MAX );
    memset( (char *)dtdh,  0, 4*MAX );
    memset( (char *)dddp,  0, 4*MAX );
    memset( (char *)mn,    0, 4*MAX );
    memset( (char *)ts,    0, 4*MAX );
    memset( (char *)toa,   0, 4*MAX );
    memset( (char *)phFound, 0, 8*MAX );
    memset( (char *)str_phFound, 0, 9*MAX );

    //printf("%f %f\n", depth, gcarc);

    //printf("%p %p %f %f\n", &h, &delta, depth, gcarc);
    ftau_(      &phase, &h, &delta,
                &tt, &dtdd, &dtdh, &dddp, &mn, &ts, &toa,
                &nphFound, phFound                );

    *_tt   = &(tt[0])          ;
    *_dtdd = &(dtdd[0])        ;
    *_dtdh = &(dtdh[0])        ;
    *_dddp = &(dddp[0])        ;
    *_mn   = &(mn[0])          ;
    *_ts   = &(ts[0])          ;
    *_toa  = &(toa[0])         ;
    *_nph  = (nphFound)        ;
    *_phnm = &(str_phFound[0]) ;
    memset((char*) str_phFound, 0, MAX * 9);
    for(i = 0; i< nphFound; ++i)
    {
        for(j = 0
              ;
            (phFound[i][j] <= 'Z' && phFound[i][j] >= 'A' ) ||
            (phFound[i][j] <= 'z' && phFound[i][j] >= 'a' )
              ;
            ++j)
        {
            str_phFound[i][j] = phFound[i][j];
        }
    }
}