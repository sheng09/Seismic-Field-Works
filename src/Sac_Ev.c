#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtime.h"
#include "libsph.h"
#include "libctau.h"
#include "liberrmsg.h"

#include "Sac_Ev.h"

static char Line[MAXLENGTH];
sacData* rdSacLst(char *filename, long *n)
{
        FILE *fp;
        //char Line[MAXLENGTH];
        long nSac = 0;
        int  i;
        sacData *sacdat;
        if ( NULL == ( fp = fopen(filename, "r") ) )
        {
            perrmsg( filename, ERR_OPEN_FILE);
            exit(1);
        }
        for(;;)
        {
                if( NULL == fgets(Line, MAXLENGTH, fp))
                        break;
                ++nSac;
        }
        if( NULL == ( sacdat = (sacData *) calloc( nSac, sizeof(sacData) ) ) )
        {
                perrmsg("rdSacLst()", ERR_ALC_MEM);
                exit(1);
        }
        fseek(fp, 0, SEEK_SET);
        for(i = 0; i < nSac; ++i)
        {
                memset(Line, 0, MAXLENGTH);

                if( NULL == ( fgets(Line, MAXLENGTH, fp) ) )
                {
                        perrmsg(filename, ERR_READ_FILE);
                        exit(1);
                }
                //dat/15.118.01.00.00.911E TZB TC 2015/04/28 01:00:00.020 
                //0 3599.98  25.175 98.4793 -12345 -12345
                sscanf(Line, "%s %s %s %s %s %f %f %f %f %f %f",
                              sacdat[i].sacnm,
                              sacdat[i].stnm,
                              sacdat[i].ntwk,
                              sacdat[i].refT.KDATE,
                              sacdat[i].refT.KTIME,
                              &(sacdat[i].b), &(sacdat[i].e), &(sacdat[i].stla), &(sacdat[i].stlo),
                              &(sacdat[i].stdp), &(sacdat[i].stel) );
                rtime( &(sacdat[i].refT) );
        }
        fclose(fp);
        *n = nSac;
        return sacdat;
}

int wrtSacLst(char *filename, sacData *sacdat, const long n)
{
        int i = 0;
        FILE *fp;
        if ( NULL == ( fp = fopen(filename, "w") ) )
        {
                perrmsg(filename, ERR_OPEN_FILE);
                exit(1);
        }
        for(i = 0; i < n; ++i)
        {
                fprintf(fp, "%s %s %s %s %s %f %f %f %f %f %f\n",
                              sacdat[i].sacnm,
                              sacdat[i].stnm,
                              sacdat[i].ntwk,
                              sacdat[i].refT.KDATE,
                              sacdat[i].refT.KTIME,
                              (sacdat[i].b), (sacdat[i].e), (sacdat[i].stla), (sacdat[i].stlo),
                              (sacdat[i].stdp), (sacdat[i].stel) );
        }
        fclose(fp);
        return 0;
}

evData* rdEvLst(char *filename, long *n)
{
        FILE *fp;
        //char Line[MAXLENGTH];
        long nEv = 0;
        int i;
        evData *evdat;
        if ( NULL == ( fp = fopen(filename, "r")) )
        {
            perrmsg(filename, ERR_OPEN_FILE);
            exit(1);
        }
        for(;;)
        {
                if( NULL == fgets(Line, MAXLENGTH, fp))
                        break;
                ++nEv;
        }
        if(NULL == ( evdat = (evData *) calloc( nEv, sizeof(sacData) ) ) )
        {
                perrmsg("rdEvLst()",ERR_ALC_MEM);
                exit(1);
        }
        fseek(fp, 0, SEEK_SET);
        for(i = 0; i < nEv; ++i)
        {
                memset(Line, 0, MAXLENGTH);
                if(NULL == fgets(Line, MAXLENGTH, fp))
                {
                        perrmsg(filename, ERR_READ_FILE);
                        exit(1);
                }
                //Counts Name   La    Lo    el     dp     KJD      KTIME           im m
                //1       JP     30    170   -12345 30     2015/04/28 00:12:13.330 Ms 5.3
                sscanf(Line,"%ld %s %f %f %f %f %s %s %s %f",
                        &(evdat[i].evcount), evdat[i].evnm,
                        &(evdat[i].evla), &(evdat[i].evlo), &(evdat[i].evel), &(evdat[i].evdp), 
                        evdat[i].evTime.KDATE, evdat[i].evTime.KTIME,
                        evdat[i].imagtype, &(evdat[i].mag) );
                rtime( &(evdat[i].evTime) );
        }
        fclose(fp);
        *n = nEv;
        return evdat;
}

