      subroutine emdlv(r,vp,vs)
c         set up information on earth model (specified by
c         subroutine call emiasp)
c         set dimension of cpr,rd  equal to number of radial
c         discontinuities in model
      parameter (ndisc=13)
      save
      character*(*) name
      character*20 modnam
      dimension cpr(ndisc)
      common/emdlc/np,rd(ndisc)
      data np,rd/ndisc, 1221.5,3430.,3480.0,
     1 3630.,5600.,5701.,5771.,5971.,
     1 6161.,6251.,6336.,6351.,6371./,
     2 rn,vn/1.5696123e-4,6.8501006/
      data modnam/'kghj'/
c
      call emiask(rn*r,rho,vp,vs)
      vp=vn*vp
      vs=vn*vs
      return
c
      entry emdld(n,cpr,name)
      n=np
      do 1 i=1,np
 1    cpr(i)=rd(i)
      name=modnam
      return
      end
c
      subroutine emiask(x0,ro,vp,vs)
c
c $$$$$ calls no other routine $$$$$
c
c   Emiask returns model parameters for the IASPEI working model 
c   (September 1990.1).  
c   Given non-dimensionalized radius x0, emiasp returns
c   non-dimensionalized density, ro, compressional velocity, vp, and
c   shear velocity, vs.  Non-dimensionalization is according to the
c   scheme of Gilbert in program EOS:  x0 by a (the radius of the
c   Earth), ro by robar (the mean density of the Earth), and velocity
c   by a*sqrt(pi*G*robar) (where G is the universal gravitational
c   constant.
c
c
      save
      parameter (nlay=14,nlayp1=nlay+1)
      dimension r(nlayp1),d(nlay,4),p(nlay,4),s(nlay,4)
c
c  For FAKEPREM
c         Center to 210km:   PREM;   
c         210km to surface:    iasp91
c
      data r/0.      ,1221.5  ,3430., 3480.0  ,
     1 3630.  ,5600.   ,5701.   ,
     1 5771.,  5971. ,6161.   ,6251.   ,6336.  ,6351.   ,6371.  ,
     2 6371. /
c                 iasp91
c     data r/0.      ,1217.1  ,3482.0  ,3631.  ,5611.   ,5711.   ,
c    1 5961.   ,6161.   ,6251.   ,6336.   ,6351.    ,6371.    ,
c    2 6371.,6371./
c    
c   Rho:   iasp91
      data ((d(i,j),j=1,4),i=1,nlay)/
     1 13.01219,  0., -8.45292, 0.,
     2 12.58416, -1.69929, -1.94128, -7.11215, 
     2 12.58416, -1.69929, -1.94128, -7.11215, 
     3  6.8143 , -1.66273, -1.18531,  0.,
     4  6.8143 , -1.66273, -1.18531,  0.,
     5  6.8143 , -1.66273, -1.18531,  0.,
     6 11.11978, -7.87054,  0.,       0.,
     7  7.15855, -3.85999,  0.,       0.,
     8  7.15855, -3.85999,  0.,       0.,
     9  7.15855, -3.85999,  0.,       0.,
     A  2.92,     0.,       0.,       0.,
     B  2.72,     0.,       0.,       0.,
     C  0.,       0.,       0.,       0.,
     D  0.,       0.,       0.,       0. /
c
c  Vp   1-8: PREM;    9-c (surface to 210km): iasp91 *0.9954 (0.46% slow) 
c                     to match isotropic PREM PKiKP (h=0km) at delta 1.0 degree.
c                     See Dziewonski & Anderson (1981)  p341 Table VIh.
      data ((p(i,j),j=1,4),i=1,nlay)/
     1 11.2622 ,  0.     , -6.3640 ,  0.0   ,
     2 11.0487 , -4.0362 ,  4.8023 ,-13.5732,
     2 19.60929,-19.936942, 4.8023 ,-13.5732,
     3 15.3891 , -5.3181 ,  5.5242 , -2.5514,
     4 24.9520 ,-40.4673 , 51.4832 ,-26.6419,
     5 29.2766 ,-23.6027 ,  5.5242 , -2.5514,
     6 19.0957 , -9.8672 ,  0.     ,  0.,
     7 39.7027 ,-32.6166 ,  0.     ,  0.,
     8 20.3926 ,-12.2569 ,  0.     ,  0.,
     9 25.29623,-17.61529,  0.     ,  0.,
     a  8.74474, -0.74606,  0.     ,  0.,
     b  6.46991,  0.     ,  0.     ,  0.,
     c  5.77314,  0.     ,  0.     ,  0.,
     d  0.     ,  0.     ,  0.     ,  0. /
c
c  Vp   1-8: PREM   9-c: iasp91 
c     data ((p(i,j),j=1,4),i=1,nlay)/
c    1 11.2622 ,  0.     , -6.3640 ,  0.0   ,
c    2 11.0487 , -4.0362 ,  4.8023 ,-13.5732,
c    3 15.3891 , -5.3181 ,  5.5242 , -2.5514,
c    4 24.9520 ,-40.4673 , 51.4832 ,-26.6419,
c    5 29.2766 ,-23.6027 ,  5.5242 , -2.5514,
c    6 19.0957 , -9.8672 ,  0.     ,  0.,
c    7 39.7027 ,-32.6166 ,  0.     ,  0.,
c    8 20.3926 ,-12.2569 ,  0.     ,  0.,
c    9 25.41389,-17.69722,  0.     ,  0.,
c    a  8.78541, -0.74953,  0.     ,  0.,
c    b  6.5    ,  0.     ,  0.     ,  0.,
c    c  5.8    ,  0.     ,  0.     ,  0.,
c    d  0.     ,  0.     ,  0.     ,  0. /
c
c           IASPEI91
c     data ((p(i,j),j=1,4),i=1,nlay)/
c    1 11.24094,  0.     , -4.09689,  0.,
c    2 10.03904,  3.75665,-13.67046,  0.,
c    3 14.49470, -1.47089,  0.     ,  0.,
c    4 25.1486 ,-41.1538 , 51.9932 ,-26.6083,
c    5 25.96984,-16.93412,  0.     ,  0.,
c    6 29.38896,-21.40656,  0.     ,  0.,
c    7 30.78765,-23.25415,  0.     ,  0.,
c    8 25.41389,-17.69722,  0.     ,  0.,
c    9  8.78541, -0.74953,  0.     ,  0.,
c    a  6.5    ,  0.     ,  0.     ,  0.,
c    b  5.8    ,  0.     ,  0.     ,  0.,
c    c  0.     ,  0.     ,  0.     ,  0.,
c    d  0.     ,  0.     ,  0.     ,  0.,
c     data p/11.24094,10.03904,14.49470,25.1486 ,25.969838,29.38896,
c    1 30.78765,25.41389, 8.785412, 6.5   , 5.8   ,2*0.,
c    2        0.   , 3.75665, -1.47089,-41.1538, -16.934118,-21.40656,
c    2-23.25415,-17.69722,-0.7495294, 4*0.,
c    3       -4.09689,-13.67046, 0.0  ,51.9932,9*0.,
c    4        0.     , 0.      , 0.     ,-26.6083,9*0./
c
c  Vs   1-8: PREM   9-c:(from surface to 210km) iasp91*0.98765 (1.28% slow) 
c                   to match isotropic PREM ScS (h=550km) at delta 32.5 degree.
c                   See Dziewonski & Anderson (1981)  p352 Table VIq.
      data ((s(i,j),j=1,4),i=1,nlay)/
     1  3.6678  ,   0.      , -4.4475 ,   0.,
     2  0.      ,   0.      ,  0.     ,   0.,
     2  0.      ,   0.      ,  0.     ,   0.,
     3  6.9254  ,   1.4672  , -2.0834 ,  0.9783 ,
     4 11.1671  , -13.7818  , 17.4575 , -9.2777 , 
     5 22.3459  , -17.2473  , -2.0834 ,  0.9783 ,
     6  9.9839  ,  -4.9324  ,  0.     ,   0.,
     7 22.3512  , -18.5856  ,  0.     ,   0.,
     8  8.9496  ,  -4.4597  ,  0.     ,   0.,
     9  5.679188,  -1.258466,  0.     ,   0.,
     a  6.623410,  -2.220815,  0.     ,   0.,
     b  3.70368 ,   0.      ,  0.     ,   0.,
     c  3.318504,   0.      ,  0.     ,   0.,
     d  0.      ,   0.      ,  0.     ,   0. /
c
c  Vs   1-8: PREM   9-c:(from surface to 210km) iasp91 
c     data ((s(i,j),j=1,4),i=1,nlay)/
c    1  3.6678  ,   0.      , -4.4475 ,   0.,
c    2  0.      ,   0.      ,  0.     ,   0.,
c    3  6.9254  ,   1.4672  , -2.0834 ,  0.9783 ,
c    4 11.1671  , -13.7818  , 17.4575 , -9.2777 , 
c    5 22.3459  , -17.2473  , -2.0834 ,  0.9783 ,
c    6  9.9839  ,  -4.9324  ,  0.     ,   0.,
c    7 22.3512  , -18.5856  ,  0.     ,   0.,
c    8  8.9496  ,  -4.4597  ,  0.     ,   0.,
c    9  5.750203,  -1.274202,  0.     ,   0.,
c    a  6.706232,  -2.248585,  0.     ,   0.,
c    b  3.75    ,   0.      ,  0.     ,   0.,
c    c  3.36    ,   0.      ,  0.     ,   0.,
c    d  0.      ,   0.      ,  0.     ,   0. /
c   Vs        IASPEI91
c     data ((s(i,j),j=1,4),i=1,nlay)/
c    1  3.56454 ,   0.      , -3.45241,   0.,
c    2  0.      ,   0.      ,  0.     ,   0.,
c    3  8.16616 ,  -1.58206 ,  0.     ,   0.,
c    4 12.9303  , -21.2590  , 27.8988 , -14.1080, 
c    5 20.768902, -16.531471,  0.     ,   0.,
c    6 17.70732 , -13.50652 ,  0.     ,   0.,
c    7 15.24213 , -11.08553 ,  0.     ,   0.,
c    8  5.750203,  -1.274202,  0.     ,   0.,
c    9  6.706232,  -2.248585,  0.     ,   0.,
c    A  3.75    ,   0.      ,  0.     ,   0.,
c    B  3.36    ,   0.      ,  0.     ,   0.,
c    C  0.      ,   0.      ,  0.     ,   0.,
c    D  0.      ,   0.      ,  0.     ,   0. /
      data xn,rn,vn/6371.,.18125793,.14598326/,i/1/
c
      x=amax1(x0,0.)
      x1=xn*x
 2    if(x1.ge.r(i)) go to 1
      i=i-1
      go to 2
 1    if(x1.le.r(i+1).or.i.ge.nlay) go to 3
      i=i+1
      if(i.lt.nlay) go to 1
 3    ro=rn*(d(i,1)+x*(d(i,2)+x*(d(i,3)+x*d(i,4))))
      vp=vn*(p(i,1)+x*(p(i,2)+x*(p(i,3)+x*p(i,4))))
      vs=vn*(s(i,1)+x*(s(i,2)+x*(s(i,3)+x*s(i,4))))
      return
      end
