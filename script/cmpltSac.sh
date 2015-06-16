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

ln -s $DIR TMP 2>&-

awk '{if(substr($0,1,1) != "#" && substr($0,1,1) != "\t" ) {print $0} }' $INFO > tmplist
i="1"

##1       2        3        4             5          6
##Counts  StaName  DasName  PendulumName  Longitude  Latitude
#01       SHZ      9EA1     T3E21         98.45652   25.03042
#02       LJZ      9F47     T3D87         98.46242   25.06306
#03       SJT      9FDA     T3E06         98.45752   25.09469

#Complete Station info
while read LINE; do

	Name=`echo $LINE  | awk '{print $2}'`
	Das=`echo $LINE   | awk '{print $3}'`
	Pend=`echo $LINE  | awk '{print $4}'`
	Lon=`echo $LINE   | awk '{print $5}'`
	Lat=`echo $LINE   | awk '{print $6}'`

	echo "echo $i Processing: $LINE $VERBOSE" | sh
######################################################################################################
#23456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012
#  1         2            3           4           5         6         7          8           9
#  KNETWK    KSTNM        stla        stlo        cmpaz     cmpinc    kcmpnm     LPSPOL      IDEP
#
#For BHZ
sac > /dev/null << EOF
r $DIR/*$Das.1.sac
ch KNETWK TC KSTNM  $Name stla   $Lat stlo   $Lon cmpaz   0 cmpinc  0 kcmpnm BHZ LPSPOL TRUE IDEP IVEL
w over
q
EOF
#For BHN
sac > /dev/null << EOF
r $DIR/*$Das.2.sac
ch KNETWK TC KSTNM  $Name stla   $Lat stlo   $Lon cmpaz   0 cmpinc 90 kcmpnm BHN LPSPOL TRUE IDEP IVEL
w over
q
EOF
#For BHE
sac > /dev/null << EOF
r $DIR/*$Das.3.sac
ch KNETWK TC KSTNM  $Name stla   $Lat stlo   $Lon cmpaz  90 cmpinc 90 kcmpnm BHE LPSPOL TRUE IDEP IVEL
w over
q
EOF
######################################################################################################
	i=`expr $i + 1`
done < tmplist

rm tmplist TMP -f 
