c   The "j" parameters (1 st line) are intended to be user settable:
c        jsrc   Maximum number of discrete model slowness samples above
c               the maximum source depth of interest.
c               **set in limits.inc**
c        jseg   Maximum number of different types of travel-times
c               considered.
c               **set in limits.inc**
c        jbrn   Maximum number of different travel-time branches to be
c               searched.
c               **set in limits.inc**
c        jout   Maximum length of all travel-time branches strung
c               together.
c               **set in limits.inc**
c        jtsm   Maximum length of the tau depth increments.
c               **should be the same as jslo in limits.inc**
c        jxsm   Maximum number of x-values needed for the depth
c               increments.
c        jbrnu  Maximum length of the up-going branches.
c        jbrna  Maximum length of branches which may need
c               re-interpolation.
c
      include 'limits.inc'
      parameter(jxsm=jbrn,jbrnu=jbrn,jbrna=jbrn)
c   A few derived parameters are also needed.
      parameter(jtsm=jslo,jrec=jtsm+jxsm,jtsm0=jtsm+1)
