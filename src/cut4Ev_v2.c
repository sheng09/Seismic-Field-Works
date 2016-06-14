#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtime.h"
#include "libsph.h"
#include "libctau.h"
#include "liberrmsg.h"

#include "Sac_Ev.h"
static char HMSG[]=
{"\
Description: Generate SAC command for selecting and cutting sac file.\n\
Usage: %-6s -C[O|P]/t1/t2 -Gdeg1/deg2 [-Mmin/max] -Eevent.list -Dsacinfo [-Pprefilename] [-Ttailname] [-V]\n\
\n\
(-C [O|P|T]/t1/t2)    : window for cutting data \n\
           -CO/t1/t2  :  [O + t1, O + t2] \n\
           -CT/t1/t2  :  [refT + t1, refT + t2] \n\
                          refT(refercen Time) should be given within event.list.\n\
           -CP/t1/t2  :  [Parrival + t1, Parrival + t2] \n\
                          Parrival is theoretical traveltime, which may be wrong\n\
                          for some kind of event-station distance.\n\
(-G deg1/deg2)  : windows data within great circle distance [deg1, deg2]\n\
[-M min/max]    : magnitude interval\n\
                  Defualt is select all magnitude.\n\
(-E event.list) : events list file\n\
(-D sacinfo)    : sac files info list file\n\
[-P Prefilename]: pre for output sac files' name. \n\
                  No more than 7 characters!\n\
                  Characters must be [A-z,a-z,_]!\n\
[-T tailname]   : tailname specify which kind of filename extentsion is used. \n\
                  \"-TBH\" for BH[NZE]\n\
                  \"-TSH\" for SH[NZE]\n\
[-V]            : open verbose\n\
[-H] Display this message.\n\
\n\
This program is used to generate 'sacCMD.sh' bash script for select and cut sac files.\n\
Input arguments include 'event.list', which contains lists of events,and 'sacinfo', which\n\
  contaions information of your sac files database.\n\
\n\
Below are introduction of the format of 'event.list' and 'sacinfo'.\n\
event.list file format is:\n\
  ********************************************************\n\
     1 JP 30.0 170.0 -12345 30  2015/04/28 00:12:13.330  Ms 5.3\n\
     2 US 35.0 165.0 -12345 100 2015/04/28 01:13:14.220  Ms 6.2\n\
     3 US 35.0 165.0 -12345 100 2015/04/28 01:23:14.220  Ms 7.0\n\
     ...\n\
  ********************************************************\n\
  Its columns are 1.Counts, 2.Event name, 3.evla,   4.evlo, 5.evel,   6.evdp\n\
                  7.8. origin time of the event,  9.10. magnitude and its type\n\
  Help yourself to generate this format file.\n\
\n\
sacinfo file format is:\n\
  ****************************************************************************************************\n\
    dat/15.118.01.00.00.911E TZB TC 2015/04/28 01:00:00.020 0 3599.98 25.1750 98.4793 -12345 -12345\n\
    dat/15.118.01.00.00.9EA1 SHZ TC 2015/04/28 01:00:00.020 0 3599.98 25.0304 98.4565 -12345 -12345\n\
    dat/15.118.01.00.00.9F40 DYC TC 2015/04/28 01:00:00.020 0 3599.98 25.1352 98.4681 -12345 -12345\n\
    dat/15.118.01.00.00.9F47 LJZ TC 2015/04/28 01:00:00.020 0 3599.98 25.0631 98.4624 -12345 -12345\n\
    ...\n\
  ****************************************************************************************************\n\
  Its columns are: 1.pre-filename, 2.station name,  3.network name,\n\
                   4.5. reference time of sac file, 6.b, 7.e\n\
                   8. stla of sac file (station),   9.stlo of sac file (station)\n\
                   10.stel of sac file (station)    11.stdp of sac file (station)\n\
  You can generate this format file using program \"saclst\" supported by SAC package:\n\
    \"saclst kstnm knetwk kzdate kztime b e stla stlo stel stdp f *.BHZ  \\|\n\
    sed 's/.BHZ//g' | sort -n > sacinfo.tmp\"\n\
  Please note that 'pre-filename' corresponding to three files *.BHN, *.BHE and *.BHZ,\n\
    thus you just need to generate 'file' for *.BHZ only.\n\
\n\
Example:\n\
  cut4Ev -C P/-10/60 -G 30/90 -M 5.5/6.0 -E event.list -D sacinfo -P ISC -V\n\
\n\
"};
int main(int argc, char *argv[])
{
    FILE *fp;
    char *strEv = NULL, *strSac = NULL;
    int   fgEv = 0, fgSac = 0, fggc = 0, fgmag = 0;
    int   i;
    char  sacCMD[32]="sacCMD.sh";
    sacData *sacdat;
    evData  *evdat;
    long nsac, nev;
    float pre, suf;
    char  cutRef = 'P';
    float gcmin = -180.0f, gcmax = 720.0f;
    float minMag = -1.0, maxMag = 10.0;
    int   fgpre_suf = 0;
    int   fgverbose = 0;
    char  sacnmPre[128] = "EV";

    DAT_TYPE datType;

    //Point pe, ps;
    for(i = 1; i < argc ; ++i)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'E':
                        strEv = &(argv[i][2]);
                        fgEv = 1;
                        break;
                case 'D':
                        strSac = &(argv[i][2]);
                        fgSac = 1;
                        break;
                case 'C':
                        sscanf( &(argv[i][2]), "%c/%f/%f", &cutRef, &pre, &suf);
                        fgpre_suf = 1;
                        break;
                case 'G':
                        sscanf( &(argv[i][2]), "%f/%f", &gcmin, &gcmax);
                        fggc = 1;
                        break;
                case 'M': // Add -M option by WangSheng 2015/12/02
                        sscanf( &(argv[i][2]),"%f/%f", &minMag, &maxMag);
                        fgmag = 1;
                        break;
                case 'P': //Add -P option by WangSheng 2015/12/02
                        strcpy(sacnmPre, &(argv[i][2]));
                        break;
                case 'T': //Add -T option by WangSheng 2016/06/01
                        if( strcmp( &(argv[i][2]), "BH" ) == 0)
                            datType = BH_SUFFIX;
                        else if( strcmp( &(argv[i][2]), "SH" ) == 0)
                            datType = SH_SUFFIX;
                        //printf("%d \n", datType);
                        break;
                case 'O':
                        strcpy(sacCMD, &(argv[i][2]) );
                        break;
                case 'V':
                        fgverbose = 1;
                        break;
                case 'H':
                        fprintf(stderr, HMSG, argv[0]);
                        exit(0);
                        break;
            }
        }
    }

    if(fgEv != 1 || fgSac != 1 || fgpre_suf != 1)
    {
        perrmsg("",ERR_MORE_ARGS);
        //printf("%s\n", );
        fprintf(stderr, HMSG, argv[0]);
        exit(1);
    }
    if(cutRef != 'P' && cutRef != 'O' && cutRef != 'T')
    {
        perrmsg("",ERR_BAD_ARGS);
        fprintf(stderr, "\' -C%c\'\n", cutRef);
        exit(1);
    }

    // Add -M option by WangSheng 2015/12/02
    if(minMag > maxMag || maxMag < 0.0 || minMag > 10.0)
    {
        perrmsg("",ERR_BAD_ARGS);
        fprintf(stderr, "\' -M %f/%f \'\n", minMag, maxMag);
        exit(1);
    }

    //Add -P option by WangSheng 2015/12/02
    sacnmPre[127] = 0;
    //for(i = 6; i >=0 ;--i)
    //{
    //    if( ! ( ( sacnmPre[i] < 'z' && sacnmPre > 'a' ) || ( sacnmPre[i] < 'Z' && sacnmPre > 'A' ) ) )
    //    {
    //        sacnmPre[i] = '_';
    //    }
    //}
    //
    printf("Initialization...\n");
    sacdat = rdSacLst(strSac, &nsac);
    evdat  = rdEvLst(strEv, &nev);

    //wrtSacLst("ws_sac", sacdat, nsac);
    //wrtEvLst("ws_ev", evdat, nev);

    if( NULL == ( fp = fopen(sacCMD,"w") ) )
    {
        perrmsg(sacCMD,ERR_OPEN_FILE);
        exit(1);
    }
    fprintf(fp, "#!/bin/bash\n" );
    //Add by wangsheng 2015/09/02
    fprintf(fp, "mkdir _cut4Ev_out 2>&- || rm _cut4Ev_out/* -rf 2>&-\n");
    //
    if(fgverbose == 1)
        fprintf(fp, "sac << EOF\n" );
    else if(fgverbose == 0)
        fprintf(fp, "sac >/dev/null << EOF\n" );
    fdFile( evdat, nev, sacdat, nsac, cutRef, pre, suf, gcmin, gcmax , minMag, maxMag, sacnmPre, datType, fp);
    fprintf(fp, "q\nEOF\n" );

    printf("Finished!\n\n");
    printf("Please run the script of \"sacCMD.sh\"\n");
    printf("Output data is with the directory of \"_cut4Ev_out\"\n\n");
    free(sacdat);
    free(evdat);
    return 0;
}