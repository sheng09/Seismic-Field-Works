#!/bin/bash
#  Version 2015.06.03
#  This bash script is used to pick SAC_DATA according to EVENT and STATION
#    Given EVENT and STATION info, the traveltime of P phase is given, then 
#    the record which interact with [tP-ta, tP+tb] will be selected. 
#  Besides, the SAC_HEADER about EVENT will be assigned, such as kevnm, evla, evla,
#    evdp, mag, o, t1(Theoretical Traveltime of P phase).
#
#!!!NOTE: Some time series may spread over two continuous records.
#
#  Wangsheng 
#  wangsheng.cas@gmail.com
#  2015-06-03

HMSG="
Usage: pkEv.sh -D Directory -S stainfo.list -E evtinfo.list -A pretime(Second) -B suftime(Second) [-V]\n
\n
~~~~~~~~~~~~~~~~Example of stainfo.list:~~~~~~~~~~~~~~~~~~~~\n
#Counts StaName DasName PendulumName Longitude Latitude\n
01 SHZ 9EA1 T3E21 98.45652 25.03042\n
02 LJZ 9F47 T3D87 98.46242 25.06306\n
03 SJT 9FDA T3E06 98.45752 25.09469\n
\n
~~~~~~~~~~~~~~~~Example of evtinfo.list:~~~~~~~~~~~~~~~~~~~~\n
#日期	    时间	        纬度(°)	经度(°)	深度(km)	震级类型	震级值	事件类型	参考地名\n
2015-05-01	16:06:03.0	-5.15	151.80	50	Ms	6.8	eq	新不列颠地区\n
2015-04-30	18:45:05.5	-5.40	151.75	60	Ms	6.7	eq	新不列颠地区\n
2015-04-30	00:09:52.3	40.05	142.60	40	Ms	5.1	eq	日本本州东岸近海\n
\n
~~~~~~~~~~~~~~~~Example Tree of Directory:~~~~~~~~~~~~~~~~~~\n
SAC\n
├── 15.122.04.00.00.9F40.1.sac\n
├── 15.122.04.00.00.9F40.2.sac\n
├── 15.122.04.00.00.9F40.3.sac\n
├──...\n
\n
Description:\n
Given \"stainfo.list\" file, \"evtinfo.list\" file, \"Directory\" containing SAC_DATA, and \"pretime\",\n
\"suftime\", pkEv.sh will calculate theoretical traveltime \"t1\" of P phase using taup package for\n
each Ev-St couple(Note: none P phase exists for some Ev-St couple because of the Shadow Area.). \n
Then, pkEv.sh will selected relevant file that contain [t1+ pretime, t1+ suftime], assign sachdr\n
information contain \"kevnm evla evlo evdp mag o t1\" and cut time series into \n
[t1+ pretime, t1+ suftime] period (Note: Some time interval may spread over two file, thus merge \n
operation will be called).\n
Finally, all the selected data will classified according to Event name.\n
\n
Judgement about the existence of data corresponding to  relative time period will be operated.\n
\n
Note:\n
Because the time within evtinfo.list is Beijing(GMT+8) time. Thus an operation of shifting GMT+8\n
to GMT+0 time is done. You can rewrite it from line 126-128 in the script.\n
\n
Eg:\n
pkEv.sh -D ./SAC -S ./stainfo.list -E ./evtinfo.list -A -30 -B 200 -V\n
\n
Wangsheng IGG-CAS\n
wangsheng.cas@gmail.com"

VERBOSE=" > /dev/null"

while  getopts  "D:S:E:A:B:V"  arg #选项后面的冒号表示该选项需要参数
do
         case  $arg  in
             D)
                DIR=$OPTARG
                ;;
             S)
                STA=$OPTARG
                ;;
             E)
                EVENT=$OPTARG
                ;;
             A)
                A=$OPTARG
                ;;
             B)
                B=$OPTARG
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

if [[ -z $DIR || -z $STA || -z $EVENT || -z $A || -z $B ]]; then
	echo -e $HMSG 
	exit 1
fi

awk '{if(substr($0,1,1) != "#" && substr($0,1,1) != "\t" ) {print $0} }' $EVENT > tmpevt
#日期	    时间	        纬度(°)	经度(°)	深度(km)	震级类型	震级值	事件类型	参考地名
#2015-05-01	16:06:03.0	-5.15	151.80	50	Ms	6.8	eq	新不列颠地区
#2015-04-30	18:45:05.5	-5.40	151.75	60	Ms	6.7	eq	新不列颠地区
#2015-04-30	00:09:52.3	40.05	142.60	40	Ms	5.1	eq	日本本州东岸近海
#2015-04-29	08:26:50.0	25.90	128.25	10	Ms	5.1	eq	琉球群岛
#2015-04-29	02:56:54.2	17.20	-94.95	110	mb	5.9	eq	墨西哥

awk '{if(substr($0,1,1) != "#" && substr($0,1,1) != "\t" ) {print $0} }' $STA   > tmpsta
##Counts  StaName  DasName  PendulumName  Longitude  Latitude
#01       SHZ      9EA1     T3E21         98.45652   25.03042
#02       LJZ      9F47     T3D87         98.46242   25.06306

