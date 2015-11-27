Package for Seismic Field Works(PSFW)
=====================================================
#1. Hello World!
Seismic data processing is complicated for big data avalanche, complex interaction among different data and the balance between effiency and accuracy. An high level automated standard is required, and this demand stimulates the birth of different kind of packages and toolkits. This Package, called PSFW, attempt to provide a concise and effective experience to anyone who in need in seimic data processing.

All the algorithms and program realization of PSFW by different kind of programming language are built step by step, thus modification is inevitable and everything will be out of date from the long view. The built in philosophy of this toolkit are "independent", "concise", "effective" and "application-oriented".

PSFW involves works of other, some of which are revised slightly under their license. These great works from pioneers are listed at the end. Thanks for their built in free spirit of GNU.


#2. Tools
##2.1 Data truncation
Seismic field observation provides us continuous data but we sometims need the trace related to events or special time period. <font color=blue>cut4Ev </font> enables data truncation by generate a sac script given continous data information and required time interval.
##2.2 N-Root stacking
<font color=blue>NRootStack </font> is used to do stacking for any number of sac data.
##2.3 Cluster
Clustering is common in seismic processing according to ray parameter or (back) azimuth angles. <font color=blue>clusterRPBaz </font> provides equalized clustering for sac data. However, real data never shows a even distribution but always concentrate for seismic active area or dense station coverage. <font color=blue>RpKmean </font> employ K-mean cluster algorithm to generate a rational clustering.
##2.4 Time processing
<font color=blue>GMTime</font> provides transformation between YYY/MM/DD and YYYY/DDD, and time shiftting.
<font color=blue>DTime</font> is used to calculate time interval of two time points.
##2.5 Spherical geometry calculation
<font color=blue>GCDis</font> is used to calculate great circle distance, azimuth angle and back azimuth angle.
<font color=blue>spDis</font> is used to calculate spherical distance from a point to a "spherical line".

<!#3. Receiver Function Processing!>
<!##2.1 pickRf!>
<!Determine an RF is good or bad according to obvious criterias, including eing postive around ZERO", "P phase must be dominant", and so on.!>
<!##2.2 pickRf_WV!>
<!Given a number of RFs and a reference RF, determine which is good or bad according to their similarity to the reference RF. The reference RF comes from input or just the stacking result of all the RFs.!>



