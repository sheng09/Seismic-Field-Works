#!/bin/bash

#####
#    0. prepare eventfile of "event.lst"
#####

#####
#    1.generate sacinfo file
#####
saclst kstnm knetwk kzdate kztime b e stla stlo stel stdp f dat/*.BHZ | sed 's/.BHZ//g' | sort -n > sacinfo

#####
#    2. run cut4Ev and generate "sacCMD.sh"
#####
../../src/cut4Ev -CT/-200/3600 -G30/90 -M2.0/10.0 -Eevent.lst -Dsacinfo -PTEST -TBH  -V

#####
#    3. run "sacCMD.sh" to cut data
#####
bash ./sacCMD.sh

#####
#    4. Plot to "exam_cut4Ev.pdf".
#####
sac >/dev/null <<EOF
r ./all.sac _cut4Ev_out/*BHZ
color on inc on
#qdp off
p2
saveimg exam_cut4Ev.pdf -f
ed x
q
EOF


rm sacCMD.sh