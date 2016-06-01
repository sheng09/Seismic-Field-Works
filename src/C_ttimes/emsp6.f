      subroutine emdlv(r,vp,vs)
c         set up information on earth model (specified by
c         subroutine call emiasp)
c         set dimension of cpr,rd  equal to number of radial
c         discontinuities in model
      save
      character*(*) name
      character*20 modnam
      parameter (nd=11)
      dimension cpr(nd)
      common/emdlc/np,rd(nd)
      data np,rd/11,1215.,3480.,3630.,5600.,5711.,5961.,6161.,
     1 6251.,6336.,6351.,6371./,
     2 rn,vn/1.5696123e-4,6.8501006/
      data modnam/'sp6'/
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
c   Emiask returns model parameters for the SP6 working model 
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
      parameter (nlay=12,nlayp1=nlay+1)
      dimension r(nlayp1),d(nlay,4),p(nlay,4),s(nlay,4)
      data r/0.      ,1215.   ,3480.   ,3630.  ,5600.   ,5711.   ,
     1       5961.   ,6161.   ,6251.   ,6336.  ,6351.   ,6371.  ,
     2 6371. /
c
c    Density - not supplied with SP6, but stolen from AK135.
      data ((d(i,j),j=1,4),i=1,nlay)/
     1 13.01217,  0., -8.45095, 0.,
     2 12.58923, -1.75061, -1.78105, -7.27099, 
     3 12.84645,  1.36611,  0.,       0.,
     4 23.61837,-35.52920, 45.20724,-23.92870,
     5  6.8143 , -1.66273, -1.18531,  0.,
     6 11.11978, -7.87054,  0.,       0.,
     7  7.15855, -3.85999,  0.,       0.,
     8  7.15855, -3.85999,  0.,       0.,
     9  7.15855, -3.85999,  0.,       0.,
     A  2.92,     0.,       0.,       0.,
     B  2.72,     0.,       0.,       0.,
     C  0.,       0.,       0.,       0. /
c
c  Vp
      data ((p(i,j),j=1,4),i=1,nlay)/
     1 11.29719,  0.,      -8.88699, 0.,
     2 11.31616, -7.09314, 15.75426,-25.70488, 
     3 12.84645,  1.36611,  0.,       0.,
     4 23.61837,-35.52920, 45.20724,-23.92870,
     5 26.01542,-17.00747,  0.,       0.,
     6 29.39809,-21.40010,  0.     ,  0.,
     7 30.78588,-23.25239,  0.     ,  0.,
     8 25.40956,-17.69281,  0.     ,  0.,
     9  8.78541, -0.74953,  0.     ,  0.,
     a  6.5    ,  0.,       0.     ,  0.,
     b  5.8    ,  0.     ,  0.     ,  0.,
     c  0.     ,  0.     ,  0.     ,  0. /
c
c  Vs
      data ((s(i,j),j=1,4),i=1,nlay)/
     1  3.6678  ,   0.      , -4.44749,   0.,
     2  0.      ,   0.      ,  0.     ,   0.,
     3  5.65120 ,   2.78686 ,  0.     ,   0.,
     4 11.87772 , -17.43557 , 23.32985, -12.31633, 
     5 17.57267 , -12.92378 ,  0.     ,   0.,
     6 17.72032 , -13.49239 ,  0.     ,   0.,
     7 15.24313 , -11.08653 ,  0.     ,   0.,
     8  5.75198 ,  -1.27602 ,  0.     ,   0.,
     9  6.70623 ,  -2.24858 ,  0.     ,   0.,
     a  3.75    ,   0.      ,  0.     ,   0.,
     b  3.36    ,   0.      ,  0.     ,   0.,
     c  0.      ,   0.      ,  0.     ,   0. /
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