int wrtEvLst(char *filename, evData *evdat, const long n)
{
        int i = 0;
        FILE *fp;
        if ( NULL == ( fp = fopen(filename, "w") ) )
        {
                perrmsg(filename, ERR_OPEN_FILE);
                exit(1);
        }
        for(i = 0; i < n; ++i)
        {
                fprintf(fp, "%ld %s %f %f %f %f %s %s %s %f\n",
                        evdat[i].evcount, evdat[i].evnm,
                        (evdat[i].evla), (evdat[i].evlo), (evdat[i].evel), (evdat[i].evdp),
                        evdat[i].evTime.KDATE, evdat[i].evTime.KTIME,
                        evdat[i].imagtype, evdat[i].mag );
        }
        fclose(fp);
        return 0;
}


static int       ISHead, ISTail;
static sacData   *lst[MERGEMAX];
static long      nlst;
static evData    *ev;

int fdFile(evData  *evdat,  const long _evnum ,
           sacData *sacdat, const long _sacnum,
            const float pre, const float suf,
            const float gcmin, const float gcmax,
            FILE *fp)
{

        int i,j;
        //sacData *sacdat = _sacdat;
        //evData  *evdat  = _evdat;

        //sacData **lst = (sacData **) calloc(MERGEMAX, sizeof(*sacData));
        int   found;
        float delta,trvt, p, dtES, dtES_head;
        float *t, *rp, *dtdh, *dddp, *mn, *ts, *toa;
        int   nph;
        char  (*phnm)[9];

        //char Line[MAXLENGTH];
        for(j = 0; j < _evnum; ++j)
        {
                delta = DisLaLo(evdat[j].evla, evdat[j].evlo, sacdat[0].stla, sacdat[0].stlo);
                if( gcmin > delta || gcmax < delta ) // Out of range
                        continue;
                ctau("P   ", evdat[j].evdp, delta, &t, &rp, &dtdh, &dddp, &mn, &ts, &toa, &nph, &phnm);
                trvt = t[0];
                p    = rp[0];

                //printf("%f %f %f %f :", evdat[j].evla, evdat[j].evlo, sacdat[0].stla, sacdat[0].stlo);
                //printf("%f %f %f\n", delta,  evdat[j].evdp, trvt );
                found = 0; // 0: Not found 1: Found
                nlst = 0L; // Number of sac files intersect with the time interval

                // Find the sac file that totally cover the time interval I want
                for( i = 0; i < _sacnum; ++i )
                {
                        dtES = dt( &(sacdat[i].refT), &(evdat[j].evTime) );
                        if( (dtES + sacdat[i].b) <= (trvt + pre) &&
                            (dtES + sacdat[i].e) >= (trvt + suf)    )
                            //                              B  SAC FILE     E
                            // ----------------O------------|********A******|---------   original trace
                            //                 |<--     trvt      -->|
                            // ---------------------------------|********|------------   trace I want
                            //                              trvt+pre    trvt+suf
                        {
                                lst[nlst] = &( sacdat[i] );
                                nlst  = 1L;
                                found = 1;
                                dtES_head = dtES;
                                //Add by wangsheng 2015/09/18
                                //For debug
                                #ifdef DEBUG
                                    fprintf(fp, "# Just one: %f %f %f %f\n", delta, evdat[j].evdp, trvt, dtES);
                                #endif
                                ////////////////////
                                break;
                        }
                        /*{
                                // [trvt+pre, trvt+suf] is included in [b,e]
                                found = 1;
                                //For BHZ
                                fprintf(fp, "r %s.BHZ\n", sacdat[i].sacnm );
                                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                                    -dtES ,p, evdat[j].evla, evdat[j].evlo, evdat[j].evdp, evdat[j].evel, trvt - dtES);
                                fprintf(fp, "w %s.BHZ \n",Line);
                                //For BHN
                                fprintf(fp, "r %s.BHN\n", sacdat[i].sacnm );
                                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                                    -dtES ,p, evdat[j].evla, evdat[j].evlo, evdat[j].evdp, evdat[j].evel, trvt - dtES);
                                fprintf(fp, "w %s.BHN \n",Line);
                                //For BHE
                                fprintf(fp, "r %s.BHE\n", sacdat[i].sacnm );
                                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                                    -dtES ,p, evdat[j].evla, evdat[j].evlo, evdat[j].evdp, evdat[j].evel, trvt - dtES);
                                fprintf(fp, "w %s.BHE \n",Line);

                                //Cut
                                fprintf(fp, "cut t0 %f %f\n", pre,  suf);
                                fprintf(fp, "r %s.BH? \n",Line );
                                fprintf(fp, "w over \n" );
                                fprintf(fp, "cut off \n");
                                break;
                        }*/
                }

                // Find all the sac files that intersect with the time interval I want
                if( found != 1)
                {
                        ISHead = 0;
                        ISTail = 0;
                        for( i = 0; i < _sacnum; ++i)
                        {
                                dtES = dt( &(sacdat[i].refT), &(evdat[j].evTime) );
                                /*
                                if(
                                     (
                                        //                              B   SAC FILE    E
                                        // ----------------O------------|************A**|---------
                                        //                 |<--          trvt        -->|
                                        // ---------------------------------------|**********|----
                                        //                                    trvt+pre    trvt+suf
                                        ((trvt + pre) <= (dtES + sacdat[i].e))
                                        &&
                                        ((trvt + suf) >= (dtES + sacdat[i].e))
                                     )
                                     ||
                                     (
                                        //                              B  SAC FILE     E
                                        // ----------------O------------|**A************|---------
                                        //                 |<--   trvt  -->|
                                        // -------------------------|**********|------------------
                                        //                       trvt+pre    trvt+suf
                                        ((trvt + pre) <= (dtES + sacdat[i].b))
                                        &&
                                        ((trvt + suf) >= (dtES + sacdat[i].b))
                                     )
                                     ||
                                     (
                                        //                              B  SAC FILE     E
                                        // ----------------O------------|********A******|---------
                                        //                 |<--     trvt      -->|
                                        // -------------------------|************************|----
                                        //                        trvt+pre                trvt+suf
                                        ((trvt + pre) <= (dtES + sacdat[i].b))
                                        &&
                                        ((trvt + suf) >= (dtES + sacdat[i].e))
                                     )
                                  )*/
                                //Revised by wangsheng 2015/09/18
                                if(
                                            //                              B   SAC FILE    E
                                            // ----------------O------------|************A**|---------
                                            //                 |<--          trvt        -->|
                                            // ---------------------------------------|**********|----
                                            //                                    trvt+pre    trvt+suf
                                            ((trvt + pre) <= (dtES + sacdat[i].e))
                                            &&
                                            ((trvt + suf) >= (dtES + sacdat[i].e))
                                   )
                                {
                                        dtES_head = dtES;
                                        ISHead = 1;
                                        lst[nlst] = &( sacdat[i] );
                                        ++nlst;
                                }
                                else if(
                                            //                              B  SAC FILE     E
                                            // ----------------O------------|**A************|---------
                                            //                 |<--   trvt  -->|
                                            // -------------------------|**********|------------------
                                            //                       trvt+pre    trvt+suf
                                            ((trvt + pre) <= (dtES + sacdat[i].b))
                                            &&
                                            ((trvt + suf) >= (dtES + sacdat[i].b))
                                        )
                                {
                                        ISTail = 1;
                                        lst[nlst] = &( sacdat[i] );
                                        ++nlst;
                                }
                                else if(
                                            //                              B  SAC FILE     E
                                            // ----------------O------------|********A******|---------
                                            //                 |<--     trvt      -->|
                                            // -------------------------|************************|----
                                            //                        trvt+pre                trvt+suf
                                            ((trvt + pre) <= (dtES + sacdat[i].b))
                                            &&
                                            ((trvt + suf) >= (dtES + sacdat[i].e))
                                        )
                                {
                                        lst[nlst] = &( sacdat[i] );
                                        ++nlst;
                                }
                        }
                        if( ISHead != 1 || ISTail != 1) // The head and tail of the trace I want must be covered by some sacfiles
                                continue;
                        //Add by wangsheng 2015/09/18
                        //For debug
                        #ifdef DEBUG
                        fprintf(fp, "# %d %d\n", ISHead, ISTail);
                        fprintf(fp, "# More than one: %d %f %f %f %f\n", nlst, delta, evdat[j].evdp, trvt, dtES);
                        #endif
                }

                if( nlst > 0L)
                {
                        memset(Line, 0, MAXLENGTH);
                        sprintf(Line, "._out/EV%05ld", evdat[j].evcount);
                        ev = &(evdat[j]);

                        //Add by wangsheng 2015/09/18
                        //For Debug
                        #ifdef DEBUG
                        fprintf(fp, "# %s\n", lst[0]->sacnm);
                        fprintf(fp, "# SACTIME: %04d/%02d/%02d %02d:%02d:%02d.%03d\n", lst[0]->refT.year, lst[0]->refT.mon, lst[0]->refT.day, 
                                                       lst[0]->refT.hour, lst[0]->refT.min, lst[0]->refT.sec, lst[0]->refT.msec);
                        fprintf(fp, "# EVTIME: %04d/%02d/%02d %02d:%02d:%02d.%03d\n", ev->evTime.year, ev->evTime.mon, ev->evTime.day,
                                                       ev->evTime.hour, ev->evTime.min, ev->evTime.sec, ev->evTime.msec);
                        #endif
                        /////////////////
                        geneSacCmd(Line, trvt - dtES_head, dtES_head, p,
                                   pre, suf,
                                   fp );
                        //printf("%f %f %f\n", trvt , dtES_head, trvt - dtES_head );
                }
                /*{
                        //For BHZ
                        fprintf(fp, "r ");
                        for( i = 0; i < nlst; ++i)
                        {
                                fprintf(fp, "%s.BHZ ", lst[i]->sacnm );
                        }
                        fprintf(fp, "\nch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                            -dtES ,p, evdat[j].evla, evdat[j].evlo, evdat[j].evdp, evdat[j].evel, trvt - dtES);
                        fprintf(fp, "w %s.BHZ \n",Line);
                        //For BHN
                        fprintf(fp, "r ");
                        for( i = 0; i < nlst; ++i)
                        {
                                fprintf(fp, "%s.BHN ", lst[i]->sacnm );
                        }
                        fprintf(fp, "\nch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                            -dtES ,p, evdat[j].evla, evdat[j].evlo, evdat[j].evdp, evdat[j].evel, trvt - dtES);
                        fprintf(fp, "w %s.BHN \n",Line);
                        //For BHE
                        fprintf(fp, "r ");
                        for( i = 0; i < nlst; ++i)
                        {
                                fprintf(fp, "%s.BHE ", lst[i]->sacnm );
                        }
                        fprintf(fp, "\nch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                            -dtES ,p, evdat[j].evla, evdat[j].evlo, evdat[j].evdp, evdat[j].evel, trvt - dtES);
                        fprintf(fp, "w %s.BHE \n",Line);
                        //Cut
                        fprintf(fp, "cut t0 %f %f\n", pre,  suf);
                        fprintf(fp, "r %s.BH? \n",Line );
                        fprintf(fp, "w over \n" );
                        fprintf(fp, "cut off \n");
                }*/

        }
        return 1;
}

