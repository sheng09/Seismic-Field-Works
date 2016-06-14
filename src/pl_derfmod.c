#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "liberrmsg.h"

#define END 15

//#include "gmt.h"
/*
* Wangsheng IGG-CAS
* 2015/07/03
 */
typedef  struct
{
    float theta, fai, h, vp, B, C, k, E, rho, NSL;
    float depthTop, depthBot;
}layer;
typedef  struct
{
    float  obj;
    int    nl;
    layer  la[10]; // Max number of layers: 10
}MOD;

char HMSG[]="\
Usage: %s -MKMI.1.mod -DKMI.1.mod.deb -LKMI.1.log -BKMI.1.best -E<n heads>\\\n\
          -PKMI.vp.m -SKMI.vs.m -JKMI.1.obj -OKMI.1.xy \\\n\
          -pKMI.vpr  -sKMI.vpr\n\
\n\
Input files arguments:\n\
-M initial model for inversion\n\
-D deviation model for inversion\n\
-L log file of DERF-inversion\n\
-B best model\n\
-E number of heads to jump\n\
\n\
Output files arguments\n\
-S output  vs-model file for GMT plot\n\
-P output  vp-model file for GMT plot\n\
-s output  vs-model rangle for GMT plot\n\
-p ouput   vp-model rangle for GMT plot\n\
-J output object value file for GMT plot\n\
";

int readMod( FILE *fpMod, FILE *fpModDeb, MOD *mod, MOD *deb, MOD *upper, MOD *lowwer);
int readMod2(FILE *fpMod,                 MOD *mod);
int readLog(FILE *FPLog, MOD *mod, int nl);

