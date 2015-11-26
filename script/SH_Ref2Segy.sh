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
HMSG="Usage: SH_Ref2Segy.sh -D Directory  [-C Casfile] [-O Directory] [-V]\n\
Description:\n\
-D Directory   The directory that contains station directories, which is copyed from the station.\n\
-C casfile     Necessary for \'ref2segy\' command. \n\
-O Directory   Ouput directory for SEGY files. Default is [./SEGY]\n\
-V             Turn on the verbose.\n\
"
VERBOSE=" > /dev/null"
SEGY="./SEGY"
while  getopts  "D:C:O:V"  arg #选项后面的冒号表示该选项需要参数
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
        O)
            SEGY=$OPTARG
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
	echo -e $HMSG
	exit 1
fi


ln -s $CASFILE   TMPCAS

mkdir ${SEGY} 2>&- || rm ${SEGY}/* -r -f 2>&-
mkdir ${SEGY}.log  2>&-



mkdir ref2segy_tmp
i=0;
j=0;
PWD_=`pwd`
cd ref2segy_tmp
#DIR="./Data_TC"
#CASFILE="cas.file"
for STA in `ls ../${DIR}`; do
	i=`expr $i + 1`
    #echo $STA
    mkdir ../${SEGY}/${STA}
	for DATE in `ls ../${DIR}/$STA`; do
		for REF in  `ls ../${DIR}/$STA/$DATE`; do
			for FILE in `ls ../${DIR}/$STA/$DATE/$REF/1`; do
				j=`expr $j + 1`
				ref2segy -f ../${DIR}/$STA/$DATE/$REF/1/$FILE -l ../TMPCAS > /dev/null 2>/dev/null
			 	echo "echo  $i $STA $j:  ../${DIR}/$STA/$DATE/$REF/1/$FILE FINISHED! $VERBOSE" | sh
			done
		done
	done
    mv R*/* ../${SEGY}/${STA}
    rm R* -rf
    mv *    ../${SEGY}.log
done

cd $PWD_
rm ./TMPCAS ref2segy_tmp -rf
