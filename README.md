Package of Seismic Field Works(PSFW)
=====================================================
#1. What is PSFW
Facing big data avalanche, automated tools are required in substitute some manual processing routines. And the Package of Seismic Field Work(PSFW), aims to save those who are struggling with seimic data processing.

PSFW is not a single program, but a set of independent programs, built in the Unix programming philosophy.

PSFW involves works of other, some of which are revised slightly under their license.

#2. Tools
Two categories of tools are: 1. binary, 2.script. 'binary' provides users precise configuration, while 'script' for easy usage.
## binary:
- `GMTime`        Transform date format and time shift
- `DTime`         Calculate time interval given two time points
- `GCDis`         Calculate great circle distance and direction
- `spDis`         Calculate spherical distance from a point to a line
- `RpKmean`       K mean clusting
- `clusterRPBaz`  Clusting accroding to RayParameter and BackAzimuth
- `NRootStack`    N-root stacking
- `pl_derfmod`    Generate vp/vs model and deviations for GMT plot
- `SelectEq_ISC`  Select earthquake events from ISC catalogue
- `cut4Ev`        Customized time series cut given database and reference time interval

## script:
cmpltSac4Ev.sh 
- `cmpltSac.sh`     Complete event info into sac header
- `cmpltSac4St.sh`  Complete station info into sac header
- `pkSt.sh`         Select Stations
- `rpCluster.py`    Clustering accroding to RayParameter and BackAzimuth
- `SH_Ref2Segy.sh`  Batch transforming reftek format data to segys
- `SH_Segy2Sac.sh`  Batch transforming segy format data to sac format
- `pkEv.sh`         A shell to cut4Ev, for customized data cutting
- `rcvfn.sh`        Batch distilling receiver functions 
- `SH_SortSegy.sh`  Sort segy format data
- `SH_SortSac.sh`   Sort sac format data
