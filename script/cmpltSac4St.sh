#!/bin/bash
# Wangsheng, IGG-CAS
# wangsheng.cas@gmail.com
# 2015/7/22
HMSG="Usage: DOII.sh -D <directory> -E <event.list>"
while  getopts  "D:E:V"  arg #选项后面的冒号表示该选项需要参数
do
         case  $arg  in
             D)
                DIR=$OPTARG
                ;;
             E)
                EVLIST=$OPTARG
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
if [[ -z $DIR || -z $EVLIST ]]; then
    echo -e $HMSG
    exit 1
fi
#a.sh -D <directory> -E <event list>
#DIR="dat"
#EVLIST="event.list"

#####
#    0. Generate the list of events
#####
#    Format of event.list
#
#    #1      2      3     4     5      6      7        8
#    #Counts Name   La    Lo    el     dp     KJD      KTIME
#    1       JP     30    170   -12345 30     2015/04/28 00:12:13.330
#    2       US     35    165   -12345 100    2015/04/28 01:13:14.220
#    ...
#
#####
awk '{if(substr($0,1,1) != "#" && substr($0,1,1) != "\t" ) {print $0} }' ${EVLIST} > evlist.tmp


#####
#    1. Generate the list of sac files info
#####
#    Format of sacinfo.tmp
#
#    #1                       2   3  4          5            6 7       8       9       10     11
#    #Name                    ST  NW KZDATE     KZTIME       B E       STLA    STLO    STEL    STDP
#    dat/15.118.01.00.00.911E TZB TC 2015/04/28 01:00:00.020 0 3599.98  25.175 98.4793 -12345 -12345
#    dat/15.118.01.00.00.9EA1 SHZ TC 2015/04/28 01:00:00.020 0 3599.98 25.0304 98.4565 -12345 -12345
#    dat/15.118.01.00.00.9F40 DYC TC 2015/04/28 01:00:00.020 0 3599.98 25.1352 98.4681 -12345 -12345
#    ...
#
#####
>sacinfo.tmp
saclst kstnm knetwk kzdate kztime b e \
       stla stlo stel stdp f ${DIR}/*.BHZ  | \
       sed 's/.BHZ//g' | \
       sort -n >> sacinfo.tmp
#####
#    2. Generate SAC script file
#####
cut4Ev -E evlist.tmp -D sacinfo.tmp
chmod 777 sacCMD.sh

rm *.tmp -rf