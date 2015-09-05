#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sac.h"

char HMSG[] = "Usage: %s -O<output sac file> -N<N-th root>  <Input file list>\n";

//float rnd_uni(long *idum);
//float gasdev(long *idum);

int main(int argc, char *argv[])
{
        float NRoot = 0.0;
        char *strOut = NULL;
        int  fgout  = 0;
        int  i,j,num = 0;
        SACHEAD sachdr;
        SACHEAD hdrout = sac_null;
        float   *data, *ak = NULL;
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
                    hdrout.user0 = 0.0;
                    ak = (float *) calloc(sachdr.npts, sizeof(float) );
                    memset(ak, 0, sizeof(float) * sachdr.npts );
                }


                read_sachead(argv[i], &sachdr);
                data = read_sac( argv[i], &sachdr );
                ++num;
                for(j = 0; j < sachdr.npts; ++j)
                {
                    //Add by Wangsheng 2015/09/04
                    //printf("%f\n", data[i]);
                    if( fabs(data[j]) > 1.0e-6 )
                    {
                            ak[j] +=  (expf(logf( fabsf(data[j]) ) / NRoot ) ) * ( data[j] / fabsf(data[j]) );
                      //      printf("hello\n");
                    }

                    //if( data[j] - 0.0 > 1.0e-7)
                    //    ak[j] +=  (expf(logf( fabsf(data[j]) ) / NRoot ) );
                    //else if( data[j] - 0.0 < 1.0e-7)
                    //    ak[j] +=  (expf(logf( fabsf(data[j]) ) / NRoot ) * -1.0);
                }
                hdrout.user0 += sachdr.user0;
                //printf("%f ",sachdr.user0 );
                //printf("%f %f\n", data[0], ak[0] );
                free(data);
            }
        }

        hdrout.user0 /= (float)(num);
        //printf("\n%f\n", hdrout.user0);
        for(j = 0; j < sachdr.npts; ++j)
        {
            ak[j] = ( ak[j] /  (float)(num) );
            ak[j] = powf(fabsf(ak[j]), NRoot - 1.0) * ak[j];
        }
        //ak[j] =  powf( (ak[j]) / (float)(num), NRoot - 1 ) * ak[j];
        write_sac(strOut, hdrout, ak);
        free(ak);
        return 0;
}