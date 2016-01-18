#ifndef __SAC_EV__
#define __SAC_EV__

#define MERGEMAX  50
#define MAXLENGTH 1024

#define CUT_REF_P 'P'
#define CUT_REF_O 'O'

typedef struct
{
        char  sacnm[64];
        char  stnm[8];
        char  ntwk[8];
        Time  refT;
        float b,e;
        float stla, stlo, stdp, stel;
} sacData;
typedef struct
{
        long  evcount;
        char  evnm[16];
        float evla, evlo, evdp, evel, mag;
        char  imagtype[4];
        Time  evTime;
} evData;

/*
 * Read sac files list data from a file, return a data vector and its length.
 *
 * Arguments:
 *                char filename  :
 *                long *n        :  number of records
 * Return   :
 *                sacdData*      :  pointer that points to a dat vector
 *
 * Description:
 *   Read data from a file whose format is:
 *
 ******************************************************************************************************
 *   dat/15.118.01.00.00.911E TZB TC 2015/04/28 01:00:00.020 0 3599.98 25.1750 98.4793 -12345 -12345  *
 *   dat/15.118.01.00.00.9EA1 SHZ TC 2015/04/28 01:00:00.020 0 3599.98 25.0304 98.4565 -12345 -12345  *
 *   dat/15.118.01.00.00.9F40 DYC TC 2015/04/28 01:00:00.020 0 3599.98 25.1352 98.4681 -12345 -12345  *
 *   dat/15.118.01.00.00.9F47 LJZ TC 2015/04/28 01:00:00.020 0 3599.98 25.0631 98.4624 -12345 -12345  *
 *   ...                                                                                              *
 ******************************************************************************************************
 *   Its columns are: 1.pre-filename, 2.station name,  3.network name,
 *                    4.5. reference time of sac file, 6.b, 7.e
 *                    8. stla of sac file (station),   9.stlo of sac file (station)
 *                    10.stel of sac file (station)
 *                    11.stdp of sac file (station)
 *
 *   You can generate this format file using program "saclst" supported by SAC package:
 *        "saclst kstnm knetwk kzdate kztime b e stla stlo stel stdp f *.BHZ  | \
 *          sed 's/.BHZ//g' | sort -n > sacinfo.tmp"
 *
 *   Please note that 'pre-filename' corresponding to three files *.BHN, *.BHE and *.BHZ,
 *     thus you just need to generate 'file' for *.BHZ only.
 *
 */
sacData* rdSacLst(char *filename, long *n);
//Write 'sacdat' into file, the output file format is the same as the input file of rdSacLst()
int wrtSacLst(char *filename, sacData *sacdat, const long n);

/*
 * Read events list data from a file, return a data vector and its length
 *
 * Arguments:
 *                 char *filename :
 *                 long *n        :   number of records
 *
 * Return:
 *                 evData*        :   pointer that points to a dat vector
 *
 * Description:
 *   Read data from a file whose format is:
 *
 ****************************************************************
 *   1 JP 30.0 170.0 -12345 30  2015/04/28 00:12:13.330 Ms 5.3  *
 *   2 US 35.0 165.0 -12345 100 2015/04/28 01:13:14.220 Ms 6.2  *
 *   3 US 35.0 165.0 -12345 100 2015/04/28 01:23:14.220 Ms 7.0  *
 *   ...                                                        *
 ****************************************************************
 *   Its columns are 1.Counts, 2.Event name,
 *                   3.evla,   4.evlo,
 *                   5.evel,   6.evdp
 *                   7.8. origin time of the event
 *                   9.10 magnitude
 *
 *   Help yourself to generate this format file.
 */
evData* rdEvLst(char *filename, long *n);
//Write 'evdat' into filename, the output file format is the same as the input file of rdEvLst()
int wrtEvLst(char *filename, evData *evdat, const long n);

int fdFile(evData  *evdat,  const long _evnum ,
           sacData *sacdat, const long _sacnum,
            const char cutRef, const float pre, const float suf,
            const float gcmin, const float gcmax,
            const float minMag, const float maxMag,  // Add by WangSheng 2015/12/02
            char  *sacnmPre,
            FILE *fp);


int geneSacCmd(char *outfile, const float ref_time, const float pre, const float suf,
               const float t_origin, const float t_p, const float rayp,
               FILE *fp);
#endif