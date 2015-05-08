#!/bin/bash
#  Version 2015.05.07  
#  This bash script is used to transform REF file to SEGY file.
#  Wangsheng
#  2015-05-07
i=0;
j=0;
DIR="./Data_TC"
CASFILE="cas.file"

for STA in `ls $DIR`; do
	i=`expr $i + 1`
	for DATE in `ls $DIR/$STA`; do
		for REF in  `ls $DIR/$STA/$DATE`; do
			for FILE in `ls $DIR/$STA/$DATE/$REF/1`; do
				j=`expr $j + 1`
				ref2segy -f $DIR/$STA/$DATE/$REF/1/$FILE -l $CASFILE > /dev/null 2>/dev/null
			 	echo  "$i $STA $j:  $DIR/$STA/$DATE/$REF/1/$FILE FINISHED!"
			done
		done
	done	
	
done

