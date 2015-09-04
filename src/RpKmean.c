#include <stdio.h>
#include <stdlib.h>
#include "libKmean.h"
#include "liberrmsg.h"
#include "libnumrec.h"
#include "libsph.h"

static char HMSG[]=
{"\
Description: Kmean for data. \n\
Usage: %-6s -M <number of groups> -N <iter_max> -D <dimension> -T <max radius> -I <filename> -O <filename> -C <filename> -S <seed>\n\
\n\
      -M   number of groups\n\
      -N   max number of iteration\n\
      -D   dimensions of the input data.\n\
              (2D corresponds to lat-lon dat)\n\
              (3D corresponds to X-Y-Z data)\n\
      -T   radius thresold\n\
      -I   data input file. It must be related to '-D dimension'\n\
      -O   data output file\n\
      -C   center data output fule\n\
      -S   seed. Default value is '-5'\n\
     [-H]  Display this message.\n\
\n\
"};


int main(int argc, char *argv[])
{
	int i;
	int   ncnt, iter_max = 10000, dimension;
	float threshold = 100.0;
	long  seed = -5L;
	char  *ifile = NULL, *ofile = NULL, *oCfile = NULL;
	int   fg_ncnt = 0, fg_iter = 0, fg_dimen = 0, fg_thre = 0;
	int   fg_ifile = 0, fg_ofile = 0, fg_oCfile = 0;

	Prj_Kmean prj;

	for(i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'M':
					sscanf(argv[i+1], "%d", &ncnt);
					fg_ncnt = 1;
					break;
				case 'N':
					sscanf(argv[i+1], "%d", &iter_max);
					fg_iter = 1;
					break;
				case 'D':
					sscanf(argv[i+1], "%d", &dimension);
					fg_dimen = 1;
					break;
				case 'T':
					sscanf(argv[i+1], "%f", &threshold);
					fg_thre = 1;
					break;
				case 'I':
					ifile = argv[i+1];
					fg_ifile = 1;
					break;
				case 'O':
					ofile = argv[i+1];
					fg_ofile = 1;
					break;
				case 'C':
					oCfile = argv[i+1];
					fg_oCfile = 1;
					break;
				case 'S':
					sscanf(argv[i+1], "%ld", &seed);
					break;
				case 'H':
					fprintf(stderr, HMSG, argv[0]);
					exit(0);
					break;
			}
		}
	}

	if(fg_ncnt != 1 || fg_iter != 1 || fg_dimen != 1 || fg_thre != 1 || fg_ifile != 1
		|| fg_ofile != 1 || fg_oCfile != 1)
	{
		perrmsg("",ERR_MORE_ARGS);
		fprintf(stderr, HMSG, argv[0]);
		exit(0);
	}


	//sscanf(argv[1], "%d", &ncnt);
	//sscanf(argv[2], "%d", &iter_max);
	//sscanf(argv[3], "%d", &dimension);
	//sscanf(argv[4], "%f", &threshold);

	iniKmean(&prj, ncnt, iter_max, dimension, threshold);
	rdDat(&prj, ifile );
	kMean(&prj, &seed);
	wrtDat(&prj, ofile);
	wrtCnt(&prj, oCfile);

	fprintf(stdout,"Iter_N: %d stderr: %f \n", prj.iter_c, (prj.value)[(prj.iter_c)-1] );
	//printf("%d %f\n", prj.worst, prj.radius[prj.worst]);
	clear(&prj);
	return 0;
}