int geneSacCmd(char *outfile, const float t0, const float dtES, const float p,
               const float pre, const float suf,
               FILE *fp)
{
    (ev->evnm)[15] = 0;
        int i;
        if(nlst == 1L)
        {
                //For BHZ
                fprintf(fp, "r %s.BHZ\n", (*lst)->sacnm );
                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                    -dtES ,p, ev->evla, ev->evlo, ev->evdp, ev->evel, t0);
                fprintf(fp, "w %s.BHZ \n",outfile);
                //For BHN
                fprintf(fp, "r %s.BHN\n", (*lst)->sacnm );
                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                    -dtES ,p, ev->evla, ev->evlo, ev->evdp, ev->evel, t0);
                fprintf(fp, "w %s.BHN \n",outfile);
                //For BHE
                fprintf(fp, "r %s.BHE\n", (*lst)->sacnm );
                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                    -dtES ,p, ev->evla, ev->evlo, ev->evdp, ev->evel, t0);
                fprintf(fp, "w %s.BHE \n",outfile);

                //Cut
                fprintf(fp, "cut t0 %f %f\n", pre,  suf);
                //fprintf(fp, "%s\n", );
                fprintf(fp, "r %s.BH? \n",outfile );
                //Add by wangsheng 2015/09/01
                //fprintf(fp, "ch allt (0 - &1,o&) IZTYPE IO kevnm %ld mag %f\n", ev->evcount, ev->mag);
                fprintf(fp, "ch allt (0 - &1,o&) IZTYPE IO kevnm %15s mag %f\n", ev->evnm, ev->mag);
                //Add by wangsheng 2015/11/08
                fprintf(fp, "rmean;rtr;rmean;taper\n");
                //
                fprintf(fp, "w over \n" );
                fprintf(fp, "cut off \n");
                return 0;
        }
        else if(nlst > 1L)
        {
                //For BHZ
                fprintf(fp, "r ");
                for( i = 0; i < nlst; ++i)
                {
                        fprintf(fp, "%s.BHZ ", lst[i]->sacnm );
                }
                fprintf(fp, "\nmerge\n");
                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                    -dtES ,p, ev->evla, ev->evlo, ev->evdp, ev->evel, t0);
                fprintf(fp, "w %s.BHZ \n",outfile);
                //For BHN
                fprintf(fp, "r ");
                for( i = 0; i < nlst; ++i)
                {
                        fprintf(fp, "%s.BHN ", lst[i]->sacnm );
                }
                fprintf(fp, "\nmerge\n");
                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                    -dtES ,p, ev->evla, ev->evlo, ev->evdp, ev->evel, t0);
                fprintf(fp, "w %s.BHN \n",outfile);
                //For BHE
                fprintf(fp, "r ");
                for( i = 0; i < nlst; ++i)
                {
                        fprintf(fp, "%s.BHE ", lst[i]->sacnm );
                }
                fprintf(fp, "\nmerge\n");
                fprintf(fp, "ch o %f user0 %f evla %f evlo %f evdp %f evel %f t0 %f\n", 
                    -dtES ,p, ev->evla, ev->evlo, ev->evdp, ev->evel, t0);
                fprintf(fp, "w %s.BHE \n",outfile);
                //Cut
                fprintf(fp, "cut t0 %f %f\n", pre,  suf);
                fprintf(fp, "r %s.BH? \n",outfile );
                //Add by wangsheng 2015/09/01
                //fprintf(fp, "ch allt (0 - &1,o&) IZTYPE IO kevnm %ld mag %f\n", ev->evcount, ev->mag);
                fprintf(fp, "ch allt (0 - &1,o&) IZTYPE IO kevnm %15s mag %f\n", ev->evnm, ev->mag);
                //Add by wangsheng 2015/11/08
                fprintf(fp, "rmean;rtr;rmean;taper\n");
                //
                fprintf(fp, "w over \n" );
                fprintf(fp, "cut off \n");
                return 0;
        }
        return 1;
}