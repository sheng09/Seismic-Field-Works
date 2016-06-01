/*

Wangsheng IGG-CAS
wangsheng.cas@gmail.com

2015/8/27
     Hello world

 */

#ifndef LIBTAUP
#define LIBTAUP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 60
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
          char  ((**_phnm)[9]) ); // phase name, ended by '\0'

#endif
