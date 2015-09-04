#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "libnumrec.h"

float rnd_uni(long *idum)
/**C*F****************************************************************
**                                                                  **
** SRC-FUNCTION   :rnd_uni()                                        **
** LONG_NAME      :random_uniform                                   **
** AUTHOR         :(see below)                                      **
**                                                                  **
** DESCRIPTION    :rnd_uni() generates an equally distributed rnd_uni-  **
**                 dom number in the interval [0,1]. For further    **
**                 reference see Press, W.H. et alii, Numerical     **
**                 Recipes in C, Cambridge University Press, 1992.  **
**                                                                  **
** FUNCTIONS      :none                                             **
**                                                                  **
** GLOBALS        :none                                             **
**                                                                  **
** PARAMETERS     :*idum    serves as a seed value                  **
**                                                                  **
** PRECONDITIONS  :*idum must be negative on the first call.        **
**                                                                  **
** POSTCONDITIONS :*idum will be changed                            **
**                                                                  **
***C*F*E*************************************************************/
{
        long j;
        long k;
        static long idum2=123456789;
        static long iy=0;
        static long iv[NTAB];
        float temp;

        if (*idum <= 0)
        {
                if (-(*idum) < 1)
                        *idum=1;
                else
                        *idum = -(*idum);
                idum2 = (*idum);
                for (j=NTAB+7;j>=0;j--)
                {
                        k     = (*idum)/IQ1;
                        *idum = IA1*(*idum-k*IQ1)-k*IR1;
                        if (*idum < 0)
                                *idum += IM1;
                        if (j < NTAB)
                                iv[j] = *idum;
                }
                iy=iv[0];
        }
        k     = (*idum)/IQ1;
        *idum = IA1*(*idum-k*IQ1)-k*IR1;
        if (*idum < 0)
              *idum += IM1;
        k     = idum2/IQ2;
        idum2 = IA2*(idum2-k*IQ2)-k*IR2;
        if (idum2 < 0)
              idum2 += IM2;
        j     = iy/NDIV;
        iy    = iv[j]-idum2;
        iv[j] =  *idum;
        if (iy < 1)
              iy += IMM1;
        if ((temp=AM*iy) > RNMX)
              return RNMX;
        else
              return temp;
}/*------End of rnd_uni()--------------------------*/


float gasdev(long *idum)
{
        static int iset = 0;
        static float gset;
        float fac, rsq, v1, v2;
        if(*idum < 0) iset = 0;
        if(iset == 0)
        {
                do
                {
                        v1  = 2.0 * rnd_uni(idum) - 1.0;
                        v2  = 2.0 * rnd_uni(idum) - 1.0;
                        rsq = v1 * v1 + v2 *v2;
                }
                while(rsq >= 1.0 || rsq == 0.0);
                fac = sqrt(-2.0 * log(rsq)/rsq );
                gset = v1 * fac;
                iset = 1;
                return v2 * fac;
        }
        else
        {
                iset = 0;
                return gset;
        }
}