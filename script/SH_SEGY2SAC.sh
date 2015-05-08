#!/bin/bash
#  Version 2015.05.07  
#  This bash script is used to transfrom SEGY file to SAC file.
#  Wangsheng
#  2015-05-07
Pendulum="9EA1 9F47 9FDA 9F40 911E 9FDE 9EE4 9F58 912A"
i=1
while [[ $i -le 9 ]];
do
	PEND=`echo $Pendulum | awk -F' ' '{print $'$i'}'`
	echo $PEND
	rm SAC_$PEND -r
	mkdir SAC_$PEND
	cd SEGY_$PEND
		segy2sac *
		mv *.sac ../SAC_$PEND
	cd ..
	i=`expr $i + 1`
done