#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI (4.0 * atanf(1.0))
#define RAD 6371.0

static pre[2][12] = {{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
                     {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}};

typedef struct
{
        long  count;
        char  evnm[8];
        int   nzyear, nzmon, nzday, nzjday;
        int   nzhour, nzmin, nzsec, nzmsec;
        float evla, evlo, evdp;
        float trvt, rayp;
}EVLIST;

typedef struct
{
        long count;
        char filename[128];
        int   nzyear, nzmon, nzday, nzjday;
        int   nzhour, nzmin, nzsec, nzmsec;
        float b,e;
}SACLIST;
EVLIST* read_evlist(char *filename, long *_num);
SACLIST* read_saclist(char *filename, long *_num);
float dt(EVLIST *ev, SACLIST *sac);
void fun(SACLIST *head, SACLIST *tail, EVLIST *ev, float pre, float suf, FILE *fp);
void GenerateCMD(EVLIST *_evlst, long _evnum , SACLIST *_saclst, long _sacnum,
                 float pre, float suf,
                 FILE *fp);
/*
int main(int argc, char const *argv[])
{
    SACLIST sac;
    EVLIST  ev;
    sac.nzyear = 2017;
    sac.nzmon  = 01;
    sac.nzday  = 01;
    sac.nzhour = 00;
    sac.nzmin  = 00;
    sac.nzsec  = 00;
    sac.nzmsec = 00;
    if(  sac.nzyear % 4 != 0 ||
        (sac.nzyear % 100 == 0 && sac.nzyear % 400 != 0)
      )
            sac.nzjday = sac.nzday + pre[0][ sac.nzmon -1 ];
    else
            sac.nzjday = sac.nzday + pre[1][ sac.nzmon -1 ];

    ev.nzyear = 2015;
    ev.nzmon  = 01;
    ev.nzday  = 01;
    ev.nzhour = 00;
    ev.nzmin  = 00;
    ev.nzsec  = 00;
    ev.nzmsec = 00;
    if(  ev.nzyear % 4 != 0 ||
        (ev.nzyear % 100 == 0 && ev.nzyear % 400 != 0)
      )
            ev.nzjday = ev.nzday + pre[0][ ev.nzmon -1 ];
    else
            ev.nzjday = ev.nzday + pre[1][ ev.nzmon -1 ];

    printf("%f\n", dt(&ev, &sac) );
    return 0;
}
*/
EVLIST* read_evlist(char *filename, long *_num)
{
        char Line[1024];
        int  i;
        long num=0;
        FILE *fp;
        float D2KM;

        fp = fopen(filename,"r");
        for(num = 0; fgets(Line, 1024, fp) != NULL; ++num)
                ;
        fseek(fp, 0, SEEK_SET);
        EVLIST *evlst;
        evlst = (EVLIST *) calloc(num, sizeof(EVLIST));

        D2KM = 180.0/PI/RAD;
        for(i = 0; i < num; ++i)
        {
                memset(Line, 0, 1024);
                fgets(Line, 1024, fp);
                //d s  d    d  d  d  d  d  d   f      f       f       f
                //1 JP 2015/04/28 01:00:00.012 60.320 120.123 100.000 102.181
                sscanf(Line, "%ld %s %d/%d/%d %d:%d:%d.%d %f %f %f %f %f",
                        &(evlst[i].count),
                         (evlst[i].evnm),
                        &(evlst[i].nzyear),
                        &(evlst[i].nzmon),
                        &(evlst[i].nzday),
                        &(evlst[i].nzhour),
                        &(evlst[i].nzmin),
                        &(evlst[i].nzsec),
                        &(evlst[i].nzmsec),
                        &(evlst[i].evla),
                        &(evlst[i].evlo),
                        &(evlst[i].evdp),
                        &(evlst[i].trvt),
                        &(evlst[i].rayp)      );
                evlst[i].rayp *= D2KM;
                if(  evlst[i].nzyear % 4 != 0 ||
                    (evlst[i].nzyear % 100 == 0 && evlst[i].nzyear % 400 != 0)
                  )
                        evlst[i].nzjday = evlst[i].nzday + pre[0][ evlst[i].nzmon -1 ];
                else
                        evlst[i].nzjday = evlst[i].nzday + pre[1][ evlst[i].nzmon -1 ];
        }
        fclose(fp);
        *_num = num;
        return evlst;
}

SACLIST* read_saclist(char *filename, long *_num)
{
        char Line[1024];
        int  i;
        long num=0;
        FILE *fp;
        fp = fopen(filename,"r");
        for(num = 0; fgets(Line, 1024, fp) != NULL; ++num)
                ;
        fseek(fp, 0, SEEK_SET);
        SACLIST *saclst;
        saclst = (SACLIST *) calloc(num, sizeof(SACLIST));
        for( i = 0; i < num; ++i)
        {
                //d  s
                //71 dat/15.118.23.00.00.911E.2.sac 2015/04/28 23:00:00.020 0 3599.98
                memset(Line, 0, 1024);
                fgets(Line, 1024, fp);
                sscanf(Line, "%ld %s %d/%d/%d %d:%d:%d.%d %f %f",
                        &(saclst[i].count),
                         (saclst[i].filename),
                        &(saclst[i].nzyear),
                        &(saclst[i].nzmon),
                        &(saclst[i].nzday),
                        &(saclst[i].nzhour),
                        &(saclst[i].nzmin),
                        &(saclst[i].nzsec),
                        &(saclst[i].nzmsec),
                        &(saclst[i].b),
                        &(saclst[i].e)
                        );
                if(  saclst[i].nzyear % 4 != 0 ||
                    (saclst[i].nzyear % 100 == 0 && saclst[i].nzyear % 400 != 0)
                  )
                        saclst[i].nzjday = saclst[i].nzday + pre[0][ saclst[i].nzmon -1 ];
                else
                        saclst[i].nzjday = saclst[i].nzday + pre[1][ saclst[i].nzmon -1 ];
        }
        fclose(fp);
        *_num = num;
        return saclst;
}
float dt(EVLIST *ev, SACLIST *sac)
{
        double a, b;
        double dt = 0.0;
        int i;
        if( ev->nzyear > sac->nzyear )
        {
                for( i = sac->nzyear ; i < ev->nzyear ; ++i )
                {
                        if(i % 4 != 0 || ( i % 100 == 0 && i % 400 != 0 ) )
                                dt += 365.0 * 24.0 * 3600.0;
                        else
                                dt += 366.0 * 24.0 * 3600.0;
                }
        }
        else if( ev->nzyear < sac->nzyear )
        {
                for( i = ev->nzyear ; i < sac->nzyear ; ++i )
                {
                        if(i % 4 != 0 || ( i % 100 == 0 && i % 400 != 0 ) )
                                dt -= 365.0 * 24.0 * 3600.0;
                        else
                                dt -= 366.0 * 24.0 * 3600.0;
                }
        }
        //printf("%lf\n", dt);
        if( ev->nzyear % 4 != 0 ||
           (ev->nzyear % 100 == 0 && ev->nzyear % 400 != 0 ) )
                a = ev->nzjday * 24.0 * 3600.0
                    + ev->nzhour * 3600.0 + ev->nzmin * 60.0 + ev->nzsec + ev->nzmsec * 0.001;
        else
                a = ev->nzjday * 24.0 * 3600.0
                    + ev->nzhour * 3600.0 + ev->nzmin * 60.0 + ev->nzsec + ev->nzmsec * 0.001;

        if( sac->nzyear % 4 != 0 ||
           (sac->nzyear % 100 == 0 && sac->nzyear % 400 != 0 ) )
                b = sac->nzjday * 24.0 * 3600.0
                    + sac->nzhour * 3600.0 + sac->nzmin * 60.0 + sac->nzsec + sac->nzmsec * 0.001 ;
        else
                b = sac->nzjday * 24.0 * 3600.0
                    + sac->nzhour * 3600.0 + sac->nzmin * 60.0 + sac->nzsec + sac->nzmsec * 0.001;
        dt += (a - b);
        return  (float)dt;
}
void GenerateCMD(EVLIST *_evlst, long _evnum , SACLIST *_saclst, long _sacnum,
                 float pre, float suf,
                 FILE *fp)
{
        int i,j;
        SACLIST *saclst = _saclst;
        SACLIST *head, *tail;
        EVLIST  *evlst;
        int iev=0, isac=0;
        for( i = 0, evlst = _evlst ;
             i < _evnum            ;
             ++i, ++evlst  )
        {
                //1.  trvt+  evlst + pre - saclst[0] - b < 0.0
                //           evlst       - saclst[0]     < b-pre-trvt
                //2.  trvt+  evlst + suf - saclst[N] - e > 0.0
                //           evlst       - saclst[N]     > e-suf-trvt
                if( dt(evlst, &(_saclst[0]) )           < _saclst[0].b - pre - evlst->trvt ||
                    dt(evlst, &(_saclst[_sacnum - 1]) ) > _saclst[_sacnum - 1].e - suf - evlst->trvt   )
                {//Event record is out of range
                        continue;
                }
                for(;; ++saclst)
                {
                        // trvt+ evlst + pre - saclst - b < 0.0
                        //       evlst       - saclst     < b - pre - trvt
                        if( dt(evlst, saclst) < saclst->b - pre - evlst->trvt)
                        {
                                --saclst;
                                head = saclst;
                                break;
                        }
                }
                for(;; ++saclst)
                {
                        // trvt+ evlst + suf - saclst - e <= 0.0
                        //       evlst       - saclst     <= e - suf - trvt
                        if( dt(evlst, saclst) <= saclst->e - suf - evlst->trvt )
                        {
                                tail = saclst;
                                break;
                        }
                }
                saclst = head;
                fun(head, tail, evlst, pre, suf, fp);
        }
}

void fun(SACLIST *head, SACLIST *tail, EVLIST *ev, float pre, float suf, FILE *fp)
{
    char *nm;
    char outfile[1024];
    memset(outfile, 0, 1024);
    nm = strstr( head->filename, "SAC");
    nm -= 4;

    fprintf(fp, "\n#####\n");
    if( (tail - head) == 0)
    {//Single Record
        fprintf(fp, "r  %s\n", head->filename);
        fprintf(fp, "ch user0 %f evla %f evlo %f evdp %f kevnm %s\n",
                ev->rayp, ev->evla, ev->evlo, ev->evdp, ev->evnm );
        fprintf(fp, "ch  o gmt %d %d %d %d %d %03d\n",
                ev->nzyear, ev->nzjday, ev->nzhour, ev->nzmin, ev->nzsec, ev->nzmsec);
        fprintf(fp, "traveltime model iasp91 picks 0 phase P\n");
        fprintf(fp, "w out/%04d.%03d.%02d.%02d.%02d.%03d.%s\n",
                ev->nzyear, ev->nzjday, ev->nzhour, ev->nzmin, ev->nzsec, ev->nzmsec, nm);

        fprintf(fp, "cut t0 %f %f\n", pre,  suf);
        fprintf(fp, "r out/%04d.%03d.%02d.%02d.%02d.%03d.%s\n",
                ev->nzyear, ev->nzjday, ev->nzhour, ev->nzmin, ev->nzsec, ev->nzmsec, nm);
        fprintf(fp, "w over\n");
        fprintf(fp, "cut off\n");
        //printf("w %s_%s\n", head->filename, ev->evnm);
    }
    else if( (tail - head) > 0 )
    {//Multiple Records
        fprintf(fp,"r ");
        for(; (tail - head) >=0; ++head )
        {
            fprintf(fp," %s \n", head->filename);
        }
        fprintf(fp, "\n");
        fprintf(fp, "merge\n");
        fprintf(fp, "ch user0 %f evla %f evlo %f evdp %f kevnm %s\n",
                ev->rayp, ev->evla, ev->evlo, ev->evdp, ev->evnm );
        fprintf(fp, "ch  o gmt %d %d %d %d %d %03d\n",
                ev->nzyear, ev->nzjday, ev->nzhour, ev->nzmin, ev->nzsec, ev->nzmsec);
        fprintf(fp, "traveltime model iasp91 picks 0 phase P\n");
        fprintf(fp, "w out/%04d.%03d.%02d.%02d.%02d.%03d.%s\n",
                ev->nzyear, ev->nzjday, ev->nzhour, ev->nzmin, ev->nzsec, ev->nzmsec, nm);

        fprintf(fp, "cut t0 %f %f\n", pre,  suf);
        fprintf(fp, "r out/%04d.%03d.%02d.%02d.%02d.%03d.%s\n",
                ev->nzyear, ev->nzjday, ev->nzhour, ev->nzmin, ev->nzsec, ev->nzmsec, nm);
        fprintf(fp, "w over\n");
        fprintf(fp, "cut off\n");
        //printf("w %s_%s\n", head->filename, ev->evnm);
    }
    else
    {//error
    }
}

int main(int argc, char *argv[])
{
    SACLIST *saclst;
    EVLIST  *evlst;
    long nsac, nev;
    FILE *fp;
    int i;
    char *fileSacInfo = NULL, *fileEVList = NULL;
    for(i = 0; i < argc; ++i)
    {
            if(argv[i][0] == '-')
            {
                switch(argv[i][1])
                {
                        case 'D':
                                fileSacInfo = argv[i+1];
                                break;
                        case 'E':
                                fileEVList  = argv[i+1];
                                break;
                        case 'H':
                                fprintf(stderr, "Usage: %s -D <sacinfo> -E <evlist>\n", argv[0]);
                                exit(1);
                }
            }
    }
    if( fileSacInfo == NULL || fileEVList == NULL )
    {
            fprintf(stderr, "Usage: %s -D <sacinfo> -E <evlist>\n", argv[0]);
            exit(1);
    }
    fp = fopen("sacCMD.sh","w");
    saclst = read_saclist(fileSacInfo, &nsac);
    evlst  = read_evlist(fileEVList, &nev);

    fprintf(fp, "sac << EOF\n" );
    GenerateCMD( evlst, nev, saclst, nsac, -5.0, 5.0, fp);
    fprintf(fp, "q\nEOF\n" );
    free(saclst);
    free(evlst);
    return 0;
}