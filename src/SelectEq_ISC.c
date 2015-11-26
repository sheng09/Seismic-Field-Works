#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <string.h>
#include "liberrmsg.h"

#define  PI 3.1415926535
#define  LAT2T(x) (0.5*PI-(x)/180.0*PI)
#define  LON2F(x) ((x)/180.0*PI)
#define  T2LAT(x) ((0.5*PI-x)*180.0/PI)
#define  F2LON(x) ((x)*180.0/PI)
#define  DIS(f1,t1,f2,t2) (180.0/PI*acos(sin((t1))*sin((t2))*cos((f1)-(f2))+cos((t1))*cos((t2))))

static char HMSG[]=
{"\n\
SelectEq  - Select earthquakes given a central point and a radius interval.\n\
            The input data is from ISC Bulletin event catalogue.\n\
\n\
Usage: SelectEq  -I <infile name>\n\
                 -C <longitude/latitude>\n\
                 -R <radius floor/radius ceil>  [0 < radius < 180]\n\
                 -N <number of headlines>\n\
                 -M <min magnitude>\n\
\n\
Description:\n\
          The longitude and latitude is in DEGREE format.\n\
              Longitude: [0,360], Latitude: [-90,90].\n\
          The radius is in DEGREE format.\n\
              Radius: (0,PI)\n\
Example:\n\
\n\
          SelectEq -I isc.dat -C 110.0/35.4 -R 30.0/120.0 -N 20 -M 7.0 \n\
\n\
Author: Wangsheng, IGGCAS, PRC.\n\
Email:  wangsheng.cas@gmail.com\
\n\
"};
int main(int argc, char  *argv[])
{
	char  *str_file;
	char  *str_center;
	char  *str_radius;
	char  *str_n;
    char  *str_mag0;
	int   flag_file    = 0,
	      flag_center  = 0,
		  flag_radius  = 0,
		  flag_n       = 0,
          flag_mag0    = 0;
    int   oc;
    char  ec;
    int   _Index;

    char day[11], dayt[12], MagType[3];
    double depth;
    static struct  option _Option[]=
    {
    	{"infile",  required_argument,  NULL,   'I'},
    	{"center",  required_argument,  NULL,   'C'},
    	{"radius",  required_argument,  NULL,	'R'},
    	{"number",  required_argument,  NULL,   'N'},
        {"mag"   ,  required_argument,  NULL,   'M'},
    	{0,0,0,0}
    };
    while( (oc= getopt_long(argc, argv, ":I:C:R:N:M:H", _Option, &_Index)) != -1 )
    {
    	switch(oc)
    	{
    		case 'I':
    			str_file     = optarg;
    			flag_file    = 1;
    			break;
    		case 'C':
    			str_center   = optarg;
    			flag_center  = 1;
    			break;
    		case 'R':
    			str_radius   = optarg;
    			flag_radius  = 1;
    			break;
    		case 'N':
    			str_n        = optarg;
    			flag_n       = 1;
    			break;
            case 'M':
                str_mag0     = optarg;
                flag_mag0    = 1;
                break;
    		case 'H':
    			fprintf(stderr, " %s\n", HMSG);
    			exit(0);
    		case ':':
				ec = (char) (oc);
				fprintf(stderr, "\'-%c\' OR \'--  \'required parameter\n",ec);
				exit(0);
			case '?':
				fprintf(stderr, "Wrong argument!\n");
				exit(0);
    	}
    }
    if(flag_file != 1 || flag_radius != 1 || flag_center != 1 || flag_n != 1 || flag_mag0 != 1)
    {
    	perrmsg( HMSG, ERR_MORE_ARGS);
        //(stderr, " More arguments are required!\n");

        //fprintf(stderr, " %s\n", HMSG );
    	exit(0);
    }
    ////////////////////////////////////////////
    FILE   *fp;
    double lon0,lat0;
    double f0,  t0;
    double r0,r1;
    int    n;
    double mag0;
    if((fp = fopen(str_file,"r"))== NULL)
    {
        perrmsg(str_file, ERR_OPEN_FILE);
    	//fprintf(stderr, "Err: can't open file '%s' for read!\n", str_file);
    	exit(0);
    }
    sscanf(str_center,"%lf/%lf",&lon0,&lat0);
    if(lon0 < 0 || lon0 > 360 || lat0 < -90 || lat0 > 90)
    {
        //perrmsg();
    	fprintf(stderr, "Err: invalid center coordinates! '%s'\n",str_center );
    	exit(0);
    }
    f0 = LON2F(lon0);
    t0 = LAT2T(lat0);
    sscanf(str_radius,"%lf/%lf",&r0,&r1);
    if(r0 < 0.0 || r1 < 0.0 || r0 >= r1)
    {
    	fprintf(stderr, "Err: invalid radius interval! '%s'\n",str_radius );
    	exit(0);
    }
    sscanf(str_n,"%d",&n);
    if(n < 0)
    {
    	fprintf(stderr, "Err: invalid number of headlines! '%s'\n",str_n );
    	exit(0);
    }
    sscanf(str_mag0,"%lf",&mag0);
    if(mag0 < 0.0)
    {
        fprintf(stderr, "Err: invalid magnitude setting! '%s'\n",str_mag0 );
        exit(0);
    }
    ////////////////////////////////////////////////////////
    char Line[4096];
    int  i = 0;
    //char ctmp[128];
    double lon,lat,mag;
    double dis;
    double f,t;

    for (i = 0; i < n; ++i)
    {
    	memset(Line,0,4096);
    	if( fgets(Line,4096,fp) == NULL) break;
    }
    while(1)
    {
    	memset(Line,0,4096);
    	if( fgets(Line,4096,fp) == NULL) break;
    	//13325010,ISC      ,2008-01-01,01:01:17.09, 27.4375, 139.9896,443.3,
    	sscanf(&(Line[43]),"%lf,%lf,%lf",&lat,&lon,&depth);
        sscanf(&(Line[92]),"%lf",&mag);
        if(mag >= mag0)
        {
            f = LON2F(lon);
            t = LAT2T(lat);
            dis = DIS(f0,t0,f,t);
            //printf("%d %lf %lf\n",i,lat,lon );
            if(dis < r1 && dis > r0)
            {
                /*
0         1         2         3         4         5         6         7         8         9
0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
   380100,ISC      ,1990-01-01,14:49:00.39,-10.4982, 161.3621, 35.3,      ,BJI      ,mb    , 5.5
                 */
                /*
                年/月/日 时:分:秒 纬度 经度 深度 震级 震级标度
                2015/03/23 07:27:07.0 26.67 108.78 8 3.3 Ms
                2015/03/30 09:47:34.6 26.65 108.82 7 5.5 Mw
                2015/03/31 04:21:55.0 26.60 108.90 10 3.4 Ms
                 */
                strncpy(day, &(Line[20]), 10);
                day[10] = 0;
                day[4] = '/';
                day[7] = '/';

                strncpy(dayt, &(Line[31]), 11 );
                dayt[11] = 0;
                if(dayt[8] == ' ')
                {
                    dayt[8]  = '.';
                    dayt[9]  = '0';
                    dayt[10] = '0';
                }

                strncpy(MagType, &(Line[85]), 2 );
                MagType[2] = 0;
                if(MagType[1] == ' ')
                    MagType[1] = 'b';

                printf("%s %s %6.2lf %7.2lf %8.2lf %3.1lf %s\n", day, dayt, lat, lon, depth, mag, MagType );

                //Line[67] = 0;
                //printf("%s\n", Line);
                //printf("%9.4lf %9.4lf %9.4lf %9.4lf %9.4lf\n",lon0,lat0,lon,lat,dis );
                //printf("%9.4lf %9.4lf %9.4lf %9.4lf %9.4lf\n",f0,t0,f,t,dis );
            }
        }
    }
    fclose(fp);
	return 0;
}