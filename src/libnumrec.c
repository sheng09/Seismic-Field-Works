#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "libnumrec.h"
#include "liberrmsg.h"
#include <string.h>
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

float* linearStack(float **trace, int ntrace, int len, float **ak)
{
        int i, j;
        float *x;
        memset(*ak, len, sizeof(float));
        for( i = 0; i < ntrace; ++i)
        {
                x = trace[i];
                for(j = 0; j < len; ++j)
                {
                        (*ak)[j] += x[j];
                }
        }
        for( j = 0 ; j < len; ++j)
        {
                (*ak)[j] /= (float) (ntrace);
        }
        return *ak;
}

float* nrootStack(float **trace, int ntrace, int len, float **ak, float NRoot)
{
        int i, j;
        float *x;
        memset(*ak, len, sizeof(float));
        if( NRoot == 1.0f )
        {
                linearStack(trace, ntrace, len, ak);
                return *ak;
        }
        for( i = 0; i < ntrace; ++i)
        {
                x = trace[i];
                for(j = 0; j < len; ++j)
                {
                        if(x[j] > 1.0e-6f)
                        {
                                (*ak)[j] += (expf(logf( fabsf(x[j]) ) / NRoot ) ) * ( x[j] / fabsf(x[j]) );
                        }
                }
        }
        for( j = 0 ; j < len; ++j)
        {
                (*ak)[j] /= (float) (ntrace);
                (*ak)[j] = powf(fabsf( (*ak)[j] ), NRoot - 1.0) * (*ak)[j];
        }
        return *ak;
}


//Integral for given 'trace' from 'pre' to 'suf' using trapezoid method
float integf(float *trace, int pre, int suf, float dx)
{
    float ak = 0.5f * (trace[pre] + trace[suf]);
    int i;
    for( i = pre +1 ; i < suf; ++i )
        ak += trace[i];
    ak *= dx;
    return ak;
}

//Find the max value of given 'trace', and its 'index'
float fdmaxf(float *trace, int len, int *index)
{
    int i;
    float max = trace[0];
    for( i = 1; i < len; ++i)
    {
        if( max < trace[i] )
        {
            *index = i;
            max = trace[i];
        }
    }
    return max;
}

//Correlation for two vector
float corf(float *trace1, float *trace2, int len)
{
    int i;
    float ak = 0.0f, ac1 = 0.0f, ac2 = 0.0f ;
    for(i = 0; i < len; ++i)
    {
        ak  += trace1[i] * trace2[i];
        ac1 += trace1[i] * trace1[i];
        ac2 += trace2[i] * trace2[i];
    }
    ak *= sqrtf(1.0f/ ac1 / ac2);
    return ak;
}

//Correlation for two vector interval [b,e]
float corf_2(float *trace1, float *trace2, int len, int b, int e)
{
    if(b < 0 || e < 0 || b > len || e > len || e < b)
    {
        perrmsg( "corf_2", WARN_OUT_RANGE);
        exit(1);
    }
    int i;
    float ak = 0.0f, ac1 = 0.0f, ac2 = 0.0f ;
    for(i = b; i <= e; ++i)
    {
        ak  += trace1[i] * trace2[i];
        ac1 += trace1[i] * trace1[i];
        ac2 += trace2[i] * trace2[i];
    }
    //Add by Wangsheng 2015/11/08
    if(ac1 > 1.0e-8 && ac2 > 1.0e-8)
        ak *= sqrtf(1.0f/ ac1 / ac2);
    else
        ak = 0.0f;
    return ak;
}