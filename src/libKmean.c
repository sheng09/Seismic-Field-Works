#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "liberrmsg.h"

#include "libKmean.h"
#include "libsph.h"
#include "libnumrec.h"

int minmax(Prj_Kmean *prj)
{
	int i;
	int j;
	for(j = 0; j < prj->dimension; ++j)
	{
		(prj->min)[j] = (prj->pts)[0][j];
		(prj->max)[j] = (prj->pts)[0][j];
	}
	for(i = 0; i < prj->npts; ++i)
	{
		for(j = 0; j < prj->dimension; ++j)
		{
			if( (prj->min)[j] > (prj->pts)[i][j] )
				(prj->min)[j] = (prj->pts)[i][j];
			else if( (prj->max)[j] < (prj->pts)[i][j] )
				(prj->max)[j] = (prj->pts)[i][j];
		}
	}
	for(j = 0 ; j < (prj->dimension); ++j)
	{
		(prj->dmm)[j] = (prj->max)[j] - (prj->min)[j];
	}
	return 0;
}
int calCnt(Prj_Kmean *prj, long *seed)
{
	int i, j;
	memset( prj->cnt, 0, prj->nCnt * sizeof(XYZ) );
	memset( prj->counts, 0, prj->nCnt * sizeof(int) );


	for( i = 0; i < prj->npts; ++i)
	{
		for(j = 0; j < (prj->dimension); ++j)
		{
			(prj->cnt)[ (prj->clas)[i] ][j] += (prj->pts)[i][j];
		}
		(prj->counts)[ (prj->clas)[i] ] += 1;
	}
	for( i = 0; i < prj->nCnt; ++i)
	{
		for(j = 0; j < (prj->dimension); ++j)
		{
			if( (prj->counts)[i] > 0)
			{
				(prj->cnt)[i][j] /= ISZERO( (prj->counts)[i] );
				(prj->cnt)[i][j] += ((rnd_uni(seed) - 0.5f ) * 1.0f * (prj->radius)[i] * 0.5);
			}
			else
			{
				//printf("%d %d ", (prj->worst), (prj->nCnt) );
				//int ccc = (prj->worst);
				//printf("hello\n");
				//printf("%f \n", (prj->radius)[ ccc ] );
				//// pick a point in the worst group
				(prj->cnt)[i][j] = (rnd_uni(seed) - 0.5f ) * 1.0f * (prj->radius)[ (prj->worst) ]
							+ (prj->cnt)[ (prj->worst) ][j];
			}
			//else
			//	(prj->cnt)[i][j] = rnd_uni(seed) * (prj->dmm)[j] + (prj->min)[j];
		}
	}

	return 0;
}
int geClass(Prj_Kmean *prj)
{
	int i, j;
	float v,vmin;
	for(i = 0; i < prj->npts; ++i)
	{
		for( j = 0, vmin = distK_mean( &((prj->pts)[i]), prj->cnt, prj->dimension ) ;
		     j < (prj->nCnt); ++j)
		{
			v = distK_mean( &((prj->pts)[i]), &(prj->cnt[j]), prj->dimension );
			if( v < vmin)
			{
				vmin = v;
				(prj->clas)[i] = j;
			}
		}
	}
	return 0;
}

float calVar(Prj_Kmean *prj)
{
	memset(prj->var, 0, sizeof(float) * (prj->nCnt));
	memset(prj->radius, 0, sizeof(float) * (prj->nCnt));
	int i;
	float d;
	for(i = 0L; i < prj->npts; ++i)
	{
		d= distK_mean(&((prj->pts)[i]), &((prj->cnt)[ (prj->clas)[i] ]), prj->dimension );
		(prj->var)[ (prj->clas)[i] ] += POW(d);
		if( (prj->radius)[ (prj->clas)[i] ] < d )
			(prj->radius)[ (prj->clas)[i] ] = d;
	}
	//acqWorst(prj);
	return 0;
}
int acqWorst(Prj_Kmean *prj)
{
	int i;
	float bad = 0.0;
	int  ibad = 0L;
	for( i = 0L; i < prj->nCnt; ++i )
	{
		if(bad < (prj->var)[i] )
		{
			bad = (prj->var)[i];
			ibad = i;
		}
	}
	prj->worst = ibad;
	return ibad;
}
float calVal(Prj_Kmean *prj)
{
	(prj->value)[prj->iter_c] = 0.0f;
	int i;
	for( i = 0; i < prj->nCnt; ++i)
	{
		(prj->value)[prj->iter_c] += (prj->var)[i];
	}
	(prj->value)[prj->iter_c] /= ISZERO(prj->nCnt);
	//for(i = 0L; i < prj->npts; ++i)
	//{
	//	(prj->value)[prj->iter_c] += distK_mean(
	//		&((prj->pts)[i]), &((prj->cnt)[ (prj->clas)[i] ]), prj->dimension );
	//}
	return (prj->value)[prj->iter_c];
}
int rndCnt(Prj_Kmean *prj, long *seed )
{
	int i;
	int  j;
	int index;
	for(i = 0; i < (prj->nCnt); ++i)
	{
		for(j = 0; j < (prj->dimension); ++j)
		{
			index = ceil((rnd_uni(seed) * prj->npts) - 1);
			if(index < 0L)
				index = 0L;
			else if(index > (prj->npts) -1)
				index = (prj->npts) -1;
			//(prj->cnt)[i][j] =  rnd_uni(seed) * (prj->dmm)[j] + (prj->min)[j];
			(prj->cnt)[i][j] = (prj->pts)[index][j];
		}
	}
	return 0;
}
float distK_mean( XYZ *p1, XYZ *p2, int dimension )
{
	if(dimension == 1)
		return fabsf( (*p1)[0] - (*p2)[0] );
	else if(dimension == 3)
		return sqrt( POW((*p1)[0]-(*p2)[0]) + POW((*p1)[1]-(*p2)[1]) + POW((*p1)[2]-(*p2)[2]) );
	else if(dimension == 2)
	{
		return DisLaLo((*p1)[0], (*p1)[1], (*p2)[0], (*p2)[1]);
	}
	perrmsg("Err: dimension in distK_mean()\n   ", ERR_BAD_ARGS);
	exit(1);
}

