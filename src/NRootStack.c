#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sac.h"
#include "libnumrec.h"

#define MAXSTACKFILE 1000

char HMSG[] = "Usage: %s -O<output sac file> -N<N-th root>  <Input file list>\n";

//float rnd_uni(long *idum);
//float gasdev(long *idum);

int main(int argc, char *argv[])
{
        float NRoot = 0.0;
        char *strOut = NULL;
        int  fgout  = 0;
        int  i,num = 0;
        SACHEAD sachdr;
        SACHEAD hdrout = sac_null;
        float   *data, *ak = NULL;
        float   *lst[MAXSTACKFILE];
        for(i = 1; i < argc; ++i)
        {
                if(argv[i][0] == '-')
                {
                        switch(argv[i][1])
                        {
                                case 'O':
                                        fgout  = 1;
                                        strOut = &(argv[i][2]);
                                        break;
                                case 'H':
                                        fprintf(stderr, HMSG, argv[0]);
                                        exit(1);
                                        break;
                                case 'N':
                                        sscanf(argv[i],"-N%f",&NRoot);
                                        break;
                                default:
                                        fprintf(stderr, "Wrong Arguments!\n");
                                        exit(1);
                                        break;
                        }
                }
        }

        if(fgout == 0 || NRoot <= 0)
        {
                fprintf(stderr, HMSG, argv[0]);
                exit(1);
        }
        ////////////////////////////////////////////////////////////////////////////////////////
        num = 0;
        for(i = 1; i < argc ; ++i)
        {
            if(argv[i][0] != '-')
            {
                if( num == 0)
                {
                    read_sachead(argv[i], &sachdr);
                    hdrout = sachdr;
                    hdrout.user0 = 0.0f;
                    // Add by wangsheng 2015/09/25
                    hdrout.evla  = sachdr.evla;
                    hdrout.evlo  = sachdr.evlo;

                    ak = (float *) calloc(sachdr.npts, sizeof(float) );
                    //memset(ak, 0, sizeof(float) * sachdr.npts );
                }


                read_sachead(argv[i], &sachdr);
                data = read_sac( argv[i], &sachdr );
                lst[num] = data;
                ++num;
                // Commented by wangsheng 2015/09/18. Now use a 'stack' function in numrec library
                //for(j = 0; j < sachdr.npts; ++j)
                //{
                //Add by Wangsheng 2015/09/04
                //    //printf("%f\n", data[i]);
                //    if( fabs(data[j]) > 1.0e-6 )
                //    {
                //            ak[j] +=  (expf(logf( fabsf(data[j]) ) / NRoot ) ) * ( data[j] / fabsf(data[j]) );
                //    }
                //}
                hdrout.user0 += sachdr.user0;
                // Add by wangsheng 2015/09/25
                //hdrout.evla  += sachdr.evla;
                //hdrout.evlo  += sachdr.evlo;
                //free(data);
            }
        }

        //Add by wangsheng 2015/09/18
        nrootStack(lst, num, hdrout.npts, &ak, NRoot);
        //
        hdrout.user0 /= (float)(num);
        //hdrout.evla  /= (float)(num);
        //hdrout.evlo  /= (float)(num);
        //for(j = 0; j < sachdr.npts; ++j)
        //{
        //    ak[j] = ( ak[j] /  (float)(num) );
        //    ak[j] = powf(fabsf(ak[j]), NRoot - 1.0) * ak[j];
        //}
        write_sac(strOut, hdrout, ak);
        free(ak);
        return 0;
}
