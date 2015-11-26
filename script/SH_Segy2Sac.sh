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
HMSG="Usage: SH_Segy2Sac.sh.sh -D Directory -O Directory [-V]\n\
-D Directory  The directory that contains station segy directories, which is copyed from the station.\n\
-O Directory  Ouput directory for SAC files. Default is [./SAC]\n\
-V            Verbose
"
SAC="./SAC"
VERBOSE=" > /dev/null"
while  getopts  "D:O:V"  arg #选项后面的冒号表示该选项需要参数
do
         case  $arg  in
             D)
                DIR=$OPTARG
                #echo \# Directory: $DIR
                ;;
             O)
                SAC=$OPTARG
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
	echo -e $HMSG
	exit 1
fi
#ln -s $DIR TMP

mkdir ${SAC} 2>&- || rm ${SAC}/* -rf 2>&-

ls $DIR > tmp.dir
while read STA
do
    mkdir ${SAC}/${STA}
    segy2sac ${DIR}/${STA}/* -d ${SAC}/${STA}
done < tmp.dir
rm tmp.dir
