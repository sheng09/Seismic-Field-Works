      subroutine emdlv(r,vp,vs)
c         set up information on earth model (specified by
c         subroutine call emiasp)
c         set dimension of cpr,rd  equal to number of radial
c         discontinuities in model
      save
c     parameter (np=12)
      parameter (np=11)
      character*(*) name
      character*20 modnam
      dimension cpr(np)
      common /emdlc/rd(np)
      data rd /
     1  1221.5,
     2  3480.0,
     3  3630.0,
     4  5600.0,
     5  5701.0,
     6  5771.0,
     7  5971.0,
     8  6151.0,
c    9  6291.0,
     a  6346.6,
     b  6356.6,
     c  6371./
      data modnam/'prem1s'/
c
      call vprem(r,vp,vs,qp,qs)
      return
c
      entry emdld(n,cpr,name)
      n=np
      do 1 i=1,np
 1    cpr(i)=rd(i)
      name=modnam
      return
      end

      subroutine vprem(r,vp,vs,qp,qs)
C     VPREM  --  Return PREM velocities.
C
C     Called via:
C        call vprem(r,vp,vs,qp,qs)
C
C     Assumes:
C        r - radius in km
C
C     Returns:
C        vp, vs - P and S velocity at that radius in km/sec.
C        qp, qs - P and S attenuation factor at 1 sec.
      
      parameter (re=6371.0, qinf=1e30, qkdef=57823.)
      eval(x,a,b,c,d) = ((d*x + c)*x + b)*x + a
      qavg(qka,qma,vpa,vsa) =
     +   vpa**2 / ((vpa**2-4./3.*vsa**2)/qka + 4./3.*vsa**2/qma)

      rn = r/re
      if (r .lt. 1221.5) then
	 vp = eval(rn,11.2622,0.,-6.3640,0.)
	 vs = eval(rn,3.6678,0.,-4.4475,0.)
	 qs = 84.6
	 qp = qavg(1327.7,84.6,vp,vs)
      else if (r .lt. 3480.0) then
	 vp = eval(rn,11.0487,-4.0362,4.8023,-13.5732)
	 vs = eval(rn,0.,0.,0.,0.)                 
	 qs = qinf
	 qp = qkdef
      else if (r .lt. 3630.0) then
	 vp = eval(rn,15.3891,-5.3181,5.5242,-2.5514)
	 vs = eval(rn,6.9254,1.4672,-2.0834,0.9783)  
	 qs = 312.0
	 qp = qavg(qkdef,312.0,vp,vs)
      else if (r .lt. 5600.0) then
	 vp = eval(rn,24.952,-40.4673,51.4832,-26.6419)
	 vs = eval(rn,11.1671,-13.7818,17.4575,-9.2777) 
	 qs = 312.
	 qp = qavg(qkdef,312.,vp,vs)
      else if (r .lt. 5701.0) then
	 vp = eval(rn,29.2766,-23.6027,5.5242,-2.5514)
	 vs = eval(rn,22.3459,-17.2473,-2.0834,0.9783)
	 qs = 312.
	 qp = qavg(qkdef,312.,vp,vs)
      else if (r .lt. 5771.0) then
	 vp = eval(rn,19.0957,-9.8672,0.,0.)
	 vs = eval(rn,9.9839,-4.9324,0.,0.)
	 qs = 143.
	 qp = qavg(qkdef,143.,vp,vs)
      else if (r .lt. 5971.0) then
	 vp = eval(rn,39.7027,-32.6166,0.,0.)
	 vs = eval(rn,22.3512,-18.5856,0.,0.)
	 qs = 143.
	 qp = qavg(qkdef,143.,vp,vs)
      else if (r .lt. 6151.0) then
C        220
	 vp = eval(rn,20.3926,-12.2569,0.,0.)
	 vs = eval(rn,8.9496,-4.4597,0.,0.)
	 qs = 143.
	 qp = qavg(qkdef,143.,vp,vs)
      else if (r .lt. 6291.0) then
C        80
	 vp = eval(rn,4.1875,3.9382,0.,0.)
	 vs = eval(rn,2.1519,2.3481,0.,0.)
	 qs = 80.
	 qp = qavg(qkdef,80.,vp,vs)
      else if (r .lt. 6346.6) then
C        24 km
	 vp = eval(rn,4.1875,3.9382,0.,0.)
	 vs = eval(rn,2.1519,2.3481,0.,0.)
	 qs = 600.
	 qp = qavg(qkdef,600.,vp,vs)
      else if (r .lt. 6356.6) then
	 vp = eval(rn,6.8,0.,0.,0.)
	 vs = eval(rn,3.9,0.,0.,0.)
	 qs = 600.
	 qp = qavg(qkdef,600.,vp,vs)
      else if (r .le. 6371.0) then
	 vp = eval(rn,5.8,0.,0.,0.) 
	 vs = eval(rn,3.2,0.,0.,0.)
	 qs = 600.
	 qp = qavg(qkdef,600.,vp,vs)
      else
	 vp = 0.0
	 vs = 0.0
	 qp = 57823
	 qs = qinf
      endif
      end
