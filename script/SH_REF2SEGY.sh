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
HMSG="Usage: SH_REF2SEGY.sh -D Dircetory [-C Casfile] [-V]"
VERBOSE=" > /dev/null"
while  getopts  "D:C:V "  arg #选项后面的冒号表示该选项需要参数
do
    case  $arg  in
        D)
            DIR=$OPTARG
            echo  \#Dircetory: $DIR
            ;;
        C)
            CASFILE=$OPTARG
            echo  \#Cas config file: $CASFILE
            ;;
        V)
            VERBOSE=""
            ;;
        ?)
        	echo  " unkonw argument "
    		exit  1
    ;;
    esac
done

if [[ -z $DIR ]]; then
	echo $HMSG
	exit 1
fi
i=0;
j=0;
#DIR="./Data_TC"
#CASFILE="cas.file"
for STA in `ls $DIR`; do
	i=`expr $i + 1`
	for DATE in `ls $DIR/$STA`; do
		for REF in  `ls $DIR/$STA/$DATE`; do
			for FILE in `ls $DIR/$STA/$DATE/$REF/1`; do
				j=`expr $j + 1`
				ref2segy -f $DIR/$STA/$DATE/$REF/1/$FILE -l $CASFILE > /dev/null 2>/dev/null
			 	echo "echo  $i $STA $j:  $DIR/$STA/$DATE/$REF/1/$FILE FINISHED! $VERBOSE" | sh
			done
		done
	done
done