int iniKmean(Prj_Kmean *prj, int nCnt, int iter_max, int dimension, float threshold)
{
	prj->nCnt      = nCnt;
	prj->iter_max  = iter_max;
	prj->dimension = dimension;
	prj->threshold = threshold;
	prj->cnt       = (XYZ *) calloc( prj->nCnt, sizeof(XYZ) );
	prj->counts    = (int *) calloc( prj->nCnt, sizeof(int) );
	prj->var       = (float *) calloc( prj->nCnt, sizeof(float) );
	prj->radius    = (float *) calloc( prj->nCnt, sizeof(float) );
	prj->value     = (float *) calloc( prj->iter_max, sizeof(float) );

	return 0;
}
#define LINEMAX 1024
static char Line[LINEMAX];
int rdDat(Prj_Kmean *prj, char *filename)
{
	FILE *fp;
	int i,nline = 0;
	if(NULL ==( fp = fopen(filename, "r") ))
	{
		perrmsg(filename, ERR_OPEN_FILE);
		exit(1);
	}
	for(;;)
	{
		if(NULL == fgets(Line, LINEMAX, fp) )
			break;
		++nline;
	}
	prj->npts = nline;
	prj->tags = (TAG *) calloc( nline, sizeof(TAG) );
	prj->pts  = (XYZ *) calloc( nline, sizeof(XYZ) );
	prj->clas = (int *) calloc( nline, sizeof(int) );
	fseek(fp, 0, SEEK_SET);
	for(i = 0; i < nline; ++i)
	{
		memset(Line, 0, LINEMAX);
		fgets(Line, LINEMAX, fp);
		if(prj->dimension == 1)
			sscanf(Line, "%f %s", &((prj->pts)[i][0]), (prj->tags)[i] );
		if(prj->dimension == 2)
			sscanf(Line, "%f %f %s", &((prj->pts)[i][0]), &((prj->pts)[i][1]), (prj->tags)[i]);
		if(prj->dimension == 3)
			sscanf(Line, "%f %f %f %s",
			&((prj->pts)[i][0]), &((prj->pts)[i][1]), &((prj->pts)[i][2]), (prj->tags)[i]);
	}
	fclose(fp);
	return 0;
}
int wrtCnt(Prj_Kmean *prj, char *filename)
{
	int i;
	int j;
	FILE *fp;
	if(NULL ==( fp = fopen(filename, "w") ))
	{
		perrmsg(filename, ERR_OPEN_FILE);
		exit(1);
	}
	for( i = 0; i < prj->nCnt; ++i)
	{
		if( (prj->counts)[i] > 0 )
		{
			fprintf(fp, "%5d ", i+1);
			for(j = 0; j < prj->dimension; ++j)
			{
				fprintf(fp, "%12.7f ", (prj->cnt)[i][j]);
			}
			fprintf(fp, " %5d %12.7f %12.7f\n" , (prj->counts)[i], (prj->radius)[i], (prj->var)[i]);
		}
	}
	fclose(fp);
	return 0;
}
int wrtDat(Prj_Kmean *prj, char *filename)
{
	int i;
	int j;
	FILE *fp;
	if(NULL ==( fp = fopen(filename, "w") ))
	{
		perrmsg(filename, ERR_OPEN_FILE);
		exit(1);
	}
	for( i = 0; i < prj->npts; ++i)
	{
		for(j = 0; j < prj->dimension; ++j)
		{
			fprintf(fp, "%f ", (prj->pts)[i][j]);
		}
		fprintf(fp, " %s %d\n", (prj->tags)[i], (prj->clas)[i] + 1 );
	}
	fclose(fp);
	return 0;
}
int clear(Prj_Kmean *prj)
{
	free(prj->pts);
	free(prj->clas);
	free(prj->cnt);
	free(prj->counts);
	free(prj->value);
	return 0;
}
int kMean(Prj_Kmean *prj, long *seed)
{
	float v;
	prj->iter_c = 0;
	//1. Initilization to get random center
	minmax( prj );
	rndCnt( prj, seed );
	//2. The 1st time to kMean points
	geClass( prj );
	//
	calVar(prj);
	acqWorst(prj);
	v = calVal(prj);
	(prj->value)[ (prj->iter_c)++ ] = v;
	for( 	;
		(prj->iter_c) < (prj->iter_max) &&   (prj->radius)[prj->worst] > prj->threshold ;
		++(prj->iter_c) )
	{
		//printf("\n%d %f--------\n", (prj->iter_c), v);
		//for( i = 0; i < 6; ++i)
		//{
		//	printf("%d %f\n",(prj->clas)[i], (prj->pts)[i][0] );
		//}
		calCnt(prj, seed);
		geClass(prj);
		calVar(prj);
		acqWorst(prj);
		v = calVal(prj);
		//printf("-----------------------\n");
		//for( i = 0; i < 3; ++i)
		//{
		//	printf("%d %f %f\n", i, (prj->cnt)[i][0], (prj->cnt)[i][1] );
		//}
		(prj->value)[ prj->iter_c ] = v;
	}
	calCnt(prj, seed);
	calVar(prj);
	return 0;
}