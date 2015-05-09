#!/bin/bash
# Version 2015-05-09
# Copyed from 'SortSegy.sh'
# Wangsheng
 
HMSG="Usage: SH_SortSac.sh -D Dircetory -S String [-V]"
VERBOSE=" > /dev/null"
while  getopts  "D:S:V"  arg
do
    case  $arg  in
        D)
            DIR=$OPTARG
            ;;
        S)
            STR=$OPTARG
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

if [[ -z $DIR ]] || [[ -z $STR ]]; then
	echo $HMSG
	exit 1
fi


NUM=`echo $STR | awk  '{print NF}'` #Number of Keywords

ln -s $DIR ./TMP
#STR="9EA1 9F47 9FDA 9F40 911E 9FDE 9EE4 9F58 912A"
i=1
while [[ $i -le $NUM ]];
do
    KEYWORDS=`echo $STR | awk -F' ' '{print $'$i'}'`
    
    if [[ -a SAC_$KEYWORDS ]]; then
        rm SAC_$KEYWORDS/*
    else
        mkdir SAC_$KEYWORDS
    fi
    
    echo "echo $i $KEYWORDS" | sh
    i=`expr $i + 1`

    cp TMP/*$KEYWORDS* SAC_$KEYWORDS/
done

rm TMP -r