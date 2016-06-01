c     This file 'libftau.f' is revised from ttimes.f, which is from ttimes-Package.
c     ftau(...) is used to calculate traveltime given seismic phase, delta and eq-depth
c     If you prefer C language programming, ctau(...) from libctau.c could be your option.
c
c     Wangsheng IGG-CAS
c     wangsheng.cas@gmail.com
c     2015/8/27
c
c     phlst: seismic phase (only one)
c     zs   : depth(km) of eq
c     delta: great circle distance(Deg)
c     tt   : list of traveltime(s)
c     dtdd : ray parameter (s/deg)
c     dtdh : dt/dh
c     dddp : dt/dp
c     mn   : traveltime minutes (min)
c     ts   : traveltime seconds (s)
c     toa  : takeoff-angle      (?DEG?)
c     n    : number of phase found
c     phcd : list of phase name, ended by '\0'

      subroutine ftau(phlst,zs,delta,tt,dtdd,dtdh,dddp,mn,ts,toa,
     1 n,phcd)
      save
      real    zs, delta
      parameter (max=60, Re=6371.0)
      logical log,prnt(3),okm,overb,otoa
      character*8 phcd(max),phlst(1)
      character*64 modnam, arg
      dimension tt(max),dtdd(max),dtdh(max),dddp(max),mn(max),ts(max)
      dimension toa(max),usrc(2)
      data in/1/,prnt(3)/.true./
      data okm/.false./, overb/.true./, otoa/.false./
      include 'modnam.inc'
      include 'version.inc'

c      zs = 30.0
c      delta = 80.0
c      phlst(1) = 'P'
      ix = indexr(modnam,'/')+1
      modnam(ix:) = 'iasp91'

      pi      = 4*atan(1.0)
      degkm   = Re*pi/180
      prnt(1) = .false.
      prnt(2) = .false.
      iskip   = 0

      call tabin(in,modnam,prnt)
      call brnset(1,phlst,prnt)
c      write(*,*) phlst
c      write(*,*) zs
      if(zs.lt.0.) go to 13
      call depset(zs,usrc)
      if (usrc(1) .gt. 0) then
         vsrc = ((Re-zs)/Re)/usrc(1)
         etap = (Re-zs)/(vsrc*180/pi)
      endif
      if (usrc(2) .gt. 0) then
         vsrc = ((Re-zs)/Re)/usrc(2)
         etas = (Re-zs)/(vsrc*180/pi)
      endif

      call trtm(delta,max,n,tt,dtdd,dtdh,dddp,phcd)
      if(n.le.0)then
      else


	 do 4 i=1,n
            mn(i)=int(tt(i)/60.)
            ts(i)=amod(tt(i),60.)
	    if (0 .ne. index('pP',phcd(i)(1:1))) then
	       eta = etap
	    else
	       eta = etas
	    endif
	    toa(i) = 180/pi*asin(abs(dtdd(i))/eta)
	    if (dtdh(i) .gt. 0) toa(i) = 180 - toa(i)
	    if(okm)then
	       dtdd(i)=dtdd(i)/degkm
	       dddp(i)=dddp(i)/degkm**2
	    endif
 4       continue
c--------------------------------------
      endif
c--------------------------------------
 13   continue
      close(in)
      if (prnt(1) .or. prnt(2)) close(10)
c      call exit(0)
      end

      integer function indexr(string,ch)
      character string*(*), ch*(*)

      lch = len(ch)
      do 1 i=len(string)-lch,1,-1
	 if (string(i:i+lch-1) .eq. ch) go to 10
1     continue
      i = 0
10    continue
      indexr = i
      end


      subroutine add(a,b,c)
      real*4 a,b,c
      c = a + b
      end