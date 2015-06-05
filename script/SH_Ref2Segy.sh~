#!/bin/bash
#  Version 2015.05.07  
#  This bash script is used to transform REF file to SEGY file.
#  Wangsheng
#  2015-05-07
#
#  Version 2015.05.08
#  Add getopts(-D -C -V) functions.
#  Wangsheng
#  2015-05-08
HMSG="Usage: SH_Ref2Segy.sh -D Directory [-C Casfile] [-V]"
VERBOSE=" > /dev/null"
while  getopts  "D:C:V"  arg #选项后面的冒号表示该选项需要参数
do
    case  $arg  in
        D)
            DIR=$OPTARG
            #echo  \#Directory: $DIR
            ;;
        C)
            CASFILE=$OPTARG
            #echo  \#Cas config file: $CASFILE
            ;;
        V)
            VERBOSE=""
            ;;
        ?)
        	#echo  " unkonw argument "
    		exit  1
    ;;
    esac
done

if [[ -z $DIR ]]; then
	echo $HMSG
	exit 1
fi

PWD=`pwd`
ln -s $DIR     ./TMPDIR
ln -s $CASFILE ./TMPCAS

if [[ -a ./SEGY ]]; then
    rm ./SEGY/*
else
    mkdir ./SEGY
fi
cd ./SEGY

i=0;
j=0;
#DIR="./Data_TC"
#CASFILE="cas.file"
for STA in `ls ../TMPDIR`; do
	i=`expr $i + 1`
	for DATE in `ls ../TMPDIR/$STA`; do
		for REF in  `ls ../TMPDIR/$STA/$DATE`; do
			for FILE in `ls ../TMPDIR/$STA/$DATE/$REF/1`; do
				j=`expr $j + 1`
				ref2segy -f ../TMPDIR/$STA/$DATE/$REF/1/$FILE -l ../TMPCAS > /dev/null 2>/dev/null
			 	echo "echo  $i $STA $j:  ../TMPDIR/$STA/$DATE/$REF/1/$FILE FINISHED! $VERBOSE" | sh
			done
		done
	done
done

cd $PWD
rm ./TMPDIR ./TMPCAS -f
