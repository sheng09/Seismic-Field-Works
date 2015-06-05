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
HMSG="Usage: SH_Segy2Sac.sh.sh -D Directory [-V]"
VERBOSE=" > /dev/null"
while  getopts  "D:P:V"  arg #选项后面的冒号表示该选项需要参数
do
         case  $arg  in
             D)
                DIR=$OPTARG
                #echo \# Directory: $DIR
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

if [[ -z $DIR ]]; then
	echo $HMSG
	exit 1
fi
ln -s $DIR TMP

segy2sac TMP/R*/*

if [[ -a SAC ]]; then
	rm SAC/*
else
	mkdir SAC
fi

mv TMP/R*/*.sac SAC
rm TMP -f
