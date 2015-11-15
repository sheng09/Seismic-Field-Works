#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liberrmsg.h"
#include <math.h>

typedef struct
{
	char  filenm[1024];
	float rp;
	float baz;
	int   tagRp;
	int   tagBaz;
	int   tag;
	void *next;
}SFile;

char HMSG[] = "\
Usage: %s -Iinfile -Rrpmin/rpmax/bazmin/bazmin -Ddrp/dbaz -Ooutfile\n\
\n\
Description: cluster according to raypara and(or) baz\n\
\n\
-Iinfile   input file has the format:\n\
           filename1 raypara1 baz1\n\
           filename2 raypara2 baz2\n\
           ...\n\
-Rrpmin/rpmax/bazmin/bazmin\n\
           range for clustering. default value is [-R30/90/0/360]\n\
-Ddrp/dbaz\n\
           increment for clustering. default value is [-D1/360]\n\
-Ooutfile  output file has the format:\n\
           >i N\n\
           filename1\n\
           filename2\n\
           ...\n\
           filenameN\n\
           >i+1 N+1\n\
           ...\n\
           ...\n\
\n\
Example:\n\
    If I want to cluster according to RAYPARA only, and increment is 1.0S/DEG.\n\
    	clusterRPBaz -Iinfile -Ooutfile -R30/90/0/360 -D1.0/360\n\
    If I want to cluster according to BAZ only, and increment is 5DEG.\n\
    	clusterRPBaz -Iinfile -Ooutfile -R30/90/0/360 -D10/5\n\
    If I want to cluster according to RAYPARA and BAZ, and increments are\n\
    0.5S/DEG and 5DEG.\n\
    	clusterRPBaz -Iinfile -Ooutfile -R30/90/0/360 -D0.5/5\n\
";

int main(int argc, char *argv[])
{
	int  i, nfile, j;
	int  fginfile = 0, fgoutfile = 0;
	char *strinfile = NULL, *stroutfile = NULL;
	char Line[1024];
	FILE *fpin, *fpout;
	SFile *data, **grpHdr, **tmp, *stmp;
	int  *ngrp;
	float rpMin  = 4.0f, rpMax  = 9.0f , rpD  = 1.0f;
	float bazMin = 0.0f , bazMax = 360.0f, bazD = 20.0f;
	int   rpN, bazN, N;
	for(i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'I':
					strinfile = &(argv[i][2]);
					fginfile  = 1;
					break;
				case 'R':
					sscanf(argv[i], "-R%f/%f/%f/%f", 
						&(rpMin), &(rpMax), &(bazMin), &(bazMax) );
					break;
				case 'O':
					stroutfile = &(argv[i][2]);
					fgoutfile = 1;
					break;
				case 'D':
					sscanf(argv[i], "-D%f/%f",
						&(rpD), &(bazD));
					break;
				case 'H':
					fprintf(stderr, HMSG, argv[0]);
					break;
				default:
					break;
			}
		}
	}
	if(fginfile != 1 || fgoutfile != 1)
	{
		fprintf(stderr, HMSG, argv[0]);
		exit(1);
	}
	rpN   = ceil( (rpMax  - rpMin ) / rpD  );
	bazN  = ceil( (bazMax - bazMin) / bazD );
	N     = rpN * bazN;

	// Allocate memory for group list header and temp
	if( NULL == ( grpHdr = ( SFile**) calloc( sizeof(SFile*), N ) ) )
	{
		perrmsg(argv[0], ERR_ALC_MEM);
		exit(1);
	}
	if( NULL == ( tmp = ( SFile **) calloc( N, sizeof(SFile *) ) ) )
	{
		perrmsg(argv[0], ERR_ALC_MEM);
		exit(1);
	}

	// ngrp is the number of members for each group
	if( NULL == ( ngrp = (int *) calloc( N, sizeof(int) ) ) )
	{
		perrmsg(argv[0], ERR_ALC_MEM);
		exit(1);
	}

	// Format of input file
	//
	// filename raypara baz
	// ...
	if( ( fpin = fopen(strinfile, "r") ) == NULL )
	{
		perrmsg(strinfile, ERR_READ_FILE);
		exit(1);
	}
	// Format of output file
	//
	// >i N
	// filename1
	// filename2
	// ...
	// filenameN
	// >i+1 N+1
	// ...
	// ...
	if( ( fpout = fopen(stroutfile, "w") ) == NULL )
	{
		perrmsg(stroutfile, ERR_OPEN_FILE);
		exit(1);
	}
	for(nfile = 0;;)
	{
		if( fgets(Line, 1024, fpin) == NULL )
			break;
		++nfile;
	}
	fseek(fpin, 0, SEEK_SET);
	if( ( data = (SFile *) calloc(nfile, sizeof(SFile)) ) == NULL )
	{
		perrmsg(argv[0], ERR_ALC_MEM);
		exit(1);
	}

	// Read data from input file and discard data whose rp or baz is out of range
	for( i = 0 ; i < nfile; ++i )
	{
		fscanf(fpin, "%s %f %f\n", data[i].filenm, &(data[i].rp), &(data[i].baz) );
		if( data[i].rp  < rpMin  || data[i].rp  > rpMax || 
			data[i].baz < bazMin || data[i].baz > bazMax )
		{
			perrmsg("", WARN_OUT_RANGE);
			fprintf(stderr, "\"%s %f %f\" discarded\n", data[i].filenm, data[i].rp, data[i].baz);
			memset(data[i].filenm, 0, 1024);
			--i;
		}
	}
	nfile = i;

	// Cluster
	for( i = 0 ; i < nfile; ++i)
	{
		data[i].tagRp  = floorf( (data[i].rp  - rpMin ) / rpD  );
		data[i].tagBaz = floorf( (data[i].baz - bazMin) / bazD );
		data[i].tag    = data[i].tagRp * bazN + data[i].tagBaz;
		++ngrp[data[i].tag];

		if(grpHdr[data[i].tag] == NULL )
		{
			grpHdr[data[i].tag] = &(data[i]);
			tmp[data[i].tag]   = &(data[i]);
		}
		else
		{
			tmp[data[i].tag]->next  = &(data[i]);
			tmp[data[i].tag]        = &(data[i]);
		}
	}

	//Output
	j = 0;
	for( i = 0; i < N; ++i)
	{
		if( grpHdr[i] != NULL)
		{
			//fprintf(fpout, ">%d %d\n", j++, i);
			fprintf(fpout, "> %d %d\n", ++j, ngrp[i]);
			//fprintf(fpout, "> %d %d\n", ++j, ngrp[i]);
			stmp = grpHdr[i];
			for(;stmp != NULL;)
			{
				//fprintf(fpout, " %04d %s %f %f\n", i, stmp->filenm, stmp->rp, stmp->baz  );
				fprintf(fpout, "%d %s\n", j, stmp->filenm);
				stmp = stmp->next;
			}
		}
	}
	fprintf(stderr, "Infile: %d NGroup: %d\n", nfile, j);
	free(ngrp);
	free(grpHdr);
	free(tmp);
	free(data);
	fclose(fpin);
	fclose(fpout);
	return 0;
}










