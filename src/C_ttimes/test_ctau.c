#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libctau.h"

//This is a example program for ctau usage
int main(int argc, char const *argv[])
{
    int i,n;
    float h = 20.0, delta = 60.0;
    char  phase[] = "PKiKP";
    float *tt, *dtdd, *dtdh, *dddp, *mn, *ts, *toa;
    int   nph   ;
    char  (*phnm)[9];

    ctau(phase, h, delta, &tt, &dtdd, &dtdh, &dddp, &mn, &ts, &toa, &nph, &phnm );
    //'Depth: ',zs,'Delta: ', delta,'Phase: ', phlst
    printf("An example or test for ctau.\n");
    for(i = 0; i< nph; ++i)
    {
        printf("Counts: %3d Phase: %-8s T: %9.3f DtDd: %9.3f Takeoff Angel: %10.7f\n", i+1, phnm[i] ,tt[i], dtdd[i], toa[i] );
    }
    return 0;
}