int main(int argc, char *argv[])
{
    int   fMod = 0, fDeb = 0, fLog = 0, foutVSMod = 0, foutVPMod = 0,foutObj = 0, fBestMod = 0, fnhead = 0;
    int   foutVSR=0, foutVPR=0;
    char *strMod = NULL, *strModDeb = NULL, *strLog = NULL, *stroutVSMod = NULL, *stroutVPMod =NULL, *stroutObj = NULL, *strBestMod = NULL;
    char *stroutVSR = NULL, *stroutVPR = NULL;
    FILE *FPMod, *FPModDeb, *FPLog, *FPoutVSMod, *FPoutVPMod, *FPoutObj, *FPBestMod, *FPoutVSR, *FPoutVPR;
    MOD  mod, deb, upper, lowwer, InvMod;
    int i, j;
    int nhead = 0;
    char line[1024];
    float ObjSTD, ObjMin, ObjMax;
    float vs;
    float VSMax, VSMin, DepMax, DepMin, VPMax, VPMin;
    for(i = 1; i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'M':
                    strMod = &(argv[i][2]);
                    fMod   = 1;
                    break;
                case 'D':
                    strModDeb = &(argv[i][2]);
                    fDeb      = 1;
                    break;
                case 'L':
                    strLog = &(argv[i][2]);
                    fLog   = 1;
                    break;
                case 'B':
                    strBestMod = &(argv[i][2]);
                    fBestMod = 1;
                    break;
                case 'S':
                    stroutVSMod = &(argv[i][2]);
                    foutVSMod   = 1;
                    break;
                case 'P':
                    stroutVPMod = &(argv[i][2]);
                    foutVPMod   = 1;
                    break;
                case 'p':
                    stroutVPR = &(argv[i][2]);
                    foutVPR   = 1;
                    break;
                case 's':
                    stroutVSR = &(argv[i][2]);
                    foutVSR   = 1;
                    break;
                case 'J':
                    stroutObj = &(argv[i][2]);
                    foutObj = 1;
                    break;
                case 'E':
                    sscanf(&(argv[i][2]), "%d", &nhead);
                    fnhead = 1;
                    break;
                case 'H':
                    fprintf(stderr, HMSG,argv[0]);
                    exit(1);
                    break;
                default:
                    fprintf(stderr, HMSG,argv[0]);
                    exit(1);
                    break;
            }
        }
    }
    if( fMod == 0 || fDeb == 0 || fLog == 0 || fBestMod == 0 ||
        foutVSMod == 0 || foutVPMod == 0 || foutObj == 0 || fnhead == 0 ||
        foutVPR   == 0 || foutVSR == 0)
    {
        perrmsg("",ERR_MORE_ARGS);
        fprintf(stderr, HMSG,argv[0]);
        exit(1);
    }

    if( (FPMod = fopen(strMod, "r")) == NULL )
    {
        perrmsg( strMod, ERR_READ_FILE);
        exit(1);
    }
    if( (FPModDeb = fopen(strModDeb, "r")) == NULL )
    {
        perrmsg(strModDeb, ERR_READ_FILE);
        exit(1);
    }
    if( (FPLog = fopen(strLog, "r")) == NULL )
    {
        perrmsg(strLog, ERR_READ_FILE);
        exit(1);
    }
    if( (FPBestMod = fopen(strBestMod, "r")) == NULL )
    {
        perrmsg(strBestMod, ERR_READ_FILE);
        exit(1);
    }
    if( (FPoutVSMod = fopen(stroutVSMod, "w")) == NULL )
    {
        perrmsg(stroutVSMod, ERR_OPEN_FILE);
        exit(1);
    }
    if( (FPoutVPMod = fopen(stroutVPMod, "w")) == NULL )
    {
        perrmsg(stroutVPMod, ERR_OPEN_FILE);
        exit(1);
    }
    if( (FPoutObj = fopen(stroutObj, "w")) == NULL )
    {
        perrmsg(stroutObj, ERR_OPEN_FILE);
        exit(1);
    }
    if( (FPoutVSR = fopen(stroutVSR, "w")) == NULL )
    {
        perrmsg(stroutVSR, ERR_OPEN_FILE);
        exit(1);
    }
    if( (FPoutVPR = fopen(stroutVPR, "w")) == NULL )
    {
        perrmsg(stroutVPR, ERR_OPEN_FILE);
        exit(1);
    }


    readMod(FPMod, FPModDeb, &mod, &deb, &upper, &lowwer);
    //
    for (i = 0; i < nhead; ++i)
    {
        if( fgets(line, 1024, FPLog) == NULL)
        {
            perrmsg( strLog, ERR_READ_FILE);
            exit(1);
            break;
        }

    }
    for(i = 0, readLog(FPLog, &InvMod, mod.nl),
        ObjSTD = InvMod.obj/200.0,
        ObjMin = InvMod.obj,
        ObjMax = InvMod.obj,
        VSMin =  (InvMod.la)[0].vp / (InvMod.la)[0].k,
        VSMax =  (InvMod.la)[0].vp / (InvMod.la)[0].k,
        VPMin =  (InvMod.la)[0].vp ,
        VPMax =  (InvMod.la)[0].vp ,
        DepMin=  (InvMod.la)[0].depthTop,
        DepMax=  (InvMod.la)[0].depthTop
            ;
        readLog(FPLog, &InvMod, mod.nl) != END
            ;
        ++i )
    {
        if(i % 2 == 0)
        {
            fprintf(FPoutVSMod, "> -Z%f\n",InvMod.obj/ObjSTD);
            fprintf(FPoutVPMod, "> -Z%f\n",InvMod.obj/ObjSTD);
            for(j = 0; j <= mod.nl; ++j )
            {
                vs = (InvMod.la)[j].vp / (InvMod.la)[j].k;
                fprintf(FPoutVSMod, "%f %f %f\n", vs, (InvMod.la)[j].depthTop, InvMod.obj/ObjSTD );
                fprintf(FPoutVSMod, "%f %f %f\n", vs, (InvMod.la)[j].depthBot, InvMod.obj/ObjSTD );

                fprintf(FPoutVPMod, "%f %f %f\n", (InvMod.la)[j].vp, (InvMod.la)[j].depthTop, InvMod.obj/ObjSTD );
                fprintf(FPoutVPMod, "%f %f %f\n", (InvMod.la)[j].vp, (InvMod.la)[j].depthBot, InvMod.obj/ObjSTD );
                //fprintf(FPoutVSMod, "%d %f %f %f %f\n",i, (InvMod.la)[j].depthTop, vs , InvMod.obj, InvMod.obj/ObjSTD );
                //fprintf(FPoutVSMod, "%d %f %f %f %f\n",i, (InvMod.la)[j].depthBot, vs , InvMod.obj, InvMod.obj/ObjSTD );

                //if( (InvMod.la)[j].depthTop > DepMax )
                //DepMax = (InvMod.la)[j].depthTop;
                //else if( (InvMod.la)[j].depthTop < DepMin)
                //    DepMin = (InvMod.la)[j].depthTop;
                //if( (InvMod.la)[j].depthBot > DepMax )
                //    DepMax = (InvMod.la)[j].depthBot;
                //else if( (InvMod.la)[j].depthBot < DepMin )
                //    DepMin = (InvMod.la)[j].depthBot;

                if( vs > VSMax )
                    VSMax = vs;
                else if(vs < VSMin)
                    VSMin = vs;

                if( (InvMod.la)[j].vp > VPMax )
                    VPMax = (InvMod.la)[j].vp;
                else if((InvMod.la)[j].vp < VPMin)
                    VPMin = (InvMod.la)[j].vp;
            }
        }
        fprintf(FPoutObj, "%d %f %f\n", i, InvMod.obj, InvMod.obj/ObjSTD  );

        if( InvMod.obj > ObjMax)
            ObjMax = InvMod.obj;
        else if(InvMod.obj < ObjMin)
            ObjMin = InvMod.obj;

        //printf("%d\n", i);
    }
    //printf("fdfdfdfdfd\n");



    readMod2(FPBestMod, &mod);

    fprintf(FPoutVPMod, "> -Z%f\n",-100.0f);
    for(j = 0; j <= mod.nl; ++j )
    {
        fprintf(FPoutVPMod, "%f %f %f\n", (mod.la)[j].vp, (mod.la)[j].depthTop, -100.0f);
        fprintf(FPoutVPMod, "%f %f %f\n", (mod.la)[j].vp, (mod.la)[j].depthBot, -100.0f);
    }
    fprintf(FPoutVSMod, "> -Z%f\n",-100.0f);
    for(j = 0; j <= mod.nl; ++j )
    {
        vs = (mod.la)[j].vp / (mod.la)[j].k;
        fprintf(FPoutVSMod, "%f %f %f\n", vs, (mod.la)[j].depthTop, -100.0f);
        fprintf(FPoutVSMod, "%f %f %f\n", vs, (mod.la)[j].depthBot, -100.0f);
    }

    //output lowwer and upper model
    for(j = 0; j <= mod.nl; ++j)
    {
        vs = (lowwer.la)[j].vp / (lowwer.la)[j].k;
        fprintf(FPoutVPR, "%f %f\n", (lowwer.la)[j].vp, (lowwer.la)[j].depthTop);
        fprintf(FPoutVPR, "%f %f\n", (lowwer.la)[j].vp, (lowwer.la)[j].depthBot);

        fprintf(FPoutVSR, "%f %f\n", vs, (lowwer.la)[j].depthTop);
        fprintf(FPoutVSR, "%f %f\n", vs, (lowwer.la)[j].depthBot);
    }

    for(j = mod.nl; j >= 0; --j)
    {
        vs = (upper.la)[j].vp / (upper.la)[j].k;
        fprintf(FPoutVPR, "%f %f\n", (upper.la)[j].vp, (upper.la)[j].depthBot);
        fprintf(FPoutVPR, "%f %f\n", (upper.la)[j].vp, (upper.la)[j].depthTop);

        fprintf(FPoutVSR, "%f %f\n", vs, (upper.la)[j].depthBot);
        fprintf(FPoutVSR, "%f %f\n", vs, (upper.la)[j].depthTop);
    }

    // The late model is not the best model. Read the best model from the file 2015/09/17
    //
    // For the last model 2015/09/01
    // fprintf(FPoutVSMod, "> -Z%f\n",-100.0f);
    //for(j = 0; j <= mod.nl; ++j )
    //{
    //    vs = (InvMod.la)[j].vp / (InvMod.la)[j].k;
    //    fprintf(FPoutVSMod, "%f %f %f\n", vs, (InvMod.la)[j].depthTop, InvMod.obj/ObjSTD );
    //    fprintf(FPoutVSMod, "%f %f %f\n", vs, (InvMod.la)[j].depthBot, InvMod.obj/ObjSTD );
    //}
    //fprintf(FPoutVPexitMod, "> -Z%f\n",-100.0f);
    //for(j = 0; j <= mod.nl; ++j )
    //{
    //    fprintf(FPoutVPMod, "%f %f %f\n", (InvMod.la)[j].vp, (InvMod.la)[j].depthTop, InvMod.obj/ObjSTD );
    //    fprintf(FPoutVPMod, "%f %f %f\n", (InvMod.la)[j].vp, (InvMod.la)[j].depthBot, InvMod.obj/ObjSTD );
    //}
    fprintf(stdout, "VS:  [ %f , %f ]\n",VSMin - 0.1f*(VSMax - VSMin), VSMax + 0.1f*(VSMax - VSMin) );
    fprintf(stdout, "VP:  [ %f , %f ]\n",VPMin - 0.1f*(VPMax - VPMin), VPMax + 0.1f*(VPMax - VPMin) );
    fprintf(stdout, "OBJ: [ %f , %f ]\n",ObjMin -0.1f*(ObjMax - ObjMin) , ObjMax + 0.1f*(ObjMax - ObjMin) );
    fprintf(stdout, "CPT: [ %f , %f ]\n",ObjMin/ObjSTD, ObjMax/ObjSTD );
    fprintf(stdout, "Num: %d\n", i);
    fclose(FPMod);
    fclose(FPModDeb);
    fclose(FPLog);
    fclose(FPoutVSMod);
    fclose(FPoutVPMod);
    fclose(FPoutObj);
    fclose(FPoutVPR);
    fclose(FPoutVSR);
    return 0;
}

