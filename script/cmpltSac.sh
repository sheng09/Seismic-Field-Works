#!/bin/bash
#  Version 2015.06.03
#  This bash script is used to complete sac header info
#  Wangsheng
#  2015-06-03
#
#Description
HMSG="
Usage: cmpltSac -D Directory -S stainfo.list [-V]\n
\n
~~~~~~~~~~~~~~~~Example of stainfo.list:~~~~~~~~~~~~~~~~~~~~\n
#Counts  StaName  DasName  PendulumName  Longitude  Latitude\n
01       SHZ      9EA1     T3E21         98.45652   25.03042\n
02       LJZ      9F47     T3D87         98.46242   25.06306\n
03       SJT      9FDA     T3E06         98.45752   25.09469\n
~~~~~~~~~~~~~~~~Example Tree of Directory:~~~~~~~~~~~~~~~~~~\n
SAC\n
├── 15.122.04.00.00.9F40.1.sac\n
├── 15.122.04.00.00.9F40.2.sac\n
├── 15.122.04.00.00.9F40.3.sac\n
├──...\n
\n
Description:\n
Given \"stainfo.list\" file, and \"Directory\",  sachdr information \n
including \"KNETWK KSTNM stla stlo cmpaz cmpinc kcmpnm LPSPOL\n
 IDEP\" of all the sac_file within the \"Directory\" will be assigned.\n
 [-V] option is used to  outpout log.\n
\n
Eg:
cmpltSac -D ./SAC -S ./stainfo.list -V\n
\n
Wangsheng IGG-CAS\n
wangsheng.cas@gmail.com"

VERBOSE=" > /dev/null"
while  getopts  "D:S:V"  arg #选项后面的冒号表示该选项需要参数
do
         case  $arg  in
             D)
                DIR=$OPTARG
                ;;
             S)
                INFO=$OPTARG
                ;;
             V)
                VERBOSE=""
                ;;
             ?)  #当有不认识的选项的时候arg为 ?
            	#echo  " unkonw argument "
        		exit  1
        ;;
        esac
done

if [[ -z $DIR || -z $INFO ]]; then
	echo -e $HMSG 
	exit 1
fi


awk '{if(substr($0,1,1) != "#" && substr($0,1,1) != "\t" ) {print $0} }' $INFO > tmplist
i="1"

##1       2        3        4             5          6
##Counts  StaName  DasName  PendulumName  Longitude  Latitude
#01       SHZ      9EA1     T3E21         98.45652   25.03042
#02       LJZ      9F47     T3D87         98.46242   25.06306
#03       SJT      9FDA     T3E06         98.45752   25.09469

PWD_=`pwd`

> cmpltSac.log

cd ${DIR}
ls > sta.tmp
while read STA
do
    echo Processing ${STA}
    cd ${STA}
    #                                                                                            YYYY DDD   MM   HH   SS  FFFF    CCC
    saclst nzyear nzjday nzhour nzmin nzsec nzmsec f *.1.sac  | awk -vSTA=${STA} '{printf("mv %s %04d.%03d.%02d.%02d.%02d.%04d.TC.%3s.00.BHZ\n", $1,$2,$3,$4,$5,$6,$7*10,STA) }' | sh
    saclst nzyear nzjday nzhour nzmin nzsec nzmsec f *.2.sac  | awk -vSTA=${STA} '{printf("mv %s %04d.%03d.%02d.%02d.%02d.%04d.TC.%3s.00.BHN\n", $1,$2,$3,$4,$5,$6,$7*10,STA) }' | sh
    saclst nzyear nzjday nzhour nzmin nzsec nzmsec f *.3.sac  | awk -vSTA=${STA} '{printf("mv %s %04d.%03d.%02d.%02d.%02d.%04d.TC.%3s.00.BHE\n", $1,$2,$3,$4,$5,$6,$7*10,STA) }' | sh
    Lon=`awk -vSTA=${STA} '$2==STA{print $5}' ${PWD_}/tmplist`
    Lat=`awk -vSTA=${STA} '$2==STA{print $6}' ${PWD_}/tmplist`

    echo \#\!/bin/bash                        >  cmd.sh
    echo "sac >> ${PWD_}/cmpltSac.log << EOF" >> cmd.sh
    #                                                                 filenm                 kstnm    stlo         stla          !!!       !!         !!
    ls *.BHZ | awk -vSTA=${STA} -vLon=${Lon} -vLat=${Lat} '{ printf("r %s \nch KNETWK TC KSTNM %s stlo %10.5f stla %10.5f kcmpnm BHZ cmpaz 0  cmpinc 0  LPSPOL TRUE IDEP IVEL\nwh\n", $1, STA, Lon, Lat ) }' >> cmd.sh
    ls *.BHN | awk -vSTA=${STA} -vLon=${Lon} -vLat=${Lat} '{ printf("r %s \nch KNETWK TC KSTNM %s stlo %10.5f stla %10.5f kcmpnm BHN cmpaz 0  cmpinc 90 LPSPOL TRUE IDEP IVEL\nwh\n", $1, STA, Lon, Lat ) }' >> cmd.sh
    ls *.BHE | awk -vSTA=${STA} -vLon=${Lon} -vLat=${Lat} '{ printf("r %s \nch KNETWK TC KSTNM %s stlo %10.5f stla %10.5f kcmpnm BHE cmpaz 90 cmpinc 90 LPSPOL TRUE IDEP IVEL\nwh\n", $1, STA, Lon, Lat ) }' >> cmd.sh

    echo "q"   >> cmd.sh
    echo "EOF" >> cmd.sh
    bash cmd.sh
    rm cmd.sh
    cd ..
done < sta.tmp

rm sta.tmp
cd ${PWD_}