i=0
mkdir SAC_EV 2>&- || rm SAC_EV/* -r -f 2>&-

while read evLINE; do

	i=`expr $i + 1`

	#Acquire event info
	evNM=`echo $evLINE   | awk '{print $9}'`
	evMAG=`echo $evLINE  | awk '{print $7}'`

	evla=`echo $evLINE   | awk '{print $3}'`
	evlo=`echo $evLINE   | awk '{print $4}'`
	evdp=`echo $evLINE   | awk '{print $5}'`

	Date=`echo $evLINE   | awk '{print $1}'`
	Time=`echo $evLINE   | awk '{print $2}'`

	#Acquire time info
	evJD=`GMTime -D$Date -T$Time -S-28800 | awk '{print $1}'`
	evDT=`GMTime -D$Date -T$Time -S-28800 | awk '{print $2}'`
	evTM=`GMTime -D$Date -T$Time -S-28800 | awk '{print $3}'`

	evY=`echo  $evJD | awk -F- '{print $1}'`
	evD=`echo  $evJD | awk -F- '{print $2}'`
	evH=`echo  $evTM | awk -F: '{print $1}'`
	evM=`echo  $evTM | awk -F: '{print $2}'`
	evS=`echo  $evTM | awk -F: '{print $3}' | awk -F. '{print $1}'`
	evMS=`echo $evTM | awk -F: '{print 0substr($3,3,3)}' | awk '{print 1000*$1}'`

	FILENM=`echo $i $evNM | awk '{printf("SAC_EV/%04d%s",$1,$2)}'`
	echo "echo \#1\#$FILENM $VERBOSE" | sh
	echo "echo \#2\#$evLINE $VERBOSE" | sh

	#Create a new directory or clear the existing directory
	mkdir $FILENM 2>&- || rm $FILENM/* -r -f 2>&-
	#echo $evY $evD $evH $evM $evS $evMS
	while read stLINE; do
		#Acquire station info
		stlo=`echo   $stLINE  | awk '{print $5}'`
		stla=`echo   $stLINE  | awk '{print $6}'`
		kstDAS=`echo $stLINE  | awk '{print $3}'`

		#Calculate theoretical traveltime of P phase and corresponding arrival time
		Dis=`GCDis -E$evlo/$evla -S$stlo/$stla | awk '{print $2}'`
		TrlT=`taup_time -mod prem -h $evdp -ph P -deg $Dis | awk 'NR==6{print $4}'`

		#Judge whether P phase exists
		if [[ ! -z $TrlT && $Dis > 30 && $Dis < 90 ]]; then
			DTA=$(echo "$TrlT+$A"|bc)
			DTB=$(echo "$TrlT+$B"|bc)

			#Arrival time
			#artDT=`GMTime -D$evDT -T$evTM -S$TrlT | awk '{print $2}'`
			artJD=`GMTime -D$evDT -T$evTM -S$TrlT | awk '{print $1}'`
			artTM=`GMTime -D$evDT -T$evTM -S$TrlT | awk '{print $3}'`
			artY=`echo  $artJD | awk -F- '{print $1}'`
			artD=`echo  $artJD | awk -F- '{print $2}'`
			artH=`echo  $artTM | awk -F: '{print $1}'`
			artM=`echo  $artTM | awk -F: '{print $2}'`
			artS=`echo  $artTM | awk -F: '{print $3}' | awk -F. '{print $1}'`
			artMS=`echo $artTM | awk -F: '{print 0substr($3,3,3)}' | awk '{print 1000*$1}'`

			#Pre arrival time
			AJD=`GMTime -D$evDT -T$evTM -S$DTA | awk '{print $1}'`
			ATM=`GMTime -D$evDT -T$evTM -S$DTA | awk '{print $3}'`
			#Suf arrival time
			BJD=`GMTime -D$evDT -T$evTM -S$DTB | awk '{print $1}'`
			BTM=`GMTime -D$evDT -T$evTM -S$DTB | awk '{print $3}'`

			ANM1=`echo $AJD | awk -F- '{print $1}' | awk '{print substr($1,3,2)}'`
			ANM2=`echo $AJD | awk -F- '{print $2}'`
			ANM3=`echo $ATM | awk -F: '{print $1}'`
			ANM=$ANM1.$ANM2.$ANM3

			BNM1=`echo $BJD | awk -F- '{print $1}' | awk '{print substr($1,3,2)}'`
			BNM2=`echo $BJD | awk -F- '{print $2}'`
			BNM3=`echo $BTM | awk -F: '{print $1}'`
			BNM=$BNM1.$BNM2.$BNM3

			#Judge whether the time series spread over two continuous records.
			if [[ $ANM == $BNM ]]; then
				if [[ ! -z `ls $DIR/$ANM*$kstDAS* 2>/dev/null` ]]; then
					#Copy SAC_DATA to SAC_EV/EV/ Directory
					cp $DIR/$ANM*$kstDAS* $FILENM
#Sac Processing ###################################################
sac > /dev/null << EOF || echo "Fatal Err: Insufficient length $DIR/$ANM*$kstDAS*" ||  exit
r $FILENM/$ANM*$kstDAS*
#Remove mean value and linear trace 
rmean
rtr
taper
#Assign Header info
ch kevnm $evNM evla $evla evlo $evlo evdp $evdp mag $evMAG a -12345
#Assign time info
ch o  gmt $evY  $evD  $evH  $evM  $evS  $evMS
ch t1 gmt $artY $artD $artH $artM $artS $artMS
w over
q
EOF

#Cut time 
sac > /dev/null << EOF
cuterr fatal; cut t1 $A $B
r $FILENM/$ANM*$kstDAS*.sac
w over
q
EOF
					echo "echo $stLINE $VERBOSE" | sh
				fi
			else
			#Merge records
				KMERGE=0
				if [[ ! -z `ls $DIR/$ANM*$kstDAS* 2>/dev/null` &&  ! -z `ls $DIR/$BNM*$kstDAS* 2>/dev/null` ]]; then
					KMERGE=1
					echo -e "Note: Merge happen! Time Series spread over two kinds of file:\n$DIR/$ANM*$kstDAS*.[123].sac $DIR/$BNM*$kstDAS*.[123].sac\n"
					#Copy SAC_DATA to SAC_EV/EV/ Directory
					cp $DIR/$ANM*$kstDAS* $FILENM
					cp $DIR/$BNM*$kstDAS* $FILENM
#Sac Processing ###################################################
sac > /dev/null << EOF
#Merge date
r $FILENM/$ANM*$kstDAS*.1.sac $FILENM/$BNM*$kstDAS*.1.sac
merge
w over
r $FILENM/$ANM*$kstDAS*.2.sac $FILENM/$BNM*$kstDAS*.2.sac
merge
w over
r $FILENM/$ANM*$kstDAS*.3.sac $FILENM/$BNM*$kstDAS*.3.sac
merge
w over	
#	
r $FILENM/$ANM*$kstDAS*.sac
#Remove mean value and linear trace 
rmean
rtr
taper
#Assign Header info
ch kevnm $evNM evla $evla evlo $evlo evdp $evdp mag $evMAG a -12345
#Assign time info
ch o  gmt $evY  $evD  $evH  $evM  $evS  $evMS
ch t1 gmt $artY $artD $artH $artM $artS $artMS
w over
q
EOF

#Cut time 
sac > /dev/null <<EOF
cuterr fatal; cut t1 $A $B
r $FILENM/$ANM*$kstDAS*.sac
w over
q
EOF
					#Remove unnecessary file
					rm $FILENM/$BNM*$kstDAS*.[123].sac
					echo "echo $stLINE $VERBOSE" | sh
				fi

				#$DIR/$BNM*$kstDAS*.[123].sac Don't exist
				if [[ $KMERGE == 0 && ! -z `ls $DIR/$ANM*$kstDAS* 2>/dev/null` ]]; then
					echo -e "Warning: Time Series spread over two kinds of file: \n$DIR/$ANM*$kstDAS*.[123].sac $DIR/$BNM*$kstDAS*.[123].sac\nBUT $DIR/$BNM*$kstDAS*.[123].sac DON'T exist!"

					#					cp $DIR/$ANM*$kstDAS* $FILENM
					#sac > /dev/null  << EOF
					#r $DIR/$ANM*$kstDAS*
					#ch kevnm $evNM evla $evla evlo $evlo evdp $evdp mag $evMAG a -12345
					#ch o gmt $evY $evD $evH $evM $evS $evMS
					#ch t1 gmt $artY $artD $artH $artM $artS $artMS
					#w over
					#q
					#EOF
					#					echo "echo $stLINE $VERBOSE" | sh
				fi
				#$DIR/$ANM*$kstDAS*.[123].sac Don't exist
				if [[ $KMERGE == 0 && ! -z `ls $DIR/$BNM*$kstDAS* 2>/dev/null` ]]; then
					echo -e "Warning: Time Series spread over two kinds of file: \n  $DIR/$ANM*$kstDAS*.[123].sac $DIR/$BNM*$kstDAS*.[123].sac\n  BUT $DIR/$ANM*$kstDAS*.[123].sac DON'T exist!"
					#					cp $DIR/$BNM*$kstDAS* $FILENM
					#sac > /dev/null << EOF
					#r $DIR/$BNM*$kstDAS*
					#ch kevnm $evNM evla $evla evlo $evlo evdp $evdp mag $evMAG a -12345
					#ch o gmt $evY $evD $evH $evM $evS $evMS
					#ch t1 gmt $artY $artD $artH $artM $artS $artMS
					#w over
					#q
					#EOF
					#					echo "echo $stLINE $VERBOSE" | sh
				fi
			fi
		fi

	done < tmpsta
	echo "echo \#E\#$VERBOSE" | sh
	#Remove empty directory and make new ones with sufname "Empty" 
	rmdir $FILENM 2>&- && mkdir $FILENM"Empty"
done < tmpevt


rm tmpevt tmpsta
