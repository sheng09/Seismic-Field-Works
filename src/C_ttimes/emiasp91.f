      subroutine emdlv(r,vp,vs)
c         set up information on earth model (specified by
c         subroutine call emiasp)
c         set dimension of cpr,rd  equal to number of radial
c         discontinuities in model
      save
      character*(*) name
      character*20 modnam
      dimension cpr(11)
      common/emdlc/np,rd(11)
      data np,rd/11,1217.1,3482.0,3631.,5611.,5711.,5961.,6161.,
     1 6251.,6336.,6351.,6371./,rn,vn/1.5696123e-4,6.8501006/
      data modnam/'iasp91'/
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
      parameter (nlay=13,nlayp1=nlay+1)
      dimension r(nlayp1),d(nlay,4),p(nlay,4),s(nlay,4)
      data r/0.      ,1217.1  ,3482.0  ,3631.  ,5611.   ,5711.   ,
     1 5961.   ,6161.   ,6251.   ,6336.   ,6351.    ,6371.    ,
     2 6371.,6371./
      data ((d(i,j),j=1,4),i=1,nlay)/
     1 13.01219,  0., -8.45292, 0.,
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
      data p/11.24094,10.03904,14.49470,25.1486 ,25.969838,29.38896,
     1 30.78765,25.41389, 8.785412, 6.5   , 5.8   ,2*0.,
     2        0.   , 3.75665, -1.47089,-41.1538, -16.934118,-21.40656,
     2-23.25415,-17.69722,-0.7495294, 4*0.,
     3       -4.09689,-13.67046, 0.0  ,51.9932,9*0.,
     4        0.     , 0.      , 0.     ,-26.6083,9*0./
      data ((s(i,j),j=1,4),i=1,nlay)/
     1  3.56454 ,   0.      , -3.45241,   0.,
     2  0.      ,   0.      ,  0.     ,   0.,
     3  8.16616 ,  -1.58206 ,  0.     ,   0.,
     4 12.9303  , -21.2590  , 27.8988 , -14.1080, 
     5 20.768902, -16.531471,  0.     ,   0.,
     6 17.70732 , -13.50652 ,  0.     ,   0.,
     7 15.24213 , -11.08553 ,  0.     ,   0.,
     8  5.750203,  -1.274202,  0.     ,   0.,
     9  6.706232,  -2.248585,  0.     ,   0.,
     A  3.75    ,   0.      ,  0.     ,   0.,
     B  3.36    ,   0.      ,  0.     ,   0.,
     C  0.      ,   0.      ,  0.     ,   0.,
     D  0.      ,   0.      ,  0.     ,   0. /
      data xn,rn,vn/6371.,.18125793,.14598326/,i/1/
c
      x=amax1(x0,0.)
      x1=xn*x
 2    if(x1.ge.r(i)) go to 1
      i=i-1
      go to 2
 1    if(x1.le.r(i+1).or.i.ge.11) go to 3
      i=i+1
      if(i.lt.11) go to 1
 3    ro=rn*(d(i,1)+x*(d(i,2)+x*(d(i,3)+x*d(i,4))))
      vp=vn*(p(i,1)+x*(p(i,2)+x*(p(i,3)+x*p(i,4))))
      vs=vn*(s(i,1)+x*(s(i,2)+x*(s(i,3)+x*s(i,4))))
      return
      end