int readMod(FILE *fpMod, FILE *fpModDeb, MOD *mod, MOD *deb, MOD *upper, MOD *lowwer)
{
    char line[1024];
    int i;
    float depth = 0.0;
    float depthL = 0.0;
    float depthU = 0.0;
    //Read MOD
    if( NULL == fgets(line, 1024, fpMod)) exit(1);
    if( NULL == fgets(line, 1024, fpMod)) exit(1);
    if( NULL == fgets(line, 1024, fpMod)) exit(1);
    memset(line, 0, 1024);
    if( NULL == fgets(line, 1024, fpMod)) exit(1);
    sscanf(line,"%d", &(mod-> nl));
    for(i = 0; i <= mod->nl; ++i)
    {
        memset(line, 0, 1024);
        if( NULL == fgets(line, 1024, fpMod)) exit(1);
        sscanf(line, "%f %f", &( (mod->la)[i].theta), &( (mod->la)[i].fai) );
        memset(line, 0, 1024);
        if( NULL == fgets(line, 1024, fpMod)) exit(1);
        sscanf(line, "%f %f %f %f %f %f %f %f",
                &( (mod->la)[i].h),
                &( (mod->la)[i].vp),
                &( (mod->la)[i].B),
                &( (mod->la)[i].C),
                &( (mod->la)[i].k),
                &( (mod->la)[i].E),
                &( (mod->la)[i].rho),
                &( (mod->la)[i].NSL) );

        (mod->la)[i].depthTop = depth;
        depth = (mod->la)[i].h;
        (mod->la)[i].depthBot = depth;
    }

    //Read MOD DEB
    if( NULL == fgets(line, 1024, fpModDeb)) exit(1);
    memset(line, 0, 1024);
    if( NULL == fgets(line, 1024, fpModDeb)) exit(1);
    sscanf(line,"%d", &(deb-> nl));
    for(i = 0; i <= deb->nl; ++i)
    {
        memset(line, 0, 1024);
        if( NULL == fgets(line, 1024, fpModDeb)) exit(1);
        sscanf(line, "%f %f", &( (deb->la)[i].theta), &( (deb->la)[i].fai) );
        memset(line, 0, 1024);
        if( NULL == fgets(line, 1024, fpModDeb)) exit(1);
        sscanf(line, "%f %f %f %f %f %f %f %f",
                &( (deb->la)[i].h),
                &( (deb->la)[i].vp),
                &( (deb->la)[i].B),
                &( (deb->la)[i].C),
                &( (deb->la)[i].k),
                &( (deb->la)[i].E),
                &( (deb->la)[i].rho),
                &( (deb->la)[i].NSL) );
    }

    if(deb->nl != mod->nl)
    {
        fprintf(stderr, "Err:\n" );
        exit(1);
    }
    for(i = 0; i < mod->nl; ++i)
    {
        (upper->la)[i].theta    = (mod->la)[i].theta + (deb->la)[i].theta ;
        (upper->la)[i].fai      = (mod->la)[i].fai   + (deb->la)[i].fai   ;
        (upper->la)[i].h        = (mod->la)[i].h     + (deb->la)[i].h     ;
        (upper->la)[i].vp       = (mod->la)[i].vp    + (deb->la)[i].vp    ;
        (upper->la)[i].B        = (mod->la)[i].B     + (deb->la)[i].B     ;
        (upper->la)[i].C        = (mod->la)[i].C     + (deb->la)[i].C     ;
        (upper->la)[i].k        = (mod->la)[i].k     - (deb->la)[i].k     ;
        (upper->la)[i].E        = (mod->la)[i].E     + (deb->la)[i].E     ;
        (upper->la)[i].rho      = (mod->la)[i].rho   + (deb->la)[i].rho   ;
        (upper->la)[i].NSL      = (mod->la)[i].NSL   + (deb->la)[i].NSL   ;
        //(upper->la)[i].depthTop = (mod->la)[i].depthTop - (deb->la)[i].h     ;
        //(upper->la)[i].depthBot = (mod->la)[i].depthBot - (deb->la)[i].h     ;
        (upper->la)[i].depthTop = depthU;
        depthU                  = (mod->la)[i].h - (deb->la)[i].h;
        (upper->la)[i].depthBot = depthU;

        (lowwer->la)[i].theta   = (mod->la)[i].theta - (deb->la)[i].theta ;
        (lowwer->la)[i].fai     = (mod->la)[i].fai   - (deb->la)[i].fai   ;
        (lowwer->la)[i].h       = (mod->la)[i].h     - (deb->la)[i].h     ;
        (lowwer->la)[i].vp      = (mod->la)[i].vp    - (deb->la)[i].vp    ;
        (lowwer->la)[i].B       = (mod->la)[i].B     - (deb->la)[i].B     ;
        (lowwer->la)[i].C       = (mod->la)[i].C     - (deb->la)[i].C     ;
        (lowwer->la)[i].k       = (mod->la)[i].k     + (deb->la)[i].k     ;
        (lowwer->la)[i].E       = (mod->la)[i].E     - (deb->la)[i].E     ;
        (lowwer->la)[i].rho     = (mod->la)[i].rho   - (deb->la)[i].rho   ;
        (lowwer->la)[i].NSL     = (mod->la)[i].NSL   - (deb->la)[i].NSL   ;
        //(lowwer->la)[i].depthTop = (mod->la)[i].depthTop + (deb->la)[i].h     ;
        //(lowwer->la)[i].depthBot = (mod->la)[i].depthBot + (deb->la)[i].h     ;
        (lowwer->la)[i].depthTop = depthL;
        depthL                   = (mod->la)[i].h + (deb->la)[i].h;
        (lowwer->la)[i].depthBot = depthL;

        //printf("%d %f %f %f %f %f\n",i, (lowwer->la)[i].depthTop, (lowwer->la)[i].depthBot,(mod->la)[i].depthTop, (mod->la)[i].depthBot, (deb->la)[i].h);
    }

    i = mod->nl;
    (upper->la)[i].theta    = (mod->la)[i].theta + (deb->la)[i].theta ;
    (upper->la)[i].fai      = (mod->la)[i].fai   + (deb->la)[i].fai   ;
    (upper->la)[i].h        = (mod->la)[i].h     + (deb->la)[i].h     ;
    (upper->la)[i].vp       = (mod->la)[i].vp    + (deb->la)[i].vp    ;
    (upper->la)[i].B        = (mod->la)[i].B     + (deb->la)[i].B     ;
    (upper->la)[i].C        = (mod->la)[i].C     + (deb->la)[i].C     ;
    (upper->la)[i].k        = (mod->la)[i].k     - (deb->la)[i].k     ;
    (upper->la)[i].E        = (mod->la)[i].E     + (deb->la)[i].E     ;
    (upper->la)[i].rho      = (mod->la)[i].rho   + (deb->la)[i].rho   ;
    (upper->la)[i].NSL      = (mod->la)[i].NSL   + (deb->la)[i].NSL   ;
    //(upper->la)[i].depthTop = (mod->la)[i].depthTop - (deb->la)[i].h     ;
    //(upper->la)[i].depthBot = (mod->la)[i].depthBot - (deb->la)[i].h     ;
    (upper->la)[i].depthTop = depthU;
    depthU                  = (mod->la)[i].h - (deb->la)[i].h;
    (upper->la)[i].depthBot = depthU;

    (lowwer->la)[i].theta   = (mod->la)[i].theta - (deb->la)[i].theta ;
    (lowwer->la)[i].fai     = (mod->la)[i].fai   - (deb->la)[i].fai   ;
    (lowwer->la)[i].h       = (mod->la)[i].h     - (deb->la)[i].h     ;
    (lowwer->la)[i].vp      = (mod->la)[i].vp    - (deb->la)[i].vp    ;
    (lowwer->la)[i].B       = (mod->la)[i].B     - (deb->la)[i].B     ;
    (lowwer->la)[i].C       = (mod->la)[i].C     - (deb->la)[i].C     ;
    (lowwer->la)[i].k       = (mod->la)[i].k     + (deb->la)[i].k     ;
    (lowwer->la)[i].E       = (mod->la)[i].E     - (deb->la)[i].E     ;
    (lowwer->la)[i].rho     = (mod->la)[i].rho   - (deb->la)[i].rho   ;
    (lowwer->la)[i].NSL     = (mod->la)[i].NSL   - (deb->la)[i].NSL   ;
    //(lowwer->la)[i].depthTop = (mod->la)[i].depthTop + (deb->la)[i].h     ;
    //(lowwer->la)[i].depthBot = (mod->la)[i].depthBot + (deb->la)[i].h     ;
    (lowwer->la)[i].depthTop = depthL;
    depthL                   = (mod->la)[i].h + (deb->la)[i].h;
    (lowwer->la)[i].depthBot = depthL;
    return 0;
}
int readMod2(FILE *fpMod,                 MOD *mod)
{
    char line[1024];
    int i;
    float depth = 0.0;
    //Read MOD
    if( NULL == fgets(line, 1024, fpMod)) exit(1);
    if( NULL == fgets(line, 1024, fpMod)) exit(1);
    if( NULL == fgets(line, 1024, fpMod)) exit(1);
    memset(line, 0, 1024);
    if( NULL == fgets(line, 1024, fpMod)) exit(1);
    sscanf(line,"%d", &(mod-> nl));
    for(i = 0; i <= mod->nl; ++i)
    {
        memset(line, 0, 1024);
        if( NULL == fgets(line, 1024, fpMod)) exit(1);
        sscanf(line, "%f %f", &( (mod->la)[i].theta), &( (mod->la)[i].fai) );
        memset(line, 0, 1024);
        if( NULL == fgets(line, 1024, fpMod)) exit(1);
        sscanf(line, "%f %f %f %f %f %f %f %f",
                &( (mod->la)[i].h),
                &( (mod->la)[i].vp),
                &( (mod->la)[i].B),
                &( (mod->la)[i].C),
                &( (mod->la)[i].k),
                &( (mod->la)[i].E),
                &( (mod->la)[i].rho),
                &( (mod->la)[i].NSL) );

        (mod->la)[i].depthTop = depth;
        depth = (mod->la)[i].h;
        (mod->la)[i].depthBot = depth;
    }
    return 0;
}
int readLog(FILE *FPLog, MOD *mod, int nl)
{
    int i;
    float depth = 0.0;
    char line[1024];
    memset(line, 0, 1024);
    if( NULL == fgets(line, 1024, FPLog) ) return END;
    sscanf(line, "Obj= %f", &(mod->obj));

    //Read inversed model
    for(i = 0; i <= nl; ++i)
    {
        memset(line, 0, 1024);
        if( NULL == fgets(line, 1024, FPLog)) exit(1);
        sscanf(line, "%f %f %f %f %f %f %f %f %f %f",
                &( (mod->la)[i].theta),
                &( (mod->la)[i].fai),
                &( (mod->la)[i].h),
                &( (mod->la)[i].vp),
                &( (mod->la)[i].B),
                &( (mod->la)[i].C),
                &( (mod->la)[i].k),
                &( (mod->la)[i].E),
                &( (mod->la)[i].rho),
                &( (mod->la)[i].NSL) );

        //(mod->la)[i].depthTop = (mod->la)[i-1].h;
        (mod->la)[i].depthTop = depth;
        depth = (mod->la)[i].h;
        //depth += (mod->la)[i].h;
        //
        (mod->la)[i].depthBot = (mod->la)[i].h;
    }
    return 0;
}