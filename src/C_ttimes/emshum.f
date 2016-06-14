      subroutine emdld(n,cpr,name)
      parameter (re=6371.0,nd=6)
      dimension cpr(nd),rd(nd)
      character*(*) name
      character*20 modnam
      logical first
      dimension cpr(nd),rd(nd)
      data rd/0.,410.,660.,2740.,
     +   2889.0,5153.9/
      data modnam/'shum'/, first/.true./
      if (first) then
	 first = .false.
	 do 10 i=1,nd
	    rd(i) = re-rd(i)
10       continue
      endif
      n=nd
      do 1 i=1,nd
 1    cpr(i)=rd(nd-i+1)
      name=modnam
      return
      end

      subroutine emdlv(r,vp,vs)
      call vshum(r,vp,vs,rho)
      end
 
      subroutine vjb(r,vp,vs,ro)
      parameter (re=6371.0)
      call vshum(re-r,vp,vs,ro)
      end
      subroutine vshum(x0,vp,vs,ro)
c
c $$$$$ calls no other routine $$$$$
c
c   Emiask returns model parameters for the IASPEI working model 
c   (September 1990.1) modified by G. Abers 1-D velocity model for the
c   Shumagins.  
c
c   Assumes:
c      x0 - radis (km)
c
c   Returns:
c      rho - density (g/cm3)
c      vp, vs - p, s velocity (km/sec)

      parameter (nlay=15,nlayp1=nlay+1)
      save
      dimension r(nlayp1),d(nlay,4),p(nlay,4),s(nlay,4),ii(nlay)
      data r/
     1 0.,
     2 1217.1,
     3 3482.0,
     4 3631.,
     5 5611.,
     6 5711.,
     7 5961.,
C      210 km
     8 6161.,
C      145 km
     9 6226.,
C      75 km
     A 6296.,
C      40 km
     B 6331.,
C      30 km
     C 6341.,
C      20 km
     D 6351.,
C      10 km
     E 6361.,
C      0 km
     F 6371.,
     G 6371./

C     ii specifies the type of interpolation to use:
C        0 for polynomial, 1 for interval
       data ii / 7*0, 8*1/

C     Density for this model isn't right.
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
     D  0.,       0.,       0.,       0.,
     E  0.,       0.,       0.,       0.,
     F  0.,       0.,       0.,       0. /

      data ((p(i,j),j=1,4),i=1,nlay)/
     1 11.24094   ,   0.       ,  -4.09689,  0.,
     2 10.03904   ,   3.75665  , -13.67046,  0.,
     3 14.49470   ,  -1.47089  ,   0.,       0.,
     4 25.1486    , -41.1538   ,  51.9932, -26.6083, 
     5 25.969838  , -16.934118 ,   0.,       0.,
     6 29.38896   , -21.40656  ,   0.,       0.,
     7 30.78765   , -23.25415  ,   0.,       0.,
C      145-210
     8  8.30      ,   8.30      ,   0.,       0.,
C       75-145
     9  8.03      ,   8.30      ,   0.,       0.,
C       40-75
     A  7.66      ,   8.03      ,   0.,       0.,
C       30-40
     B   7.32     ,   7.66      ,   0.,       0.,
C       20-30
     C   6.92     ,   7.32      ,   0.,       0.,
C       10-20
     D   6.56     ,   6.92      ,   0.,       0.,
C        0-10
     E   6.21     ,   6.56      ,   0.,       0.,
     F 0.       ,   0.,          0.,       0. /

      data ((s(i,j),j=1,4),i=1,nlay)/
     1   3.56454 ,    0.        , -3.45241,   0.,
     2   0.      ,    0.        ,  0.     ,   0.,
     3   8.16616 ,   -1.58206   ,  0.     ,   0.,
     4  12.9303  ,  -21.2590    , 27.8988 , -14.1080, 
     5  20.768902,  -16.531471  ,  0.     ,   0.,
     6  17.70732 ,  -13.50652   ,  0.     ,   0.,
     7  15.24213 ,  -11.08553   ,  0.     ,   0.,
     8   4.522   ,    4.522     ,  0.     ,   0.,
     9   4.522   ,    4.522     ,  0.     ,   0.,
     A   4.38    ,    4.522     ,  0.     ,   0.,
     B   4.11    ,    4.38      ,  0.     ,   0.,
     C   3.89    ,    4.11      ,  0.     ,   0.,
     D   3.75    ,    3.89      ,  0.     ,   0.,
     E   3.31    ,    3.75      ,  0.     ,   0.,
     F   0.      ,    0.        ,  0.     ,   0. /
      data rn/6371./,i/1/
c
      x1 = min(rn,max(x0,0.))
      x = x1/rn
 2    if(x1.ge.r(i)) go to 1
      i=i-1
      go to 2
 1    if(x1.le.r(i+1).or.i.ge.nlay) go to 3
      i=i+1
      if(i.lt.nlay) go to 1
 3    continue
      if (ii(i) .eq. 0) then
	 ro=(d(i,1)+x*(d(i,2)+x*(d(i,3)+x*d(i,4))))
	 vp=(p(i,1)+x*(p(i,2)+x*(p(i,3)+x*p(i,4))))
	 vs=(s(i,1)+x*(s(i,2)+x*(s(i,3)+x*s(i,4))))
      else
	 x = 1.0 - (x1-r(i))/(r(i+1)-r(i))
	 ro = d(i,1) + x*(d(i,2)-d(i,1))
	 vp = p(i,1) + x*(p(i,2)-p(i,1))
	 vs = s(i,1) + x*(s(i,2)-s(i,1))
      endif
      ro=0.0
      return
      end
