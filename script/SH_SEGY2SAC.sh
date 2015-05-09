#!/bin/bash
#  Version 2015.05.07  
#  This bash script is used to transfrom SEGY file to SAC file.
#  Wangsheng
#  2015-05-07
#
#  Version 2015.05.08
#  Add getopts(-D -P -V) functions.
#  Wangsheng
#  2015-05-08
HMSG="Usage: SH_SEGY2SAC.sh.sh -D Directory -P PendulumCodes [-V]"
VERBOSE=" > /dev/null"
while  getopts  " D:P:V "  arg #选项后面的冒号表示该选项需要参数
do
         case  $arg  in
             D)
                DIR=$OPTARG
                #echo \# Directory: $DIR
                ;;
             P)
                Pendulum=$OPTARG
                ;;
             V)
				VERBOSE=""
				;;
             ?)  #当有不认识的选项的时候arg为 ?
            	echo  " unkonw argument "
        		exit  1
        ;;
        esac
done
if [[ -z $DIR ]] || [[ -z $Pendulum ]]; then
	echo $HMSG
	exit 1
fi
NUM=`echo $Pendulum | awk  '{print NF}'` #Number of Pendulums
#Pendulum="9EA1 9F47 9FDA 9F40 911E 9FDE 9EE4 9F58 912A"
i=1
while [[ $i -le $NUM ]];
do
	PEND=`echo $Pendulum | awk -F' ' '{print $'$i'}'`
	#echo $PEND
	rm SAC_$PEND -r
	mkdir SAC_$PEND
	echo "echo $i $PEND $VERBOSE" | sh
	cd SEGY_$PEND
		#segy2sac *
		cp * $PEND.sac
		mv *.sac ../SAC_$PEND
	cd ..
	i=`expr $i + 1`
done