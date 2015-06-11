#!/bin/bash
HMSG="
Usage: rcvfn.sh -D Directory -F Frequency [-V]\n
\n
Description: Given a \"Directory\" and lowpass corner \"Frequency\",\n
rcvfn.sh will calculate all the receiver functions using iterative \n
deconvolution. All the resulted receiver functions and original data\n
within \"Directory\" are placed in \$SAC_Rcvfn\$ directory.\n
[-V] option is used to output log.\n
\n
~~~~~~~~~~~~~~~~Example Tree of Directory:~~~~~~~~~~~~~~~~~~\n
SAC_ST/\n
├── 0001SHZ\n
├── 0002LJZ\n
├── 0003SJT\n
├── ...\n
\n
Eg:\n
rcvfn.sh -D SAC_ST -F 1 -V\n
\n
Wangsheng IGG-CAS\n
wangsheng.cas@gmail.com"

VERBOSE=" > /dev/null"
while  getopts  "D:F:V"  arg #选项后面的冒号表示该选项需要参数
do
         case  $arg  in
             D)
                DIR=$OPTARG
                ;;
             V)
                VERBOSE=""
                ;;
             F)
				freq=$OPTARG
				;;
             ?)  #当有不认识的选项的时候arg为 ?
            	#echo  " unkonw argument "
        		exit  1
        		;;
        esac
done

if [[ -z $DIR || -z $freq ]]; then
	echo -e $HMSG 
	exit 1
fi

rm -r -f SAC_Rcvfn 2>&-
cp -a $DIR SAC_Rcvfn
DIR="SAC_Rcvfn"
#Acquire all the Station sub-directory
ls $DIR > dirlist.tmp
echo $DIRLIST

while read SUBDIR;do
	echo "echo \#1\#$DIR/$SUBDIR $VERBOSE" | sh
	#Acquire all the SAC_BHZ_file in this station sub-directory
	ls $DIR/$SUBDIR/*.1.sac > filelist.tmp
	while read file; do
		dataz=$file
		data=`echo $file| awk '{print substr($1,0,match($1,".1.sac")) }'`
		datan=$data.2.sac
		datae=$data.3.sac
		#cp $dataz $datan $datae SAC_Rcvfn/$SUBDIR
		datar=$data.r
		datat=$data.t
		rcvfR=$data.ri
		rcvfT=$data.ti
		echo "echo  \"$data.[123rt].sac --> $data.[rt]i $VERBOSE\"" | sh

#Rotate to acquire R and T components
sac > /dev/null << EOF
r $datan $datae
rotate to gcp
ch file 1 kcmpnm R
ch file 2 kcmpnm T
w $datar $datat
q
EOF
#Acquire Vertical compenent
#cp $dataz $data.z

sac > /dev/null <<EOF
r $dataz $datar $datat
rmean;rtr;taper
lowpass BU CORNER $freq N 2
w over
q
EOF
	iter_decon -F1/3/-5 -N50 -C1/-10/30 -T0.1 $dataz $datar $datat 2>&-
sac > /dev/null <<EOF
r $rcvfR $rcvfT
ch nzyear 2015 nzjday 0 nzhour 0 nzmin 0 nzsec 0 nzmsec 0
w over
q
EOF

	done < filelist.tmp
	echo "echo \#E\# $VERBOSE" | sh
done < dirlist.tmp

rm *.tmp -r -f
#./iter_decon -F1/3/-5 -N100 -C1/-10/80 -T0.1 example/KUL.z example/KUL.[r,t]